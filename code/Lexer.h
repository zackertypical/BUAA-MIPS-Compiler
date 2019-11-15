//
// Created by 89600 on 2019/10/28.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "basics.h"
#include "Token.h"
#include "Error.h"

class Lexer {
private:
    ifstream inputFile;
    vector<Token> &tokenList;
    Error &error;
    int lineNo;
    char c;
    void parseString();
    void parseChar();
    void parseInt();
    void parseIden();
    void getChar();
    void getSym();
public:
    Lexer(string filename, vector<Token> &tokenList, Error &error);
    void parse();
};


#endif //COMPILER_LEXER_H
