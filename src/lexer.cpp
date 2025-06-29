#include "lexer.hpp"
#include <iostream>
#include <unordered_map>

// clang-format off
#define DIGIT                                                                  \
    '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6'      \
        : case '7' : case '8' : case '9'

#define ALPHA                                                                  \
    'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' : case 'g'      \
        : case 'h' : case 'i' : case 'j' : case 'k' : case 'l' : case 'm'      \
        : case 'n' : case 'o' : case 'p' : case 'q' : case 'r' : case 's'      \
        : case 't' : case 'u' : case 'v' : case 'w' : case 'x' : case 'y'      \
        : case 'z' : case 'A' : case 'B' : case 'C' : case 'D' : case 'E'      \
        : case 'F' : case 'G' : case 'H' : case 'I' : case 'J' : case 'K'      \
        : case 'L' : case 'M' : case 'N' : case 'O' : case 'P' : case 'Q'      \
        : case 'R' : case 'S' : case 'T' : case 'U' : case 'V' : case 'W'      \
        : case 'X' : case 'Y' : case 'Z'
// clang-format on

/* struct Keyword { */
/*     std::string text; */
/*     TokenType type; */
/* }; */

// TODO: Extend
static std::unordered_map<std::string, TokenType> keywords = {
    {"auto", AUTO},   {"true", TRUE},     {"false", FALSE}, {"break", BREAK},
    {"char", CHAR},   {"const", CONST},   {"do", DO},       {"double", DOUBLE},
    {"else", ELSE},   {"float", FLOAT},   {"for", FOR},     {"if", IF},
    {"int", INT},     {"long", LONG},     {"NULL", _NULL},  {"return", RETURN},
    {"short", SHORT}, {"struct", STRUCT}, {"void", VOID},   {"while", WHILE}};

// Add a token to tokens vector. Text is determined by start/current indices
void add_token(Lexer *l, const TokenType type) {
    std::string text;
    for (unsigned long i = l->start; i < l->current + 1; i++) {
        text += l->input.at(i);
    }

    Token token = {};
    token.type = type;
    token.text = text;
    token.start_pos = l->start;
    token.line = l->line;
    l->tokens.push_back(token);
}

// Finalize token information before advancing
// TODO: Delete if end_pos is unused
void consume(Lexer *l) {
    Token token = l->tokens.back();
    token.end_pos = l->current;
}

void advance(Lexer *l) {
    l->current++;
    l->column++;
}

char peek(const Lexer l, const unsigned long offset) {
    if (l.current + offset > l.input.size()) {
        return 0;
    }
    return l.input.at(l.current + offset);
}

void lex_string(Lexer *l) {
    while (peek(*l) && peek(*l) != '"') {
        if (peek(*l) == '\n') {
            l->line++;
        }
        advance(l);
    }

    if (l->current > l->input.size()) {
        lexer_error(l->line, l->column);
        return;
    }

    advance(l);
    // custom add_token()
    std::string text;
    for (unsigned long i = l->start + 1; i < l->current; i++) {
        text += l->input.at(i);
    }

    Token token = {};
    token.type = STRING;
    token.text = text;
    token.start_pos = l->start;
    token.line = l->line;
    l->tokens.push_back(token);

    consume(l);
}

bool is_digit(const char c) {
    switch (c) {
    case DIGIT:
        return true;
    default:
        return false;
    }
}

void lex_number(Lexer *l) {
    while (peek(*l) && is_digit(peek(*l))) {
        advance(l);
    }

    if (peek(*l) == '.' && is_digit(peek(*l, 2))) {
        advance(l);
        while (peek(*l) && is_digit(peek(*l))) {
            advance(l);
        }
    }

    add_token(l, NUMBER);
    consume(l);
}

bool is_alphanumeric(const char c) {
    switch (c) {
    case ALPHA:
    case DIGIT:
        return true;
    default:
        return false;
    }
}

bool is_keyword(std::string text) {
    if (keywords[text]) {
        return true;
    }

    return false;
}

void lex_alphanumeric(Lexer *l) {
    while (is_alphanumeric(peek(*l))) {
        advance(l);
    }

    std::string text;
    for (unsigned long i = l->start; i < l->current + 1; i++) {
        text += l->input.at(i);
    }

    TokenType type;
    if (is_keyword(text)) {
        type = keywords[text];
    } else {
        type = IDENTIFIER;
    }

    add_token(l, type);
    consume(l);
}

std::vector<Token> lex(const std::vector<char> input,
                       std::vector<Token> &tokens) {
    bool encountered_error = false;

    Lexer l = {};
    l.input = input;
    l.tokens = tokens;
    l.line = 1;   // line number
    l.column = 1; // column number
    l.start = 0;  // index of first character in token (relative to input)
    l.current =
        0; // index of character currently being looked at (relative to input)
    while (l.current < input.size()) {
        l.start = l.current;
        char token = input.at(l.current);

        switch (token) {
        case '(':
            add_token(&l, LEFT_PAREN);
            consume(&l);
            break;
        case ')':
            add_token(&l, RIGHT_PAREN);
            consume(&l);
            break;
        case '{':
            add_token(&l, LEFT_BRACE);
            consume(&l);
            break;
        case '}':
            add_token(&l, RIGHT_BRACE);
            consume(&l);
            break;
        case ',':
            add_token(&l, COMMA);
            consume(&l);
            break;
        case '.':
            add_token(&l, DOT);
            consume(&l);
            break;
        case '-':
            add_token(&l, MINUS);
            consume(&l);
            break;
        case '+':
            add_token(&l, PLUS);
            consume(&l);
            break;
        case ';':
            add_token(&l, SEMICOLON);
            consume(&l);
            break;
        case '*':
            add_token(&l, STAR);
            consume(&l);
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            l.line++;
            l.column = 0;
            break;
        case '!':
            if (peek(l) == '=') {
                advance(&l);
                add_token(&l, BANG_EQUAL);
            } else {
                add_token(&l, BANG);
                consume(&l);
            }
            break;
        case '=':
            if (peek(l) == '=') {
                advance(&l);
                add_token(&l, EQUAL_EQUAL);
            } else {
                add_token(&l, EQUAL);
                consume(&l);
            }
            break;
        case '>':
            if (peek(l) == '=') {
                advance(&l);
                add_token(&l, GREATER_EQUAL);
            } else {
                add_token(&l, GREATER);
                consume(&l);
            }
            break;
        case '<':
            if (peek(l) == '=') {
                advance(&l);
                add_token(&l, LESS_EQUAL);
            } else {
                add_token(&l, LESS);
                consume(&l);
            }
            break;
        case '/':
            if (peek(l) == '/') {
                while (peek(l) && peek(l) != '\n') {
                    advance(&l);
                }
            } else {
                add_token(&l, SLASH);
                consume(&l);
            }
            break;
        case '"':
            lex_string(&l);
            break;
        case DIGIT:
            lex_number(&l);
            break;
        case ALPHA:
            lex_alphanumeric(&l);
            break;
        default:
            encountered_error = true;
            lexer_error(l.line, l.column);
        }
        advance(&l);
    }

    if (encountered_error) {
        throw std::runtime_error(
            "Lexer error encountered. Terminating compilation");
    }
    return l.tokens;
}

void lexer_error(int line, int column) {
    std::cout << "Error: unexpected token at " << line << ":" << column
              << std::endl;
}
