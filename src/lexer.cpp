#include "lexer.hpp"
#include "iostream"

// Add a token to tokens vector. Text is determined by start/current indices
void add_token(Lexer* l, const TokenType type) {
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

// Finalize token information before advancing
// TODO: Delete if end_pos is unused
void consume(Lexer* l) {
    Token token = l->tokens.back();
    token.end_pos = l->current;
}

void advance(Lexer *l) {
    l->current++;
    l->column++;
}

char peek(const Lexer l) {
    if(l.current+1 > l.input.size()) {
        return 0;
    }
    return l.input.at(l.current+1);
}

std::vector<Token> lex(const std::vector<char> input,
                       std::vector<Token> &tokens) {
    bool encountered_error = false;

    Lexer l = {};
    l.input = input;
    l.tokens = tokens;
    l.line = 1;    // line number
    l.column = 1;  // column number
    l.start = 0;   // index of first character in token (relative to input)
    l.current = 0; // index of character currently being looked at (relative to input)
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
                l.column = 0;
                break;
            case '!':
                if(peek(l) == '=') {
                    advance(&l);
                    add_token(&l, BANG_EQUAL);
                } else {
                    add_token(&l, BANG);
                    consume(&l);
                }
                break;
            case '/':
                if(peek(l) == '/') {
                    while(peek(l) && peek(l) != '\n') {
                        advance(&l);
                    }
                } else {
                    add_token(&l, SLASH);
                    consume(&l);
                }
                break;
            default:
                encountered_error = true;
                lexer_error(l.line, l.column);
        }
        advance(&l);
    }

    return l.tokens;
}

void lexer_error(int line, int column) {
    std::cout << "Error: unexpected token at " << line << ":" << column << std::endl;
}
