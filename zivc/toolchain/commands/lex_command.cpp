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

    void LexerCommand::execute(const std::string &arg) {
        llvm::vfs::FileSystem &fs = *llvm::vfs::getRealFileSystem();
        auto source = ziv::toolchain::source::SourceBuffer::from_file(fs, arg);
        ziv::toolchain::lex::TokenBuffer buffer;
        ziv::toolchain::lex::Lexer lexer(*source, buffer);

        lexer.lex(); // Lex the source file

        const auto& tokens = buffer.get_tokens();

        for (const auto& token : tokens) {
            llvm::outs() << "Token: " << token.get_spelling()
                            << " (kind: " << static_cast<int>(token.get_kind())
                            << ", line: " << token.get_line()
                            << ", column: " << token.get_column()
                            << ", name: " << token.get_name() << ")\n";
        }
    };

} // namespace ziv::cli::toolchain
