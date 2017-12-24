//
// Created by capitalg on 10/25/17.
//

#include "SymbolTable.h"

Symbol &SymbolTable::find(const std::string &name) {
// auto t = std::shared_ptr<SymbolTable>(this);    // this is dangerous;
// if no one owns this outside, than t will be the only owner, once t = t->prev perform, 'this' will destruct;
    auto t = this;
    while (t != nullptr)  {
        auto it = t->table.find(name);
        if (it != t->table.cend()) return it->second;
        t = (t->prev).get();
    }
    throw symbol_not_exist_error(name);
}

bool SymbolTable::contains(const std::string &name) noexcept {
    auto t =  this;
    while (t != nullptr)  {
        auto it = t->table.find(name);
        if (it != t->table.cend()) return true;
        t = (t->prev).get();
    }
    return false;
}

Symbol &SymbolTable::insert(const Symbol &s) {
    if (contains_cur_field(s.name)) throw duplicate_symbol_error(s.name);
    Symbol cp = s;
    cp.addr = addr;
    addr += 4; // todo: addr按照type的大小增加地址
    auto pair = table.insert({cp.name, cp});
    return pair.first->second;
}

FuncSymbol::FuncSymbol(const SymbolTable &cur_table, const std::string &name, const Type &type, int addr) : Symbol(
        cur_table, name, type, addr) {}
