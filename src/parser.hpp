#pragma once
#include "lexer.hpp"

struct Parser {
    std::vector<Token> tokens;
    unsigned long current = 0;
};

class Expression {
  public:
    virtual ~Expression(){};
    virtual std::string accept(class Visitor *) { return ""; };
};
Expression *expression(Parser *p);
Expression* parse(std::vector<Token> tokens);

