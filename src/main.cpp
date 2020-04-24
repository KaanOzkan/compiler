#include "lexer.hpp"
#include <iostream>

#ifdef DEBUG_BUILD
#define dprint(x) std::cerr << x << std::endl;
#else
#define dprint(x)                                                              \
    do {                                                                       \
    } while (0)
#endif

int usage(const char *arg) {
    std::cerr << "Usage: " << arg << " code.ko" << std::endl;

    return EXIT_FAILURE;
}

void print_tokens(std::vector<std::string> tokens) {
    std::cout << "Tokens: " << std::endl;
    for (int i = 0; i < tokens.size(); i++) {
        std::cout << tokens.at(i) << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return usage(argv[0]);
    }

    std::string input_filename = argv[1];
    dprint("Sending file to lexer");
    // TODO: Token type instead of string
    std::vector<std::string> tokens;
    tokens = lex(input_filename, tokens);
    print_tokens(tokens); // debug

    return EXIT_SUCCESS;
}
