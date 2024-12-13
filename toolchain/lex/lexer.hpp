// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_LEX_LEXER_HPP
#define ZIV_TOOLCHAIN_LEX_LEXER_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "llvm/ADT/StringRef.h"
#include "toolchain/lex/token_buffer.hpp"
#include "toolchain/lex/token_kind.hpp"
#include "toolchain/source/source_buffer.hpp"

namespace ziv::toolchain::lex {
class Lexer {
public:
    Lexer(const ziv::toolchain::source::SourceBuffer& source, TokenBuffer& buffer)
        : source_(source), buffer_(buffer), cursor_(0), line_(1), column_(1) {
        initialize_handlers();
    };

    void lex();

    // Return tokens
    const std::vector<TokenBuffer::Token>& get_tokens() const {
        return buffer_.get_tokens();
    }

private:
    const ziv::toolchain::source::SourceBuffer& source_;
    TokenBuffer& buffer_;
    size_t cursor_;
    size_t line_;
    size_t column_;

    // Identation
    size_t indent_level_ = 0;
    std::vector<size_t> indent_stack_;
    size_t indent_width_ = 4;

    void track_indentation();

    using Handler = void (Lexer::*)();
    std::unordered_map<char, Handler> handlers_;
    void initialize_handlers();

    char peek() const;
    char consume();
    bool is_eof() const;
    bool is_line_terminator() const;
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

    bool can_terminate_expression(const TokenKind& kind) const;

    TokenKind lookup_keyword(const std::string& spelling);

    TokenKind lookup_symbol(const std::string& spelling);
};
}  // namespace ziv::toolchain::lex

#endif  // ZIV_TOOLCHAIN_LEX_LEXER_HPP
