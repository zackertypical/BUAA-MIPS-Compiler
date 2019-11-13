//
// Created by 89600 on 2019/10/28.
//

#include "Assert.h"

Assert::Assert(vector<Token> &tokenList, Error &error, vector<string> &middleCode) : error(error), tokenList(tokenList),
                                                                                     middleCode(middleCode) {
    tkPlace = 0;
    symType = illegal;
    symLineNo = 0;
    haveVars = 0;
    labelNo = 0;
    sym = nullptr;
    exprp = "";
    exprStack.emplace_back("#");
}

void Assert::appendMiddleCode(string string1) {
    string1 = middleCode.back() + string1;
    middleCode.pop_back();
    middleCode.push_back(string1);
}

string Assert::parsePolandExpr(string str) {
    int funcs = 0;
    int mode = 0;
    for(int i = 0; i < str.length(); i++) {
        if(!mode && str[i] == '(') {
            mode++;
            int j = i + 1;
            for (; j < str.length(); j++) {
                if (str[j] == '(') {
                    mode++;
                } if (str[j] == ')') {
                    mode--;
                    if (!mode) {
                        break;
                    }
                }
            }

            int end = i - 1;
            int stt = end - 1;
            string strtmp = str.substr(i + 2, j - i - 2);
            for (; str[stt] != ' '; stt--);
            string funcName = str.substr(stt + 1, end - stt - 1);
            str.replace(stt + 1, j - stt, "__func" + to_string(funcs));
            parsePolandFunc(strtmp, funcName, "__func" + to_string(funcs));
            funcs++;
        }
    }
    vector<string> exprList;
    while (str.find(' ') != str.npos) {
        if (str.length() == 1)
            break;
        string strtmp = str.substr(0, str.find(' '));
        str.replace(0, str.find(' ') + 1, "");
        exprList.push_back(strtmp);
    }
    int tmps = 0;
    for (int i = 0; i < exprList.size(); i++) {
        if (exprList.at(i) == "+" || exprList.at(i) == "-" || exprList.at(i) == "*" || exprList.at(i) == "/") {
            newMiddleCode("__tmp" + to_string(tmps) + " = " + exprList.at(i - 2) + " " + exprList.at(i) + " " +
                          exprList.at(i - 1));
            exprList.erase(exprList.begin() + i - 2);
            exprList.erase(exprList.begin() + i - 2);
            i -= 2;
            exprList.at(i) = "__tmp" + to_string(tmps++);
        } else if (exprList.at(i) == "+s") {
            exprList.erase(exprList.begin() + i);
            i -= 1;
        } else if (exprList.at(i) == "-s") {
            newMiddleCode("__tmp" + to_string(tmps) + " = - " + exprList.at(i - 1));
            exprList.erase(exprList.begin() + i - 1);
            i -= 1;
            exprList.at(i) = "__tmp" + to_string(tmps++);
        } else if (exprList.at(i) == "[]") {
            newMiddleCode("__tmp" + to_string(tmps) + " = " + exprList.at(i - 2) + "[" +
                          exprList.at(i - 1) + "]");
            exprList.erase(exprList.begin() + i - 2);
            exprList.erase(exprList.begin() + i - 2);
            i -= 2;
            exprList.at(i) = "__tmp" + to_string(tmps++);
        } else if (exprList.at(i) == "=") {
            newMiddleCode(exprList.at(i - 2) + " = " + exprList.at(i - 1));
        } else if (exprList.at(i) == "[]=") {
            newMiddleCode(exprList.at(i - 3) + "[" + exprList.at(i - 2) + "] = " + exprList.at(i - 1));
        }
    }
    return "__tmp" + to_string(tmps - 1);
}

void Assert::parsePolandFunc(string str, string funcName, string rets) {
    int paras = 0;
    int mode = 0;
    int i = 0;
    for(i = 0; i < str.length(); i++) {
        if (str[i] == '(') {
            mode++;
        } if (str[i] == ')') {
            mode--;
        }
        if (!mode && str[i] == ',') {
            string strtmp = str.substr(0, i);
            str.replace(0, i + 2, "");
            parsePolandExpr("__para" + to_string(paras) + " " + strtmp + "= ");
            newMiddleCode("push __para" + to_string(paras++));
        }
    }
    if (i != 0) {
        parsePolandExpr("__para" + to_string(paras) + " " + str + "= ");
        newMiddleCode("push __para" + to_string(paras++));
    }
    newMiddleCode("call " + funcName);
    newMiddleCode(rets + " = RET");
}

