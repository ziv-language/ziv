// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_LEX_LEXER_HPP
#define ZIV_TOOLCHAIN_LEX_LEXER_HPP

#include <unordered_map>
#include "toolchain/source/source_buffer.hpp"
#include "toolchain/lex/token_buffer.hpp"
#include "toolchain/lex/token_kind.hpp"
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::lex {
    class Lexer {
        public:
            Lexer(const ziv::toolchain::source::SourceBuffer &source, TokenBuffer &buffer):
                source_(source), buffer_(buffer), cursor_(0), line_(1), column_(1) {
                    initialize_handlers();
                };

            void lex();

        private:
            const ziv::toolchain::source::SourceBuffer &source_;
            TokenBuffer &buffer_;
            size_t cursor_;
            size_t line_;
            size_t column_;

            using Handler = void (Lexer::*)();
            std::unordered_map<char, Handler> handlers_;
            void initialize_handlers();

            char peek() const;
            char consume();
            bool is_eof() const;
            void add_token(TokenKind kind, llvm::StringRef spelling);

            void consume_whitespace();
            void consume_comment();
            void consume_identifier();
            void consume_number();
            void consume_string();
            void consume_char();
            void consume_punctuation();
            void consume_operator();
            void consume_unknown();
            bool skip_whitespace();

            bool is_identifier_start(char c) const;
            bool is_identifier_char(char c) const;

            TokenKind lookup_keyword(const std::string& spelling) {
                static const std::unordered_map<std::string, TokenKind> keyword_map = {
                    // General Keywords
                    {"fn", TokenKind::Fn()},
                    {"class", TokenKind::Class()},
                    {"interface", TokenKind::Interface()},
                    {"module", TokenKind::Module()},
                    {"import", TokenKind::Import()},
                    {"if", TokenKind::If()},
                    {"else", TokenKind::Else()},
                    {"for", TokenKind::For()},
                    {"in", TokenKind::In()},
                    {"while", TokenKind::While()},
                    {"break", TokenKind::Break()},
                    {"continue", TokenKind::Continue()},
                    {"do", TokenKind::Do()},
                    {"return", TokenKind::Return()},
                    // Logical Keywords
                    {"and", TokenKind::And()},
                    {"or", TokenKind::Or()},
                    {"not", TokenKind::Not()},
                    {"xor", TokenKind::Xor()},
                    // Type Keywords
                    {"int", TokenKind::Int()},
                    {"float", TokenKind::Float()},
                    {"bool", TokenKind::Bool()},
                    {"char", TokenKind::Char()},
                    {"string", TokenKind::String()},
                    {"void", TokenKind::Void()},
                    {"null", TokenKind::Null()},
                    // Boolean Keywords
                    {"true", TokenKind::True()},
                    {"false", TokenKind::False()},
                };

                auto it = keyword_map.find(spelling);
                return it != keyword_map.end() ? it->second : TokenKind::Identifier();
            };

            TokenKind lookup_symbol(const std::string& spelling) {
                static const std::unordered_map<std::string, TokenKind> symbol_map = {
                    // Symbols
                    {"->", TokenKind::Arrow()},
                    {"..", TokenKind::DoubleDot()},
                    {"==", TokenKind::DoubleEquals()},
                    {"!=", TokenKind::NotEquals()},
                    {"<=", TokenKind::LessEquals()},
                    {">=", TokenKind::GreaterEquals()},
                    {"+=", TokenKind::PlusEquals()},
                    {"-=", TokenKind::MinusEquals()},
                    {"*=", TokenKind::StarEquals()},
                    {"/=", TokenKind::SlashEquals()},
                    {"++", TokenKind::Increment()},
                    {"--", TokenKind::Decrement()},
                    {"<", TokenKind::Less()},
                    {">", TokenKind::Greater()},
                    {"+", TokenKind::Plus()},
                    {"-", TokenKind::Minus()},
                    {"*", TokenKind::Star()},
                    {"/", TokenKind::Slash()},
                    {"%", TokenKind::Percent()},
                    {"=", TokenKind::Equals()},
                    {"!", TokenKind::Bang()},
                    {"&", TokenKind::Ampersand()},
                    {"|", TokenKind::Pipe()},
                    {"^", TokenKind::Caret()},
                    {"~", TokenKind::Tilde()},
                    {".", TokenKind::Dot()},
                    {"@", TokenKind::At()},

                    // Delimiters
                    {"(", TokenKind::LParen()},
                    {")", TokenKind::RParen()},
                    {"{", TokenKind::LBrace()},
                    {"}", TokenKind::RBrace()},
                    {"[", TokenKind::LBracket()},
                    {"]", TokenKind::RBracket()},
                    {",", TokenKind::Comma()},
                    {":", TokenKind::Colon()},
                    {";", TokenKind::Semicolon()},
                };

                auto it = symbol_map.find(spelling);
                return it != symbol_map.end() ? it->second : TokenKind::Unknown();
            }
    };
} // namespace ziv::toolchain::lex

#endif // ZIV_TOOLCHAIN_LEX_LEXER_HPP
