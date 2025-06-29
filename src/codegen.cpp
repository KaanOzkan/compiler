#include "codegen.hpp"
#include <sstream>

CodeGenerator::CodeGenerator() {
    // Initialize code generator
}

std::string CodeGenerator::visit_binary_expr(Binary *expr) {
    (void)expr; // Suppress unused parameter warning
    // TODO: Generate ARM assembly for binary expressions
    return "";
}

std::string CodeGenerator::visit_grouping_expr(Grouping *expr) {
    (void)expr; // Suppress unused parameter warning
    // TODO: Generate ARM assembly for grouping expressions
    return "";
}

std::string CodeGenerator::visit_literal_expr(Literal *expr) {
    (void)expr; // Suppress unused parameter warning
    // TODO: Generate ARM assembly for literal expressions
    return "";
}

std::string CodeGenerator::visit_unary_expr(Unary *expr) {
    (void)expr; // Suppress unused parameter warning
    // TODO: Generate ARM assembly for unary expressions
    return "";
}

std::string CodeGenerator::generate(Expression* ast) {
    // TODO: Main generation entry point
    generate_prologue();
    
    // Visit the AST
    ast->accept(this);
    
    generate_epilogue();
    
    return get_assembly();
}

void CodeGenerator::emit(const std::string& instruction) {
    assembly_lines.push_back(instruction);
}

void CodeGenerator::emit_data(const std::string& data) {
    data_section.push_back(data);
}

std::string CodeGenerator::get_assembly() {
    std::stringstream output;
    
    // Data section
    if (!data_section.empty()) {
        output << ".section .data\n";
        for (const auto& line : data_section) {
            output << line << "\n";
        }
        output << "\n";
    }
    
    // Text section
    output << ".section .text\n";
    output << ".global _start\n\n";
    
    for (const auto& line : assembly_lines) {
        output << line << "\n";
    }
    
    return output.str();
}

void CodeGenerator::generate_prologue() {
    // TODO: Generate ARM function prologue
}

void CodeGenerator::generate_epilogue() {
    // TODO: Generate ARM function epilogue and exit
    emit("    @ Program exit");
    emit("    mov r7, #1       @ exit syscall");
    emit("    mov r0, #0       @ exit status");
    emit("    svc #0           @ system call");
} 