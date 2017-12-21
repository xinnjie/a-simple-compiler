//
// Created by capitalg on 12/19/17.
//

#ifndef COMPILER_TABLEMANAGER_H
#define COMPILER_TABLEMANAGER_H


#include "SymbolsManager.h"
#include "TypeManager.h"

// TableManager结合了SymbolsManager & TypeManager,方便使用时只传一个参数
class TableManager : public SymbolsManager, public TypeManager {
};


#endif //COMPILER_TABLEMANAGER_H
