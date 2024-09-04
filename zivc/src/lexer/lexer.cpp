// compiler/src/lexer/lexer.cpp

#include "lexer/lexer.hpp"

namespace ziv {
    int Lexer::get_column() { return current_column; }

    Token Lexer::get_current_token() {
        return current_token;
    }

    Token Lexer::get_next_token() {
        return current_token = get_token();
    }

    void Lexer::consume_token(Token tok) {
        assert(tok == current_token && "consume Token mismatch expectation");
        get_next_token();
    }

    llvm::StringRef Lexer::get_id() {
        assert(current_token == tok_identifier);
        return identifier_str;
    }

    double Lexer::get_value() {
        assert(current_token == tok_number);
        return numeric_value;
    }

    Location Lexer::get_last_location() { return last_location; }

    int Lexer::get_line() { return current_line; }

    int Lexer::get_next_char() {
        if (current_line_buffer.empty())
            return EOF;
        ++current_column;
        auto nextchar = current_line_buffer.front();
        current_line_buffer = current_line_buffer.drop_front();
        if (current_line_buffer.empty())
            current_line_buffer = read_next_line();
        if (nextchar == '\n') {
            ++current_line;
            current_column = 0;
        }
        return nextchar;
    }

    Token Lexer::get_token() {
        while (isspace(last_char))
            last_char = Token(get_next_char());

        last_location.line = current_line;
        last_location.column = current_column;

        if (isalpha(last_char)) {
            identifier_str = (char)last_char;
            while (isalnum((last_char = Token(get_next_char()))) || last_char == '_')
                identifier_str += (char)last_char;

            if (identifier_str == "let") { return tok_let; }
            if (identifier_str == "mut") { return tok_mut; }
            if (identifier_str == "fn") { return tok_fn; }
            if (identifier_str == "return") { return tok_return; }
            if (identifier_str == "if") { return tok_if; }
            if (identifier_str == "else") { return tok_else; }
            if (identifier_str == "match") { return tok_match; }
            if (identifier_str == "for") { return tok_for; }
            if (identifier_str == "while") { return tok_while; }
            if (identifier_str == "async") { return tok_async; }
            if (identifier_str == "await") { return tok_await; }

            return tok_identifier;
        }

        if (isdigit(last_char) || last_char == '.') {
            std::string numStr;
            do {
                numStr += last_char;
                last_char = Token(get_next_char());
            } while (isdigit(last_char) || last_char == '.');

            numeric_value = strtod(numStr.c_str(), nullptr);
            return tok_number;
        }

        if (last_char == '"') {
            std::string str;
            while ((last_char = Token(get_next_char())) != '"' && last_char != EOF){
                str += last_char;
            }
            if (last_char == EOF) {
                return tok_error;
            }
            last_char = Token(get_next_char()); // consume the closing quote
            identifier_str = str;
            return tok_string;
        }

        // Comment
        if (last_char == '#') {
            do {
                last_char = Token(get_next_char());
            } while (last_char != EOF && last_char != '\n' && last_char != '\r');

            if (last_char != EOF)
                return get_token();
        }

        if (last_char == '-') {
            if (Token next_char = Token(get_next_char()); next_char == '>') {
                last_char = Token(get_next_char());
                return tok_arrow;
            }
            return tok_minus;
        }

        if (last_char == EOF)
            return tok_eof;

        Token this_char = Token(last_char);
        last_char = Token(get_next_char());
        return this_char;
    }
}
