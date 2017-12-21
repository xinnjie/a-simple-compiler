//
// Created by capitalg on 9/21/17.
//

#include <set>
#include <iostream>
#include "Tokenizer.h"
using namespace std;

map<tag, std::string> Tokenizer::kw_t2s{
        {tag::INTEGER, "integer"},
        {tag::REAL, "real"},
        {tag::CHAR, "char"},
        {tag::STRING, "string"},
        {tag::KEYWORD, "keyword"},
        {tag::DELIMITER, "delimiter"},
        {tag::ID, "id"},
        {tag::NONE, "NONE"}

};

map<std::string, tag> Tokenizer::kw_s2t {
//        {"true", tag::TRUE},
//        {"false", tag::FALSE},


        {"int", tag::TYPE_INTEGER},
        {"real", tag::TYPE_REAL},
        {"string", tag::TYPE_STRING},
        {"char", tag::TYPE_CHAR},
        {"bool", tag::TYPE_BOOL},

        {"return", tag::RETURN},
        {"if", tag::IF},
        {"else", tag::ELSE},
        {"for", tag::FOR},
        {"while", tag::WHILE},
        {"switch", tag::SWITCH},

        {"break", tag::BREAK},
        {"continue", tag::CONTINUE}



};
set<char> Tokenizer::single_delimiters{
        '-', '+', '*', '/',
        '<', '>',
        '=',
        '!',
        '&', '|',
        '{', '}',
        '(', ')',
        ',', ';',
        '[',']'
};

std::set<char> Tokenizer::may_dual_delimiters {
        '-', '+', '*', '/',
        '<', '>',
        '=',
        '!',
        '&', '|'
};
Tokenizer::Tokenizer(): cur_state(0){
}
Token Tokenizer::scan() {

    cur_state = static_cast<int>(states::INIT_STATE);
    cur_word = "";
    while (cur_state != static_cast<int>(states::LEX_END_STATE)) {
        char peek = (char)source_stream.peek();
        switch (cur_state) {
            case static_cast<int>(states::INIT_STATE):
                if (isspace(peek)) {
                    cur_state = static_cast<int>(states::INIT_STATE);
                    // skip all the space; so we do not use getch()
                    source_stream.get();
                }
                else if (isalpha(peek)|| peek == '_') cur_state = static_cast<int>(states::L_STATE);
                else if (isdigit(peek)) cur_state = static_cast<int>(states::N_STATE);
                else if (peek == '-') {
                    cur_state = static_cast<int>(states::MINUS_STATE);
                    putch();
                }
                else if (is_delimiters(peek)) cur_state = static_cast<int>(states::DELIMITER_STATE);
                else {
                    switch (peek) {
                        case '\'': {
                            cur_state = static_cast<int>(states::CHAR_STATE);
                            break;
                        }
                        case '"': {
                            cur_state = static_cast<int>(states::STRING_STATE);
                            break;
                        }
                        default: {
                            throw syntax_error(string("illegal input: <char> ") + peek);
                        }
                    }

                }
                break;
            case static_cast<int>(states::L_STATE):  // match letter and keywords
                return match_id_keyword();
            case static_cast<int>(states::N_STATE):  // match numbers
                return match_num();
            case static_cast<int>(states::DELIMITER_STATE):
                return match_delimiter();
            case static_cast<int>(states::STRING_STATE):
                return match_string();
            case static_cast<int>(states::CHAR_STATE):
                return match_char();
            case static_cast<int>(states::MINUS_STATE): {
                if (isdigit(peek)) return match_num();
                else return match_delimiter();
            }
            default:
                throw syntax_error("no matching state");
        }
    }
    throw syntax_error("no matching Token");


}


string Tokenizer::produce(const string &source) {
    source_stream = stringstream(source);
    while (!source_stream.eof()) {
        auto t = scan();

        cout << t.to_string() << " " <<  endl;

    }
    return std::string();
}


Token Tokenizer::match_id_keyword() {
    if (isalpha(source_stream.peek()) || source_stream.peek() == '_')  putch();


    while (isalnum(source_stream.peek()) || source_stream.peek() == '_') {
        putch();
    }
    if (in_keywords(cur_word)) return Token(tag::KEYWORD, cur_word);

    return Token(tag::ID, cur_word);
}


