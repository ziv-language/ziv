// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_CLI_TOOLCHAIN_DRIVER_HPP
#define ZIV_CLI_TOOLCHAIN_DRIVER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "command.hpp"

namespace ziv::cli::toolchain {

class ToolchainDriver {
public:
    ToolchainDriver();
    void run(const std::string& command, const std::string& args);

private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands_;
};

}  // namespace ziv::cli::toolchain

#endif  // ZIV_CLI_TOOLCHAIN_DRIVER_HPP
