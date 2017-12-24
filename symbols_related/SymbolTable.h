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

class SymbolTable;

/*!
 * 每个 Symbol 对象表示一个变量，
 * 信息包括
 * 1. 变量名字
 * 2. 变量类型
 * 3. 该变量处于哪个作用域
 */
class Symbol {
public:
    const SymbolTable &cur_table; //记录该Symbols位于哪张符号表
    const std::string name;
    const Type &type;
    int addr; // 相对于当前符号表的偏移量
//  todo: symbol看起来还需要知道自己在哪一张symboltable里，方便知道自己实际偏移地址
//    todo: 将构造函数设为private，只允许在SymbolTable中间接构造新的Symbol
    // addr 应该由SymbolsTable设定
    /*!
     *
     * @param cur_table  当前作用域的 符号表，作用：计算实际偏移（表偏移+表内偏移）
     * @param name 该变量的名字
     * @param type 该变量的类型
     * @param addr 该变量在 当前符号表中的 偏移， 一般由 SymbolTable （根据表中符号类型计算当前偏移后）设置
     */
    Symbol(const SymbolTable &cur_table, const std::string &name, const Type &type, int addr = 0) :
            cur_table(cur_table), name(name), addr(addr), type(type) {}

    virtual ~Symbol() = default;

    bool is_array() {
        return type.t == tag::TYPE_ARRAY;
    }

    bool is_func() {
        return type.t == tag::TYPE_FUNC;
    }
};

class ArraySymbol : public Symbol {
public:
    const Type &element_type;

    ArraySymbol(const SymbolTable &cur_table, const std::string &name, const Type &type, const Type &element_type,
                int addr = 0) :
            Symbol(cur_table, name, type, addr), element_type(element_type) {}
};

/*!
 * FuncSymbol  函数类
 * 包括 形参、返回值
 */
class FuncSymbol : public Symbol {
public:

    FuncSymbol(const SymbolTable &cur_table, const std::string &name, const Type &type, int addr);
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
    Symbol &insert(const Symbol &s);

    /*!
     * 返回要名字为 name 的Symbol引用，若查不到，会抛出 symbol_not_exist_error 异常
     * 若要确认 符号表中是否含有 叫 name 的符号，请使用 contains or contain_cur_field 方法
     * @param name 要查找的 Symbol 名字
     * @return 名字为 name 的 Symbol 引用
     */
    Symbol &find(const std::string &name);
};





#endif //COMPILER_SYMBOL_TABLE_H
