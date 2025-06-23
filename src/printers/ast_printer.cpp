#include "ast_printer.hpp"
#include "../parser.hpp"
#include <iostream>
#include <string>

std::string AstPrinter::parenthesize(std::string name,
                         const std::vector<Expression *> exprs) {
    std::string str("(");
    str += name;
    for (unsigned long i = 0; i < exprs.size(); i++) {
        Expression *e = exprs.at(i);
        str += " ";
        str += e->accept(this);
    }
    str += ")";

    return str;
}

void AstPrinter::print(Expression *expr) {
    std::cout << expr->accept(this) << std::endl;
}

std::string AstPrinter::visit_binary_expr(Binary *expr) {
    std::vector<Expression *> v{expr->left.get(), expr->right.get()};
    return parenthesize(expr->oprt.text, v);
}

std::string AstPrinter::visit_grouping_expr(Grouping *expr) {
    std::vector<Expression *> v{expr->inner_expr.get()};
    return parenthesize("group", v);
}

std::string AstPrinter::visit_literal_expr(Literal *expr) {
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

std::string AstPrinter::visit_unary_expr(Unary *expr) {
    std::vector<Expression *> v{expr->right.get()};
    return parenthesize(expr->oprt.text, v);
}