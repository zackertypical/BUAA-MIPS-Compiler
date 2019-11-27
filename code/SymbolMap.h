//
// Created by 89600 on 2019/11/13.
//

#ifndef COMPILER_SYMBOLMAP_H
#define COMPILER_SYMBOLMAP_H

#include "babasics.h"

enum SymbolType {vint, vchar, aint, achar, pint, pchar, fint, fchar, fvoid, errors, str};

struct SymbolInf {
    string name;
    SymbolType type;
    int place; //0代表.data,1代表$sp的相对位置,2代表全局寄存器,3代表函数参数$fp,4代表临时寄存器,如果是函数,则place代表参数个数
    int addr;  //偏移地址或者全局寄存器号或者是否可内联
};

typedef struct SymbolInf symbolInf;

class SymbolMap {
private:
    SymbolMap* prevpt;
    map<string, symbolInf> symbols;
    map<string, symbolInf> funcs;
    map<string, SymbolMap*> symMaps;
    vector<symbolInf> symbolList;
    vector<symbolInf> funcList;
    int stackPlace;

public:
    int retAddr;

    SymbolMap(SymbolMap *_prev);

    int add(string name, SymbolType type, int place, int addr);

    void addFunc(string name, SymbolType type, int paras, int canInline);

    symbolInf* search(string name);

    symbolInf* searchFunc(string name);

    SymbolMap* getPrev();

    SymbolMap* getNext(string name);

    void setType(string name, SymbolType _type);

    SymbolType getType(string name);

    string getOutput(string name);

    void printSymbols(ofstream &stream);

    map<string, SymbolMap*> getNextList();

    string saveSymbol(string str, string name);

    string loadSymbol(string str, string name);
};


#endif //COMPILER_SYMBOLMAP_H
