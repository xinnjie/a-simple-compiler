//
// Created by capitalg on 10/25/17.
//

#ifndef COMPILER_TREE_H
#define COMPILER_TREE_H

#include <string>
#include <vector>
#include <memory>
#include "../tag.h"
#include "../symbols_related/SymbolTable.h"
#include "../symbols_related/Type.h"
#include "MiddleSentence.h"
#include "../symbols_related/TableManager.h"

// Node需要保有一份 TableManager & (----Type---Manager-- 划去) 的引用
// 原因：TableManager： 个别Node需要创建临时变量，那么需要利用TableManager来删减临时变量
//       cur_table： 建立临时变量需要知道当前处于哪一张 符号表, cur_table存于symbol中
class Node {
public:
//    tag Type;
    TableManager &table_manager;
    SymbolTable &cur_table;

    explicit Node(TableManager &table_manager) : table_manager(table_manager),
                                                 cur_table(table_manager.get_current_symbols()) {}


    virtual ~Node() = default;

    /*!
     *   interface for Node Type
     *   generate intermediate code
     * @param middle_sentences 生成的middle_sentences将存入到参数 middle_sentences中
     * @return 生成的middle_sentence的引用
     */
    virtual void gen(std::vector<MiddleSentence> &middle_sentences) = 0;

};

class Expr: public Node {
public:
    explicit Expr(TableManager &table_manager) : Node(table_manager) {}

    virtual const Type &get_type()= 0;

};

class Lvalue: public Expr {
public:
    explicit Lvalue(TableManager &table_manager) : Expr(table_manager) {}

    /*!
     * gen_assign_left() 方法十分类似 gen()，区别在于 该方法为 Lvalue 对象独有，专门被用在生成 assign 语句的被赋值对象
     * 一方面 对普通的 BasicLvalue 而言，甚至于gen() 没有区别，都只是将 symbol 转化为对应的 MiddleLanOperand 对象后压入 semantic 栈中
     * 另一方面 对ArrayLvalue 而言， gen() 和 gen_assgin_left 都会对下标部分进行 gen()得到最终的下标，但是gen_assign_left 少了一步最后生成临时变量的操作
     * @param middle_sentences
     */
    virtual void gen_assign_left(std::vector<MiddleSentence> &middle_sentences) = 0;
};

class BasicLvalue : public Lvalue {
public:
    const Symbol &symbol;

    BasicLvalue(TableManager &table_manager, const Symbol &symbol) : Lvalue(table_manager), symbol(symbol) {}

    void gen(std::vector<MiddleSentence> &middle_sentences) override;

    void gen_assign_left(std::vector<MiddleSentence> &middle_sentences) override;

    const Type &get_type() override {
        return symbol.type;
    }
};


class ArrayLvalue : public Lvalue {
public:
    const ArraySymbol &symbol;
    std::unique_ptr<Expr> expr;


    ArrayLvalue(TableManager &table_manager, ArraySymbol &symbol, Expr *expr) :
            Lvalue(table_manager), symbol(symbol), expr(expr) {}

    const Type &get_type() override {
        return symbol.type;
    }

    void gen_assign_left(std::vector<MiddleSentence> &middle_sentences) override;

    void gen(std::vector<MiddleSentence> &middle_sentences) override;
};


/*!
 * represent constant
 */
class Rvalue: public Expr {
public:
    const Type &type;
    std::string lexeme;

    Rvalue(TableManager &symbols, const Type &t, const std::string &lexeme) : Expr(symbols), type(t), lexeme(lexeme) {}

    const Type &get_type() override {
        return type;
    }

    void gen(std::vector<MiddleSentence> &middle_sentences) override;
};


class Stmts: public Node {
public:
    std::vector<std::unique_ptr<Node>> nodes;

    explicit Stmts(TableManager &table_manager) : Node(table_manager) {}
    void insert(Node *n) {
        nodes.push_back(std::unique_ptr<Node>(n));
    }

    void gen(std::vector<MiddleSentence> &middle_sentences) override;

};


class IfElse: public Node {
public:
    std::unique_ptr<Node> abool, true_stmt, false_stmt;

    IfElse(TableManager &table_manager, Node *abool,
           Node *true_stmt, Node *false_stmt = nullptr) :
            Node(table_manager), abool(abool), true_stmt(true_stmt), false_stmt(false_stmt) {}

    void gen(std::vector<MiddleSentence> &middle_sentences) override;
};

class BinaryOpt: public Expr {
public:
    tag opt; // && , ||
    std::unique_ptr<Expr> left, right;

    BinaryOpt(TableManager &table_manager, tag opt, Expr *left, Expr *right) :
            opt(opt), Expr(table_manager), left(left), right(right) {}

    const Type &get_type() override {
        return Type::type_lift(left->get_type(), right->get_type());
    }

    void gen(std::vector<MiddleSentence> &middle_sentences) override;

};

class UnaryOpt: public Expr {
public:
    tag opt;
    std::unique_ptr<Expr> operand;

    UnaryOpt(TableManager &table_manager, tag opt, Expr *operand) :
            opt(opt), Expr(table_manager), operand(operand) {
        // todo: operand_type support opt?
    }

    const Type &get_type() override {
        return operand->get_type();
    }

    void gen(std::vector<MiddleSentence> &middle_sentences) override;

};


class While: public Node {
public:
    std::unique_ptr<Node> loop_stmt;
    std::unique_ptr<Expr> abool;

    While(TableManager &table_manager, Expr *abool, Node *loop_stmt) :
            Node(table_manager), abool(abool), loop_stmt(loop_stmt) {}

    void gen(std::vector<MiddleSentence> &middle_sentences) override;
};

class assign: public Node {
public:
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Lvalue> left;

    assign(TableManager &table_manager, Lvalue *left, Expr *expr) :
            Node(table_manager), left(left), expr(expr) {}

    void gen(std::vector<MiddleSentence> &middle_sentences) override;
};

class decl: public Node {
public:
    Type &type;
    Symbol &symbol;

    decl(TableManager &table_manager, Type &type, Symbol &symbol) :
            Node(table_manager), type(type), symbol(symbol) {}

    void gen(std::vector<MiddleSentence> &middle_sentences) override;
};


#endif //COMPILER_TREE_H
