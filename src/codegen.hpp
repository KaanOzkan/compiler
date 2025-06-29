#pragma once
#include "parser.hpp"
#include "visitor.hpp"
#include <string>
#include <vector>

class CodeGenerator : public Visitor {
private:
    std::vector<std::string> assembly_lines;
    std::vector<std::string> data_section;
    
public:
    CodeGenerator();
    
    // Visitor pattern methods for expressions
    std::string visit_binary_expr(Binary *expr) override;
    std::string visit_grouping_expr(Grouping *expr) override;
    std::string visit_literal_expr(Literal *expr) override;
    std::string visit_unary_expr(Unary *expr) override;
    
    // Code generation methods
    std::string generate(Expression* ast);
    void emit(const std::string& instruction);
    void emit_data(const std::string& data);
    
    // Get the complete assembly output
    std::string get_assembly();
    
private:
    void generate_prologue();
    void generate_epilogue();
}; 