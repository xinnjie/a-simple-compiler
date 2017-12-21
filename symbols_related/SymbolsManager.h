//
// Created by capitalg on 12/14/17.
//

#ifndef COMPILER_SYMBOLS_MANAGER_H
#define COMPILER_SYMBOLS_MANAGER_H
#include <memory>
#include "SymbolTable.h"

// TypeManager & SymbolsManager 协同工作
// SymbolsManager 功能：保存所有符号表
//                      添加删除符号
//                      创建、回退作用域
class SymbolsManager {
private:
    static int temp_i;
    std::shared_ptr<SymbolTable> root;
    std::shared_ptr<SymbolTable> current_symbols;
    std::vector<std::shared_ptr<SymbolTable>> leaves; // the bottom symbol_tables,
    // they are used to store & delete the whole SymbolTable tree
public:
    SymbolsManager();

    void step_in();

    void step_back();

    bool contains_symbol_in_cur_field(const std::string &name);

    bool contains_symbol(const std::string &name) noexcept;

    void insert(const Symbol &s);

    Symbol &insert_temp(const Type &t);

    Symbol &find_symbol(const std::string &name);

    SymbolTable &get_current_symbols();
};

#endif //COMPILER_SYMBOLS_MANAGER_H
