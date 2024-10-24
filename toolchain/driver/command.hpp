// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DRIVER_COMMAND_HPP
#define ZIV_TOOLCHAIN_DRIVER_COMMAND_HPP

#include <string>

namespace ziv::toolchain::driver {
    class Command {
    public:
        virtual ~Command() = default; // Virtual destructor
        virtual void execute(const std::string &arg) = 0;

    };
} // namespace ziv::toolchain::driver

#endif // ZIV_TOOLCHAIN_DRIVER_COMMAND_HPP
