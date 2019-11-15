//
// Created by 89600 on 2019/10/28.
//

#include "Lexer.h"

Lexer::Lexer(string filename, vector<Token> &tokenList, Error &error) : tokenList(tokenList), error(error) {
    inputFile.open(filename, ios::binary);
    lineNo = 1;
    c = 0;
}


void Lexer::parse() {
    while(c != EOF){
        getSym();
    }
    inputFile.close();
	tokenList.emplace_back(illegal, lineNo);
}

void Lexer::getChar() {
    c = inputFile.get();
    if (c == '\n') {
        lineNo++;
    }
}

void Lexer::getSym() {
    if (c == EOF) {
        return;
    }

    if (c == '\"') {
        parseString();
    } else if (c == '\'') {
        parseChar();
    } else if (isdigit(c)) {
        parseInt();
    } else if (isalpha(c) || c == '_') {
        parseIden();
    } else if (c == '+') {
        getChar();
        tokenList.emplace_back(pluss, lineNo);
    } else if (c == '-') {
        getChar();
        tokenList.emplace_back(minu, lineNo);
    } else if (c == '*') {
        getChar();
        tokenList.emplace_back(mult, lineNo);
    } else if (c == '/') {
        getChar();
        tokenList.emplace_back(divv, lineNo);
    } else if (c == ';') {
        getChar();
        tokenList.emplace_back(semicn, lineNo);
    } else if (c == ',') {
        getChar();
        tokenList.emplace_back(comma, lineNo);
    } else if (c == '(') {
        getChar();
        tokenList.emplace_back(lparent, lineNo);
    } else if (c == ')') {
        getChar();
        tokenList.emplace_back(rparent, lineNo);
    } else if (c == '[') {
        getChar();
        tokenList.emplace_back(lbrack, lineNo);
    } else if (c == ']') {
        getChar();
        tokenList.emplace_back(rbrack, lineNo);
    } else if (c == '{') {
        getChar();
        tokenList.emplace_back(lbrace, lineNo);
    } else if (c == '}') {
        getChar();
        tokenList.emplace_back(rbrace, lineNo);
    } else if (c == '<') {
        getChar();
        if (c == '=') {
            getChar();
            tokenList.emplace_back(leq, lineNo);
        } else {
            tokenList.emplace_back(lss, lineNo);
        }
    } else if (c == '>') {
        getChar();
        if (c == '=') {
            getChar();
            tokenList.emplace_back(geq, lineNo);
        } else {
            tokenList.emplace_back(gre, lineNo);
        }
    }  else if (c == '=') {
        getChar();
        if (c == '=') {
            getChar();
            tokenList.emplace_back(eql, lineNo);
        } else {
            tokenList.emplace_back(assign, lineNo);
        }
    }  else if (c == '!') {
        getChar();
        if (c == '=') {
            getChar();
            tokenList.emplace_back(neq, lineNo);
        } else {
            error.addError(lineNo, 'a');

        }
    } else if (isspace(c) || c == '\0') {
        getChar();
    } else {
        error.addError(lineNo, 'a');
        getChar();
    }
}

void Lexer::parseString() {
    char strbuf[LINE_MAX] = {0};
    int cc = 0;
    getChar();
    if (c == '\"') {
        // error
        error.addError(lineNo, 'a');
        getChar();
    }
    while (c != '\"') {
        if (c < 32 || c > 126) {
            // error
            if (c == '\n') {
                error.addError(lineNo - 1, 'a');
                tokenList.emplace_back(illegal, lineNo);
                return;
            }
            error.addError(lineNo, 'a');
            if (c == EOF) {
                tokenList.emplace_back(illegal, lineNo);
                return;
            }
        }
        strbuf[cc++] = c;
        getChar();
    }
    tokenList.emplace_back(strcon, strbuf, lineNo);
    getChar();
}

void Lexer::parseChar() {
    char strbuf[LINE_MAX] = {0};
    int cc = 0;
    getChar();

    while (c != '\'') {
        if (c != '+' && c != '-' && c != '*' && c != '/' && c != '_' && !isalnum(c)) {
            if (c == '\n') {
                error.addError(lineNo - 1, 'a');
                tokenList.emplace_back(illegal, lineNo);
                return;
            }
            error.addError(lineNo, 'a');
            if (c == EOF) {
                tokenList.emplace_back(illegal, lineNo);
                return;
            }
        }
        strbuf[cc++] = c;
        getChar();
    }

    if (cc == 1) {
        tokenList.emplace_back(charcon, strbuf, lineNo);
        getChar();
        return;
    }

    //error
    error.addError(lineNo, 'a');
    tokenList.emplace_back(illegal, lineNo);
    getChar();
}

void Lexer::parseInt() {
    char strbuf[LINE_MAX] = {0};
    int cc = 0;
    while (isdigit(c)) {
        strbuf[cc++] = c;
        getChar();
    }
    tokenList.emplace_back(intcon, strbuf, lineNo);
}

void Lexer::parseIden() {
    char strbuf[LINE_MAX] = {0};
    int cc = 0;
    while (isalnum(c) || c == '_') {
        strbuf[cc++] = c;
        getChar();
    }
    string str(strbuf);
    if (str == "const") {
        Token tk(consttk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "int") {
        Token tk(inttk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "char") {
        Token tk(chartk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "void") {
        Token tk(voidtk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "main") {
        Token tk(maintk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "if") {
        Token tk(iftk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "else") {
        Token tk(elsetk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "do") {
        Token tk(dotk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "while") {
        Token tk(whiletk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "for") {
        Token tk(fortk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "scanf") {
        Token tk(scanftk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "printf") {
        Token tk(printftk, str, lineNo);
        tokenList.push_back(tk);
    } else if (str == "return") {
        Token tk(returntk, str, lineNo);
        tokenList.push_back(tk);
    } else {
        Token tk(idenfr, str, lineNo);
        tokenList.push_back(tk);
    }
}