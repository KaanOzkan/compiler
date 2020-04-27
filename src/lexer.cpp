#include "lexer.hpp"
#include "iostream"

// Add a token to tokens vector. Text is determined by start/current indices
void add_token(Lexer* l, TokenType type) {
    std::string text;
    for(int i = l->start; i < l->current+1; i++) {
        text += l->input.at(i);
    }

    Token token = {};
    token.type = type;
    token.text = text;
    token.start_pos = l->start;
    token.line = l->line;
    l->tokens.push_back(token);
}

// Consumes token and cleanup
void consume(Lexer* l) {
    Token token = l->tokens.back();
    token.end_pos = l->current;
    l->current = 0;
    l->start = 0;
}

std::vector<Token> lex(const std::vector<char> input,
                       std::vector<Token> &tokens) {
    bool encountered_error = false;

    Lexer l = {};
    l.input = input;
    l.tokens = tokens;
    l.line = 1;    // line number
    l.start = 0;   // index of first character in CURRENT lexeme
    l.current = 0; // index of character currently being looked at
    while (l.current < input.size()) {
        l.start = l.current;
        char token = input.at(l.current);

        switch(token) {
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
                break;
            default:
                encountered_error = true;
                lexer_error(l.line);
        }
    }

    return l.tokens;
}

// TODO: More specific error reporting
void lexer_error(int line) {
    std::cout << "Error: unexpected token at line " << line << std::endl;
}
