#include "frontend/Parser.h"

vector<string> middleCode;

int main() {
    Parser parser("testfile.txt", middleCode);
    parser.parse();
    return 0;
}