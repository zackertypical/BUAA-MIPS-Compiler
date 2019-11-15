//
// Created by 89600 on 2019/10/28.
//

#include "Token.h"

Token::Token(Words _type, string _name, int _lineNo, Attrs _attribute) {
    type = _type;
    name = _name;
    lineNo = _lineNo;
    attribute = _attribute;
}

Token::Token(Words _type, int _lineNo) {
    type = _type;
    lineNo = _lineNo;
    attribute = null;
    switch (type) {
        case pluss:
            name = "+";
            break;
        case minu:
            name = "-";
            break;
        case mult:
            name = "*";
            break;
        case divv:
            name = "/";
            break;
        case lss:
            name = "<";
            break;
        case leq:
            name = "<=";
            break;
        case gre:
            name = ">";
            break;
        case geq:
            name = ">=";
            break;
        case eql:
            name = "==";
            break;
        case neq:
            name = "!=";
            break;
        case assign:
            name = "=";
            break;
        case semicn:
            name = ";";
            break;
        case comma:
            name = ",";
            break;
        case lparent:
            name = "(";
            break;
        case rparent:
            name = ")";
            break;
        case lbrack:
            name = "[";
            break;
        case rbrack:
            name = "]";
            break;
        case lbrace:
            name = "{";
            break;
        case rbrace:
            name = "}";
            break;
        default:
            DEBUG_OUT("怎么会进到这里来的")
            name = "#ERROR#";
            break;
    }
}

Words Token::getType() {
    return type;
}

int Token::getLineNo() {
    return lineNo;
}

string Token::getName() {
    return name;
}

Attrs Token::getAttribute() {
    return attribute;
}

string Token::getTypeName() {
    switch (type) {
        case illegal:
            return "ILLEGAL";
        case idenfr:
            return "IDENFR";
        case intcon:
            return "INTCON";
        case charcon:
            return "CHARCON";
        case strcon:
            return "STRCON";
        case consttk:
            return "CONSTTK";
        case inttk:
            return "INTTK";
        case chartk:
            return "CHARTK";
        case voidtk:
            return "VOIDTK";
        case maintk:
            return "MAINTK";
        case iftk:
            return "IFTK";
        case elsetk:
            return "ELSETK";
        case dotk:
            return "DOTK";
        case whiletk:
            return "WHILETK";
        case fortk:
            return "FORTK";
        case scanftk:
            return "SCANFTK";
        case printftk:
            return "PRINTFTK";
        case returntk:
            return "RETURNTK";
        case pluss:
            return "PLUS";
        case minu:
            return "MINU";
        case mult:
            return "MULT";
        case divv:
            return "DIV";
        case lss:
            return "LSS";
        case leq:
            return "LEQ";
        case gre:
            return "GRE";
        case geq:
            return "GEQ";
        case eql:
            return "EQL";
        case neq:
            return "NEQ";
        case assign:
            return "ASSIGN";
        case semicn:
            return "SEMICN";
        case comma:
            return "COMMA";
        case lparent:
            return "LPARENT";
        case rparent:
            return "RPARENT";
        case lbrack:
            return "LBRACK";
        case rbrack:
            return "RBRACK";
        case lbrace:
            return "LBRACE";
        case rbrace:
            return "RBRACE";
        case space:
            return "SPACE";
        case eof:
            return "EOF";
        default:
            return "ILLEGAL";
    }
}
