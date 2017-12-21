//
// Created by capitalg on 12/18/17.
//

#include "Temp.h"

Temp::Temp() {
    num = i++;
}

std::string Temp::to_string() {
    return std::string("$") + std::to_string(num);
}
