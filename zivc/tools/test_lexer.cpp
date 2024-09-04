#include <iostream>
#include <fstream>
#include <string>
#include "lexer/lexer_buffer.hpp"  // Include the correct lexer header
#include "lexer/tokens.hpp"        // Include the correct token definitions

using namespace ziv;

// Function to convert token enum to a string
std::string tokenToString(const Token &token) {
    switch (token) {
        case tok_colon: return ":";
        case tok_semicolon: return ";";
        case tok_lparen: return "(";
        case tok_rparen: return ")";
        case tok_lbrace: return "{";
        case tok_rbrace: return "}";
        case tok_lbracket: return "[";
        case tok_rbracket: return "]";
        case tok_comma: return ",";
        case tok_plus: return "+";
        case tok_minus: return "-";
        case tok_star: return "*";
        case tok_slash: return "/";
        case tok_percent: return "%";
        case tok_equal: return "=";
        case tok_eof: return "EOF";
        case tok_let: return "let";
        case tok_mut: return "mut";
        case tok_fn: return "fn";
        case tok_return: return "return";
        case tok_eqeq: return "==";
        case tok_noteq: return "!=";
        case tok_gteq: return ">=";
        case tok_lteq: return "<=";
        case tok_arrow: return "->";
        case tok_if: return "if";
        case tok_else: return "else";
        case tok_match: return "match";
        case tok_for: return "for";
        case tok_while: return "while";
        case tok_async: return "async";
        case tok_await: return "await";
        case tok_identifier: return "identifier";
        case tok_number: return "number";
        case tok_string: return "string";
        case tok_error: return "error";
        default: return "unknown";
    }
}

void prettyPrintToken(const Token &token, const Location &location) {
    std::cout << "Token Type: " << tokenToString(token) << "\n";
    std::cout << "Position: Line " << location.line << ", Column " << location.column << "\n";
    std::cout << "---------------------------------\n";
}

int main(int argc, char* argv[]) {
    // Check if a file name was provided as an argument
    if (argc < 2) {
        std::cerr << "Error: No input file provided.\n";
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    // Open the file provided as an argument
    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: Cannot open file '" << argv[1] << "'.\n";
        return 1;
    }

    // Read the file content into a string
    std::string inputContent((std::istreambuf_iterator<char>(inputFile)),
                              std::istreambuf_iterator<char>());

    // Create a lexer instance with the input content
    LexerBuffer lexer(inputContent.c_str(), inputContent.c_str() + inputContent.size(), argv[1]);

    // Process the input and get tokens
    Token token;
    while ((token = lexer.get_next_token()) != tok_eof) {
        prettyPrintToken(token, lexer.get_last_location());
    }

    // Close the file
    inputFile.close();
    return 0;
}
