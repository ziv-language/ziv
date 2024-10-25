// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_COMMAND_HPP
#define ZIV_TOOLCHAIN_COMMAND_HPP

#include <string>

namespace ziv::cli::toolchain {
    class Command {
        public:
            virtual ~Command() = default;
            virtual void execute(const std::string &arg) = 0;
    };

} // namespace ziv::cli::toolchain

#endif // ZIV_TOOLCHAIN_COMMAND_HPP
