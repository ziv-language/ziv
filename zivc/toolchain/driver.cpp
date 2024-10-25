// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "driver.hpp"
#include "commands/source_command.hpp"
#include "commands/lex_command.hpp"

namespace ziv::cli::toolchain {

    ToolchainDriver::ToolchainDriver() {
        // Register commands
        commands_["source"] = std::make_unique<SourceCommand>();
        commands_["token"] = std::make_unique<TokenCommand>();
    }

    void ToolchainDriver::run(const std::string &command, const std::string &arg) {
        auto it = commands_.find(command);
        if (it == commands_.end()) {
            llvm::errs() << "Unknown command: " << command << "\n";
            return;
        }

        it->second->execute(arg);
    }

} // namespace ziv::cli::toolchain
