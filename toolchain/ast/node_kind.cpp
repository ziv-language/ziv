// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "node_kind.hpp"

namespace ziv::toolchain::ast {
    llvm::StringRef NodeKind::get_name() const {
        static const llvm::StringLiteral kind_names[] = {
            #define ZIV_NODE_KIND(NAME) #NAME,
            #include "node_kind_registry.def"
        };
        return kind_names[static_cast<int>(kind)];
    }
} // namespace ziv::toolchain::ast
