#include "parser.hpp"
#include "iostream"
#include "lexer.hpp"
#include "util.hpp"
#include "visitor.hpp"
#include "printers/ast_printer.hpp"
#include <memory>

class Binary : public Expression {
  public:
    Expression *left;
    Token oprt;
    Expression *right;

    Binary(Expression *left, Token oprt, Expression *right)
        : left(left), oprt(oprt), right(right) {}

    std::string accept(Visitor *);
};

class Grouping : public Expression {
  public:
    Expression *inner_expr;
    Grouping(Expression *inner_expr) : inner_expr(inner_expr) {}
    std::string accept(Visitor *);
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

    std::string accept(Visitor *);
};

class Unary : public Expression {
  public:
    Token oprt;
    Expression *right;
    Unary(Token oprt, Expression *right) : oprt(oprt), right(right) {}

    std::string accept(Visitor *);
};


std::string Binary::accept(Visitor *v) { return v->visit_binary_expr(this); }
std::string Grouping::accept(Visitor *v) {
    return v->visit_grouping_expr(this);
}
std::string Literal::accept(Visitor *v) { return v->visit_literal_expr(this); }
std::string Unary::accept(Visitor *v) { return v->visit_unary_expr(this); }

/*
 * Parser Helper Methods
 */

bool at_the_end(Parser *p) { return p->current == p->tokens.size(); }

Token previous(Parser *p) { return p->tokens.at(p->current - 1); }

void advance(Parser *p) {
    if (!at_the_end(p)) {
        debug_print("Advancing");
        p->current++;
    }
}

bool same_type_as_curr_token(Parser *p, TokenType type) {
    if (at_the_end(p)) {
        return false;
    }

    return type == p->tokens.at(p->current).type;
}

bool match(Parser *p, std::vector<TokenType> v) {
    for (unsigned long i = 0; i < v.size(); i++) {
        if (same_type_as_curr_token(p, v.at(i))) {
            /* debug_print(p->tokens.at(p->current).type); */
            advance(p);
            return true;
        }
    }

    return false;
}

void consume(Parser *p, TokenType type, std::string error_message) {
    if (same_type_as_curr_token(p, type)) {
        advance(p);
    } else {
        throw std::runtime_error(error_message);
    }
}

/*
 * Language Rules
 */

// TODO: Handle pointer cleanup
// TODO: Hold onto parser error instead of throwing. Skip to next statement and
// continue parsing (synchronization)

Expression *primary(Parser *p) {
    debug_print("Primary()");
    std::vector<TokenType> types{FALSE};
    if (match(p, types)) {
        return new Literal(LT_BOOLEAN, false);
    }

    types.at(0) = TRUE;
    if (match(p, types)) {
        return new Literal(LT_BOOLEAN, true);
    }

    types.at(0) = _NULL;
    if (match(p, types)) {
        return new Literal(LT_NIL, true);
    }

    types.at(0) = NUMBER;
    if (match(p, types)) {
        return new Literal(LT_NUMBER, std::stoi(previous(p).text));
    }

    types.at(0) = STRING;
    if (match(p, types)) {
        return new Literal(LT_STRING, previous(p).text);
    }

    types.at(0) = LEFT_PAREN;
    if (match(p, types)) {
        Expression *expr = expression(p);
        consume(p, RIGHT_PAREN, "Expected ')' after '('");
        return new Grouping(expr);
    }

    throw std::runtime_error("Parser error unhandled type in Expression.primary()");
}

Expression *unary(Parser *p) {
    debug_print("Unary()");
    std::vector<TokenType> types{BANG, MINUS};
    if (match(p, types)) {
        Token oprt = previous(p);
        Expression *right = unary(p);
        Unary *ptr = new Unary(oprt, right);
        return ptr;
    }

    return primary(p);
}

Expression *multiplication(Parser *p) {
    debug_print("multiplication() - starting unary");
    Expression *left = unary(p);
    debug_print("multiplication() - finished unary");

    std::vector<TokenType> types{SLASH, STAR};
    while (match(p, types)) {
        Token oprt = previous(p);
        Expression *right = unary(p);
        left = new Binary(left, oprt, right);
    }

    return left;
}

Expression *addition(Parser *p) {
    debug_print("addition() - starting multiplication");
    Expression *left = multiplication(p);
    debug_print("addition() - finished multiplication");

    std::vector<TokenType> types{MINUS, PLUS};
    while (match(p, types)) {
        Token oprt = previous(p);
        Expression *right = multiplication(p);
        left = new Binary(left, oprt, right);
    }

    return left;
}

Expression *comparison(Parser *p) {
    debug_print("comparison() - starting addition");
    Expression *left = addition(p);
    debug_print("comparison() - finished addition");

    std::vector<TokenType> types{GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while (match(p, types)) {
        Token oprt = previous(p);
        Expression *right = addition(p);
        left = new Binary(left, oprt, right);
    }

    return left;
}

Expression *equality(Parser *p) {
    debug_print("equality() - starting comparison");
    Expression *left = comparison(p);
    debug_print("equality() - finished comparison");
    std::vector<TokenType> types{EQUAL_EQUAL, BANG_EQUAL};
    while (match(p, types)) {
        Token oprt = previous(p);
        Expression *right = comparison(p);
        left = new Binary(left, oprt, right);
    }
    debug_print("equality() - finished equality");
    return left;
}

Expression *expression(Parser *p) { return equality(p); }

Expression* parse(std::vector<Token> tokens) {
    Parser p;
    p.tokens = tokens;
    Expression *e = expression(&p);
    debug_print("Parser complete");

    return e;
}
