
#include <iostream>
#include "../symbols_related/TableManager.h"
#include "../middle_lan_related/MiddleLanOperand.h"
#include "../middle_lan_related/Node.h"

using namespace std;

int main() {
    TableManager table_manager;
    auto left_operand = make_shared<MiddleLanConstant>(Rvalue(table_manager, table_manager.TYPE_INT, "15"));
    auto a = table_manager.insert_basic_variable("a", table_manager.TYPE_INT);
    auto right_operand = make_shared<MiddleLanVariable>(a);
    auto sentence = MiddleSentence(tag::ASSIGN, left_operand, nullptr, right_operand);
    return 0;
}
