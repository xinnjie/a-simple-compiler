//
// Created by capitalg on 9/21/17.
//

#ifndef COMPILER_LEX_H
#define COMPILER_LEX_H
#include <string>
#include <sstream>
#include <set>
#include <utility>
#include <map>
#include <vector>

#include "../syntax_error.h"
#include "Token.h"
#include "../tag.h"


class Tokenizer {
public:
    Tokenizer();
    // for test
    std::string produce(const std::string &source);

    // generate single Token each time
    void input_source(const std::string &source);
    Token produce();
    int cur_state;


private:
    static std::map<tag, std::string> kw_t2s;
    static std:: map<std::string, tag> kw_s2t;
    static std::set<char> single_delimiters;
    static std::set<char> may_dual_delimiters;

    std::string cur_word;
    char cur_char;
    std::stringstream source_stream;


    Token scan();


    void putch() {
        cur_char = source_stream.get();
        cur_word.push_back(cur_char);
    }


    bool in_keywords(std::string word) {
        return kw_s2t.find(word) != kw_s2t.end();
    }


    bool is_delimiters(char ch) {
        return single_delimiters.find(ch) != single_delimiters.end();
    }

    bool maybe_dual_delimiters(char ch) {
        return may_dual_delimiters.find(ch) != may_dual_delimiters.end();
    }


    Token match_id_keyword();
    Token match_num();
    Token match_char();
    Token match_string();
    Token match_delimiter();
    char give_back_printable();


};


enum class states {
    INIT_STATE=0, L_STATE, N_STATE, CHAR_STATE, STRING_STATE, TERMINATE_STATE, DELIMITER_STATE, MINUS_STATE,
    LEX_END_STATE=100,
};


#endif //COMPILER_LEX_H
