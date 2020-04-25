#include "lexer.hpp"
#include "iostream"

/* struct Lexer { */
/*     int pos; */
/*     int line; */
/*     Token* current_token; */
/* }; */

std::vector<Token> lex(const std::vector<char> input,
                       std::vector<Token> &lexemes) {
    bool encountered_error = false;

    int line = 1;    // line number
    int start = 0;   // first character in CURRENT lexeme
    int current = 0; // character currently being looked at
    while (current < input.size()) {
        start = current;
        // scan tokens

        current++; // TODO
    }

    return lexemes;
}

// TODO: More specific error reporting
void lexer_error(int line) {
    std::cout << "Error: unexpected token at line " << line << std::endl;
}
