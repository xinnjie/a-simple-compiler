//
// Created by capitalg on 12/14/17.
//

#include "SymbolsManager.h"

using namespace std;

int SymbolsManager::temp_i = 0;

SymbolsManager::SymbolsManager(): root(new SymbolTable()), current_symbols(root) {}

void SymbolsManager::step_back() {
    leaves.push_back(current_symbols);
    current_symbols = current_symbols->prev;
}

void SymbolsManager::step_in() {
    current_symbols = make_shared<SymbolTable>(current_symbols);
}

bool SymbolsManager::contains_symbol_in_cur_field(const std::string &name) {
    return current_symbols->contains_cur_field(name);
}

bool SymbolsManager::contains_symbol(const std::string &name) noexcept {
    return current_symbols->contains(name);
}

void SymbolsManager::insert(const Symbol &s) {
    current_symbols->insert(s);
}


// todo ????? 假如加上 inline 链接会失败，parser.o 找不到这个函数
SymbolTable &SymbolsManager::get_current_symbols() {
    return *current_symbols;
}

Symbol &SymbolsManager::find_symbol(const std::string &name) {
    return current_symbols->find(name);
}

Symbol &SymbolsManager::insert_temp(const Type &t) {
    string temp_name = string("temp") + std::to_string(temp_i);
    current_symbols->insert(Symbol(get_current_symbols(), temp_name, t));
    return current_symbols->find(temp_name);
}

Symbol &SymbolsManager::insert_basic_variable(const std::string &name, const Type &type) {
    return current_symbols->insert(Symbol(*current_symbols, name, type));
}
