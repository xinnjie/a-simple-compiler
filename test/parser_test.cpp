//
// Created by capitalg on 11/8/17.
//

#include <iostream>
#include "../Parser.h"

using namespace std;
int main() {
//    SymbolTable test;
    SymbolTable st1;
    st1.insert(Symbol("hello", Type(tag::TYPE_CHAR)));
    try {
        st1.find("not_exist");
    }
    catch (symbol_not_exist_error  e) {
        cout << e.what() << endl;
    }

    SymbolTable st2 = SymbolTable(st1);
    st2.find("hello");
    try {
        st2.find("not_exist");
    }
    catch (symbol_not_exist_error  e) {
        cout << e.what() << endl;
    }


//    Parser destruct test
    string destruct = "int b;";
    Parser parser0(destruct);
    parser0.parse();



    string decl = "int b;"
            "real a;"
            "char c;"
            "bool t;";
    Parser parser2(decl);
    parser2.parse();


    string assign = "int b;"
            "char a;"
            "b = 1;"
            "a = 5.4;";
    Parser parser1(assign);
    parser1.parse();



    string if_else = "int a;"
            "int b;"
            "if (a > b) {"
            "   a = a - 1;"
            "   b = b + 1;"
            "}";
    Parser parser3(if_else);
    parser3.parse();

    string while_ = "int a;"
            "int b;"
            "int[5] array;"
            "while (a > b) {"
            "   a = a - 1;"
            "   b = b + 1;"
            "   array[3] = a + b;"
            "}";
    Parser parser4(while_);
    parser4.parse();


    // todo array Type

    return 0;
}
