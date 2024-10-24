// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "command/command_manager.hpp"

int main(int argc, char **argv) {
    ziv::cli::command::CommandManager commandManager;
    commandManager.execute(argc, argv);
    return 0;
}
