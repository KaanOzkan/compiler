#include "lexer.hpp"
#include "parser.hpp"
#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int usage(const char *arg) {
    std::cerr << "Usage: " << arg << " code.ko" << std::endl;

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
    if (argc != 2) {
        return usage(argv[0]);
    }

    debug_print("Reading file");
    std::vector<char> file_chars = read_file(argv[1]);

    debug_print("Sending file to lexer");
    std::vector<Token> tokens;
    try {
        tokens = lex(file_chars, tokens);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    debug_print("Parse tokens");
    parse(tokens);

    return EXIT_SUCCESS;
}
