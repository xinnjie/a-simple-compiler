//
// Created by capitalg on 12/14/17.
//

#ifndef COMPILER_SYMBOLS_MANAGER_H
#define COMPILER_SYMBOLS_MANAGER_H
#include <memory>
#include "SymbolTable.h"

class SymbolsManager {
private:
    std::shared_ptr<SymbolTable> root;
    std::shared_ptr<SymbolTable> current_symbols;
    std::vector<std::shared_ptr<SymbolTable>> leaves; // the bottom symbol_tables,
    // they are used to store & delete the whole SymbolTable tree
public:
    SymbolsManager();
    void step_in();
    void step_back();
    bool contains_cur_field(const std::string &name);
    bool contains(const std::string &name) noexcept;
    void insert(const Symbol &s);
    Symbol &find(const std::string &name);
    SymbolTable &get_current_symbols();
};


#endif //COMPILER_SYMBOLS_MANAGER_H
