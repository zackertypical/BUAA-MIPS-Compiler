//
// Created by 89600 on 2019/10/28.
//

#include "Parser.h"

//编译器前端，输出中间代码

void Parser::parse() {
    lexer.parse();
    assert.parse();
    assert.printMiddleCode("middle_code.txt");
    error.printError("error.txt");
}

void Parser::parse(string filename) {
    lexer.parse();
    ofstream outputFile;
    outputFile.open(filename, ios::out | ios::trunc);
    for (Token tk : tokenList) {
        outputFile << tk.getTypeName() << " " << tk.getName() << " " << tk.getLineNo() << endl;
    }
    outputFile.close();
}