// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_COMPILATION_PHASE_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_COMPILATION_PHASE_HPP

#pragma once
#include "diagnostic_context.hpp"

namespace ziv::toolchain::diagnostics {

enum class CompilationPhase {
    Lexing,
    Parsing,
    SemanticAnalysis,
    CodeGeneration,
    Linking,
    Optimization,
    Execution,
};

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_COMPILATION_PHASE_HPP
