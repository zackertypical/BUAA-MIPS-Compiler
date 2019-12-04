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
    currentStack = 0;
    paras = 0;
    funcStack = 0;
    macrolabel = 0;
}

void Mips::parse() {
    while (codePlace < middleCode.size()) {
        getSent();
        instrs.push_back(currentInstr);
    }

    // 分析vector instrs中的中间代码
    // 寻找可以内联的函数，并构建函数符号表
    parseFunc();
    // 分析所有常量变量定义，构建符号表
    parseDef();
    outSymbolMap();
    // 开始翻译语句
    parseSent();
    // 处理data段
    setData();

    outMips.open("mips.txt", ios::trunc | ios::out);

    outMips << ".data" << endl;
    for (string elm : dotData) {
        outMips << elm << endl;
    }

    outMips << ".text" << endl;
    for (string elm : dotText) {
        outMips << elm << endl;
    }

    outMips.close();
}

void Mips::outToText(string str) {
    dotText.push_back(str);
}

void Mips::outToMacro(string str) {
    dotMacro[nowMacro].push_back(str);
}

void Mips::outToData(string str) {
    dotData.push_back(str);
}

void Mips::setData() {
    outToData("fplabel:");
    outToData(".space 10240");
    outToData("gplabel:");
    outToData(".space 40960");
    outToData("slabelb:");
    outToData(".asciiz \"\"");
    outToData("slabeln:");
    outToData(".asciiz \"\\n\"");
    for (int i = 0; i < constStrings.size(); i++) {
        string elm = constStrings.at(i);
        outToData("__str" + to_string(i) + ":");
        outToData(".asciiz \"" + elm + "\"");
    }
    outToData("");
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
        currentInstr.type = label;
    } else if (tmpstr == "mac") {
        currentInstr.type = mac;
    } else {
        currentInstr.type = cal;
    }
    currentInstr.iden0 = tmpstr;
    if (i == currentCode.size()) {
        return;
    }
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
                if (currentInstr.type == funcdef) {
                    currentInstr.iden1 = tmpstr.substr(0, tmpstr.size() - 2);
                }
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

void Mips::parseFunc() {
    string currentFunc;
    SymbolType currentFuncType = errors;
    int canInline = 1;
    int currentFuncParas = 0;
    for (codePlace = 0; codePlace < middleCode.size(); codePlace++) {
        currentInstr = instrs.at(codePlace);
        if (currentInstr.type == funcdef) {
            if (!currentFunc.empty()) {
                currentMap->addFunc(currentFunc, currentFuncType, currentFuncParas, canInline);
            }

            currentFunc = currentInstr.iden1;
            if (currentInstr.iden0 == "int") {
                currentFuncType = fint;
            } else if (currentInstr.iden0 == "char") {
                currentFuncType = fchar;
            } else {
                currentFuncType = fvoid;
            }
            currentFuncParas = 0;
            canInline = 1;
        } else if (currentInstr.type == paradef) {
            currentFuncParas++;
        } else if (currentInstr.type == ret) {
            if (instrs.size() <= codePlace + 1 || instrs.at(codePlace + 1).type != funcdef) {
                canInline = 0;
            }
        } else if (currentInstr.type == fcall) {
            canInline = 0;
        }
    }

    currentMap->addFunc(currentFunc, currentFuncType, currentFuncParas, 0);
}