void Assert::newMiddleCode(string string1) {
    middleCode.push_back(string1);
}

void Assert::getSym() {
    if (tokenList.size() > tkPlace) {
        sym = &tokenList.at(tkPlace++);
        symName = sym->getName();
        symType = sym->getType();
        symLineNo = sym->getLineNo();
    } else {
        return;
    }
}

int Assert::symRedefine() {
    for (Token tk : symbolSet) {
        if (tk.getName() == symName) {
            error.addError(symLineNo, 'b');
            return 0;
        }
    }
    return 1;
}

Token Assert::symFind(string idenName) {
    for (Token tk : symbolSet) {
        if (tk.getName() == idenName) {
            return tk;
        }
    }
    error.addError(symLineNo, 'c');
    return Token(illegal, symLineNo);
}

void Assert::parse() {
    getSym();
    if (symType == consttk) {
        parse_const();
    }

    if (symType == inttk || symType == chartk) {
        parse_var();
    }
    while (symType == inttk || symType == chartk || symType == voidtk) {
        if (symType == inttk || symType == chartk) {
            parse_func_def();
        } else {
            getSym();
            if (symType == maintk) {
                getSym();
                newMiddleCode("void main()");
                parse_main();
                DEBUG_OUT("程序")
            } else {
                parse_nfunc_def();
            }
        }
    }
}

void Assert::printMiddleCode(string filename) {
    ofstream of;
    of.open(filename, ios::out | ios::trunc);
    for (string elm : middleCode) {
        of << elm << endl;
    }
}

void Assert::parse_const() {
    while (symType == consttk) {
        getSym();
        parse_const_def();
    }
    DEBUG_OUT("常量说明")
}

void Assert::parse_var() {
    while (symType == inttk || symType == chartk) {
        if (is_var()) {
            haveVars = 1;
            parse_var_def();
        } else {
            break;
        }
    }
    if (haveVars) {
        DEBUG_OUT("变量说明")
    }
    haveVars = 1;
}

void Assert::parse_func_def() {
    if (symType == inttk || symType == chartk) {
        int typp = 2;
        if (symType == inttk) {
            typp = 1;
        }
        getSym();
        string funcName = parse_func_iden(typp);
        returnMap.insert(make_pair(funcName, vector<int>()));
        returnMap[funcName].push_back(typp);
        int symPlace = symbolSet.size();

        if (symType == lparent) {
            DEBUG_OUT("声明头部")
            getSym();
            parse_para_list(funcName);
            if (symType == rparent) {
                getSym();
            } else {
                error.addError(symLineNo, 'l');
                getSym();
            }
        }

        if (symType == lbrace) {
            getSym();
            parse_complex();
            if (symType == rbrace) {
                if (returnMap[funcName].size() == 1) {
                    error.addError(symLineNo, 'h');
                }
                getSym();
                DEBUG_OUT("有返回值函数定义")
                symbolSet.erase(symbolSet.begin() + symPlace, symbolSet.end());
            }
        }
    }
}

void Assert::parse_main() {
    returnMap.insert(make_pair("main", vector<int>()));
    returnMap["main"].push_back(0);
    if (symType == lparent) {
        getSym();
        if (symType == rparent) {
            getSym();
            if (symType == lbrace) {
                getSym();
                parse_complex();
                if (symType == rbrace) {
                    getSym();
                    DEBUG_OUT("主函数")
                }
            }
        } else {
            error.addError(symLineNo, 'l');
            getSym();
            if (symType == lbrace) {
                getSym();
                parse_complex();
                if (symType == rbrace) {
                    getSym();
                    DEBUG_OUT("主函数")
                }
            }
        }
    }
}

