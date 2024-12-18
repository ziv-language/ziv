// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "token_buffer.hpp"

namespace ziv::toolchain::lex {

void TokenBuffer::add_token(TokenKind kind, llvm::StringRef spelling, size_t line, size_t column) {
    tokens_.emplace_back(kind, spelling, source_buffer_.get_filename(), line, column);
}

const std::vector<TokenBuffer::Token>& TokenBuffer::get_tokens() const {
    return tokens_;
}

}  // namespace ziv::toolchain::lex
