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


set(PARSER_SOURCE_FILES syntax_error.h token_related/Token.h Parser.cpp Parser.h MiddleLanRelated/Node.cpp MiddleLanRelated/Node.h symbols_related/SymbolTable.cpp symbols_related/SymbolTable.h symbols_related/Type.cpp symbols_related/Type.h token_related/Tokenizer.cpp token_related/Tokenizer.h MiddleLanRelated/MiddleSentence.cpp MiddleLanRelated/MiddleSentence.h Temp.cpp Temp.h MiddleLanRelated/MiddleLanOperand.cpp MiddleLanRelated/MiddleLanOperand.h)
add_executable(parser ${PARSER_SOURCE_FILES} )

set(PARSER_TEST_SOURCE_FILES symbols_related/Type.cpp symbols_related/Type.h syntax_error.h token_related/Token.h Parser.cpp Parser.h test/parser_test.cpp MiddleLanRelated/Node.cpp MiddleLanRelated/Node.h symbols_related/SymbolTable.cpp symbols_related/SymbolTable.h token_related/Tokenizer.cpp token_related/Tokenizer.h symbols_related/SymbolsManager.cpp symbols_related/SymbolsManager.h symbols_related/TypeManager.cpp symbols_related/TypeManager.h symbols_related/TableManager.cpp symbols_related/TableManager.h)
add_executable(parser_test ${PARSER_TEST_SOURCE_FILES} )