void Assert::parse_nfunc_def() {
    string funcName = parse_func_iden(0);
    returnMap.insert(make_pair(funcName, vector<int>()));
    returnMap[funcName].push_back(0);
    int symPlace = symbolSet.size();
    if (symType == lparent) {
        getSym();
        parse_para_list(funcName);
        if (symType == rparent) {
            getSym();
        } else {
            error.addError(symLineNo, 'l');
            getSym();
        }
    }

    if (symType == lbrace) {
        getSym();
        parse_complex();
        if (symType == rbrace) {
            getSym();
            DEBUG_OUT("无返回值函数定义")
            symbolSet.erase(symbolSet.begin() + symPlace, symbolSet.end());
        }

    }
}

void Assert::parse_const_def() {
    if (symType == inttk || symType == chartk) {
        if (symType == inttk) {
            getSym();
            parse_const_iden(0);
            if (symType == assign) {
                getSym();
                parse_const_num();
            }
            while (symType == comma) {
                getSym();
                parse_const_iden(0);
                if (symType == assign) {
                    getSym();
                    parse_const_num();
                }
            }
            DEBUG_OUT("常量定义")
            if (symType != semicn) {
                error.addError(symLineNo - 1, 'k');
            } else {
                getSym();
            }
        } else {
            getSym();
            parse_const_iden(1);
            if (symType == assign) {
                getSym();
                parse_const_char();
            }
            while (symType == comma) {
                getSym();
                parse_const_iden(1);
                if (symType == assign) {
                    getSym();
                    parse_const_char();
                }
            }
            DEBUG_OUT("常量定义")
            if (symType != semicn) {
                error.addError(symLineNo - 1, 'k');
            } else {
                getSym();
            }
        }
    }
}

bool Assert::is_var() {
    if (tokenList.size() > tkPlace + 2) {
        return !(tokenList.at(tkPlace + 1).getType() == lparent);
    } else {
        return false;
    }
}

void Assert::parse_var_def() {
    int typp = 1;
    if (symType == inttk) {
        typp = 0;
    }
    getSym();
    parse_var_iden(typp);

    if (symType == lbrack) {
        getSym();
        if (symType == intcon) {
            appendMiddleCode("[" + symName + "]");
            getSym();
        } else {
            error.addError(symLineNo, 'i');
        }
        if (symType == rbrack) {
            getSym();
        } else {
            error.addError(symLineNo, 'm');
        }
    }

    while (symType == comma) {
        getSym();
        parse_var_iden(typp);

        if (symType == lbrack) {
            getSym();
            if (symType == intcon) {
                appendMiddleCode("[" + symName + "]");
                getSym();
            } else {
                error.addError(symLineNo, 'i');
            }
            if (symType == rbrack) {
                getSym();
            } else {
                error.addError(symLineNo, 'm');
            }
        }
    }
    DEBUG_OUT("变量定义")
    if (symType != semicn) {
        error.addError(symLineNo - 1, 'k');
    } else {
        getSym();
    }
}

void Assert::parse_para_list(string funcName) {
    while (symType == inttk || symType == chartk) {
        int typp = 1;
        if (symType == inttk) {
            typp = 0;
        }
        getSym();
        parse_para_iden(typp);
        functionMap[funcName].push_back(typp);
        while (symType == comma) {
            getSym();
            typp = 1;
            if (symType == inttk) {
                typp = 0;
            }
            getSym();
            parse_para_iden(typp);
            functionMap[funcName].push_back(typp);
        }
    }
    DEBUG_OUT("参数表")
}

void Assert::parse_const_iden(int i) {
    if (symRedefine()) {
        //symbolSet.emplace_back(symType, symName, symLineNo, i ? constchar : constint);
    }
    if (i) {
        newMiddleCode("const char " + symName + " = ");
    } else {
        newMiddleCode("const int " + symName + " = ");
    }
    getSym();
}

void Assert::parse_var_iden(int i) {
    if (symRedefine()) {
        //symbolSet.emplace_back(symType, symName, symLineNo, i ? varchar : varint);
    }
    if (i) {
        newMiddleCode("var char " + symName);
    } else {
        newMiddleCode("var int " + symName);
    }
    getSym();
}