void Mips::parseDef() {
    int currentFuncParas = 0;
    for (codePlace = 0; codePlace < middleCode.size(); codePlace++) {
        currentInstr = instrs.at(codePlace);
        if (currentInstr.type == funcdef) {
            if (currentMap->getPrev() != nullptr) {
                currentMap = currentMap->getPrev();
            }
            currentFuncParas = currentMap->searchFunc(currentInstr.iden1)->place;
            currentMap = currentMap->getNext(currentInstr.iden1);
            currentStack = 0;
        } else if (currentInstr.type == paradef) {
            if (currentInstr.iden1 == "int") {
                currentMap->addForce(currentInstr.iden2 + "_Fake", pint, 3, -currentFuncParas * 4);
                currentMap->addForce(currentInstr.iden2, vint, 1, currentStack * 4);
                currentStack++;
            } else {
                currentMap->addForce(currentInstr.iden2 + "_Fake", pchar, 3, -currentFuncParas * 4);
                currentMap->addForce(currentInstr.iden2, vchar, 1, currentStack * 4);
                currentStack++;
            }
            currentFuncParas--;
        } else if (currentInstr.type == constdef || currentInstr.type == vardef) {
            if (currentMap->getPrev() == nullptr) {
                if (currentInstr.iden1 == "int") {
                    if (currentInstr.iden2.back() == ']') {
                        int i;
                        for (i = 0; i < currentInstr.iden2.size(); i++) {
                            if (currentInstr.iden2.at(i) == '[') {
                                break;
                            }
                        }
                        int n = stoi(currentInstr.iden2.substr(i + 1, currentInstr.iden2.size() - i - 1));
                        for (int j = 0; j < n; j++) {
                            currentMap->addForce(currentInstr.iden2.substr(0, i) + "[" + to_string(j) + "]", aint, 0,
                                            currentAddr * 4);
                            currentAddr++;
                        }
                    } else {
                        currentMap->addForce(currentInstr.iden2, vint, 0, currentAddr * 4);
                        currentAddr++;
                    }
                } else {
                    if (currentInstr.iden2.back() == ']') {
                        int i;
                        for (i = 0; i < currentInstr.iden2.size(); i++) {
                            if (currentInstr.iden2.at(i) == '[') {
                                break;
                            }
                        }
                        int n = stoi(currentInstr.iden2.substr(i + 1, currentInstr.iden2.size() - i - 1));
                        for (int j = 0; j < n; j++) {
                            currentMap->addForce(currentInstr.iden2.substr(0, i) + "[" + to_string(j) + "]", achar, 0,
                                            currentAddr * 4);
                            currentAddr++;
                        }
                    } else {
                        currentMap->addForce(currentInstr.iden2, vchar, 0, currentAddr * 4);
                        currentAddr++;
                    }
                }
            } else {
                if (currentInstr.iden1 == "int") {
                    if (currentInstr.iden2.back() == ']') {
                        int i;
                        for (i = 0; i < currentInstr.iden2.size(); i++) {
                            if (currentInstr.iden2.at(i) == '[') {
                                break;
                            }
                        }
                        int n = stoi(currentInstr.iden2.substr(i + 1, currentInstr.iden2.size() - i - 1));
                        for (int j = 0; j < n; j++) {
                            currentMap->addForce(currentInstr.iden2.substr(0, i) + "[" + to_string(j) + "]", aint, 1,
                                            currentStack * 4);
                            currentStack++;
                        }
                    } else {
                        currentMap->addForce(currentInstr.iden2, vint, 1, currentStack * 4);
                        currentStack++;
                    }
                } else {
                    if (currentInstr.iden2.back() == ']') {
                        int i;
                        for (i = 0; i < currentInstr.iden2.size(); i++) {
                            if (currentInstr.iden2.at(i) == '[') {
                                break;
                            }
                        }
                        int n = stoi(currentInstr.iden2.substr(i + 1, currentInstr.iden2.size() - i - 1));
                        for (int j = 0; j < n; j++) {
                            currentMap->addForce(currentInstr.iden2.substr(0, i) + "[" + to_string(j) + "]", achar, 1,
                                            currentStack * 4);
                            currentStack++;
                        }
                    } else {
                        currentMap->addForce(currentInstr.iden2, vchar, 1, currentStack * 4);
                        currentStack++;
                    }
                }
            }
        } else if (currentInstr.type == cal) {
            if (currentInstr.iden0.back() != ']') {
                if (!currentInstr.iden4.empty()) {
                    if (currentMap->add(currentInstr.iden0, vint, 1, currentStack * 4)) {
                        currentStack++;
                    }
                } else {
                    SymbolType typee = currentMap->getType(currentInstr.iden2);
                    if (currentMap->add(currentInstr.iden0, typee, 1, currentStack * 4)) {
                        currentStack++;
                    }
                }
            }
        } else if (currentInstr.type == push) {
            if (currentInstr.iden1.at(0) == '\"') {
                constStrings.push_back(currentInstr.iden1.substr(1, currentInstr.iden1.size() - 2));
                currentMap->add("__str" + to_string(constStrings.size() - 1), str, 0, constStrings.size() - 1);
                instrs.at(codePlace).iden1 = "__str" + to_string(constStrings.size() - 1);
            }
        } else if (currentInstr.type == fcall) {
            if (currentInstr.iden0 == "char") {
                currentMap->add("RET", vchar, 1, currentStack * 4);
            } else {
                currentMap->add("RET", vint, 1, currentStack * 4);
            }
            currentStack++;
        }
    }
}

