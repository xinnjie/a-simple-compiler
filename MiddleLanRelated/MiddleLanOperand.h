//
// Created by capitalg on 12/19/17.
//

#ifndef COMPILER_MIDDLELANOPERAND_H
#define COMPILER_MIDDLELANOPERAND_H


#include <string>
#include <ostream>
#include "../symbols_related/Type.h"
#include "../symbols_related/SymbolTable.h"
#include "Node.h"

class Rvalue;

class ArrayLvalue;

class BasicLvalue;

class MiddleLanOperand {
public:
    const Type type;

    explicit MiddleLanOperand(const Type &type) : type(type) {
    }

    virtual std::string to_string() const = 0;

    virtual ~MiddleLanOperand() = default;
};

// MiddleLandConstant <-> Rvalue
// implicit transformation
class MiddleLanConstant : public MiddleLanOperand {
public:
    std::string lexeme;

    explicit MiddleLanConstant(const Rvalue &rvalue);

    std::string to_string() const override;

};


class MiddleLanVariable : public MiddleLanOperand {
public:
    const Symbol &symbol;

    explicit MiddleLanVariable(const Symbol &symbol);

    explicit MiddleLanVariable(const BasicLvalue &basic_lvalue);

    std::string to_string() const override;
};

class MiddleLanArray : public MiddleLanOperand {
public:
    const ArraySymbol &symbol;
    const Type &element_type;
    std::shared_ptr<MiddleLanOperand> index; // 可以是常量类型 eg.a[4]
    // 也可以是变量类型 eg.a[$t]
    MiddleLanArray(const ArraySymbol &symbol, std::shared_ptr<MiddleLanOperand> index);

//    /*!
//     * todo: 目前参数不能是lvalue，因为MiddleLanArray构造函数第三个参数是 const MiddleLanOperand &index, 无法对应到Lvalue.expr（类型为 unique_ptr<Expr>）
//     * @param lvalue
//     */
//    MiddleLanArray(const ArrayLvalue &lvalue);
    std::string to_string() const override;
};


#endif //COMPILER_MIDDLELANOPERAND_H
