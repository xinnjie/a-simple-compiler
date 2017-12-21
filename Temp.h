//
// Created by capitalg on 12/18/17.
//

#ifndef COMPILER_TEMP_H
#define COMPILER_TEMP_H


#include <string>

class Temp {
private:
    static int i;
    int num;
public:
    Temp();

    std::string to_string();
};


#endif //COMPILER_TEMP_H
