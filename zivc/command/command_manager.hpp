// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_COMMAND_MANAGER_HPP
#define ZIV_COMMAND_MANAGER_HPP

#include <string>

namespace ziv::cli::command {

class CommandManager {
    public:
        void execute(int argc, char **argv);

    private:
        void handle_source(const std::string &filename);
};

} // namespace ziv::zivc::command

#endif // ZIV_COMMAND_MANAGER_HPP
