#include "parser.hpp"
#include "iostream"
#include "lexer.hpp"

class Expression {
  public:
    virtual ~Expression(){};
    virtual std::string accept(class Visitor *) = 0;
};

class Binary : public Expression {
  public:
    const Expression *left;
    const Token oprt;
    const Expression *right;

    Binary(Expression *left, Token oprt, Expression *right)
        : left(left), oprt(oprt), right(right) {
        std::cout << "operator: " << oprt.text << std::endl; // TODO: remove
    }

    std::string accept(Visitor *);
};

// TODO: Use token type from lexer.hpp ?
enum LiteralType { LT_NUMBER, LT_STRING, LT_BOOLEAN, LT_NIL };

class Literal : public Expression {
    LiteralType type;
    int number;
    std::string str;
    bool boolean;

  public:
    LiteralType literal_type;

    Literal(LiteralType literal_type) : literal_type(literal_type) {
        // TODO
        switch (literal_type) {
        case LT_NUMBER:
            break;
        case LT_STRING:
            break;
        case LT_BOOLEAN:
            break;
        case LT_NIL:
            break;
        }
    };

    std::string accept(Visitor *);
};

class Visitor : public Expression {
  public:
    virtual ~Visitor(){};
    virtual std::string visit_binary_expr(Binary *) = 0;
    virtual std::string visit_literal_expr(Literal *) = 0;
};

class AstPrinter : public Visitor {
  public:
    std::string print(Expression expr) { return expr.accept(this); }

    std::string parenthesize(std::string, const Expression *left,
                             const Expression *right) {
        // TODO: Take multiple arguments
        // TODO: Pretty print

        return "--";
    }

    std::string visit_binary_expr(Binary *expr) {
        return parenthesize(expr->oprt.text, expr->left, expr->right);
    }
    std::string visit_literal_expr(Literal *expr) {
        if (expr->literal_type != LT_NIL) {
            // TODO: equivalent
            /* return std::string(expr->literal_type); */
            return "visiting literal";
        }
        return "nil";
    }

    std::string accept(Visitor *) {
        throw std::runtime_error("Not implemented");
    }
};

std::string Binary::accept(Visitor *v) { return v->visit_binary_expr(this); }
std::string Literal::accept(Visitor *v) { return v->visit_literal_expr(this); }

void parse(std::vector<Token> tokens) {
    Literal l1(LT_NUMBER);
    Literal l2(LT_NUMBER);
    Binary b(&l1, tokens[0], &l2);

    AstPrinter printer;
    // TODO: print
}