// match positive number, if the number is negtive, then that has been taken cared during the ::method:: scan()
Token Tokenizer::match_num() {
//
    cur_state = 0;
    while (cur_state != 2 && cur_state != 3) {
        char peek = (char)source_stream.peek();
        switch (cur_state) {
            case 0:
                if (isdigit(peek)) cur_state = 0;  // still integer
                else if (peek == '.') cur_state = 1; // is float
                else cur_state = 2;
                break;
            case 1:
                if (isdigit(peek)) {
                    cur_state = 1;
                }
                else  {
                    cur_state = 3;
                }
                break;
            case 2:
                // state 2 is the integer end state
                break;
            case 3:
                //  state 3 is float end state
                break;
            default:
                cout << "something wrong during match num" << endl;
        }
        if (cur_state != 2  && cur_state != 3) putch();
    }
    if (cur_state == 2) return Token(tag::INTEGER, cur_word);
    return Token(tag::REAL,  cur_word);

}

Token Tokenizer::match_char() {
    if (source_stream.peek() == '\'') {
        putch();
    }
    cur_word.push_back(give_back_printable());

    if (source_stream.peek() == '\'') {
        putch();
    }
    else throw syntax_error("char missing '''");
    return Token(tag::CHAR, cur_word);
}

Token Tokenizer::match_string() {
    cur_state = 0;
    if (source_stream.peek() == '"') {
        putch();
    }
    while (source_stream.peek() != '"' && !source_stream.eof()) cur_word.push_back(give_back_printable());
    if (source_stream.eof()) throw syntax_error("missing \"");
    putch();
    return Token(tag::STRING, cur_word);
}

Token Tokenizer::match_delimiter() {
    static set<char> may_dual{
            '+', '*', '/',
            '<', '>',
            '=',
            '!'
    };

    cur_state = 0;
    // if cur_word == "-", then state is switch to match "=" state
    if (cur_word == "-") cur_state = 2;
    while (cur_state != static_cast<int>(states::TERMINATE_STATE)) {
        char peek = source_stream.peek();
        switch (cur_state) {
            case 0: {
                if (may_dual.find(peek) != may_dual.end()) cur_state = 2;
                else if (peek == '&') cur_state = 3;
                else if (peek == '|') cur_state = 4;
                else cur_state = 1;
                break;
            }
            case 1: {
                cur_state = static_cast<int>(states::TERMINATE_STATE);
                break;
            }
            case 2 : {
                if (source_stream.peek() == '=') cur_state = 1;
                else cur_state = static_cast<int>(states::TERMINATE_STATE);
                break;
            }
            case 3: {
                if (source_stream.peek() == '&') cur_state = 1;
                else cur_state = static_cast<int>(states::TERMINATE_STATE);
                break;
            }
            case 4: {
                if (source_stream.peek() == '|') cur_state = 1;
                else cur_state = static_cast<int>(states::TERMINATE_STATE);
            }
            default: {
                throw syntax_error(string("can not match delimiter ") + peek);
            }
        }
        // if cur_word == "-" , because it is put in during the scan(), we do not need put it again.// todo minus problem
        if (cur_state != static_cast<int>(states::TERMINATE_STATE) ||  (cur_word == "-" && isdigit(source_stream.peek()))) putch();
    }

    return Token(tag::DELIMITER, cur_word);
}


// return every printable as char
char Tokenizer::give_back_printable(){
    int state = 0;
    static map<char, char> escapes{
            {'b', '\b'},
            {'n', '\n'},
            {'t', '\t'},
            {'\\','\\'},
            {'\'', '\''},
            {'\"', '\"'},
    };
    char result = 0;
    while (state != 3) {
        char peek = source_stream.peek();
        switch (state) {
            case 0: {
                if (peek == '\\') {
                    state = 2;
                }
                else if (isprint(peek) || isspace(peek)){
                    result = peek;
                    state = 1;
                } else throw syntax_error("find_type unprintable character");
                break;
            }
            case 1: {
                state = 3; // 3 as terminated state
                break;
            }
            case 2: {
                if (escapes.find(peek) != escapes.end()) {
                    result = escapes[peek];
                    state = 1;
                }
                else {
                    throw syntax_error("unknown escape");
                }
                break;
            }
        }
        if (state != 3) {
            source_stream.get();
        }
    }

    return result;
}

Token Tokenizer::produce() {
    if (!source_stream.eof()) {
        auto t = scan();
        return t;
    }
    return Token();
}

void Tokenizer::input_source(const string &source) {
    source_stream = stringstream(source);
}




