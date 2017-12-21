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

    Node(TableManager &table_manager) : table_manager(table_manager), cur_table(table_manager.get_current_symbols()) {}


    virtual ~Node(){};
    // interface for Node Type
    // generate intermediate code
    virtual MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) = 0;

};

class Expr: public Node {
public:
    Expr(TableManager &table_manager) : Node(table_manager) {}

    virtual const Type &get_type()= 0;

};



class Lvalue: public Expr {
public:
    const Symbol &symbol;
    std::unique_ptr<Expr> expr;


    // not array
    Lvalue(TableManager &table_manager, Symbol &symbol) : Expr(table_manager), symbol(symbol), expr(nullptr) {}
    // is array
    Lvalue(TableManager &table_manager, Symbol &symbol, Expr *expr) :
            Expr(table_manager), symbol(symbol), expr(expr) {}

    const Type &get_type() override {
        return symbol.type;
    }


    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;
};


// represent constant
class Rvalue: public Expr {
public:
    const Type &type;
    std::string lexeme;

    Rvalue(TableManager &symbols, const Type &t, const std::string &lexeme) : Expr(symbols), type(t), lexeme(lexeme) {}

    const Type &get_type() override {
        return type;
    }

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;
};


class Stmts: public Node {
public:
    std::vector<std::unique_ptr<Node>> nodes;

    Stmts(TableManager &table_manager) : Node(table_manager) {}
    void insert(Node *n) {
        nodes.push_back(std::unique_ptr<Node>(n));
    }

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;

};


class IfElse: public Node {
public:
    std::unique_ptr<Node> abool, true_stmt, false_stmt;

    IfElse(TableManager &table_manager, Node *abool,
           Node *true_stmt, Node *false_stmt = nullptr) :
            Node(table_manager), abool(abool), true_stmt(true_stmt), false_stmt(false_stmt) {}

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;
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

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;

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

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;

};


class While: public Node {
public:
    std::unique_ptr<Node> loop_stmt;
    std::unique_ptr<Expr> abool;

    While(TableManager &table_manager, Expr *abool, Node *loop_stmt) :
            Node(table_manager), abool(abool), loop_stmt(loop_stmt) {}

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;
};

class assign: public Node {
public:
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Lvalue> left;

    assign(TableManager &table_manager, Lvalue *left, Expr *expr) :
            Node(table_manager), left(left), expr(expr) {}

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;
};

class decl: public Node {
public:
    Type &type;
    Symbol &symbol;

    decl(TableManager &table_manager, Type &type, Symbol &symbol) :
            Node(table_manager), type(type), symbol(symbol) {}

    MiddleSentence &gen(std::vector<MiddleSentence> &middle_sentences) override;
};


#endif //COMPILER_TREE_H
