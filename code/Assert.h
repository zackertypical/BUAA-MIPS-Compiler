//
// Created by 89600 on 2019/10/28.
//

#ifndef COMPILER_ASSERT_H
#define COMPILER_ASSERT_H

#include "basics.h"
#include "Error.h"
#include "Token.h"

class Assert {
private:
    int tkPlace;
    int labelNo;
    string lastFunc;
    string exprp;
    vector<string> exprStack;
    vector<string> exprps;
    Token *sym;
    Words symType;
    string symName;
    int symLineNo;
    int haveVars;
    Error &error;
    vector<Token> &tokenList;
    vector<Token> symbolSet;
    vector<string> &middleCode;
    map<string, vector<int>> functionMap;
    map<string, vector<int>> returnMap;
public:
    Assert(vector<Token> &tokenList, Error &error, vector<string> &middleCode);

    void getSym();

    int symRedefine();

    void parse_const();

    void parse_var();

    void parse_func_def();

    void parse_main();

    void parse_nfunc_def();

    void parse_const_def();

    bool is_var();

    void parse_var_def();

    void parse_para_list(string funcName);

    void parse_const_iden(int i);

    void parse_complex();

    string parse_func_iden(int i);

    void parse_num();

    void parse_char();

    void parse_const_char();

    void parse_const_num();

    void parse_var_iden(int i);

    void parse_sents();

    void parse_sent();

    void parse_cond();

    void parse_while();

    void parse_dowhile();

    void parse_for();

    void parse_scanf();

    void parse_printf();

    void parse_return();

    void parse_assign(string idenName);

    void parse_func(string funcName);

    int parse_expr();

    Attrs parse_iden();

    void parse_str();

    Token symFind(string idenName);

    void parse_cal_list(string basicString);

    int parse_term();

    int parse_term(string sign);

    int parse_factor();

    void parse();

    void appendMiddleCode(string string1);

    void newMiddleCode(string string1);

    void parse_para_iden(int i);

    void parse_func(string funcName, int ic);

    string parsePolandExpr(string str);

    void parsePolandFunc(string str, string funcName, string rets);

    void printMiddleCode(string filename);
};


#endif //COMPILER_ASSERT_H
