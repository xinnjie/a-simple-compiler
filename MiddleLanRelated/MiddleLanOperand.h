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

class MiddleLanOperand {
public:
    const Type type;

    MiddleLanOperand(const Type &type);

    virtual std::string to_string() const = 0;
};

// MiddleLandConstant <-> Rvalue
// implicit transformation
class MiddleLanConstant : public MiddleLanOperand {
public:
    std::string lexeme;

    MiddleLanConstant(const Rvalue &rvalue);

    std::string to_string() const override;
};


class MiddleLanVariable : public MiddleLanOperand {
public:
    const Symbol &symbol;

    explicit MiddleLanVariable(const Symbol &symbol);

    std::string to_string() const override;
};

class MiddleLanArray : public MiddleLanOperand {
public:
    const Symbol &symbol;
    const MiddleLanOperand &index; // 可以是常量类型 eg.a[4]
    // 也可以是变量类型 eg.a[$t]
    MiddleLanArray(const Type &type, const Symbol &symbol,
                   const MiddleLanOperand &index);

    std::string to_string() const override;
};


#endif //COMPILER_MIDDLELANOPERAND_H