void Assert::parse_para_iden(int i) {
    if (symRedefine()) {
        symbolSet.emplace_back(symType, symName, symLineNo, i ? varchar : varint);
        if (i) {
            newMiddleCode("para char " + symName);
        } else {
            newMiddleCode("para int " + symName);
        }
    }
    getSym();
}

string Assert::parse_func_iden(int i) {
    if (symRedefine()) {
        if (i) {
            symbolSet.emplace_back(symType, symName, symLineNo, (i - 1) ? funcchar : funcint);
            if (i - 1) {
                newMiddleCode("char " + symName + "()");
            } else {
                newMiddleCode("int " + symName + "()");
            }
        } else {
            symbolSet.emplace_back(symType, symName, symLineNo, funcvoid);
            newMiddleCode("void " + symName + "()");
        }
    }
    string funcName = symName;
    functionMap.insert(make_pair(symName, vector<int>()));
    lastFunc = symName;
    getSym();
    return funcName;
}

void Assert::parse_num() {
    if (symType == pluss || symType == minu) {
        exprp += symName;
        getSym();
    }
    exprp += symName + " ";
    getSym();
    if (symType != intcon) {
        //error
    }
}

void Assert::parse_char() {
    getSym();
    if (symType != charcon) {
        //error
    }
}

void Assert::parse_const_char() {
    if (symType != charcon) {
        error.addError(symLineNo, 'o');
    } else {
        appendMiddleCode(to_string((int)symName[0]));
    }
    getSym();
}

void Assert::parse_const_num() {
    if (symType == pluss || symType == minu) {
        appendMiddleCode(symName);
        getSym();
    }
    if (symType != intcon) {
        error.addError(symLineNo, 'o');
    } else {
        appendMiddleCode(symName);
    }
    getSym();
}

void Assert::parse_complex() {
    if (symType == consttk) {
        parse_const();
    }

    if ((symType == inttk) || (symType == chartk)) {
        parse_var();
    }

    parse_sents();

    DEBUG_OUT("复合语句")
}

void Assert::parse_sents() {
    while (symType == iftk || symType == dotk || symType == whiletk || symType == idenfr || symType == scanftk ||
           symType == printftk || symType == returntk || symType == fortk || symType == semicn || symType == lbrace) {
        parse_sent();
    }
    DEBUG_OUT("语句列")
}

void Assert::parse_sent() {
    if (symType == iftk) {
        getSym();
        if (symType == lparent) {
            getSym();
            parse_cond();
            int label = labelNo++;
            newMiddleCode("BZ label_" + to_string(label));
            if (symType == rparent) {
                getSym();
                parse_sent();
                int labele = labelNo++;
                newMiddleCode("GOTO label_" + to_string(labele));
                newMiddleCode("label_" + to_string(label) + ":");
                if (symType == elsetk) {
                    getSym();
                    parse_sent();
                }
                newMiddleCode("label_" + to_string(labele) + ":");
                DEBUG_OUT("条件语句")
            } else {
                error.addError(symLineNo, 'l');
                getSym();
            }
        }
    } else if (symType == whiletk) {
        parse_while();
    } else if (symType == dotk) {
        parse_dowhile();
    } else if (symType == fortk) {
        parse_for();
    } else if (symType == scanftk) {
        parse_scanf();
        if (symType != semicn) {
            error.addError(symLineNo - 1, 'k');
        } else {
            getSym();
        }
    } else if (symType == printftk) {
        parse_printf();
        if (symType != semicn) {
            error.addError(symLineNo - 1, 'k');
        } else {
            getSym();
        }
    } else if (symType == returntk) {
        parse_return();
        if (symType != semicn) {
            error.addError(symLineNo - 1, 'k');
        } else {
            getSym();
        }
    } else if (symType == idenfr) {
        string zn = symName;
        parse_iden();

        if (symType == assign || symType == lbrack) {
            parse_assign(zn);
            parsePolandExpr(exprps.back());
        } else if (symType == lparent) {
            parse_func(zn, 1);
            exprps.back() = "__ " + exprps.back() + "= ";
            parsePolandExpr(exprps.back());
        }
        if (symType != semicn) {
            error.addError(symLineNo - 1, 'k');
        } else {
            getSym();
        }
    } else if (symType == lbrace) {
        getSym();
        parse_sents();
        if (symType == rbrace) {
            getSym();
        }
    } else {
        getSym();
    }
    DEBUG_OUT("语句")
}

