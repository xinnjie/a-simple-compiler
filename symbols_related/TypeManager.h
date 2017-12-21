//
// Created by capitalg on 12/14/17.
//

#ifndef COMPILER_TYPEMANAGER_H
#define COMPILER_TYPEMANAGER_H


#include <string>
#include "Type.h"
#include "../syntax_error.h"

// TypeManager & SymbolsManager 协同工作
class TypeManager {
public:
    Type &TYPE_INT, &TYPE_BOOL, &TYPE_CHAR, &TYPE_REAL, &TYPE_ARRAY, &TYPE_STRUCT, &TYPE_FUNC;

    std::map<std::string, std::unique_ptr<Type>> types;
    std::vector<std::unique_ptr<Type>> anonymous_types;

    Type &find_type(const std::string &name);

    ArrayType &create_array_type(const std::string &type_name, const Type &element_type, int length);

    FuncType &create_func_type(const std::string &func_name, const Type &return_type, const std::vector<Type*> &arguments);

    ArrayType &create_array_type(const Type &element_type, int length);

    FuncType &create_func_type(const Type &return_type, const std::vector<Type*> &arguments);

    Type &get_basic_type(tag t) {
        switch (t) {
            case tag::TYPE_BOOL:
                return find_type("bool");
            case tag::TYPE_INTEGER:
                return find_type("int");
            case tag::TYPE_CHAR:
                return find_type("char");
            case tag::TYPE_REAL:
                return find_type("real");
            default:
                throw type_not_exist_error("is not basic type");
        }
    }

    TypeManager() : TYPE_INT(*(new Type(tag::TYPE_INTEGER))),
                    TYPE_BOOL(*(new Type(tag::TYPE_BOOL))),
                    TYPE_CHAR(*(new Type(tag::TYPE_CHAR))),
                    TYPE_REAL(*(new Type(tag::TYPE_REAL))),
                    TYPE_ARRAY(*(new Type(tag::TYPE_ARRAY))),
                    TYPE_STRUCT(*(new Type(tag::TYPE_STRUCT))),
                    TYPE_FUNC(*(new Type(tag::TYPE_FUNC))) {
//        types["int"] = std::make_unique<Type>(tag::TYPE_INTEGER);
        types["int"] = std::unique_ptr<Type>(&TYPE_INT);
//        types["real"] = std::make_unique<Type>(tag::TYPE_REAL);
        types["real"] = std::unique_ptr<Type>(&TYPE_REAL);
//        types["char"] = std::make_unique<Type>(tag::TYPE_CHAR);
        types["char"] = std::unique_ptr<Type>(&TYPE_CHAR);
//        types["bool"] = std::make_unique<Type>(tag::TYPE_BOOL);
        types["bool"] = std::unique_ptr<Type>(&TYPE_BOOL);
//        types["array"] = std::make_unique<Type>(tag::TYPE_ARRAY);
        types["array"] = std::unique_ptr<Type>(&TYPE_ARRAY);
        types["struct"] = std::unique_ptr<Type>(&TYPE_STRUCT);
        types["func"] = std::unique_ptr<Type>(&TYPE_FUNC);
    }

};


#endif //COMPILER_TYPEMANAGER_H
