//
// Created by capitalg on 12/19/17.
//

#include "MiddleLanOperand.h"
#include "Node.h"

std::string MiddleLanConstant::to_string() const {
    return lexeme;
}

MiddleLanConstant::MiddleLanConstant(const Rvalue &rvalue) : MiddleLanOperand(rvalue.type), lexeme(rvalue.lexeme) {}



MiddleLanVariable::MiddleLanVariable(const Symbol &symbol) : MiddleLanOperand(symbol.type), symbol(symbol) {}

std::string MiddleLanVariable::to_string() const {
    return symbol.name;
}

MiddleLanVariable::MiddleLanVariable(const BasicLvalue &basic_lvalue) : MiddleLanOperand(basic_lvalue.symbol.type),
                                                                        symbol(basic_lvalue.symbol) {
}


std::string MiddleLanArray::to_string() const {
    return symbol.name + "[" + index->to_string() + "]";
}

MiddleLanArray::MiddleLanArray(const ArraySymbol &symbol, std::shared_ptr<MiddleLanOperand> index)
        : MiddleLanOperand(symbol.type), symbol(symbol), index(index), element_type(symbol.element_type) {}



