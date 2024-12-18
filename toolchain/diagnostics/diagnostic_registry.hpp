// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_REGISTRY_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_REGISTRY_HPP

#pragma once
#include <array>
#include <string_view>

#include "diagnostic_metadata.hpp"

namespace ziv::toolchain::diagnostics {
class DiagnosticKind;

class DiagnosticRegistry {
public:
    static const DiagnosticMetadata& get_metadata(const DiagnosticKind& kind);

private:
    static const std::array<DiagnosticMetadata, 1024>& get_metadata_table();
};
}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_REGISTRY_HPP
