#pragma once
#include "../visitor.hpp"
#include <iostream>
#include <string>
#include <vector>

class Expression;

class AstPrinter : public Visitor {
    std::string parenthesize(std::string name,
                             const std::vector<Expression *> exprs);

  public:
    void print(Expression *expr);
    std::string visit_binary_expr(class Binary *expr);
    std::string visit_grouping_expr(class Grouping *expr);
    std::string visit_literal_expr(class Literal *expr);
    std::string visit_unary_expr(class Unary *expr);
};