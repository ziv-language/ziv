// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "lex_command.hpp"

#include "toolchain/ast/tree.hpp"
#include "toolchain/parser/parser.hpp"
#include "toolchain/ast/tree.hpp"
#include "toolchain/diagnostics/diagnostic_consumer.hpp"

namespace ziv::cli::toolchain {

    void LexerCommand::execute(const std::string &arg) {
        llvm::vfs::FileSystem &fs = *llvm::vfs::getRealFileSystem();
        auto source = ziv::toolchain::source::SourceBuffer::from_file(fs, arg);
        ziv::toolchain::lex::TokenBuffer buffer;
        auto diagnostics = std::make_shared<ziv::toolchain::diagnostics::ConsoleDiagnosticConsumer>(*source);
        ziv::toolchain::lex::Lexer lexer(*source, buffer, diagnostics);

    lexer.lex();  // Lex the source file

    const auto& tokens = lexer.get_tokens();

    for (const auto& token : tokens) {
        llvm::outs() << "Token: " << token.get_spelling()
                     << " (kind: " << static_cast<int>(token.get_kind())
                     << ", line: " << token.get_line() << ", column: " << token.get_column()
                     << ", name: " << token.get_name() << ")\n";
    }
};

}  // namespace ziv::cli::toolchain
