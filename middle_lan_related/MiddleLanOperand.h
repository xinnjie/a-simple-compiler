//
// Created by capitalg on 12/19/17.
//

#ifndef COMPILER_MIDDLELANOPERAND_H
#define COMPILER_MIDDLELANOPERAND_H


#include <string>
#include <ostream>
#include "../symbols_related/Type.h"
#include "../symbols_related/SymbolTable.h"

class Rvalue;
class BasicLvalue;

class MiddleLanOperand {
public:
    const Type type;

    explicit MiddleLanOperand(const Type &type) : type(type) {
    }

    virtual ~MiddleLanOperand() {};

    virtual std::string to_string() const = 0;

};

// MiddleLandConstant <-> Rvalue
// implicit transformation
class MiddleLanConstant : public MiddleLanOperand {
public:
    std::string lexeme;

    explicit MiddleLanConstant(const Rvalue &rvalue);

    std::string to_string() const override;

    ~MiddleLanConstant() override {}
};


class MiddleLanVariable : public MiddleLanOperand {
public:
    const Symbol &symbol;

    explicit MiddleLanVariable(const Symbol &symbol);

    explicit MiddleLanVariable(const BasicLvalue &basic_lvalue);

    std::string to_string() const override;

    ~MiddleLanVariable() override {}
};

class MiddleLanArray : public MiddleLanOperand {
public:
    const ArraySymbol &symbol;
    const Type &element_type;
    std::shared_ptr<MiddleLanOperand> index; // 可以是常量类型 eg.a[4]
    // 也可以是变量类型 eg.a[$t]

    MiddleLanArray(const ArraySymbol &symbol, std::shared_ptr<MiddleLanOperand> index);

    std::string to_string() const override;

    ~MiddleLanArray() override {}
};


#endif //COMPILER_MIDDLELANOPERAND_H