void Assert::parse_cond() {
    int left = parse_expr();
    if (left) {
        error.addError(symLineNo, 'f');
    }
    exprps.back() = "__cond1 " + exprps.back() + "= ";
    parsePolandExpr(exprps.back());
    if (symType == lss || symType == leq || symType == gre || symType == geq || symType == eql || symType == neq) {
        string tmpf = symName;
        getSym();
        int right = parse_expr();
        if (right) {
            error.addError(symLineNo, 'f');
        }
        exprps.back() = "__cond2 " + exprps.back() + "= ";
        parsePolandExpr(exprps.back());
        newMiddleCode("__cond1 " + tmpf + " __cond2");
    } else {
        newMiddleCode("__cond1");
    }
    DEBUG_OUT("条件")
}

void Assert::parse_while() {
    getSym();
    if (symType == lparent) {
        getSym();
        int label = labelNo++;
        int labele = labelNo++;
        newMiddleCode("label_" + to_string(label) + ":");
        parse_cond();
        newMiddleCode("BZ label_" + to_string(labele));
        if (symType == rparent) {
            getSym();
            parse_sent();
            newMiddleCode("GOTO label_" + to_string(label));
            newMiddleCode("label_" + to_string(labele) + ":");
            DEBUG_OUT("while循环语句")
        } else {
            error.addError(symLineNo, 'l');
            getSym();
        }
    }
}

void Assert::parse_dowhile() {
    getSym();
    int label = labelNo++;
    newMiddleCode("label_" + to_string(label) + ":");
    parse_sent();
    if (symType == whiletk) {
        getSym();
        if (symType == lparent) {
            getSym();
            parse_cond();
            newMiddleCode("BNZ label_" + to_string(label));
            if (symType == rparent) {
                getSym();
                DEBUG_OUT("dowhile循环语句")
            } else {
                error.addError(symLineNo, 'l');
                getSym();
            }
        }
    } else {
        error.addError(symLineNo, 'n');
        if (symType == lparent) {
            getSym();
            parse_cond();
            if (symType == rparent) {
                getSym();
                DEBUG_OUT("dowhile循环语句（没while）")
            } else {
                error.addError(symLineNo, 'l');
                getSym();
            }
        }
    }
}

void Assert::parse_for() {
    getSym();
    if (symType == lparent) {
        getSym();
        parse_iden();
        if (symType == assign) {
            getSym();
            parse_expr();
            if (symType != semicn) {
                error.addError(symLineNo, 'k');
            }
            getSym();
            int label = labelNo++;
            int labele = labelNo++;
            newMiddleCode("label_" + to_string(labele) + ":");
            parse_cond();
            newMiddleCode("BZ label_" + to_string(label));
            if (symType != semicn) {
                error.addError(symLineNo, 'k');
            }
            getSym();
            string tsym = symName;
            parse_iden();
            if (symType == assign) {
                getSym();
                parse_iden();
                if (symType == pluss || symType == minu) {
                    string top = symName;
                    getSym();
                    if (symType == intcon) {
                        string tlength = symName;
                        getSym();
                        if (symType == rparent) {
                            getSym();
                            parse_sent();
                            DEBUG_OUT("for循环语句")
                            newMiddleCode(tsym + " = " + tsym + " " + top + " " + tlength);
                            newMiddleCode("BZ label_" + to_string(labele));
                            newMiddleCode("label_" + to_string(label) + ":");
                        } else {
                            error.addError(symLineNo, 'l');
                        }
                    }
                }
            }
        }
    }
}

void Assert::parse_scanf() {
    getSym();
    if (symType == lparent) {
        getSym();
        newMiddleCode("push " + symName);
        parse_iden();
        while (symType == comma) {
            getSym();
            newMiddleCode("push " + symName);
            parse_iden();
        }
        if (symType == rparent) {
            getSym();
            newMiddleCode("call scanf");
        } else {
            error.addError(symLineNo, 'l');
            getSym();
        }
    }
}

