// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_kind.hpp"

namespace ziv::toolchain::diagnostics {

llvm::StringRef DiagnosticKind::get_name() const {
    static constexpr llvm::StringLiteral names[] = {
#define ZIV_DIAGNOSTIC_KIND(NAME) #NAME,
#include "diagnostic_kind_registry.def"
#undef ZIV_DIAGNOSTIC_KIND
    };
    return names[static_cast<size_t>(kind_)];
}

}  // namespace ziv::toolchain::diagnostics
