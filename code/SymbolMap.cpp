//
// Created by 89600 on 2019/11/13.
//

#include "SymbolMap.h"


SymbolMap::SymbolMap(SymbolMap *_prev) {
    prevpt = _prev;
}

int SymbolMap::add(string name, SymbolType type, int place, int addr) {
    symbolInf tmp;
    tmp.name = name;
    tmp.type = type;
    tmp.place = place;
    tmp.addr = addr;
    if (search(name) == nullptr) {
        symbols.insert(make_pair(name, tmp));
        symbolList.push_back(tmp);
        return 1;
    }
    return 0;
}

void SymbolMap::addFunc(string name, SymbolType type, int paras, int canInline) {
    symbolInf tmp;
    tmp.name = name;
    tmp.type = type;
    tmp.place = paras;
    tmp.addr = canInline;
    funcs.insert(make_pair(name, tmp));
    funcList.push_back(tmp);
    SymbolMap* sym = new SymbolMap(this);
    symMaps.insert(make_pair(name, sym));
}

symbolInf* SymbolMap::search(string name) {
    if (symbols.find(name) != symbols.end()) {
        return &symbols[name];
    } else if (prevpt != nullptr) {
        return prevpt->search(name);
    }
    return nullptr;
}

symbolInf* SymbolMap::searchFunc(string name) {
    if (prevpt != nullptr){
        return prevpt->search(name);
    } else if (funcs.find(name) != funcs.end()) {
        return &funcs[name];
    }
    return nullptr;
}


SymbolMap* SymbolMap::getPrev() {
    return prevpt;
}

SymbolMap* SymbolMap::getNext(string name) {
    return symMaps[name];
}

void SymbolMap::setType(string name, SymbolType _type) {
    symbols[name].type = _type;
}

SymbolType SymbolMap::getType(string name){
    if (symbols.find(name) != symbols.end()) {
        return symbols[name].type;
    } else if (prevpt != nullptr){
        return prevpt->getType(name);
    }
    if (isdigit(name.at(0)) || name.at(0) == '-') {
        return vint;
    } else if (name.at(0) == '\'') {
        return vchar;
    }
    return errors;
}

string SymbolMap::getOutput(string name) {
    symbolInf* sym = search(name);
    if (sym->place == 4) {
        return "$t" + to_string(sym->addr);
    } else if (sym->place == 3) {
        return to_string(sym->addr) + "($fp)";
    } else if (sym->place == 2) {
        return "$s" + to_string(sym->addr);
    } else if (sym->place == 1) {
        return to_string(sym->addr) + "($sp)";
    } else {
        return to_string(sym->addr) + "($gp)";
    }
}

map<string, SymbolMap*> SymbolMap::getNextList() {
    return symMaps;
}

string SymbolMap::saveSymbol(string str, string name) {
    // var name = str
    // str是常量或者寄存器
    string output = getOutput(name);
    if (str[0] == '$' && output[0] == '$') {
        return "move " + output + ", " + str;
    } else if (str[0] == '$') {
        return "sw " + str + ", " + output;
    } else if (output[0] == '$') {
        return "li " + output + ", " + str;
    } else {
        return "li $t8, " + str + "\nsw $t8 " + output;
    }
}

string SymbolMap::loadSymbol(string str, string name) {
    // str = var name
    // str是寄存器
    string output = getOutput(name);
    if (output[0] == '$') {
        return "move " + str + ", " + output;
    } else {
        return "lw " + str + ", " + output;
    }
}

void SymbolMap::printSymbols(ofstream &stream) {
    for (symbolInf elm : symbolList) {
        stream.setf(ios::left);
        stream << "|";
        stream.width(17);
        stream << elm.name;
        stream.width();
        stream << "|";
        stream.width(6);
        switch (elm.type) {
            case vint:
            case aint:
            case pint:
                stream << "int";
                break;
            case vchar:
            case achar:
            case pchar:
                stream << "char";
                break;
            case str:
                stream << "string";
                break;
            default:
                stream << "error";
                break;
        }
        stream.width();
        stream << "|";
        stream.width(7);
        switch (elm.place) {
            case 0:
                stream << ".data";
                break;
            case 1:
                stream << "$sp";
                break;
            case 2:
                stream << "$s" + to_string(elm.addr);
                break;
            case 3:
                stream << "$fp";
                break;
            case 4:
                stream << "$t" + to_string(elm.addr);
                break;
            default:
                stream << "unknown";
                break;
        }
        stream.width();
        stream << "|";
        stream.width(9);
        stream << elm.addr;
        stream.width();
        stream << "|" << endl;
    }
}