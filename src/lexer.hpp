#pragma once

#include <vector>
#include <string>

// TODO: Extend
enum TokenType {
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    SEMICOLON,
    EQUAL,
    IDENTIFIER,
    STRING_LITERAL,
    NUMBER,
    KEYWORD
};

struct Token {
    TokenType type;
    int start_pos;
    int end_pos;
    std::string lexeme;
};

std::vector<Token> lex(std::vector<char> input, std::vector<Token> &lexemes);

void lexer_error(int line);
