//
// Created by 89600 on 2019/11/13.
//

#include "Mips.h"

Mips::Mips(vector<string> &middleCode) : middleCode(middleCode) {
    symbolMaps.emplace_back(nullptr);
    currentMap = new SymbolMap(nullptr);
    codePlace = 0;
    currentCode = "";
    currentAddr = 0;
    paras = 0;
    funcStack = 0;
    pushes = 0;
}

void Mips::parse() {
    outMips.open("mips.txt", ios::trunc | ios::out);
    outToMips(".macro push %x");
    outToMips("sw %x, 0($fp)");
    outToMips("addiu $fp, $fp, 4");
    outToMips(".end_macro");
    outToMips("");
    outToMips(".macro pop %x");
    outToMips("addiu $fp, $fp, -4");
    outToMips("lw %x, 0($fp)");
    outToMips(".end_macro");
    outToMips("");
    outToMips(".text");
    outToMips("la $fp, splabel");
    outToMips("la $t7, slabelb");

    while (codePlace < middleCode.size()) {
        getSent();
        instrs.push_back(currentInstr);
        parseSent();
    }
    outToMips("");
    outToMips(".data");
    outToMips("splabel:");
    outToMips(".space 1024");
    outToMips("slabelb:");
    outToMips(".asciiz \"\"");
    outToMips("slabeln:");
    outToMips(".asciiz \"\\n\"");
    for (int i = 0; i < constStrings.size(); i++) {
        string elm = constStrings.at(i);
        outToMips("slabel_" + to_string(i) + ":");
        outToMips(".asciiz " + elm);
    }

    outMips.close();
}

void Mips::outToMips(string str) {
    outMips << str << endl;
}

void Mips::getSent() {
    currentCode = middleCode.at(codePlace++);
    currentInstr.iden0 = "";
    currentInstr.iden1 = "";
    currentInstr.iden2 = "";
    currentInstr.iden3 = "";
    currentInstr.iden4 = "";
    int i, base = 0, c = 0;
    for (i = base; i < currentCode.size(); i++) {
        if (currentCode[i] == ' ') {
            break;
        }
    }
    string tmpstr = currentCode.substr(0, i);
    if (tmpstr == "ret") {
        currentInstr.type = ret;
        if (i == currentCode.size()) {
            return;
        }
    } else if (tmpstr == "const") {
        currentInstr.type = constdef;
    } else if (tmpstr == "var") {
        currentInstr.type = vardef;
    } else if (tmpstr == "int" || tmpstr == "char" || tmpstr == "void") {
        currentInstr.type = funcdef;
    } else if (tmpstr == "para") {
        currentInstr.type = paradef;
    } else if (tmpstr == "push") {
        currentInstr.type = push;
    } else if (tmpstr == "call") {
        currentInstr.type = fcall;
    } else if (tmpstr == "GOTO") {
        currentInstr.type = bgoto;
    } else if (tmpstr == "BNZ") {
        currentInstr.type = bnz;
    } else if (tmpstr == "BZ") {
        currentInstr.type = bz;
    } else if (tmpstr.back() == ':') {
        currentInstr.iden0 = tmpstr;
        currentInstr.type = label;
        return;
    } else {
        currentInstr.type = cal;
    }
    currentInstr.iden0 = tmpstr;
    while (true) {
        base = i + 1;
        int inquote = 0;
        for (i = base; i < currentCode.size(); i++) {
            if (currentCode[i] == '\"') {
                inquote = 1 - inquote;
            }
            if (currentCode[i] == ' ' && !inquote) {
                break;
            }
        }
        tmpstr = currentCode.substr(base, i - base);
        switch (c) {
            case 0:
                currentInstr.iden1 = tmpstr;
                break;
            case 1:
                currentInstr.iden2 = tmpstr;
                break;
            case 2:
                currentInstr.iden3 = tmpstr;
                break;
            case 3:
                currentInstr.iden4 = tmpstr;
                break;
            default:
                break;
        }
        c++;
        if (i == currentCode.size()) {
            return;
        }
    }
}

