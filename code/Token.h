//
// Created by 89600 on 2019/10/28.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include "basics.h"

enum Words {
    illegal,
    idenfr,
    intcon,
    charcon,
    strcon,
    consttk,
    inttk,
    chartk,
    voidtk,
    maintk,
    iftk,
    elsetk,
    dotk,
    whiletk,
    fortk,
    scanftk,
    printftk,
    returntk,
    pluss,
    minu,
    mult,
    divv,
    lss,
    leq,
    gre,
    geq,
    eql,
    neq,
    assign,
    semicn,
    comma,
    lparent,
    rparent,
    lbrack,
    rbrack,
    lbrace,
    rbrace,
    space,
    eof
};

enum Attrs{
    null,
    constint,
    constchar,
    varint,
    varchar,
    funcint,
    funcchar,
    funcvoid
};

class Token {
private:
    Words type;
    string name;
    Attrs attribute;
    int lineNo;

public:
    Token(Words _type, string _name, int _lineNo, Attrs _attribute = null);
    Token(Words _type, int _lineNo);
    Words getType();
    int getLineNo();
    string getName();
    Attrs getAttribute();
    string getTypeName();
};


#endif //COMPILER_TOKEN_H
