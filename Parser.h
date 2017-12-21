//
// Created by capitalg on 10/25/17.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H


#include "token_related/Tokenizer.h"
#include "MiddleLanRelated/Node.h"
#include "symbols_related/Type.h"

class Parser {
private:
    Tokenizer lex;
    Token cur_token;
    unique_ptr<Stmts> prog;
    map<string, Token> keyword_more_specific;
    void move();
    void match(tag t);
    void match(const string &lexeme);
    void match(const vector<string> &lexemes);
    bool maybe(tag t);
    bool maybe(const string &lexeme);
    bool maybe(const vector<string> &lexemes);
    Token if_keyword_more_specific(const Token &t);

//    SymbolsManager symbols_manager;
//    TypeManager type_manager;
    TableManager table_manager;


public:
    explicit Parser(const string &source);

    void parse();

    //todo: change raw pointer to unique_ptr
    Stmts *match_stmts();

    Node *match_stmt();

    decl *match_decl();

    Expr *match_expr();

    Expr *match_expr(tag t);

    assign *match_assign();

    Lvalue *match_lvalue();

    Rvalue *match_rvalue();

    Type &match_type();
};


#endif //COMPILER_PARSER_H
