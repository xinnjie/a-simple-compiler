//
// Created by capitalg on 10/25/17.
//

#include <stack>
#include "Node.h"

using namespace std;
//关于为什么使用 shared_ptr, semantic 中难免会多次加入 一个变量  比如 b = a + a, a 会被 压栈两次
static std::stack<std::shared_ptr<MiddleLanOperand>> semantic;

// todo: nodes 也需要访问 typemanager & symbolsmanager的权限
MiddleSentence &IfElse::gen(std::vector<MiddleSentence> &middle_sentences) {
}


MiddleSentence &BinaryOpt::gen(std::vector<MiddleSentence> &middle_sentences) {
    std::shared_ptr<MiddleLanOperand> right_operand = semantic.top();
    semantic.pop();
    std::shared_ptr<MiddleLanOperand> left_operand = semantic.top();
    semantic.pop();
    const Type &t = right_operand->type;
    auto temp_ptr = make_shared<MiddleLanVariable>(table_manager.insert_temp(t));
    semantic.push(temp_ptr);
    middle_sentences.emplace_back(MiddleSentence(opt, left_operand, right_operand, temp_ptr));
    return middle_sentences.back();
}

MiddleSentence &While::gen(std::vector<MiddleSentence> &middle_sentences) {

}

MiddleSentence &UnaryOpt::gen(std::vector<MiddleSentence> &middle_sentences) {

}

MiddleSentence &Lvalue::gen(std::vector<MiddleSentence> &middle_sentences) {

}

MiddleSentence &Rvalue::gen(std::vector<MiddleSentence> &middle_sentences) {

}

MiddleSentence &Stmts::gen(std::vector<MiddleSentence> &middle_sentences) {

}

MiddleSentence &decl::gen(std::vector<MiddleSentence> &middle_sentences) {

}

MiddleSentence &assign::gen(std::vector<MiddleSentence> &middle_sentences) {
}

