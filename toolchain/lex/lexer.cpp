// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "lexer.hpp"

namespace ziv::toolchain::lex {

        void Lexer::initialize_handlers() {
        // Single line comments
        handlers_['#'] = &Lexer::consume_comment;

        // Strings and characters
        handlers_['"'] = &Lexer::consume_string;
        handlers_['\''] = &Lexer::consume_char;

        // Operators and punctuation
        handlers_['+'] = &Lexer::consume_operator;
        handlers_['-'] = &Lexer::consume_operator;
        handlers_['*'] = &Lexer::consume_operator;
        handlers_['/'] = &Lexer::consume_operator;
        handlers_['%'] = &Lexer::consume_operator;
        handlers_['='] = &Lexer::consume_operator;
        handlers_['!'] = &Lexer::consume_operator;
        handlers_['<'] = &Lexer::consume_operator;
        handlers_['>'] = &Lexer::consume_operator;
        handlers_['&'] = &Lexer::consume_operator;
        handlers_['|'] = &Lexer::consume_operator;
        handlers_['^'] = &Lexer::consume_operator;
        handlers_['~'] = &Lexer::consume_operator;

        // Delimiters
        handlers_['('] = &Lexer::consume_punctuation;
        handlers_[')'] = &Lexer::consume_punctuation;
        handlers_['{'] = &Lexer::consume_punctuation;
        handlers_['}'] = &Lexer::consume_punctuation;
        handlers_['['] = &Lexer::consume_punctuation;
        handlers_[']'] = &Lexer::consume_punctuation;
        handlers_[','] = &Lexer::consume_punctuation;
        handlers_[';'] = &Lexer::consume_punctuation;
        handlers_['.'] = &Lexer::consume_operator;
        handlers_[':'] = &Lexer::consume_operator;
        handlers_['@'] = &Lexer::consume_operator;
    }

    void Lexer::lex() {
        while(!is_eof()) {
            char current = peek();
            if (handlers_.count(current)) {
                (this->*handlers_[current])();
            }
            else if (std::isspace(current)) {
                consume_whitespace();
            }
            else if (std::isalpha(current) || current == '_') {
                consume_identifier();
            }
            else if (std::isdigit(current)) {
                consume_number();
            }
            else {
                consume_unknown();
            }
        }

        add_token(TokenKind::Eof(), "");
    }

    char Lexer::peek() const {
        return cursor_ < source_.get_contents().size() ? source_.get_contents()[cursor_] : '\0';
    }

    char Lexer::consume() {
        char current = peek();
        if (current == '\n') {
            line_++;
            column_ = 1;
        }
        else {
            column_++;
        }
        cursor_++;
        return current;
    }

    bool Lexer::is_eof() const {
        return cursor_ >= source_.get_contents().size();
    }

    void Lexer::add_token(TokenKind kind, llvm::StringRef spelling) {
        buffer_.add_token(kind, spelling, line_, column_);
    }

    void Lexer::consume_whitespace() {
        while (std::isspace(peek())) {
            consume();
        }
    }

    void Lexer::consume_comment() {
        consume(); // Initial '#'

        if (peek() == '-' && source_.get_contents()[cursor_ + 1] == '-') {
            // Multi-line comment
            consume(); // First '-'
            consume(); // Second '-'

            while (!is_eof()) {
                if (peek() == '-' &&
                    cursor_ + 2 < source_.get_contents().size() &&
                    source_.get_contents()[cursor_ + 1] == '-' &&
                    source_.get_contents()[cursor_ + 2] == '#') {
                    consume(); // First '-'
                    consume(); // Second '-'
                    consume(); // '#'
                    return;
                }
                consume();
            }

            // Error: Unterminated multi-line comment
            add_token(TokenKind::Error(), "Unterminated multi-line comment");
        } else {
            // Single-line comment
            while (!is_eof() && peek() != '\n') {
                consume();
            }
        }
    }

    void Lexer::consume_identifier() {
        std::string spelling;
        while (is_identifier_char(peek())) {
            spelling += consume();
        }
        TokenKind kind = lookup_keyword(spelling);
        add_token(kind, spelling);
    }

