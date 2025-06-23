#pragma once
#include "lexer.hpp"
#include <memory>

struct Parser {
    std::vector<Token> tokens;
    unsigned long current = 0;
};

class Expression {
  public:
    virtual ~Expression(){};
    virtual std::string accept(class Visitor *) { return ""; };
};

class Binary : public Expression {
  public:
    std::unique_ptr<Expression> left;
    Token oprt;
    std::unique_ptr<Expression> right;

    Binary(std::unique_ptr<Expression> left, Token oprt, std::unique_ptr<Expression> right)
        : left(std::move(left)), oprt(oprt), right(std::move(right)) {}

    std::string accept(class Visitor *);
};

class Grouping : public Expression {
  public:
    std::unique_ptr<Expression> inner_expr;
    Grouping(std::unique_ptr<Expression> inner_expr) : inner_expr(std::move(inner_expr)) {}

    std::string accept(class Visitor *);
};

enum LiteralType { LT_NUMBER, LT_STRING, LT_BOOLEAN, LT_NIL };

class Literal : public Expression {
  public:
    int number;
    std::string str;
    bool boolean;
    LiteralType literal_type;

    Literal(LiteralType literal_type, int number)
        : number(number), literal_type(literal_type) {}

    Literal(LiteralType literal_type, std::string str)
        : str(str), literal_type(literal_type) {}

    Literal(LiteralType literal_type, bool boolean)
        : boolean(boolean), literal_type(literal_type) {}

    std::string accept(class Visitor *);
};

class Unary : public Expression {
  public:
    Token oprt;
    std::unique_ptr<Expression> right;
    Unary(Token oprt, std::unique_ptr<Expression> right) : oprt(oprt), right(std::move(right)) {}

    std::string accept(class Visitor *);
};

std::unique_ptr<Expression> expression(Parser *p);
std::unique_ptr<Expression> parse(std::vector<Token> tokens);

