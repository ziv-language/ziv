// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DRIVER_SOURCE_COMMAND_HPP
#define ZIV_TOOLCHAIN_DRIVER_SOURCE_COMMAND_HPP

#include "toolchain/driver/command.hpp"
#include "toolchain/source/source_buffer.hpp"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/VirtualFileSystem.h"

namespace ziv::toolchain::driver {
    class SourceCommand : public Command {
    public:
        void execute(const std::string &arg) override;
    };
} // namespace ziv::toolchain::driver

#endif // ZIV_TOOLCHAIN_DRIVER_SOURCE_COMMAND_HPP
