#include "codegen.hpp"
#include <sstream>

CodeGenerator::CodeGenerator() {
    // Initialize code generator
}

std::string CodeGenerator::visit_binary_expr(Binary *expr) {
    // Visit left operand
    std::string left = expr->left->accept(this);

    // Visit right operand
    std::string right = expr->right->accept(this);

    // Generate assembly for binary operation
    if (expr->oprt.type == PLUS) {
        emit("    // Binary addition: " + left + " + " + right);
        emit("    mov x0, #" + left + "       // load left operand");
        emit("    mov x1, #" + right + "       // load right operand");
        emit("    add x0, x0, x1        // add x1 to x0");
        return "x0"; // Result is in x0
    }

    // TODO: Handle other operators (-, *, /, etc.)
    return "";
}

std::string CodeGenerator::visit_grouping_expr(Grouping *expr) {
    (void)expr; // Suppress unused parameter warning
    // TODO: Generate ARM assembly for grouping expressions
    return "";
}

std::string CodeGenerator::visit_literal_expr(Literal *expr) {
    switch (expr->literal_type) {
    case LT_NUMBER:
        return std::to_string(expr->number);
    case LT_STRING:
        // TODO: Handle string literals
        return expr->str;
    case LT_BOOLEAN:
        return expr->boolean ? "1" : "0";
    case LT_NIL:
        return "0";
    }
    return "";
}

std::string CodeGenerator::visit_unary_expr(Unary *expr) {
    (void)expr; // Suppress unused parameter warning
    // TODO: Generate ARM assembly for unary expressions
    return "";
}

std::string CodeGenerator::generate(Expression *ast) {
    // TODO: Main generation entry point
    generate_prologue();

    // Visit the AST
    ast->accept(this);

    generate_epilogue();

    return get_assembly();
}

void CodeGenerator::emit(const std::string &instruction) {
    assembly_lines.push_back(instruction);
}

void CodeGenerator::emit_data(const std::string &data) {
    data_section.push_back(data);
}

std::string CodeGenerator::get_assembly() {
    std::stringstream output;

    // Data section
    if (!data_section.empty()) {
        output << ".section .data\n";
        for (const auto &line : data_section) {
            output << line << "\n";
        }
        output << "\n";
    }

    // Text section
    output << ".section __TEXT,__text\n";
    output << ".globl _main\n\n";
    output << "_main:\n";

    for (const auto &line : assembly_lines) {
        output << line << "\n";
    }

    return output.str();
}

void CodeGenerator::generate_prologue() {
    // TODO: Generate ARM function prologue
}

void CodeGenerator::generate_epilogue() {
    // TODO: Generate ARM function epilogue and exit
    emit("    // Program exit");
    emit("    mov x0, #0       // exit status");
    emit("    mov x16, #1      // exit syscall");
    emit("    svc #0x80        // system call");
}