#include "Parser.h"
#include "Mips.h"

vector<string> middleCode;

int main() {
    Parser parser("testfile.txt", middleCode);
    parser.parse();
    Mips mips(middleCode);
    mips.parse();
    return 0;
}