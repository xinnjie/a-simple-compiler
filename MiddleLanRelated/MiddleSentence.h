//
// Created by capitalg on 12/15/17.
//

#ifndef COMPILER_MIDDLELAN_H
#define COMPILER_MIDDLELAN_H


#include <string>
#include <ostream>
#include "../tag.h"
#include "MiddleLanOperand.h"

class MiddleSentence {
public:
    tag opt;
    const std::shared_ptr<MiddleLanOperand> left_oprand, right_operand, result;

    MiddleSentence(tag opt, const std::shared_ptr<MiddleLanOperand> &left_oprand,
                   const std::shared_ptr<MiddleLanOperand> &right_operand,
                   const std::shared_ptr<MiddleLanOperand> &result);

    friend std::ostream &operator<<(std::ostream &os, const MiddleSentence &lan);
};


#endif //COMPILER_MIDDLELAN_H