void Mips::outSymbolMap() {
    ofstream outSym;
    outSym.open("symbolset.txt", ios::trunc | ios::out);
    if (currentMap->getPrev() != nullptr) {
        currentMap = currentMap->getPrev();
    }
    outSym << "Symbol Table: global" << endl;
    outSym << "--------------------------------------------" << endl;
    outSym << "|   symbol name   | type | place | address |" << endl;
    outSym << "--------------------------------------------" << endl;
    currentMap->printSymbols(outSym);
    outSym << "--------------------------------------------" << endl;
    outSym << endl;

    map<string, SymbolMap *> theMap = currentMap->getNextList();
    map<string, SymbolMap *>::iterator iter;

    for (iter = theMap.begin(); iter != theMap.end(); iter++) {
        outSym << "Symbol Table: " + iter->first << endl;
        outSym << "--------------------------------------------" << endl;
        outSym << "|   symbol name   | type | place | address |" << endl;
        outSym << "--------------------------------------------" << endl;
        iter->second->printSymbols(outSym);
        outSym << "--------------------------------------------" << endl;
        outSym << endl;
    }

    outSym.close();
}

void Mips::parseSent() {
    string lastFunc;
    outToText("la $fp, fplabel");
    outToText("la $gp, gplabel");
    for (codePlace = 0; codePlace < middleCode.size(); codePlace++) {
        currentInstr = instrs.at(codePlace);
        switch (currentInstr.type) {
            case constdef:
                if (currentMap->getPrev() != nullptr && currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    outToMacro(currentMap->saveSymbol(currentInstr.iden4, currentInstr.iden2));
                } else {
                    outToText(currentMap->saveSymbol(currentInstr.iden4, currentInstr.iden2));
                }
                break;
            case paradef:
                if (currentInstr.iden2 != "__retaddr") {
                    if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                        outToMacro(currentMap->loadSymbol("$t8", currentInstr.iden2 + "_Fake"));
                        outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden2));
                    } else {
                        outToText(currentMap->loadSymbol("$t8", currentInstr.iden2 + "_Fake"));
                        outToText(currentMap->saveSymbol("$t8", currentInstr.iden2));
                    }
                }
                break;
            case funcdef:
                // 处理上一个函数的结尾
                if (currentMap->getPrev() != nullptr) {
                    currentMap = currentMap->getPrev();
                    if (currentMap->searchFunc(lastFunc)->addr) {
                        if (currentMap->getNext(lastFunc)->search("__retval") != nullptr) {
                            outToMacro(currentMap->getNext(lastFunc)->loadSymbol("$v1", "__retval"));
                        }
                        outToMacro("addiu $fp, $fp, " + to_string(currentMap->searchFunc(lastFunc)->place * -4));
                        outToMacro("addiu $sp, $sp, 1024");
                    } else {
                        if (currentMap->getNext(lastFunc)->search("__retval") != nullptr) {
                            outToText(currentMap->getNext(lastFunc)->loadSymbol("$v1", "__retval"));
                        }
                        outToText("lw $ra, " + currentMap->getNext(lastFunc)->getOutput("__retaddr"));
                        outToText("addiu $fp, $fp, " + to_string(currentMap->searchFunc(lastFunc)->place * -4));
                        outToText("addiu $sp, $sp, 1024");
                        outToText("jr $ra");
                        outToText("");
                    }
                } else {
                    outToText("jal flabel_main");
                }
                // 处理这个函数的开头
                lastFunc = currentInstr.iden1;
                if (currentMap->searchFunc(currentInstr.iden1)->addr) {
                    nowMacro = "flabel_" + currentInstr.iden1;
                    dotMacro.insert(make_pair(nowMacro, vector<string>()));
                    outToMacro("addiu $sp, $sp, -1024");
                } else {
                    outToText("flabel_" + currentInstr.iden1 + ":");
                    outToText("addiu $sp, $sp, -1024");
                    if (currentInstr.iden1 != "main") {
                        outToText(currentMap->getNext(currentInstr.iden1)->saveSymbol("$ra", "__retaddr"));
                    }
                }
                currentMap = currentMap->getNext(currentInstr.iden1);
                break;
            case ret:
                // 只处理不在函数最后的return
                if (instrs.size() > codePlace + 1 && instrs.at(codePlace + 1).type != funcdef) {
                    if (!currentInstr.iden1.empty()) {
                        outToText(currentMap->loadSymbol("$v1", "__retval"));
                    }
                    outToText("lw $ra, " + currentMap->getOutput("__retaddr"));
                    outToText("addiu $fp, $fp, " + to_string(currentMap->getPrev()->searchFunc(lastFunc)->place * -4));
                    outToText("addiu $sp, $sp, 1024");
                    outToText("jr $ra");
                }
                break;
            case push:
                if (currentInstr.iden1 != "__retaddr") {
                    if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                        if (currentInstr.iden1.substr(0, 5) == "__str") {
                            outToMacro("la $a3, " + currentInstr.iden1);
                        } else {
                            outToMacro(currentMap->loadSymbol("$t8", currentInstr.iden1));
                            outToMacro("sw $t8, 0($fp)");
                            outToMacro("addiu $fp, $fp, 4");
                        }
                    } else {
                        if (currentInstr.iden1.substr(0, 5) == "__str") {
                            outToText("la $a3, " + currentInstr.iden1);
                        } else {
                            outToText(currentMap->loadSymbol("$t8", currentInstr.iden1));
                            outToText("sw $t8, 0($fp)");
                            outToText("addiu $fp, $fp, 4");
                        }
                    }
                } else {
                    if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                        outToMacro("addiu $fp, $fp, 4");
                    } else {
                        outToText("addiu $fp, $fp, 4");
                    }
                }
                break;
            case fcall:
                if (currentMap->getPrev()->searchFunc(currentInstr.iden1)->addr) {
                    outToText("# inline function " + currentInstr.iden1);
                    for (string elm : dotMacro["flabel_" + currentInstr.iden1]) {
                        if (elm.back() == '?') {
                            outToText(elm.substr(0, elm.size() - 2) + '_' + to_string(macrolabel) + ":");
                        } else if (elm.back() == '!') {
                            outToText(elm.substr(0, elm.size() - 1) + '_' + to_string(macrolabel));
                        } else {
                            outToText(elm);
                        }
                    }
                    macrolabel++;
                    outToText("# inline end");
                    outToText(currentMap->saveSymbol("$v1", "RET"));
                } else {
                    outToText("jal flabel_" + currentInstr.iden1);
                    outToText(currentMap->saveSymbol("$v1", "RET"));
                }
                if (currentMap->getPrev()->searchFunc(currentInstr.iden1)->type == fchar) {
                    currentMap->setType("RET", vchar);
                } else {
                    currentMap->setType("RET", vint);
                }
                break;
            case cal:
                if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    if (currentInstr.iden1 == "=") {
                        if (!currentInstr.iden3.empty()) {
                            if (currentMap->search(currentInstr.iden2) != nullptr) {
                                outToMacro(currentMap->loadSymbol("$t8", currentInstr.iden2));
                            } else {
                                outToMacro("li $t8, " + currentInstr.iden2);
                            }
                            if (currentMap->search(currentInstr.iden4) != nullptr) {
                                outToMacro(currentMap->loadSymbol("$t9", currentInstr.iden4));
                            } else {
                                outToMacro("li $t9, " + currentInstr.iden4);
                            }
                            if (currentInstr.iden3 == "+") {
                                outToMacro("add $t8, $t8, $t9");
                                outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "-") {
                                outToMacro("sub $t8, $t8, $t9");
                                outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "*") {
                                outToMacro("mul $t8, $t8, $t9");
                                outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "/") {
                                outToMacro("div $t8, $t9");
                                outToMacro("mflo $t8");
                                outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "[]=") {
                                outToMacro("sll $t8, $t8, 2");
                                outToMacro(
                                        "addiu $t8, $t8, " + to_string(currentMap->search(currentInstr.iden0)->addr));
                                if (currentMap->search(currentInstr.iden0)->place == 0) {
                                    outToMacro("add $t8, $t8, $gp");
                                } else {
                                    outToMacro("add $t8, $t8, $sp");
                                }
                                outToMacro("sw $t9, 0($t8)");
                            } else if (currentInstr.iden3 == "[]") {
                                outToMacro("sll $t9, $t9, 2");
                                outToMacro(
                                        "addiu $t9, $t9, " + to_string(currentMap->search(currentInstr.iden2)->addr));
                                if (currentMap->search(currentInstr.iden2)->place == 0) {
                                    outToMacro("add $t9, $t9, $gp");
                                } else {
                                    outToMacro("add $t9, $t9, $sp");
                                }
                                outToMacro("lw $t8, 0($t9)");
                                outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            }
                        } else {
                            if (currentMap->search(currentInstr.iden2) != nullptr) {
                                outToMacro(currentMap->loadSymbol("$t8", currentInstr.iden2));
                            } else {
                                outToMacro("li $t8, " + currentInstr.iden2);
                            }
                            outToMacro(currentMap->saveSymbol("$t8", currentInstr.iden0));
                        }
                    } else {
                        condType = currentInstr.iden1;
                    }
                } else {
                    if (currentInstr.iden1 == "=") {
                        if (!currentInstr.iden3.empty()) {
                            if (currentMap->search(currentInstr.iden2) != nullptr) {
                                outToText(currentMap->loadSymbol("$t8", currentInstr.iden2));
                            } else {
                                outToText("li $t8, " + currentInstr.iden2);
                            }
                            if (currentMap->search(currentInstr.iden4) != nullptr) {
                                outToText(currentMap->loadSymbol("$t9", currentInstr.iden4));
                            } else {
                                outToText("li $t9, " + currentInstr.iden4);
                            }
                            if (currentInstr.iden3 == "+") {
                                outToText("add $t8, $t8, $t9");
                                outToText(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "-") {
                                outToText("sub $t8, $t8, $t9");
                                outToText(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "*") {
                                outToText("mul $t8, $t8, $t9");
                                outToText(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "/") {
                                outToText("div $t8, $t9");
                                outToText("mflo $t8");
                                outToText(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            } else if (currentInstr.iden3 == "[]=") {
                                outToText("sll $t8, $t8, 2");
                                outToText("addiu $t8, $t8, " + to_string(currentMap->search(currentInstr.iden0)->addr));
                                if (currentMap->search(currentInstr.iden0)->place == 0) {
                                    outToText("add $t8, $t8, $gp");
                                } else {
                                    outToText("add $t8, $t8, $sp");
                                }
                                outToText("sw $t9, 0($t8)");
                            } else if (currentInstr.iden3 == "[]") {
                                outToText("sll $t9, $t9, 2");
                                outToText("addiu $t9, $t9, " + to_string(currentMap->search(currentInstr.iden2)->addr));
                                if (currentMap->search(currentInstr.iden2)->place == 0) {
                                    outToText("add $t9, $t9, $gp");
                                } else {
                                    outToText("add $t9, $t9, $sp");
                                }
                                outToText("lw $t8, 0($t9)");
                                outToText(currentMap->saveSymbol("$t8", currentInstr.iden0));
                            }
                        } else {
                            if (currentMap->search(currentInstr.iden2) != nullptr) {
                                outToText(currentMap->loadSymbol("$t8", currentInstr.iden2));
                            } else {
                                outToText("li $t8, " + currentInstr.iden2);
                            }
                            outToText(currentMap->saveSymbol("$t8", currentInstr.iden0));
                        }
                    } else {
                        condType = currentInstr.iden1;
                    }
                }
                if (currentInstr.iden0 == "__print") {
                    currentMap->setType("__print", currentMap->getType(currentInstr.iden2));
                }
                if (currentInstr.iden0.substr(0, 6) == "__func") {
                    currentMap->setType(currentInstr.iden0, currentMap->getType(currentInstr.iden2));
                }
                break;
            case label:
                if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    outToMacro(currentInstr.iden0 + "?");
                } else {
                    outToText(currentInstr.iden0);
                }
                break;
            case bgoto:
                if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    outToMacro("j " + currentInstr.iden1 + "!");
                } else {
                    outToText("j " + currentInstr.iden1);
                }
                break;
            case bnz:
                if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    outToMacro(currentMap->loadSymbol("$t8", "__cond1"));
                    outToMacro(currentMap->loadSymbol("$t9", "__cond2"));
                    outToMacro("sub $t8, $t8, $t9");
                    if (condType == "==") {
                        outToMacro("beqz $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == "!=") {
                        outToMacro("bnez $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == ">=") {
                        outToMacro("bgez $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == "<=") {
                        outToMacro("blez $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == ">") {
                        outToMacro("bgtz $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == "<") {
                        outToMacro("bltz $t8, " + currentInstr.iden1 + "!");
                    }
                } else {
                    outToText(currentMap->loadSymbol("$t8", "__cond1"));
                    outToText(currentMap->loadSymbol("$t9", "__cond2"));
                    outToText("sub $t8, $t8, $t9");
                    if (condType == "==") {
                        outToText("beqz $t8, " + currentInstr.iden1);
                    } else if (condType == "!=") {
                        outToText("bnez $t8, " + currentInstr.iden1);
                    } else if (condType == ">=") {
                        outToText("bgez $t8, " + currentInstr.iden1);
                    } else if (condType == "<=") {
                        outToText("blez $t8, " + currentInstr.iden1);
                    } else if (condType == ">") {
                        outToText("bgtz $t8, " + currentInstr.iden1);
                    } else if (condType == "<") {
                        outToText("bltz $t8, " + currentInstr.iden1);
                    }
                }
                break;
            case bz:
                if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    outToMacro(currentMap->loadSymbol("$t8", "__cond1"));
                    outToMacro(currentMap->loadSymbol("$t9", "__cond2"));
                    outToMacro("sub $t8, $t8, $t9");
                    if (condType == "==") {
                        outToMacro("bnez $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == "!=") {
                        outToMacro("beqz $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == ">=") {
                        outToMacro("bltz $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == "<=") {
                        outToMacro("bgtz $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == ">") {
                        outToMacro("blez $t8, " + currentInstr.iden1 + "!");
                    } else if (condType == "<") {
                        outToMacro("bgez $t8, " + currentInstr.iden1 + "!");
                    }
                } else {
                    outToText(currentMap->loadSymbol("$t8", "__cond1"));
                    outToText(currentMap->loadSymbol("$t9", "__cond2"));
                    outToText("sub $t8, $t8, $t9");
                    if (condType == "==") {
                        outToText("bnez $t8, " + currentInstr.iden1);
                    } else if (condType == "!=") {
                        outToText("beqz $t8, " + currentInstr.iden1);
                    } else if (condType == ">=") {
                        outToText("bltz $t8, " + currentInstr.iden1);
                    } else if (condType == "<=") {
                        outToText("bgtz $t8, " + currentInstr.iden1);
                    } else if (condType == ">") {
                        outToText("blez $t8, " + currentInstr.iden1);
                    } else if (condType == "<") {
                        outToText("bgez $t8, " + currentInstr.iden1);
                    }
                }
                break;
            case mac:
                if (currentMap->getPrev()->searchFunc(lastFunc)->addr) {
                    if (currentInstr.iden1 == "scanf") {
                        SymbolType type = currentMap->search(currentInstr.iden2)->type;
                        if (type == vchar || type == achar || type == pchar) {
                            outToMacro("li $v0, 12");
                            outToMacro("syscall");
                            outToMacro(currentMap->saveSymbol("$v0", currentInstr.iden2));
                        } else {
                            outToMacro("li $v0, 5");
                            outToMacro("syscall");
                            outToMacro(currentMap->saveSymbol("$v0", currentInstr.iden2));
                        }
                    } else if (currentInstr.iden1 == "printf_string") {
                        outToMacro("move $a0, $a3");
                        outToMacro("li $v0, 4");
                        outToMacro("syscall");
                        outToMacro("la $a3, slabelb");
                    } else if (currentInstr.iden1 == "printf_expr") {
                        outToMacro(currentMap->loadSymbol("$a0", "__print"));
                        SymbolType type = currentMap->search("__print")->type;
                        if (type == vchar || type == achar || type == pchar) {
                            outToMacro("li $v0, 11");
                            outToMacro("syscall");
                        } else {
                            outToMacro("li $v0, 1");
                            outToMacro("syscall");
                        }
                    } else if (currentInstr.iden1 == "printf_line") {
                        outToMacro("la $a0, slabeln");
                        outToMacro("li $v0, 4");
                        outToMacro("syscall");
                    } else if (currentInstr.iden1 == "printf_char") {
                        outToMacro(currentMap->loadSymbol("$a0", "__print"));
                        outToMacro("li $v0, 11");
                        outToMacro("syscall");
                    }
                } else {
                    if (currentInstr.iden1 == "scanf") {
                        SymbolType type = currentMap->search(currentInstr.iden2)->type;
                        if (type == vchar || type == achar || type == pchar) {
                            outToText("li $v0, 12");
                            outToText("syscall");
                            outToText(currentMap->saveSymbol("$v0", currentInstr.iden2));
                        } else {
                            outToText("li $v0, 5");
                            outToText("syscall");
                            outToText(currentMap->saveSymbol("$v0", currentInstr.iden2));
                        }
                    } else if (currentInstr.iden1 == "printf_string") {
                        outToText("move $a0, $a3");
                        outToText("li $v0, 4");
                        outToText("syscall");
                        outToText("la $a3, slabelb");
                    } else if (currentInstr.iden1 == "printf_expr") {
                        SymbolType type = currentMap->search("__print")->type;
                        outToText(currentMap->loadSymbol("$a0", "__print"));
                        if (type == vchar || type == achar || type == pchar) {
                            outToText("li $v0, 11");
                            outToText("syscall");
                        } else {
                            outToText("li $v0, 1");
                            outToText("syscall");
                        }
                    } else if (currentInstr.iden1 == "printf_line") {
                        outToText("la $a0, slabeln");
                        outToText("li $v0, 4");
                        outToText("syscall");
                    } else if (currentInstr.iden1 == "printf_char") {
                        outToText(currentMap->loadSymbol("$a0", "__print"));
                        outToText("li $v0, 11");
                        outToText("syscall");
                    }
                }
                break;
            default:
                break;
        }
    }
}