//
// Created by 89600 on 2019/11/13.
//

#ifndef COMPILER_MIPS_H
#define COMPILER_MIPS_H

#include "SymbolMap.h"

enum InstrType {constdef, vardef, funcdef, paradef, cal, ret, push, fcall, label, bgoto, bnz, bz, mac};

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
    vector<string> dotMacro;
    vector<string> dotData;
    vector<string> dotText;
    ofstream outMips;

    SymbolMap *currentMap;
    int codePlace;
    int inlines;
    int currentAddr;
    int currentStack;
    int paras;
    int funcStack;
    int pushes;
    string currentCode;
    instr currentInstr;
    string condType;
public:
    Mips(vector<string> &middleCode);

    void parse();

    void getSent();

    void outToMacro(string str);

    void outToData(string str);

    void outToText(string str);

    void setMacro();

    void setData();

    void parseFunc();

    void parseDef();

    void outSymbolMap();

    void parseSent();
};


#endif //COMPILER_MIPS_H
