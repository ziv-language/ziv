// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_METADATA_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_METADATA_HPP

#pragma once
#include <array>
#include <string_view>

#include "diagnostic_base.hpp"

namespace ziv::toolchain::diagnostics {

struct DiagnosticMetadata {
    std::string_view code;
    Severity severity;
    std::string_view message;
    std::string_view hint;
    std::array<std::string_view, 4> notes;
    std::string_view doc_path;
};

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_METADATA_HPP
