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
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    STAR,
    EQUAL,
    IDENTIFIER,
    STRING_LITERAL,
    NUMBER,
    KEYWORD
};

struct Token {
    TokenType type;
    std::string text;
    int start_pos;
    int end_pos;
    int line;
};

struct Lexer {
    int line;
    int current;
    int start;
    Token* current_token;
    std::vector<char> input;
    std::vector<Token> tokens;
};

void add_token(Lexer* l, TokenType type);
void consume(Lexer* l);
std::vector<Token> lex(std::vector<char> input, std::vector<Token> &lexemes);
void lexer_error(int line);
