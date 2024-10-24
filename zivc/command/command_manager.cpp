// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "command_manager.hpp"
#include "toolchain/driver/driver.hpp"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

namespace ziv::cli::command {

    static llvm::cl::opt<bool> toolchain_command(
        "toolchain",
        llvm::cl::desc("Run the toolchain driver")
    );

    static llvm::cl::opt<std::string> source_command(
        "source",
        llvm::cl::desc("Review the source buffer"),
        llvm::cl::value_desc("filename")
    );

    void CommandManager::execute(int argc, char **argv) {
        std::string about = "Ziv Programming Language";
        llvm::cl::ParseCommandLineOptions(argc, argv, about.c_str());

        if (toolchain_command) {
            if (source_command.empty()) {
                llvm::errs() << "Error: No source buffer specified with --source\n";
                return;
            }
            handle_source(source_command);
        }
        else {
            llvm::errs() << "Error: No command specified\n";
        }
    }

    void CommandManager::handle_source(const std::string &filename) {
        ziv::toolchain::driver::Driver driver;
        driver.run("source", filename);
    }

} // namespace ziv::command
