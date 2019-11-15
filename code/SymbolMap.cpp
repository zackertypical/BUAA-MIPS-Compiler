//
// Created by 89600 on 2019/11/13.
//

#include "SymbolMap.h"

SymbolMap::SymbolMap(SymbolMap *_prev) {
    prevpt = _prev;
}

void SymbolMap::add(string name, SymbolType type, int size, int addr) {
    symbolInf tmp;
    tmp.name = name;
    tmp.type = type;
    tmp.size = size;
    tmp.addr = addr;
    symbols.insert(make_pair(name, tmp));
    symbolList.push_back(tmp);
}

int SymbolMap::search(string name) {
    if (symbols.find(name) != symbols.end()) {
        return symbols[name].addr;
    } else if (prevpt != nullptr){
        return prevpt->search(name);
    }
    return -1;
}

int SymbolMap::findPlace(string name) {
    for (int i = 0; i < symbolList.size(); i++) {
        if (symbolList.at(i).name == name) {
            return i;
        }
    }
    return -1;
}

SymbolMap* SymbolMap::getPrev() {
    return prevpt;
}

string SymbolMap::getPara(string name, int para) {
    int base = findPlace(name);
    return symbolList.at(base + para).name;
}

void SymbolMap::setStackPlace(int stkp) {
    stackPlace = stkp;
}

int SymbolMap::getStackPlace() {
    return stackPlace;
}

void SymbolMap::setType(string name, SymbolType _type) {
    symbols[name].type = _type;
}

SymbolType SymbolMap::findType(string name){
    if (symbols.find(name) != symbols.end()) {
        return symbols[name].type;
    } else if (prevpt != nullptr){
        return prevpt->findType(name);
    }
    cout << name << endl;
    return errors;
}

string SymbolMap::getOutput(string name, int paras) {
    SymbolType type = findType(name);
    int addr = search(name);
    if (type == pint || type == pchar) {
        return to_string(addr - paras * 4 - 4) + "($fp)";
    } else {
        return to_string(-addr) + "($sp)";
    }
}
