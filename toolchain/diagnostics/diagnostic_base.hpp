// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_BASE_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_BASE_HPP

#pragma once
#include <cstdint>

#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::diagnostics {

class DiagnosticKind;

enum class Severity : uint8_t {
#define ZIV_DIAGNOSTIC_SEVERITY(NAME) NAME,
#include "diagnostic_kind_registry.def"
#undef ZIV_DIAGNOSTIC_SEVERITY
};

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_BASE_HPP
