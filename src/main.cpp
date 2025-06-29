#include "lexer.hpp"
#include "parser.hpp"
#include "util.hpp"
#include "printers/ast_printer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

int usage(const char *arg) {
    std::cerr << "Usage: " << arg << " code.ko [--print=parser]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  --print=parser    Print AST after parser phase" << std::endl;

    return EXIT_FAILURE;
}

void print_tokens(std::vector<Token> tokens) {
    std::cout << "Tokens: " << std::endl;
    for (unsigned long i = 0; i < tokens.size(); i++) {
        std::cout << tokens.at(i).text << " : " << tokens.at(i).type
                  << std::endl;
    }
    std::cout << std::endl;
}

std::vector<char> read_file(std::string filename) {
    std::ifstream file(filename);
    std::ostringstream ss;
    ss << file.rdbuf();
    const std::string &file_contents = ss.str();
    std::vector<char> file_chars(file_contents.begin(), file_contents.end());

    return file_chars;
}

int main(int argc, char const *argv[]) {
    if (argc < 2 || argc > 3) {
        return usage(argv[0]);
    }

    std::string filename = argv[1];
    bool print_parser = false;

    // Check for --print=parser flag
    if (argc == 3) {
        std::string flag = argv[2];
        if (flag == "--print=parser") {
            print_parser = true;
        } else {
            std::cerr << "Unknown flag: " << flag << std::endl;
            return usage(argv[0]);
        }
    }

    debug_print("Reading file");
    std::vector<char> file_chars = read_file(filename);

    debug_print("Sending file to lexer");
    std::vector<Token> tokens;
    try {
        tokens = lex(file_chars, tokens);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    debug_print("Parse tokens");
    Parser parser;
    auto ast = parser.parse(tokens);

    if (print_parser) {
        debug_print("Print AST");
        AstPrinter printer;
        printer.print(ast.get());
    }

    return EXIT_SUCCESS;
}
