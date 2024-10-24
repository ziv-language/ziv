// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "driver.hpp"
#include "commands/source_command.hpp"

namespace ziv::toolchain::driver {

    Driver::Driver() {
        // Register commands
        commands_["source"] = std::make_unique<SourceCommand>();
    }

    void Driver::run(const std::string &command, const std::string &arg) {
        auto it = commands_.find(command);
        if (it == commands_.end()) {
            llvm::errs() << "Unknown command: " << command << "\n";
            return;
        }

        it->second->execute(arg);
    }
} // namespace ziv::toolchain::driver
