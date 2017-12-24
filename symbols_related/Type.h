//
// Created by capitalg on 10/26/17.
//

#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include <vector>
#include <map>
#include <cassert>
#include "../tag.h"

class Type {
public:
    tag t;
    Type(tag t) : t(t) {}

    virtual ~Type() = default;


    // only for basic Type
    static const Type &type_lift(const Type &t1, const Type &t2) {
        assert(is_basic(t1) && is_basic(t2));
        if (t1 == t2) return t1;
        if (t1 < t2) return t2;
        else return t1;
    }


    bool operator==(const Type &rhs) const {
        return t == rhs.t;
    }

    bool operator!=(const Type &rhs) const {
        return !(rhs == *this);
    }

    const static Type INT_TYPE, CHAR_TYPE,
            REAL_TYPE, BOOL_TYPE;
private:
    bool operator<(const Type &rhs) const {
        static const std::map<tag, int> level{
                {tag::TYPE_BOOL, 0},
                {tag::TYPE_CHAR, 1},
                {tag::TYPE_INTEGER, 2},
                {tag::TYPE_REAL, 3}
        };
        return level.at(t) < level.at(rhs.t);
    }

    bool operator>(const Type &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Type &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Type &rhs) const {
        return !(*this < rhs);
    }
    static bool is_basic(const Type &t)  {
        return t == INT_TYPE || t == REAL_TYPE || t == CHAR_TYPE || t == BOOL_TYPE;
    }
};


// 这些对象对成员没有 归属 关系，只保留引用
class ArrayType : public Type {
public:
    const Type &element_type;
    int length;

    ArrayType(const Type &element_type, int length) :
            Type(tag::TYPE_ARRAY), length(length), element_type(element_type){}

};

class FuncType : public Type {
public:
    const Type &return_type;
    std::vector<Type*> arguments;

    FuncType(const Type &return_type, const std::vector<Type*> &arguments) : Type(tag::TYPE_FUNC),
                                                                  return_type(return_type), arguments(arguments) {}
};


#endif //COMPILER_TYPE_H