void Assert::parse_printf() {
    getSym();
    if (symType == lparent) {
        getSym();
        if (symType == strcon) {
            newMiddleCode("push \"" + symName + "\"");
            parse_str();
        } else {
            parse_expr();
            exprps.back() = "__print " + exprps.back() + "= ";
            parsePolandExpr(exprps.back());
            newMiddleCode("push __print");
        }
        if (symType == rparent) {
            getSym();
            DEBUG_OUT("printf语句")
        } else if (symType == comma) {
            getSym();
            parse_expr();
            exprps.back() = "__print " + exprps.back() + "= ";
            parsePolandExpr(exprps.back());
            newMiddleCode("push __print");
            if (symType == rparent) {
                getSym();
            } else {
                error.addError(symLineNo, 'l');
            }
            newMiddleCode("call printf");
            DEBUG_OUT("printf语句")
        } else {
            error.addError(symLineNo, 'l');
        }
    }

}

void Assert::parse_return() {
    getSym();
    if (symType == lparent) {
        getSym();
        int type = parse_expr() + 1;
        if (returnMap[lastFunc].at(0)) {
            if (returnMap[lastFunc].at(0) != type) {
                error.addError(symLineNo, 'h');
            }
        } else {
            error.addError(symLineNo, 'g');
        }
        if (symType == rparent) {
            getSym();
            exprps.back() = "__retval " + exprps.back() + "= ";
            parsePolandExpr(exprps.back());
            newMiddleCode("ret __retval");
        } else {
            error.addError(symLineNo, 'l');
            getSym();
        }
    } else {
        if (returnMap[lastFunc].at(0)) {
            error.addError(symLineNo, 'h');
        } else {
            newMiddleCode("ret ");
        }
    }

    returnMap[lastFunc].push_back(1);
}

void Assert::parse_assign(string idenName) {
    if (symType == lbrack) {
        getSym();
        int type = parse_expr();
        if (type) {
            error.addError(symLineNo, 'i');
        }
        if (symType == rbrack) {
            getSym();
            getSym();
            parse_expr();
            Token tk = symFind(idenName);
            if (tk.getAttribute() == constint || tk.getAttribute() == constchar) {
                error.addError(symLineNo, 'j');
            } else {
                exprps.back() = idenName + " " + exprps.at(exprps.size() - 2) + exprps.back() + "[]= ";
            }
        } else {
            error.addError(symLineNo, 'm');
        }
    } else {
        getSym();
        parse_expr();
        Token tk = symFind(idenName);
        if (tk.getAttribute() == constint || tk.getAttribute() == constchar) {
            error.addError(symLineNo, 'j');
        } else {
            exprps.back() = idenName + " " + exprps.back() + "= ";
        }
    }
}

void Assert::parse_func(string funcName) {
    getSym();
    exprp += "( ";
    exprStack.emplace_back("(");
    parse_cal_list(funcName);
    if (symType == rparent) {
        exprp += ") ";
        getSym();
    } else {
        error.addError(symLineNo, 'l');
        getSym();
    }
}

void Assert::parse_func(string funcName, int ic) {
    getSym();
    exprp += funcName + " ( ";
    exprStack.emplace_back("(");
    parse_cal_list(funcName);
    if (symType == rparent) {
        exprp += ") ";
        if (exprStack.back() == "#") {
            exprps.push_back(exprp);
            exprp = "";
        }
        getSym();
    } else {
        error.addError(symLineNo, 'l');
        getSym();
    }
}

void Assert::parse_str() {
    getSym();
}

