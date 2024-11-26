// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_LEX_TOKEN_BUFFER_HPP
#define ZIV_TOOLCHAIN_LEX_TOKEN_BUFFER_HPP

#include <vector>
#include <cstdint>
#include "llvm/ADT/StringRef.h"
#include "toolchain/source/source_buffer.hpp"
#include "toolchain/lex/token_kind.hpp"

namespace ziv::toolchain::lex {

class TokenBuffer {
    public:
        struct Token {
            TokenKind kind;
            llvm::StringRef spelling;
            size_t line;
            size_t column;

        Token(TokenKind kind, llvm::StringRef spelling, size_t line, size_t column):
            kind(kind), spelling(spelling), line(line), column(column) {};

        TokenKind get_kind() const { return kind; };

        llvm::StringRef get_spelling() const { return kind.get_spelling(); };

        llvm::StringRef get_name() const { return kind.get_name(); };

        size_t get_line() const { return line; };

        size_t get_column() const { return column; };
        };

        // get last token
        TokenKind get_last_token() const {
            return tokens_.empty() ? TokenKind::Sof() : tokens_.back().kind;
        }

        void add_token(TokenKind kind, llvm::StringRef spelling, size_t line, size_t column);

        const std::vector<Token>& get_tokens() const;

    private:
        std::vector<Token> tokens_;

};

} // namespace ziv::toolchain::lex


#endif // ZIV_TOOLCHAIN_LEX_TOKEN_BUFFER_HPP
