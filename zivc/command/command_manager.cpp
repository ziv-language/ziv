// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "command_manager.hpp"
#include "zivc/toolchain/driver.hpp"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

namespace ziv::cli::command {

    enum class Command {
        Toolchain,
        Compile,
        Execute,
        Invalid // Default value
    };

    static llvm::cl::SubCommand toolchain_command("toolchain", "Run the toolchain driver");

    static llvm::cl::opt<std::string> source_command(
        "source",
        llvm::cl::desc("Review the source buffer"),
        llvm::cl::value_desc("filename")
    );

    static llvm::cl::opt<std::string> token_command(
        "token",
        llvm::cl::desc("Review the token buffer"),
        llvm::cl::value_desc("token")
    );

    void CommandManager::execute(int argc, char **argv) {
        std::string about = "Ziv Programming Language";
        llvm::cl::ParseCommandLineOptions(argc, argv, about.c_str());

        if (toolchain_command) {
            if (!source_command.getValue().empty()) {
                handle_source(source_command);
            }
            if (!token_command.getValue().empty()) {
                handle_token(token_command);
            }
        } else {
            llvm::errs() << "Error: No command specified\n";
        }
    }

    void CommandManager::handle_source(const std::string &filename) {
        ziv::cli::toolchain::ToolchainDriver driver;
        driver.run("source", filename);
    }

    void CommandManager::handle_token(const std::string &token) {
        ziv::cli::toolchain::ToolchainDriver driver;
        driver.run("token", token);
    }

} // namespace ziv::command
