// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "symbol.hpp"

namespace ziv::toolchain::semantics {

llvm::StringRef Symbol::get_kind_name(Kind kind) {
    static const llvm::StringLiteral names[] = {
#define ZIV_SYMBOL_KIND(NAME) #NAME,
#include "symbol_kind_registry.def"
    };
    return names[static_cast<size_t>(kind)];
}

}  // namespace ziv::toolchain::semantics
