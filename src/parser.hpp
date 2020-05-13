#pragma once
#include "lexer.hpp"

struct Parser {
    std::vector<Token> tokens;
    unsigned long current = 0;
};

void parse(std::vector<Token> tokens);

