//
// Created by capitalg on 10/25/17.
//

#include <iostream>
#include "Parser.h"
Parser::Parser(const string &source) { vector<Token> keywords {
            Token(tag::IF, "if"),
            Token(tag::ELSE, "else"),
            Token(tag::WHILE, "while"),
            Token(tag::FOR, "for"),
            Token(tag::SWITCH, "switch"),
            Token(tag::BREAK, "break"),
            Token(tag::CONTINUE, "continue"),

            Token(tag::RETURN, "return"),
            Token(tag::TRUE, "true"),
            Token(tag::FALSE, "false"),
            Token(tag::TYPE_CHAR, "char"),
            Token(tag::TYPE_INTEGER, "int"),
            Token(tag::TYPE_REAL, "real"),
            Token(tag::TYPE_BOOL, "bool")
    };
    for (auto keyword : keywords) {
        keyword_more_specific.insert(make_pair(keyword.lexeme, keyword));
    }
    lex.input_source(source);
    move();
}


void Parser::parse() {
    prog = unique_ptr<Stmts>(match_stmts());
}

bool Parser::maybe(tag t) {
    if (t == tag::BASIC_TYPE) {
        return (cur_token.t == tag::TYPE_INTEGER || cur_token.t == tag::TYPE_CHAR
            || cur_token.t == tag::TYPE_REAL || cur_token.t == tag::TYPE_BOOL);
    }
    if (t == tag::CONSTANT) {
        return (cur_token.t == tag::INTEGER || cur_token.t == tag::CHAR
               || cur_token.t == tag::REAL || cur_token.t == tag::BOOL);
    }
    return cur_token.t == t;
}

bool Parser::maybe(const string &lexeme) {
    return cur_token.lexeme == lexeme;
}

bool Parser::maybe(const vector<string> &lexemes) {
    return (find(lexemes.cbegin(), lexemes.cend(), cur_token.lexeme) != lexemes.cend());
}

void Parser::match(const string &lexeme) {
    if (maybe(lexeme)) move();
    else throw syntax_error(lexeme + " should be followed");
}

void Parser::match(tag t) {
    if (maybe(t)) move();
    else throw syntax_error("Type not matched");
}
void Parser::match(const vector<string> &lexemes) {
    if (maybe(lexemes)) move();
    else throw syntax_error("Type not matched");
}


void Parser::move() {
    cout << cur_token.lexeme << " matched" << endl;
    cur_token = if_keyword_more_specific(lex.produce());
}

Stmts * Parser::match_stmts() {
    Stmts *ss = new Stmts(symbols_manager.get_current_symbols());
    while (!maybe(tag::NONE) && !maybe("}")) {
        ss->insert(match_stmt());
    }
    return ss;
}




Node * Parser::match_stmt() {
    switch (cur_token.t) {
//         declaretion
        case tag::TYPE_STRING:
        case tag::TYPE_BOOL:
        case tag::TYPE_REAL:
        case tag::TYPE_CHAR:
        case tag::TYPE_INTEGER: {
            return match_decl();
        }
//            assignment
        case tag::ID: {
            return match_assign();
        }
        case tag::IF:{
            // if (abool) stmt [else stmt]
            match(tag::IF); match("(");
            auto abool = match_expr();
            match(")");
            auto true_stmt = match_stmt();
            Node *false_stmt = nullptr;
            if (cur_token.t == tag::ELSE) {
                match(tag::ELSE);
                false_stmt = match_stmt();
            }
            return new IfElse(symbols_manager.get_current_symbols(), abool, true_stmt, false_stmt);
        }
        case tag::WHILE:{
//            while (abool) stmt
            match(tag::WHILE); match("(");
            Expr *abool = match_expr();
            match(")");
            Node *stmt = match_stmt();
            return new While(symbols_manager.get_current_symbols(), abool, stmt);
        }
        case tag::DELIMITER: {
            match("{");
            symbols_manager.step_in();
            Node *ss = match_stmts();
            match("}");
            symbols_manager.step_back();
            return ss;
        }
        default:{
            throw syntax_error();
        }
    }
}

Token Parser::if_keyword_more_specific(const Token &t) {
    if (t.t == tag::KEYWORD) {
        return keyword_more_specific[t.lexeme];
    }
    // todo: other than keywords?
    return t;
}

decl *Parser::match_decl() {
    Type &t = match_type();
    Symbol s(cur_token.lexeme, t);
    match(tag::ID);
    if (symbols_manager.contains_cur_field(s.name)) throw symbol_redefine_error(s.name);

    symbols_manager.insert(s);
    match(";");
    return new decl(symbols_manager.get_current_symbols(), t, s);

}

Type &Parser::match_type() {
    tag category = cur_token.t;
    match(tag::BASIC_TYPE);
//    array Type
// todo: only support one dimension array of basic Type;
    if (maybe("[")) {
        match("[");
        int length = stoi(cur_token.lexeme);
        match(tag::INTEGER);
        match("]");
        return type_manager.create_array_type(type_manager.get_basic_type(category), length);
    }
    return type_manager.get_basic_type(category);
}

Expr * Parser::match_expr() {
    return match_expr(tag::BOOL);
}

