//
// Created by capitalg on 12/15/17.
//

#include <map>
#include "MiddleSentence.h"
#include "MiddleLanOperand.h"

using namespace std;

static map<tag, string> opt2str{
        {tag::ADD,      "+"},
        {tag::SUB,      "-"},
        {tag::MULTIPLY, "*"},
        {tag::DEVIDE,   "/"},
        {tag::ASSIGN,   "="}

};

std::ostream &operator<<(std::ostream &os, const MiddleSentence &lan) {
    os << "opt: " << opt2str[lan.opt] << " left_operand: " << lan.left_operand->to_string() << " right_operand: "
       << lan.right_operand->to_string()
       << " result: " << lan.result->to_string();
    return os;
}

MiddleSentence::MiddleSentence(tag opt, const shared_ptr<MiddleLanOperand> &left_oprand,
                               const shared_ptr<MiddleLanOperand> &right_operand,
                               const shared_ptr<MiddleLanOperand> &result) : opt(opt), left_operand(left_oprand),
                                                                             right_operand(right_operand),
                                                                             result(result) {}
