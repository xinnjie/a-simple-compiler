//
// Created by capitalg on 12/19/17.
//

#include "MiddleLanOperand.h"
#include "Node.h"

std::string MiddleLanConstant::to_string() const {
    return lexeme;
}

MiddleLanConstant::MiddleLanConstant(const Rvalue &rvalue) : MiddleLanOperand(rvalue.type), lexeme(lexeme) {}

MiddleLanVariable::MiddleLanVariable(const Symbol &symbol) : MiddleLanOperand(symbol.type), symbol(symbol) {}

std::string MiddleLanVariable::to_string() const {
    return symbol.name;
}

std::string MiddleLanArray::to_string() const {
    return symbol.name + "[" + index.to_string() + "]";
}

MiddleLanArray::MiddleLanArray(const Type &type, const Symbol &symbol, const MiddleLanOperand &index)
        : MiddleLanOperand(type), symbol(symbol), index(index) {}

MiddleLanOperand::MiddleLanOperand(const Type &type) : type(type) {}
