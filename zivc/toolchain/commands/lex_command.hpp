// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_LEX_COMMAND_HPP
#define ZIV_TOOLCHAIN_LEX_COMMAND_HPP

#include "zivc/toolchain/command.hpp"
#include "toolchain/lex/token_kind.hpp"
#include "toolchain/lex/lexer.hpp"
#include "toolchain/lex/token_buffer.hpp"
#include "toolchain/source/source_buffer.hpp"
#include "llvm/Support/raw_ostream.h"

namespace ziv::cli::toolchain {
    class TokenCommand : public Command {
        public:
            void execute(const std::string &args) override;
        private:
            // Mapping from token string representation to the corresponding TokenKind factory methods.
            const std::unordered_map<std::string, ziv::toolchain::lex::TokenKind (*)()> symbol_map_ = {
                #define ZIV_SYMBOL_TOKEN(NAME, VALUE) {VALUE, &ziv::toolchain::lex::TokenKind::NAME},
                #include "toolchain/lex/token_record.def"
            };

            const std::unordered_map<std::string, ziv::toolchain::lex::TokenKind (*)()> keyword_map_ = {
                #define ZIV_KEYWORD_TOKEN(NAME, VALUE) {VALUE, &ziv::toolchain::lex::TokenKind::NAME},
                #include "toolchain/lex/token_record.def"
            };

            // Mapping from token name to the corresponding TokenKind factory methods (for non-string tokens).
            const std::unordered_map<std::string, ziv::toolchain::lex::TokenKind (*)()> generic_token_map_ = {
                #define ZIV_TOKEN(NAME) {#NAME, &ziv::toolchain::lex::TokenKind::NAME},
                #include "toolchain/lex/token_record.def"
            };

    };

    class LexerCommand : public Command {
        public:
            void execute(const std::string &args) override;
    };

} // namespace ziv::cli::toolchain

#endif // ZIV_TOOLCHAIN_LEX_COMMAND_HPP
