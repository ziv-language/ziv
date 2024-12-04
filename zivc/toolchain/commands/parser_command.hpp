// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_PARSER_COMMAND_HPP
#define ZIV_TOOLCHAIN_PARSER_COMMAND_HPP

#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "toolchain/ast/tree.hpp"
#include "toolchain/parser/parser.hpp"
#include "zivc/toolchain/command.hpp"

namespace ziv::cli::toolchain {
class ParserCommand : public Command {
public:
    void execute(const std::string& args) override;
};
}  // namespace ziv::cli::toolchain

#endif  // ZIV_TOOLCHAIN_PARSER_COMMAND_HPP
