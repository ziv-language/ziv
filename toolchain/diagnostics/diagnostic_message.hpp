// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_MESSAGE_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_MESSAGE_HPP

#pragma once
#include <string>
#include <vector>

#include "diagnostic_registry.hpp"
#include "toolchain/source/source_location.hpp"

namespace ziv::toolchain::diagnostics {

struct DiagnosticMessage {
    DiagnosticMessage(DiagnosticKind k) : kind(k) {}

    std::string error_code;
    DiagnosticKind kind;
    Severity severity;
    source::SourceLocation location;
    std::string message;
    std::string hint;
    std::string source_line;
    std::vector<std::string> notes;
    std::string stack_trace;

    std::string get_doc_url() const {
        const auto& metadata = DiagnosticRegistry::get_metadata(kind);
        return "https://ziv-language.github.io/book/diagnostics/" + error_code;
    }
};

namespace color {
static const char* const RED = "\033[1;31m";
static const char* const GREEN = "\033[1;32m";
static const char* const YELLOW = "\033[1;33m";
static const char* const BLUE = "\033[1;34m";
static const char* const MAGENTA = "\033[1;35m";
static const char* const CYAN = "\033[1;36m";
static const char* const RESET = "\033[0m";
static const char* const BOLD = "\033[1m";
static const char* const UNDERLINE = "\033[4m";
}  // namespace color

inline std::string format_source_excerpt(const std::string& line, size_t col_start, size_t length) {
    if (line.empty())
        return "";

    std::string result;
    result += "    " + line + "\n";
    result += "    " + std::string(col_start - 1, ' ') + color::RED;
    result += std::string(length > 0 ? length : 1, '^');
    result += std::string(color::RESET) + "\n";
    return result;
}

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_MESSAGE_HPP
