//
// Created by capitalg on 10/25/17.
//

#ifndef COMPILER_SYMBOL_TABLE_H
#define COMPILER_SYMBOL_TABLE_H

#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>
#include "../tag.h"
#include "../syntax_error.h"
#include "Type.h"

class Symbol {
public:
    const std::string name;
    const Type &symbol_type;
    int addr;

    Symbol(const std::string &name, const Type &symbol_type, int addr = 0):
            name(name), addr(addr), symbol_type(symbol_type) {}
};



class SymbolTable {
public:
    std::unordered_map<std::string, Symbol> table;
    int addr;

    std::shared_ptr<SymbolTable> prev;
    explicit SymbolTable(std::shared_ptr<SymbolTable> prev = nullptr): prev(prev), addr(0) {}

    bool contains_cur_field(const std::string &name) {
        return table.find(name) != table.cend();
    }

    bool contains(const std::string &name) noexcept;

// modification, only able to modify current field's SymbolTable;
// and check if s already in the table, throw exception
    void insert(const Symbol &s) {
        if (contains_cur_field(s.name)) throw duplicate_symbol_error(s.name);
        Symbol cp = s;
        cp.addr = addr;
        addr += 4;
        table.insert({cp.name, cp});
    }
    Symbol &find(const std::string &name);
};





#endif //COMPILER_SYMBOL_TABLE_H
