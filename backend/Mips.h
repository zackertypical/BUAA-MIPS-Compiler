//
// Created by 89600 on 2019/11/13.
//

#ifndef COMPILER_MIPS_H
#define COMPILER_MIPS_H

#include "SymbolMap.h"

enum InstrType {constdef, vardef, funcdef, paradef, cal, ret, push, fcall, label, bgoto, bnz, bz};

struct Instr{
    InstrType type;
    string iden0;
    string iden1;
    string iden2;
    string iden3;
    string iden4;
};

typedef struct Instr instr;

class Mips {
private:
    vector<string> &middleCode;
    vector<SymbolMap> symbolMaps;
    vector<instr> instrs;
    vector<string> constStrings;
    ofstream outMips;

    SymbolMap *currentMap;
    int codePlace;
    int currentAddr;
    int paras;
    int funcStack;
    string currentCode;
    instr currentInstr;
    string condType;
public:
    Mips(vector<string> &middleCode);
    void parse();

    void getSent();

    void parseSent();

    void outToMips(string str);

    void createPreVar();
};


#endif //COMPILER_MIPS_H
