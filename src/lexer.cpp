#include "lexer.hpp"

std::vector<std::string> lex(const std::string filename,
                             std::vector<std::string> &lexemes) {
    lexemes.push_back("first token");

    return lexemes;
}
