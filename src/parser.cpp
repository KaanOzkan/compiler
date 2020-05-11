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
    Expression *left;
    Token oprt;
    Expression *right;

    Binary(Expression *left, Token oprt, Expression *right)
        : left(left), oprt(oprt), right(right) {}

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

class Visitor {
  public:
    virtual ~Visitor(){};
    virtual std::string visit_binary_expr(Binary *) = 0;
    virtual std::string visit_literal_expr(Literal *) = 0;
};

class AstPrinter : public Visitor {
  public:
    void print(Expression *expr) { std::cout << expr->accept(this) << std::endl; }

    std::string parenthesize(std::string name,
                             const std::vector<Expression *> exprs) {
        // TODO: Test
        std::string str("(");
        str += name;
        for (int i = 0; i < exprs.size(); i++) {
            Expression *e = exprs.at(i);
            str += " ";
            str += e->accept(this);
        }
        str += ")";

        return str;
    }

    std::string visit_binary_expr(Binary *expr) {
        std::vector<Expression *> v(2);
        v.at(0) = expr->left;
        v.at(1) = expr->right;
        return parenthesize(expr->oprt.text, v);
    }
    std::string visit_literal_expr(Literal *expr) {
        switch (expr->literal_type) {
        case LT_NUMBER:
            return std::to_string(expr->number);
        case LT_STRING:
            return expr->str;
        case LT_BOOLEAN:
            return expr->boolean ? "true" : "false";
        case LT_NIL:
            return "nil";
        }
    }
};

std::string Binary::accept(Visitor *v) { return v->visit_binary_expr(this); }
std::string Literal::accept(Visitor *v) { return v->visit_literal_expr(this); }

void parse(std::vector<Token> tokens) {
    // TODO: Parser loop
    Literal l1(LT_NUMBER, 1);
    Literal l2(LT_NUMBER, 2);
    Token t = {TokenType::STRING, "+", 0, 0, 0};
    Binary b(&l1, t, &l2);

    AstPrinter printer;
    printer.print(&b);
}