    void Lexer::consume_number() {
        std::string spelling;
        bool is_float = false;
        bool has_exponent = false;

        // Handle leading sign
        if (peek() == '-' || peek() == '+') {
            spelling += consume();
        }

        // Consume integer part
        while (std::isdigit(peek())) {
            spelling += consume();
        }

        // Handle decimal point
        if (peek() == '.') {
            is_float = true;
            spelling += consume();

            // Must have at least one digit after decimal
            if (!std::isdigit(peek())) {
                add_token(TokenKind::Error(), "Expected digit after decimal point");
                return;
            }

            while (std::isdigit(peek())) {
                spelling += consume();
            }
        }

        // Handle exponent
        if (peek() == 'e' || peek() == 'E') {
            is_float = true;
            has_exponent = true;
            spelling += consume();

            // Handle exponent sign
            if (peek() == '-' || peek() == '+') {
                spelling += consume();
            }

            if (!std::isdigit(peek())) {
                add_token(TokenKind::Error(), "Expected digit in exponent");
                return;
            }

            while (std::isdigit(peek())) {
                spelling += consume();
            }
        }

        // Check for invalid suffixes
        if (std::isalpha(peek()) || peek() == '_') {
            add_token(TokenKind::Error(), "Invalid number suffix");
            return;
        }

        add_token(is_float ? TokenKind::FloatLiteral() : TokenKind::IntLiteral(), spelling);
    }

    void Lexer::consume_string() {
        consume(); // Initial quote
        std::string spelling;
        bool escaped = false;

        while (!is_eof()) {
            char c = peek();
            if (c == '\n') {
                // Error: unterminated string
                add_token(TokenKind::Error(), "Unterminated string literal");
                return;
            }

            if (!escaped) {
                if (c == '"') {
                    consume();
                    add_token(TokenKind::StringLiteral(), spelling);
                    return;
                }
                if (c == '\\') {
                    escaped = true;
                    consume();
                    continue;
                }
            } else {
                escaped = false;
                switch (c) {
                    case 'n': spelling += '\n'; break;
                    case 't': spelling += '\t'; break;
                    case 'r': spelling += '\r'; break;
                    case '\\': spelling += '\\'; break;
                    case '"': spelling += '"'; break;
                    default: spelling += '\\'; spelling += c;
                }
                consume();
                continue;
            }
            spelling += consume();
        }

        // Error: EOF in string
        add_token(TokenKind::Error(), "EOF in string literal");
    }

    void Lexer::consume_char() {
        consume(); // Initial quote
        std::string spelling;
        bool escaped = false;

        if (is_eof()) {
            add_token(TokenKind::Error(), "Empty character literal");
            return;
        }

        char c = peek();
        if (c == '\\') {
            consume();
            if (is_eof()) {
                add_token(TokenKind::Error(), "Incomplete escape sequence");
                return;
            }
            c = peek();
            switch (c) {
                case 'n': spelling = "\n"; break;
                case 't': spelling = "\t"; break;
                case 'r': spelling = "\r"; break;
                case '\\': spelling = "\\"; break;
                case '\'': spelling = "'"; break;
                default:
                    add_token(TokenKind::Error(), "Invalid escape sequence");
                    return;
            }
            consume();
        } else {
            spelling = consume();
        }

        if (peek() != '\'') {
            add_token(TokenKind::Error(), "Multi-character char literal or unterminated char literal");
            return;
        }
        consume(); // Closing quote

        add_token(TokenKind::CharLiteral(), spelling);
    }

    void Lexer::consume_punctuation() {
         std::string spelling;
        spelling += consume();

        // Handle special cases like '..'
        if (spelling[0] == '.' && peek() == '.') {
            spelling += consume();
        }

        TokenKind kind = lookup_symbol(spelling);
        add_token(kind, spelling);
    }

    void Lexer::consume_operator() {
        std::string spelling;
        spelling += consume();

       // Check for two-character operators
        char next = peek();
        if (next == '=' || (spelling[0] == '-' && next == '>') ||
            (spelling[0] == '.' && next == '.') ||
            ((spelling[0] == '+' || spelling[0] == '-') && next == spelling[0])) {
            spelling += consume();
        }

        TokenKind kind = lookup_symbol(spelling);
        add_token(kind, spelling);
    }

    void Lexer::consume_unknown() {
        consume();
        add_token(TokenKind::Unknown(), "");
    }

    bool Lexer::skip_whitespace() {
        bool skipped = false;
        while (std::isspace(peek())) {
            consume();
            skipped = true;
        }
        return skipped;
    }

    bool Lexer::is_identifier_start(char c) const {
        return std::isalpha(c) || c == '_';
    }

    bool Lexer::is_identifier_char(char c) const {
        return std::isalnum(c) || c == '_';
    }

}
