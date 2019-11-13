#include "frontend/Parser.h"

vector<string> middleCode;

int main() {
    Parser parser("testfile1.txt", middleCode);
    parser.parse();
    return 0;
}