#pragma once
#include <string>

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
  public:
    virtual ~Visitor(){};
    virtual std::string visit_binary_expr(Binary *) = 0;
    virtual std::string visit_grouping_expr(Grouping *) = 0;
    virtual std::string visit_literal_expr(Literal *) = 0;
    virtual std::string visit_unary_expr(Unary *) = 0;
};