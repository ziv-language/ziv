// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "lexer.hpp"

namespace ziv::toolchain::lex {

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
    }

    void Lexer::initialize_handlers() {
        // Punctuation
        handlers_['#'] = &Lexer::consume_comment;
        handlers_['"'] = &Lexer::consume_string;
        handlers_['\''] = &Lexer::consume_char;

        // Operators
        handlers_['+'] = &Lexer::consume_operator;
        handlers_['-'] = &Lexer::consume_operator;
        handlers_['*'] = &Lexer::consume_operator;
        handlers_['/'] = &Lexer::consume_operator;
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
        consume(); // Consume the '#'
        if (peek() == '-' && source_.get_contents()[cursor_ + 1] == '-') {
            consume(); // Consume the first '-'
            consume(); // Consume the second '-'
            while (!(peek() == '-' && source_.get_contents()[cursor_ + 1] == '-' && source_.get_contents()[cursor_ + 2] == '#') && !is_eof()) {
                consume();
            }
            if (!is_eof()) {
                consume(); // Consume the first '-'
                consume(); // Consume the second '-'
                consume(); // Consume the '#'
            }
        } else {
            while (peek() != '\n' && !is_eof()) {
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
        while (std::isdigit(peek())) {
            spelling += consume();
        }
        add_token(TokenKind::IntLiteral(), spelling);
    }

    void Lexer::consume_string() {
        consume(); // Consume the '"'
        std::string spelling;
        while (peek() != '"' && !is_eof()) {
            spelling += consume();
        }
        consume(); // Consume the '"'
        add_token(TokenKind::StringLiteral(), spelling);
    }

    void Lexer::consume_char() {
        consume(); // Consume the '\''
        char spelling = consume();
        consume(); // Consume the '\''
        add_token(TokenKind::CharLiteral(), std::string(1, spelling));
    }

    void Lexer::consume_punctuation() {
        char spelling = consume();
        llvm::StringRef spelling_ref(&spelling, 1);
        add_token(TokenKind::Type(), spelling_ref);
    }

    void Lexer::consume_operator() {
        char spelling = consume();
        llvm::StringRef spelling_ref(&spelling, 1);
        add_token(TokenKind::Type(), spelling_ref);
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
