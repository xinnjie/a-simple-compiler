//
// Created by capitalg on 12/14/17.
//

#include "TypeManager.h"
#include "../syntax_error.h"


using namespace std;
Type &TypeManager::find(const std::string &name) {
    auto it = types.find(name);
    if (it != types.end()) return *(it->second);
    throw type_not_exist_error(name);

}

ArrayType &TypeManager::create_array_type(const std::string &type_name, const Type &element_type, int length) {
    auto pair = types.emplace(type_name, make_unique<ArrayType>(element_type, length));
    auto it = pair.first;
    auto& type_ptr = it->second;
    return dynamic_cast<ArrayType&>(*type_ptr);
}

FuncType &TypeManager::create_func_type(const std::string &func_name, const Type &return_type,
                                        const std::vector<Type *> &arguments) {
    auto pair = types.emplace(func_name, make_unique<FuncType>(
            return_type, arguments
    ));
    auto it = pair.first;
    auto& type_ptr = it->second;
    return dynamic_cast<FuncType&>(*type_ptr);
}

ArrayType &TypeManager::create_array_type(const Type &element_type, int length) {
    anonymous_types.emplace_back(new ArrayType(element_type, length));
    auto& type_ptr = anonymous_types.back();
    return dynamic_cast<ArrayType&>(*type_ptr);
}

FuncType &TypeManager::create_func_type(const Type &return_type, const std::vector<Type *> &arguments) {
    anonymous_types.emplace_back(new FuncType(return_type, arguments));
    auto& type_ptr = anonymous_types.back();
    return dynamic_cast<FuncType&>(*type_ptr);
}