Expr * Parser::match_expr(tag t) {
    static const map<tag, tag> next{
            {tag::BOOL, tag::JOIN},
            {tag::JOIN, tag::EQUALITY},
            {tag::EQUALITY, tag::REL},
            {tag::REL, tag::EXPR},
            {tag::EXPR, tag::TERM},
            {tag::TERM, tag::UNARY},
            {tag::UNARY, tag::FACTOR},
    };

    static const map<string, tag> lexeme2tag {
            {"||", tag::OR},
            {"&&", tag::AND},
            {"!", tag::REVERSE},
            {"==", tag::EQ},
            {"!=", tag::NE},
            {"<", tag::LT},
            {"<=", tag::LE},
            {">", tag::GT},
            {">=", tag::GE},
            {"+", tag::ADD},
            {"-", tag::MINUS},
            {"*", tag::MULTIPLY},
            {"/", tag::DEVIDE},
    };

    typedef string lexeme;
    multimap<tag, lexeme> matching_lexemes {
            {tag::BOOL, "||"},
            {tag::JOIN, "&&"},
            {tag::EQUALITY, "=="},
            {tag::REL, "<"},
            {tag::EXPR,"+"},
            {tag::TERM, "*"},
            {tag::UNARY, "!"},
    };
    matching_lexemes.insert({tag::EQUALITY, "!="});
    matching_lexemes.insert({tag::REL, ">"});
    matching_lexemes.insert({tag::REL, ">="});
    matching_lexemes.insert({tag::REL, "<="});
    matching_lexemes.insert({tag::EXPR, "-"});
    matching_lexemes.insert({tag::TERM, "/"});
    matching_lexemes.insert({tag::UNARY, "-"});


    bool match_once = false;

    switch (t) {
        case tag::EQUALITY:
        case tag::REL: match_once = true;
// { } means optional
//          match  join {|| join}
        case tag::BOOL:

//          match  equality {&& equality}
        case tag::JOIN:

//            match term {+/- term}
        case tag::EXPR:

//          match unary {*// unary}
        case tag::TERM: {
//            next Type to match
            tag to_match = next.at(t);
            Expr *left = match_expr(to_match);
            BinaryOpt *bi_opt = nullptr;
            bool meet_first = true;
            auto range = matching_lexemes.equal_range(t);

            while (find_if(range.first, range.second, [this](const multimap<tag, lexeme>::value_type &rhs) {
                return rhs.second == cur_token.lexeme;
            })  != range.second ) {
                string opt_lexeme = cur_token.lexeme;
                tag opt_tag = lexeme2tag.at(opt_lexeme);
                move();
                Expr *right = match_expr(to_match);
                if (meet_first) {
                    bi_opt = new BinaryOpt(symbols_manager.get_current_symbols(), opt_tag, left, right);
                    meet_first = false;
                    if (match_once) {
                        return  bi_opt;
                    }
                }
                else {
                    bi_opt = new BinaryOpt(symbols_manager.get_current_symbols(), opt_tag, bi_opt, right);
                }
            }
            if (bi_opt) return bi_opt;
            return left;
        }

//            !/- factor
        case tag::UNARY: {
            if (maybe("!") || maybe("-")) {
                return new UnaryOpt(symbols_manager.get_current_symbols(), lexeme2tag.at(cur_token.lexeme), match_expr(tag::FACTOR));
            }
            else return match_expr(tag::FACTOR);
        }
        case tag::FACTOR: {
            switch (cur_token.t) {
                case tag::DELIMITER: {
                    match("(");
                    Expr *bool_ = match_expr(tag::BOOL);
                    match(")");
                    return bool_;
                }
                case tag::ID: {
                    return match_lvalue();
                }
                case tag::INTEGER:
                case tag::CHAR:
                case tag::REAL: {
                    return match_rvalue();
                }
                default:{
                    throw syntax_error();
                }
            }
        }
        default: {
            throw  syntax_error();
        }
    }
}

assign * Parser::match_assign() {
    Lvalue *left = match_lvalue();
//    todo check left Type & right Type;
    match("=");
    Expr *right_expr = match_expr();
    //todo:
//    if (!Type::type_fit_assign(left, right_expr)) throw assign_type_not_match();
    match(";");
    return new assign(symbols_manager.get_current_symbols(), left, right_expr);
}

Lvalue * Parser::match_lvalue() {
    string may_id = cur_token.lexeme;
    match(tag::ID);
    auto symbol = symbols_manager.find(may_id);
    if (maybe("[")) {
//        is array
        match("[");
        Expr *expr = match_expr();
        match("]");
        return new Lvalue(symbols_manager.get_current_symbols(), symbol, expr);
    }
    return new Lvalue(symbols_manager.get_current_symbols(), symbol);
}

Rvalue * Parser::match_rvalue() {
    const string lexeme = cur_token.lexeme;
    tag t = cur_token.t;
    match(tag::CONSTANT);
    switch (t) {
        case tag::INTEGER: {
            return new Rvalue(symbols_manager.get_current_symbols(), Type::INT_TYPE, lexeme);
        }
        case tag::CHAR: {
            return new Rvalue(symbols_manager.get_current_symbols(), Type::CHAR_TYPE, lexeme);
        }
        case tag::REAL: {
            return new Rvalue(symbols_manager.get_current_symbols(), Type::REAL_TYPE, lexeme);
        }
        case tag::BOOL: {
            return new Rvalue(symbols_manager.get_current_symbols(), Type::BOOL_TYPE, lexeme);
        }
        default: {
            throw syntax_error("constant Type not matched");
        }
    }
}








