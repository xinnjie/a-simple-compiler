//
// Created by capitalg on 10/25/17.
//

#include <stack>
#include "Node.h"
#include "MiddleLanOperand.h"

using namespace std;
//关于为什么使用 shared_ptr, semantic 中难免会多次加入 一个变量  比如 b = a + a, a 会被 压栈两次
static std::stack<std::shared_ptr<MiddleLanOperand>> semantic;

void IfElse::gen(std::vector<MiddleSentence> &middle_sentences) {
}


void BinaryOpt::gen(std::vector<MiddleSentence> &middle_sentences) {
    std::shared_ptr<MiddleLanOperand> right_operand = semantic.top();
    semantic.pop();
    std::shared_ptr<MiddleLanOperand> left_operand = semantic.top();
    semantic.pop();
    const Type &t = right_operand->type;
    auto temp_ptr = make_shared<MiddleLanVariable>(table_manager.insert_temp(t));
    semantic.push(temp_ptr);
    middle_sentences.emplace_back(opt, left_operand, right_operand, temp_ptr);
}

void While::gen(std::vector<MiddleSentence> &middle_sentences) {

}

void UnaryOpt::gen(std::vector<MiddleSentence> &middle_sentences) {
    std::shared_ptr<MiddleLanOperand> operand = semantic.top();
    semantic.pop();
    const Type &t = operand->type;
    auto temp_ptr = make_shared<MiddleLanVariable>(table_manager.insert_temp(t));
    semantic.push(temp_ptr);
    middle_sentences.emplace_back(opt, operand, nullptr, temp_ptr);
}

void ArrayLvalue::gen(std::vector<MiddleSentence> &middle_sentences) {
    // 如果是数组类型的，先将数组下标中的 expr 做gen()处理，从semantic栈中取出下标
    expr->gen(middle_sentences);
    auto index = semantic.top();
    semantic.pop();

    // 确保下标类型是 整型 的
    assert(index->type == table_manager.get_basic_type(tag::TYPE_INTEGER));

    // 将数组元素赋值给一个临时变量
    auto array_ptr = make_shared<MiddleLanArray>(this->symbol, index);
    auto temp_ptr = make_shared<MiddleLanVariable>(table_manager.insert_temp(array_ptr->element_type));
    middle_sentences.emplace_back(tag::ASSIGN, array_ptr, nullptr, temp_ptr);

    // 赋值完的临时变量重新压入 semantic 栈中
    semantic.push(temp_ptr);
}

void ArrayLvalue::gen_assign_left(std::vector<MiddleSentence> &middle_sentences) {
    // 如果是数组类型的，先将数组下标中的 expr 做gen()处理，从semantic栈中取出下标
    expr->gen(middle_sentences);
    auto index = semantic.top();
    semantic.pop();

    // 确保下标类型是 整型 的
    assert(index->type == table_manager.get_basic_type(tag::TYPE_INTEGER));

    // 将数组元素压入 semantic 栈中
    auto array_ptr = make_shared<MiddleLanArray>(this->symbol, index);
    semantic.push(array_ptr);
}

void Rvalue::gen(std::vector<MiddleSentence> &middle_sentences) {
    auto constant_ptr = make_shared<MiddleLanConstant>(*this);
    semantic.push(constant_ptr);
}

void Stmts::gen(std::vector<MiddleSentence> &middle_sentences) {
    for (auto &node : nodes) {
        node->gen(middle_sentences);
    }
}

void decl::gen(std::vector<MiddleSentence> &middle_sentences) {
    return;
}

void assign::gen(std::vector<MiddleSentence> &middle_sentences) {
    // 获取 assign 右侧结果
    expr->gen(middle_sentences);
    auto right = semantic.top();
    semantic.pop();

    // 获取 assgin 结果
    left->gen_assign_left(middle_sentences);
    auto left = semantic.top();
    semantic.pop();

    // 生成赋值语句
    middle_sentences.emplace_back(tag::ASSIGN, right, nullptr, left);
}


void BasicLvalue::gen(std::vector<MiddleSentence> &middle_sentences) {
    semantic.push(make_shared<MiddleLanVariable>(*this));
}

void BasicLvalue::gen_assign_left(std::vector<MiddleSentence> &middle_sentences) {
    gen(middle_sentences);
}
