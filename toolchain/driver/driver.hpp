// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DRIVER_HPP
#define ZIV_TOOLCHAIN_DRIVER_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include "command.hpp"

namespace ziv::toolchain::driver {
    class Driver {
    public:
        Driver();
        void run(const std::string &command, const std::string &arg);

    private:
        std::unordered_map<std::string, std::unique_ptr<Command>> commands_;
    };
} // namespace ziv::toolchain::driver

#endif // ZIV_TOOLCHAIN_DRIVER_HPP
