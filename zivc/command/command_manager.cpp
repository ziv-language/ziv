// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "command_manager.hpp"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "zivc/toolchain/driver.hpp"

namespace ziv::cli::command {

enum class Command {
    Toolchain,
    Compile,
    Execute,
    Invalid  // Default value
};

static llvm::cl::SubCommand toolchain_command("toolchain", "Run the toolchain driver");

static llvm::cl::opt<bool> source_command("source",
                                          llvm::cl::desc("Review the source buffer"),
                                          llvm::cl::sub(toolchain_command));

static llvm::cl::opt<bool> lexer_command("lexer",
                                         llvm::cl::desc("Review the lexer buffer"),
                                         llvm::cl::sub(toolchain_command));

static llvm::cl::opt<bool> parser_command("dump-tree",
                                          llvm::cl::desc("Dump the AST tree"),
                                          llvm::cl::sub(toolchain_command));

static llvm::cl::opt<std::string> input_file(llvm::cl::Positional,
                                             llvm::cl::desc("<input file>"),
                                             llvm::cl::sub(toolchain_command),
                                             llvm::cl::Required);

void CommandManager::execute(int argc, char** argv) {
    std::string about = "Ziv Programming Language";
    llvm::cl::ParseCommandLineOptions(argc, argv, about.c_str());

    if (toolchain_command) {
        if (source_command) {
            handle_source(input_file);
        }
        if (lexer_command) {
            handle_lexer(input_file);
        }
        if (parser_command) {
            handle_parser(input_file);
        }
    } else {
        llvm::errs() << "Error: No command specified\n";
    }
}

void CommandManager::handle_source(const std::string& filename) {
    ziv::cli::toolchain::ToolchainDriver driver;
    driver.run("source", filename);
}

void CommandManager::handle_lexer(const std::string& filename) {
    ziv::cli::toolchain::ToolchainDriver driver;
    driver.run("lexer", filename);
}

void CommandManager::handle_parser(const std::string& filename) {
    ziv::cli::toolchain::ToolchainDriver driver;
    driver.run("parser", filename);
}

}  // namespace ziv::cli::command
