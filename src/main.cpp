#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.hpp"  

int main() {
    // Read the input file
    std::ifstream input_file("../input.pnd");
    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open ../input.pnd" << '\n';
        return 1;
    }

    // Read the entire file content into a string
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string input = buffer.str();

    // Create a Lexer instance
    Lexer lexer(input.c_str(), input.length());

    // Vector to store tokens
    std::vector<Token> tokens;

    // Perform lexical analysis
    try {
        while (true) {
            Token token = lexer_next(lexer);
            tokens.push_back(token);

            if (token.type == TokenType::END) {
                break;
            }
        }
    } catch (const LexerError& e) {
        std::cerr << "Lexer Error: " << e.what() << '\n';
        return 1;
    }

    // Print the tokens
    for (const auto& token : tokens) {
        std::cout << "Token: " << token_type_to_string(token.type)
                  << " at line " << token.position.line
                  << ", column " << token.position.col;
        
        if (token.text_len > 0) {
            std::cout << " - Text: " << std::string(token.text, token.text_len);
        }
        
        std::cout << '\n';
    }

    return 0;
}