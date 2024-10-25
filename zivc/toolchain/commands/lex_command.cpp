// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "lex_command.hpp"

namespace ziv::cli::toolchain {
    void TokenCommand::execute(const std::string &arg) {

        auto it = symbol_map_.find(arg);
        if (it != symbol_map_.end()) {
            llvm::outs() << "Symbol token: " << it->first << "\n";
            return;
        }

        it = keyword_map_.find(arg);
        if (it != keyword_map_.end()) {
            llvm::outs() << "Keyword token: " << it->first << "\n";
            return;
        }

        it = generic_token_map_.find(arg);
        if (it != generic_token_map_.end()) {
            llvm::outs() << "Generic token: " << it->first << "\n";
            return;
        }

        llvm::outs() << "Unknown token: " << arg << "\n";
    }
} // namespace ziv::cli::toolchain
