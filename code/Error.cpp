//
// Created by 89600 on 2019/10/28.
//

#include "Error.h"

bool mySort(const pair<int, char> p1, const pair<int, char> p2) {
    return p1.first < p2.first;
}

void Error::addError(int lineNo, char errType) {
    errorList.emplace_back(lineNo, errType);
}

void Error::printError() {
    if (!errorList.empty()) {
        for (pair<int, char> err : errorList) {
            cout << err.first << " " << err.second << endl;
        }
    } else {
        DEBUG_OUT("No error.")
    }
}

void Error::printError(string filename) {
    if (!errorList.empty()) {
        ofstream outputFile;
        outputFile.open(filename, ios::out | ios::trunc);
        sort(errorList.begin(), errorList.end(), mySort);
        for (pair<int, char> err : errorList) {
            outputFile << err.first << " " << err.second << endl;
        }
        outputFile.close();
    } else {
        DEBUG_OUT("No error.")
    }
}
