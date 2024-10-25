// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once
#ifndef ZIV_TOOLCHAIN_LEX_TOKEN_KIND_HPP
#define ZIV_TOOLCHAIN_LEX_TOKEN_KIND_HPP

#include <cstdint>
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::lex {

class TokenKind {

    enum class KindEnum: uint8_t {
        #define ZIV_TOKEN(NAME) NAME,
        #define ZIV_SYMBOL_TOKEN(NAME, VALUE) NAME,
        #define ZIV_KEYWORD_TOKEN(NAME, VALUE) NAME,
        #include "token_record.def"
    };

    public:
        #define ZIV_TOKEN(NAME) \
        static constexpr TokenKind NAME() { return TokenKind(KindEnum::NAME); }

        #define ZIV_SYMBOL_TOKEN(NAME, VALUE) \
            static constexpr TokenKind NAME() { return TokenKind(KindEnum::NAME); }

        #define ZIV_KEYWORD_TOKEN(NAME, VALUE) \
            static constexpr TokenKind NAME() { return TokenKind(KindEnum::NAME); }

        #include "token_record.def"

        TokenKind() = delete;

        bool operator==(const TokenKind& other) const {
            return kind == other.kind;
        }

        bool operator!=(const TokenKind& other) const {
            return kind != other.kind;
        }

        llvm::StringRef get_name() const;

        bool is_symbol() const;

        bool is_keyword() const;

        // Returns the spelling of the token kind.
        // For example, for the token kind `TokenKind::kw_fn`, this function
        // would return "fn".
        // Other examples:
        // - `TokenKind::l_paren` -> "("
        // - `TokenKind::r_paren` -> ")"
        llvm::StringRef get_spelling() const;

        // Returns the kind of the token as an integer.
        constexpr operator int() const;

    private:
        constexpr TokenKind(KindEnum kind): kind(kind) {}

        KindEnum kind;

};

} // namespace ziv::toolchain::lex

#endif // ZIV_TOOLCHAIN_LEX_TOKEN_KIND_HPP
