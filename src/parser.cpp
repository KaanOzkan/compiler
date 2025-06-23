#include "parser.hpp"
#include "iostream"
#include "lexer.hpp"
#include "util.hpp"
#include "visitor.hpp"
#include "printers/ast_printer.hpp"
#include <memory>




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
// TODO: Hold onto parser error instead of throwing. Skip to next statement and
// continue parsing (synchronization)

std::unique_ptr<Expression> primary(Parser *p) {
    debug_print("Primary()");
    std::vector<TokenType> types{FALSE};
    if (match(p, types)) {
        return std::unique_ptr<Literal>(new Literal(LT_BOOLEAN, false));
    }

    types.at(0) = TRUE;
    if (match(p, types)) {
        return std::unique_ptr<Literal>(new Literal(LT_BOOLEAN, true));
    }

    types.at(0) = _NULL;
    if (match(p, types)) {
        return std::unique_ptr<Literal>(new Literal(LT_NIL, true));
    }

    types.at(0) = NUMBER;
    if (match(p, types)) {
        return std::unique_ptr<Literal>(new Literal(LT_NUMBER, std::stoi(previous(p).text)));
    }

    types.at(0) = STRING;
    if (match(p, types)) {
        return std::unique_ptr<Literal>(new Literal(LT_STRING, previous(p).text));
    }

    types.at(0) = LEFT_PAREN;
    if (match(p, types)) {
        auto expr = expression(p);
        consume(p, RIGHT_PAREN, "Expected ')' after '('");
        return std::unique_ptr<Grouping>(new Grouping(std::move(expr)));
    }

    throw std::runtime_error("Parser error unhandled type in Expression.primary()");
}

std::unique_ptr<Expression> unary(Parser *p) {
    debug_print("Unary()");
    std::vector<TokenType> types{BANG, MINUS};
    if (match(p, types)) {
        Token oprt = previous(p);
        auto right = unary(p);
        return std::unique_ptr<Unary>(new Unary(oprt, std::move(right)));
    }

    return primary(p);
}

std::unique_ptr<Expression> multiplication(Parser *p) {
    debug_print("multiplication() - starting unary");
    auto left = unary(p);
    debug_print("multiplication() - finished unary");

    std::vector<TokenType> types{SLASH, STAR};
    while (match(p, types)) {
        Token oprt = previous(p);
        auto right = unary(p);
        left = std::unique_ptr<Binary>(new Binary(std::move(left), oprt, std::move(right)));
    }

    return left;
}

std::unique_ptr<Expression> addition(Parser *p) {
    debug_print("addition() - starting multiplication");
    auto left = multiplication(p);
    debug_print("addition() - finished multiplication");

    std::vector<TokenType> types{MINUS, PLUS};
    while (match(p, types)) {
        Token oprt = previous(p);
        auto right = multiplication(p);
        left = std::unique_ptr<Binary>(new Binary(std::move(left), oprt, std::move(right)));
    }

    return left;
}

std::unique_ptr<Expression> comparison(Parser *p) {
    debug_print("comparison() - starting addition");
    auto left = addition(p);
    debug_print("comparison() - finished addition");

    std::vector<TokenType> types{GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while (match(p, types)) {
        Token oprt = previous(p);
        auto right = addition(p);
        left = std::unique_ptr<Binary>(new Binary(std::move(left), oprt, std::move(right)));
    }

    return left;
}

std::unique_ptr<Expression> equality(Parser *p) {
    debug_print("equality() - starting comparison");
    auto left = comparison(p);
    debug_print("equality() - finished comparison");
    std::vector<TokenType> types{EQUAL_EQUAL, BANG_EQUAL};
    while (match(p, types)) {
        Token oprt = previous(p);
        auto right = comparison(p);
        left = std::unique_ptr<Binary>(new Binary(std::move(left), oprt, std::move(right)));
    }
    debug_print("equality() - finished equality");
    return left;
}

std::unique_ptr<Expression> expression(Parser *p) { return equality(p); }

std::unique_ptr<Expression> parse(std::vector<Token> tokens) {
    Parser p;
    p.tokens = tokens;
    auto e = expression(&p);
    debug_print("Parser complete");

    return e;
}