void Assert::parse_cal_list(string basicString) {
    int num = 0;
    if (symType != rparent) {
        if (symType == semicn) {
            error.addError(symLineNo, 'l');
        } else if (symType == rbrack || symType == rbrace) {
            error.addError(symLineNo, 'l');
            getSym();
        } else {
            int type = parse_expr();
            if (functionMap[basicString].size() < num) {
                error.addError(symLineNo, 'd');
            } else if (functionMap[basicString].at(num) != type) {
                error.addError(symLineNo, 'e');
            }
            while (symType == comma) {
                num++;
                exprp += ", ";
                getSym();
                int type2 = parse_expr();
                if (functionMap[basicString].size() < num) {
                    error.addError(symLineNo, 'd');
                } else if (functionMap[basicString].at(num) != type2) {
                    error.addError(symLineNo, 'e');
                }
            }
        }
        num++;
        if (functionMap[basicString].size() != num) {
            error.addError(symLineNo, 'd');
        }
    }
    exprStack.pop_back();
    DEBUG_OUT("值参数表")
}


Attrs Assert::parse_iden() {
    Token tk = symFind(symName);
    getSym();
    return tk.getAttribute();
}

int Assert::parse_expr() {
    int type1 = 0;
    if (symType == pluss || symType == minu) {
        while (exprStack.back() == "+" || exprStack.back() == "-" || exprStack.back() == "*" ||
               exprStack.back() == "/" || exprStack.back() == "[]") {
            exprp += exprStack.back() + " ";
            exprStack.pop_back();
        }
        exprStack.push_back(symName + "s");
        getSym();
    }
    type1 = parse_term();
    int type2 = 2;
    while (symType == pluss || symType == minu) {
        while (exprStack.back() == "+" || exprStack.back() == "-" || exprStack.back() == "*" ||
               exprStack.back() == "/" || exprStack.back() == "[]") {
            exprp += exprStack.back() + " ";
            exprStack.pop_back();
        }
        exprStack.push_back(symName);
        getSym();
        type2 = parse_term();
    }
    while (exprStack.back() != "#" && exprStack.back() != "(") {
        exprp += exprStack.back() + " ";
        exprStack.pop_back();
    }
    if (exprStack.back() == "#") {
        exprps.push_back(exprp);
        exprp = "";
    }
    DEBUG_OUT("表达式")
    if (type2 == 2) {
        return type1;
    }
    return 0;
}

int Assert::parse_term() {
    int type1 = parse_factor();
    int type2 = 2;
    while (symType == mult || symType == divv) {
        while (exprStack.back() == "*" || exprStack.back() == "/" || exprStack.back() == "[]") {
            exprp += exprStack.back() + " ";
            exprStack.pop_back();
        }
        exprStack.push_back(symName);
        getSym();
        type2 = parse_factor();
    }
    DEBUG_OUT("项")
    if (type2 == 2) {
        return type1;
    }
    return 0;
}

int Assert::parse_factor() {
    if (symType == idenfr) {
        string zn = symName;
        exprp += zn + " ";
        getSym();
        Token tk = symFind(zn);
        if (symType == lbrack) {
            exprStack.emplace_back("[]");
            exprStack.emplace_back("(");
            getSym();
            int type = parse_expr();
            if (type) {
                error.addError(symLineNo, 'i');
            }
            if (symType == rbrack) {
                exprStack.pop_back();
                getSym();
            } else {
                error.addError(symLineNo, 'm');
            }
        } else if (symType == lparent) {
            parse_func(zn);
            if (tk.getAttribute() == funcchar) {
                DEBUG_OUT("因子")
                return 1;
            }
        } else {
            if (tk.getAttribute() == constchar || tk.getAttribute() == varchar) {
                DEBUG_OUT("因子")
                return 1;
            }
        }
    } else if (symType == lparent) {
        exprStack.emplace_back("(");
        getSym();
        parse_expr();
        if (symType == rparent) {
            while (exprStack.back() != "(") {
                exprp += exprStack.back() + " ";
                exprStack.pop_back();
            }
            exprStack.pop_back();
            getSym();
        } else {
            error.addError(symLineNo, 'l');
            getSym();
        }
    } else if (symType == charcon) {
        exprp += to_string((int)symName[0]) + " ";
        getSym();
        DEBUG_OUT("因子")
        return 1;
    } else if (symType == pluss || symType == minu || symType == intcon) {
        parse_num();
    }

    DEBUG_OUT("因子")
    return 0;
}



