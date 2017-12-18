//
// Created by capitalg on 9/21/17.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <string>

#include "../tag.h"

using namespace std;


class Token {
public:
    tag t;
    string lexeme;
    Token(tag t, string lexeme):t(t), lexeme(lexeme){}
    Token():Token(tag::NONE, "") {}
    string to_string() const{
//        return  string("<") + kw_t2s[t] + "," + std::to_string(index) + ">" + ": " + "'" + lexeme + "'";
        return string() + "<Token: " + lexeme + ">";
    }

};

//namespace std {
//
//    template <>
//    struct hash<Token>
//    {
//        std::size_t operator()(const Token& t) const
//        {
//            using std::size_t;
//            using std::hash;
//
//            // Compute individual hash values for first,
//            // second and third and combine them using XOR
//            // and bit shifting:
//
//            return ((hash<int>()(static_cast<int>(t.t))
//                     ^ (hash<int>()(t.index) << 1)) >> 1);
//        }
//    };
//
//}

//class id: public Token {
//public:
//    string value;
//};
//
//class integer: public Token {
//public:
//    int value;
//};
//
//class real: public Token {
//public:
//    float value;
//};
//
//class keyword: public Token {
//public:
//    t value;
//};

//int main() {
//    return 0;
//}
#endif //COMPILER_TOKEN_H
