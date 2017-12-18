//
// Created by capitalg on 9/21/17.
//

#ifndef COMPILER_SYNTAX_ERROR_H
#define COMPILER_SYNTAX_ERROR_H
#include <exception>
#include <string>

class syntax_error: public std::exception {
public:
    syntax_error(std::string error_msg): error_msg(error_msg){}
    syntax_error(): syntax_error(""){}
    const char* what() const throw() {
        return (std::string("syntax syntax_error: ") + error_msg).c_str();
    }
    std::string error_msg;
};

class duplicate_symbol_error: public std::exception {
public:
    duplicate_symbol_error(std::string error_msg): error_msg(error_msg){}
    duplicate_symbol_error(): duplicate_symbol_error(""){}
    const char* what() const throw() {
    return (std::string("duplicate_symbol_error: ") + error_msg).c_str();
    }
    std::string error_msg;
};

class symbol_not_exist_error: public std::exception {
public:
    symbol_not_exist_error(std::string error_msg): error_msg(error_msg){}
    symbol_not_exist_error(): symbol_not_exist_error(""){}
    const char* what() const throw() {
        return (std::string("symbol_not_exist_error: ") + error_msg).c_str();
    }
    std::string error_msg;
};

class type_not_exist_error: public std::exception {
public:
    type_not_exist_error(std::string error_msg): error_msg(error_msg){}
    type_not_exist_error(): type_not_exist_error(""){}
    const char* what() const throw() {
        return (std::string("type_not_exist_error: ") + error_msg).c_str();
    }
    std::string error_msg;
};

class symbol_redefine_error: public std::exception {
public:
    symbol_redefine_error(std::string error_msg): error_msg(error_msg){}
    symbol_redefine_error(): symbol_redefine_error(""){}
    const char* what() const throw() {
        return (std::string("symbol_redefine_error: ") + error_msg).c_str();
    }
    std::string error_msg;
};

class assign_type_not_match: public std::exception {
public:
    assign_type_not_match(std::string error_msg): error_msg(error_msg){}
    assign_type_not_match(): assign_type_not_match(""){}
    const char* what() const throw() {
        return (std::string("assign_type_not_match: ") + error_msg).c_str();
    }
    std::string error_msg;
};
#endif //COMPILER_SYNTAX_ERROR_H
