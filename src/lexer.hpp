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
    SLASH,
    BANG,
    EQUAL,
    BANG_EQUAL,
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
    int column;
    Token* current_token;
    std::vector<char> input;
    std::vector<Token> tokens;
};

void add_token(Lexer* l, const TokenType type);
void consume(Lexer* l);
void advance(Lexer *l);
char peek(const Lexer l);
std::vector<Token> lex(std::vector<char> input, std::vector<Token> &lexemes);
void lexer_error(int line, int column);