void Mips::createPreVar() {
    currentMap->add("__cond1", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__cond2", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para0", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para1", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para2", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para3", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para4", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para5", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para6", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para7", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__para8", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp0", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp1", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp2", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp3", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp4", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp5", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp6", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp7", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__tmp8", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__func0", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__func1", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__func2", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__func3", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__func4", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__retval", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__print", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("__", vint, 1, currentAddr);
    currentAddr += 4;
    currentMap->add("RET", vint, 1, currentAddr);
    currentAddr += 4;
}

void Mips::parseSent() {
    switch (currentInstr.type) {
        case constdef:
            if (currentInstr.iden1 == "int") {
                currentMap->add(currentInstr.iden2, vint, 1, currentAddr);
            } else {
                currentMap->add(currentInstr.iden2, vchar, 1, currentAddr);
            }
            outToMips("li $t8, " + currentInstr.iden4);
            outToMips("sw $t8, " + to_string(-currentAddr) + "($sp)");
            currentAddr += 4;
            break;
        case vardef:
            if (currentInstr.iden1 == "int") {
                int i, l = 0, r = 0;
                for (i = 0; i < currentInstr.iden2.size(); i++) {
                    if (currentInstr.iden2[i] == '[') {
                        l = i + 1;
                    }
                    if (currentInstr.iden2[i] == ']') {
                        r = i;
                    }
                }
                if (l == 0) {
                    currentMap->add(currentInstr.iden2, vint, 1, currentAddr);
                    currentAddr += 4;
                } else {
                    string sub = currentInstr.iden2.substr(l, r - l);
                    r = stoi(sub);
                    for (i = 0; i < r; i++) {
                        currentMap->add(currentInstr.iden2.substr(0, l) + to_string(i) + "]", vint, 1, currentAddr);
                        currentAddr += 4;
                    }
                }
            } else {
                int i, l = 0, r = 0;
                for (i = 0; i < currentInstr.iden2.size(); i++) {
                    if (currentInstr.iden2[i] == '[') {
                        l = i + 1;
                    }
                    if (currentInstr.iden2[i] == ']') {
                        r = i;
                    }
                }
                if (l == 0) {
                    currentMap->add(currentInstr.iden2, vchar, 1, currentAddr);
                    currentAddr += 4;
                } else {
                    string sub = currentInstr.iden2.substr(l, r - l);
                    r = stoi(sub);
                    for (i = 0; i < r; i++) {
                        currentMap->add(currentInstr.iden2.substr(0, l) + to_string(i) + "]", vchar, 1, currentAddr);
                        currentAddr += 4;
                    }
                }
            }
            outToMips("sw $0, " + to_string(-currentAddr) + "($sp)");
            break;
        case funcdef:
            paras = 0;
            if (currentMap->getPrev() != nullptr) {
                currentMap = currentMap->getPrev();
            } else {
                outToMips("jal flabel_main");
                outToMips("");
                outToMips("flabel_scanf:");
                outToMips("li $v0, 5");
                outToMips("syscall");
                outToMips("sw $v0, 0($t8)");
                outToMips("jr $ra");
                outToMips("");
                outToMips("flabel_scanfc:");
                outToMips("li $v0, 12");
                outToMips("syscall");
                outToMips("sw $v0, 0($t8)");
                outToMips("jr $ra");
                outToMips("");
                outToMips("flabel_printf:");
                outToMips("li $v0, 4");
                outToMips("move $a0, $t7");
                outToMips("syscall");
                outToMips("la $t7, slabelb");
                outToMips("pop $t8");
                outToMips("li $v0, 1");
                outToMips("move $a0, $t8");
                outToMips("syscall");
                outToMips("li $v0, 4");
                outToMips("la $a0, slabeln");
                outToMips("syscall");
                outToMips("jr $ra");
                outToMips("");
                outToMips("flabel_printfc:");
                outToMips("li $v0, 4");
                outToMips("move $a0, $t7");
                outToMips("syscall");
                outToMips("la $t7, slabelb");
                outToMips("pop $t8");
                outToMips("li $v0, 11");
                outToMips("move $a0, $t8");
                outToMips("syscall");
                outToMips("li $v0, 4");
                outToMips("la $a0, slabeln");
                outToMips("syscall");
                outToMips("jr $ra");
                outToMips("");
                outToMips("flabel_printfs:");
                outToMips("li $v0, 4");
                outToMips("move $a0, $t7");
                outToMips("syscall");
                outToMips("la $t7, slabelb");
                outToMips("li $v0, 4");
                outToMips("la $a0, slabeln");
                outToMips("syscall");
                outToMips("jr $ra");
                outToMips("");

            }
            if (currentInstr.iden0 == "int") {
                currentMap->add(currentInstr.iden1.substr(0, currentInstr.iden1.size() - 2), fint, 1, currentAddr);
            } else if (currentInstr.iden0 == "char") {
                currentMap->add(currentInstr.iden1.substr(0, currentInstr.iden1.size() - 2), fchar, 1, currentAddr);
            } else {
                currentMap->add(currentInstr.iden1.substr(0, currentInstr.iden1.size() - 2), fvoid, 1, currentAddr);
            }
            outToMips("flabel_" + currentInstr.iden1.substr(0, currentInstr.iden1.size() - 2) + ":");

            symbolMaps.emplace_back(currentMap);
            currentMap = new SymbolMap(currentMap);
            currentAddr += 4;
            createPreVar();

            outToMips("push $ra");
            currentMap->add("__retaddr", pint, 1, funcStack);
            break;
        case paradef:
            if (currentInstr.iden1 == "int") {
                currentMap->add(currentInstr.iden2, pint, 1, paras * 4);
            } else {
                currentMap->add(currentInstr.iden2, pchar, 1, paras * 4);
            }
            paras++;
            break;
        case ret:
            if (!currentInstr.iden1.empty()) {
                int addr = currentMap->search(currentInstr.iden1);
                if (addr >= 0) {
                    outToMips("lw $t9, " + to_string(-addr) + "($sp)");
                }
            }
            outToMips("pop $ra");
            outToMips("addiu $fp, $fp, " + to_string(paras * 4));
            outToMips("jr $ra");
            break;
        case push:
            if (currentInstr.iden1[0] != '\"') {
                int addr = currentMap->search(currentInstr.iden1);
                if (addr >= 0) {
                    outToMips("lw $t8, " + to_string(-addr) + "($sp)");
                    outToMips("push $t8");
                    pushes++;
                }
            } else {
                int i;
                for (i = 0; i < constStrings.size(); i++) {
                    if (constStrings.at(i) == currentInstr.iden1) {
                        break;
                    }
                }

                if (i == constStrings.size()) {
                    constStrings.push_back(currentInstr.iden1);
                }
                outToMips("la $t7, slabel_" + to_string(i));
            }
            break;
        case fcall:
            if (!currentInstr.iden1.empty()) {
                if (currentInstr.iden1 == "scanf") {
                    outToMips("pop $t8");
                    outToMips("la $t8, " + currentMap->getOutput(instrs.at(instrs.size() - 2).iden1, paras));
                    SymbolType type = currentMap->findType(instrs.at(instrs.size() - 2).iden1);
                    if (type == vchar || type == pchar) {
                        currentInstr.iden1 = "scanfc";
                    }
                } else if (currentInstr.iden1 == "printf") {
                    string tstr = instrs.at(instrs.size() - 2).iden1;
                    if (tstr[0] == '\"') {
                        currentInstr.iden1 = "printfs";
                    } else {
                        SymbolType type = currentMap->findType(instrs.at(instrs.size() - 2).iden1);
                        if (type == vchar || type == pchar) {
                            currentInstr.iden1 = "printfc";
                        }
                    }
                }
                outToMips("jal flabel_" + currentInstr.iden1);
            }
            break;
        case cal:
            if (currentInstr.iden0 == "__cond1" && currentInstr.iden2 == "__cond2") {
                condType = currentInstr.iden1;
            } else {
                int type = 0;
                if (currentInstr.iden2 == "RET") {
                    outToMips("sw $t9, " + currentMap->getOutput(currentInstr.iden0, paras));
                    if (currentMap->findType(instrs.at(instrs.size() - 2).iden1) == fchar) {
                        type = 1;
                    }
                } else {
                    if (isdigit(currentInstr.iden2[0]) || currentInstr.iden2[0] == '-') {
                        outToMips("li $t1, " + currentInstr.iden2);
                    } else if (currentMap->search(currentInstr.iden2) >= 0) {
                        outToMips("lw $t1, " + currentMap->getOutput(currentInstr.iden2, paras));
                        if (currentMap->findType(currentInstr.iden2) == pchar ||
                            currentMap->findType(currentInstr.iden2) == vchar ||
                            currentMap->findType(currentInstr.iden2) == fchar) {
                            type = 1;
                        }
                    }
                    if (currentInstr.iden3.empty()) {
                        outToMips("sw $t1, " + currentMap->getOutput(currentInstr.iden0, paras));
                    } else {
                        type = 0;
                        if (isdigit(currentInstr.iden4[0]) || currentInstr.iden4[0] == '-') {
                            outToMips("li $t2, " + currentInstr.iden4);
                        } else {
                            outToMips("lw $t2, " + currentMap->getOutput(currentInstr.iden4, paras));
                        }
                        if (currentInstr.iden3 == "+") {
                            outToMips("add $t0, $t1, $t2");
                        } else if (currentInstr.iden3 == "-") {
                            outToMips("sub $t0, $t1, $t2");
                        } else if (currentInstr.iden3 == "*") {
                            outToMips("mul $t0, $t1, $t2");
                        } else if (currentInstr.iden3 == "/") {
                            outToMips("div $t0, $t1, $t2");
                        }
                        outToMips("sw $t0, " + currentMap->getOutput(currentInstr.iden0, paras));
                    }
                }
                if (currentInstr.iden0[0] == '_' && currentInstr.iden0[1] == '_') {
                    currentMap->setType(currentInstr.iden0, type ? vchar : vint);
                }
            }
            break;
        case label:
            outToMips(currentInstr.iden0);
            break;
        case bgoto:
            outToMips("j " + currentInstr.iden1);
            break;
        case bnz:
            outToMips("lw $t1, " + to_string(-currentMap->search("__cond1")) + "($sp)");
            outToMips("lw $t2, " + to_string(-currentMap->search("__cond2")) + "($sp)");
            outToMips("sub $t0, $t1, $t2");
            if (condType == "==") {
                outToMips("beqz $t0, " + currentInstr.iden1);
            } else if (condType == "!=") {
                outToMips("bnez $t0, " + currentInstr.iden1);
            } else if (condType == ">=") {
                outToMips("bgez $t0, " + currentInstr.iden1);
            } else if (condType == "<=") {
                outToMips("blez $t0, " + currentInstr.iden1);
            } else if (condType == ">") {
                outToMips("bgtz $t0, " + currentInstr.iden1);
            } else if (condType == "<") {
                outToMips("bltz $t0, " + currentInstr.iden1);
            }
            break;
        case bz:
            outToMips("lw $t1, " + to_string(-currentMap->search("__cond1")) + "($sp)");
            outToMips("lw $t2, " + to_string(-currentMap->search("__cond2")) + "($sp)");
            outToMips("sub $t0, $t1, $t2");
            if (condType == "==") {
                outToMips("bnez $t0, " + currentInstr.iden1);
            } else if (condType == "!=") {
                outToMips("beqz $t0, " + currentInstr.iden1);
            } else if (condType == ">=") {
                outToMips("bltz $t0, " + currentInstr.iden1);
            } else if (condType == "<=") {
                outToMips("bgtz $t0, " + currentInstr.iden1);
            } else if (condType == ">") {
                outToMips("blez $t0, " + currentInstr.iden1);
            } else if (condType == "<") {
                outToMips("bgez $t0, " + currentInstr.iden1);
            }
            break;
        default:
            break;
    }
}