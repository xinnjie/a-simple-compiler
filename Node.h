//
// Created by capitalg on 10/25/17.
//

#ifndef COMPILER_TREE_H
#define COMPILER_TREE_H

#include <string>
#include <vector>
#include <memory>
#include "tag.h"
#include "symbols_related/SymbolTable.h"
#include "symbols_related/Type.h"
class Node {
public:
//    tag Type;
    SymbolTable &symbols;

    Node(SymbolTable &symbols): symbols(symbols) {}


    virtual ~Node(){};
    // interface for Node Type
    // generate intermediate code
    virtual void gen() = 0;

};

class Expr: public Node {
public:
    Expr(SymbolTable &symbols): Node(symbols) {}

    virtual const Type &get_type()= 0;

};



class Lvalue: public Expr {
public:
    Symbol &symbol;
    std::unique_ptr<Expr> expr;


    // not array
    Lvalue(SymbolTable &symbols, Symbol &symbol) : Expr(symbols), symbol(symbol) ,expr(nullptr){}
    // is array
    Lvalue(SymbolTable &symbols, Symbol &symbol , Expr *expr) :
            Expr(symbols), symbol(symbol) ,expr(expr){}

    const Type &get_type() override {
        return symbol.symbol_type;
    }


    void gen() override;
};


// represent constant
class Rvalue: public Expr {
public:
    const Type &type;
    std::string lexeme;

    Rvalue(SymbolTable &symbols, const Type &t, const std::string &lexeme) : Expr(symbols), type(t), lexeme(lexeme) {}

    const Type &get_type() override {
        return type;
    }

    void gen() override;
};


class Stmts: public Node {
public:
    std::vector<std::unique_ptr<Node>> nodes;

    Stmts(SymbolTable &symbols) : Node(symbols) {}
    void insert(Node *n) {
        nodes.push_back(std::unique_ptr<Node>(n));
    }

    void gen() override;

};


class IfElse: public Node {
public:
    std::unique_ptr<Node> abool, true_stmt, false_stmt;

    IfElse(SymbolTable & symbols,Node *abool,
            Node *true_stmt, Node *false_stmt = nullptr):
            Node(symbols), abool(abool),true_stmt(true_stmt), false_stmt(false_stmt){}

    void gen() override;
};

class BinaryOpt: public Expr {
public:
    tag opt; // && , ||
    std::unique_ptr<Expr> left, right;

    BinaryOpt(SymbolTable &symbols, tag opt, Expr *left, Expr *right):
            opt(opt), Expr(symbols), left(left), right(right) {}

    const Type &get_type() override {
        return Type::type_lift(left->get_type(), right->get_type());
    }

    void gen() override;
};

class UnaryOpt: public Expr {
public:
    tag opt;
    std::unique_ptr<Expr> operand;

    UnaryOpt(SymbolTable & symbols, tag opt, Expr *operand) :
            opt(opt), Expr(symbols), operand(operand) {
        // todo: operand_type support opt?
    }

    const Type &get_type() override {
        return operand->get_type();
    }

    void gen() override;

};


class While: public Node {
public:
    std::unique_ptr<Node> loop_stmt;
    std::unique_ptr<Expr> abool;

    While(SymbolTable &symbols, Expr *abool, Node *loop_stmt):
            Node(symbols), abool(abool), loop_stmt(loop_stmt) {}

    void gen() override;
};

class assign: public Node {
public:
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Lvalue> left;
    assign(SymbolTable &symbols,Lvalue *left, Expr *expr):
            Node(symbols), left(left), expr(expr) {}

    void gen() override;
};

class decl: public Node {
public:
    Type &type;
    Symbol &symbol;

    decl(SymbolTable &symbols, Type &type, Symbol &symbol):
            Node(symbols), type(type), symbol(symbol) {}

    void gen() override;
};


#endif //COMPILER_TREE_H
