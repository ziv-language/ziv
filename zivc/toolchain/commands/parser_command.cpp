// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser_command.hpp"

#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "toolchain/ast/printer.hpp"
#include "toolchain/ast/tree.hpp"
#include "toolchain/diagnostics/diagnostic_consumer.hpp"
#include "toolchain/parser/parser.hpp"

namespace ziv::cli::toolchain {

void ParserCommand::execute(const std::string& args) {
    llvm::vfs::FileSystem& fs = *llvm::vfs::getRealFileSystem();
    auto source = ziv::toolchain::source::SourceBuffer::from_file(fs, args);
    ziv::toolchain::lex::TokenBuffer buffer = ziv::toolchain::lex::TokenBuffer(*source);
    auto consumer = std::make_shared<ziv::toolchain::diagnostics::ConsoleDiagnosticConsumer>(
        *source);
    ziv::toolchain::lex::Lexer lexer(*source, consumer);

    lexer.lex();  // Lex the source file
    consumer->print_summary();

    const auto& tokens = lexer.get_tokens();

    ziv::toolchain::ast::AST ast;
    ziv::toolchain::parser::Parser parser(tokens, ast, consumer, *source);

    parser.parse();  // Parse the token buffer
    consumer->print_summary();

    ziv::toolchain::ast::Printer printer(ast);
    printer.print(llvm::outs());
}

}  // namespace ziv::cli::toolchain
