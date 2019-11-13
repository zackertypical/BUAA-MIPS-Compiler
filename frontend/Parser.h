//
// Created by 89600 on 2019/10/28.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "basics.h"
#include "Token.h"
#include "Lexer.h"
#include "Assert.h"

class Parser {
private:
    Lexer lexer;
    Error error;
    Assert assert;
    vector<Token> tokenList;
    vector<string> &middleCode;
public:
    Parser(string filename, vector<string> &middleCode) : lexer(filename, tokenList, error), assert(tokenList, error, middleCode), middleCode(middleCode) {}
    void parse();
    void parse(string filename);
};


#endif //COMPILER_PARSER_H
