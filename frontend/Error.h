//
// Created by 89600 on 2019/10/28.
//

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include "basics.h"

class Error {
private:
    vector<pair<int, char>> errorList;
public:
    void addError(int lineNo, char errType);
    void printError(string filename);
    void printError();
};


#endif //COMPILER_ERROR_H
