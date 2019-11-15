//
// Created by 89600 on 2019/11/13.
//

#ifndef COMPILER_SYMBOLMAP_H
#define COMPILER_SYMBOLMAP_H

#include "babasics.h"

enum SymbolType {vint, vchar, aint, achar, pint, pchar, fint, fchar, fvoid, errors};

struct SymbolInf {
    string name;
    SymbolType type;
    int size;
    int addr;
};

typedef struct SymbolInf symbolInf;

class SymbolMap {
private:
    SymbolMap* prevpt;
    map<string, symbolInf> symbols;
    vector<symbolInf> symbolList;
    int stackPlace;

public:
    int retAddr;

    SymbolMap(SymbolMap *_prev);

    void add(string name, SymbolType type, int size, int addr);

    int search(string name);

    SymbolMap *getPrev();

    string getPara(string name, int para);

    int findPlace(string name);

    void setStackPlace(int stkp);

    int getStackPlace();

    SymbolType findType(string name);

    string getOutput(string name, int paras);

    void setType(string name, SymbolType _type);
};


#endif //COMPILER_SYMBOLMAP_H
