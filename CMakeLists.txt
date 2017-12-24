cmake_minimum_required(VERSION 3.7)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

set(LEX_SOURCE_FILES exercise_during_the_lesson/main.cpp token_related/Tokenizer.cpp syntax_error.h tag.h token_related/Token.h exercise_during_the_lesson/LL1.cpp exercise_during_the_lesson/LL1.h)
add_executable(lexer ${LEX_SOURCE_FILES})

set(LL1_SOURCE_FILES token_related/Tokenizer.cpp syntax_error.h tag.h token_related/Token.h exercise_during_the_lesson/LL1.cpp exercise_during_the_lesson/LL1.h exercise_during_the_lesson/LL1_main.cpp)
add_executable(ll1 ${LL1_SOURCE_FILES})

set(RECURSIVE_SOURCE_FILES exercise_during_the_lesson/recursive_main.cpp token_related/Tokenizer.cpp syntax_error.h tag.h token_related/Token.h exercise_during_the_lesson/recursive_exp.cpp exercise_during_the_lesson/recursive_exp.h)
add_executable(recursive_exp ${RECURSIVE_SOURCE_FILES})

set(LR_SOURCE_FILES token_related/Tokenizer.cpp syntax_error.h tag.h token_related/Token.h exercise_during_the_lesson/SLR.cpp exercise_during_the_lesson/SLR.h exercise_during_the_lesson/SLR_main.cpp)
add_executable(lr ${LR_SOURCE_FILES})


set(PARSER_SOURCE_FILES syntax_error.h token_related/Token.h Parser.cpp Parser.h middle_lan_related/Node.cpp middle_lan_related/Node.h symbols_related/SymbolTable.cpp symbols_related/SymbolTable.h symbols_related/Type.cpp symbols_related/Type.h token_related/Tokenizer.cpp token_related/Tokenizer.h middle_lan_related/MiddleSentence.cpp middle_lan_related/MiddleSentence.h Temp.cpp Temp.h middle_lan_related/MiddleLanOperand.cpp middle_lan_related/MiddleLanOperand.h)
add_executable(parser ${PARSER_SOURCE_FILES} )

set(PARSER_TEST_SOURCE_FILES symbols_related/Type.cpp symbols_related/Type.h syntax_error.h token_related/Token.h Parser.cpp Parser.h test/parser_test.cpp middle_lan_related/Node.cpp middle_lan_related/Node.h middle_lan_related/MiddleSentence.cpp middle_lan_related/MiddleSentence.h middle_lan_related/MiddleLanOperand.cpp middle_lan_related/MiddleLanOperand.cpp symbols_related/SymbolTable.cpp symbols_related/SymbolTable.h token_related/Tokenizer.cpp token_related/Tokenizer.h symbols_related/SymbolsManager.cpp symbols_related/SymbolsManager.h symbols_related/TypeManager.cpp symbols_related/TypeManager.h symbols_related/TableManager.cpp symbols_related/TableManager.h)
add_executable(parser_test ${PARSER_TEST_SOURCE_FILES} )

set(MIDDLE_LAN_TEST symbols_related/Type.cpp symbols_related/Type.h syntax_error.h token_related/Token.h Parser.cpp Parser.h middle_lan_related/Node.cpp middle_lan_related/Node.h middle_lan_related/MiddleLanOperand.cpp middle_lan_related/MiddleLanOperand.h middle_lan_related/MiddleSentence.h middle_lan_related/MiddleSentence.cpp symbols_related/SymbolTable.cpp symbols_related/SymbolTable.h token_related/Tokenizer.cpp token_related/Tokenizer.h symbols_related/SymbolsManager.cpp symbols_related/SymbolsManager.h symbols_related/TypeManager.cpp symbols_related/TypeManager.h symbols_related/TableManager.cpp symbols_related/TableManager.h test/middle_lan_test.cpp)
add_executable(middle_lan_test ${MIDDLE_LAN_TEST})