// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <unordered_map>
#include "token_kind.hpp"

namespace ziv::toolchain::lex {

llvm::StringRef TokenKind::get_name() const {
    static constexpr llvm::StringLiteral names[] = {
        #define ZIV_TOKEN(NAME) #NAME,
        #define ZIV_SYMBOL_TOKEN(NAME, VALUE) #NAME,
        #define ZIV_KEYWORD_TOKEN(NAME, VALUE) #NAME,
        #include "token_record.def"
    };
    return names[static_cast<int>(kind)];
}

bool TokenKind::is_symbol() const {
    static constexpr bool is_symbol[] = {
        #define ZIV_TOKEN(NAME) false,
        #define ZIV_SYMBOL_TOKEN(NAME, VALUE) true,
        #define ZIV_KEYWORD_TOKEN(NAME, VALUE) false,
        #include "token_record.def"
    };
    return is_symbol[static_cast<int>(kind)];
}

bool TokenKind::is_keyword() const {
    static constexpr bool is_keyword[] = {
        #define ZIV_TOKEN(NAME) false,
        #define ZIV_SYMBOL_TOKEN(NAME, VALUE) false,
        #define ZIV_KEYWORD_TOKEN(NAME, VALUE) true,
        #include "token_record.def"
    };
    return is_keyword[static_cast<int>(kind)];
}

llvm::StringRef TokenKind::get_spelling() const {

    static constexpr llvm::StringLiteral spellings[] = {
        #define ZIV_TOKEN(NAME) #NAME,
        #define ZIV_SYMBOL_TOKEN(NAME, VALUE) VALUE,
        #define ZIV_KEYWORD_TOKEN(NAME, VALUE) VALUE,
        #include "token_record.def"
    };
    return spellings[static_cast<int>(kind)];
}

} // namespace ziv::toolchain::lex
