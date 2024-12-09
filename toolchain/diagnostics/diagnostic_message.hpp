// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_MESSAGE_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_MESSAGE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "llvm/Support/raw_ostream.h"
#include "diagnostic_kind.hpp"
#include "toolchain/source/source_location.hpp"

namespace ziv::toolchain::diagnostics {

struct DiagnosticMessage {
    DiagnosticMessage() : kind(DiagnosticKind::InvalidCharacter()),
                         severity(Severity::Error) {}

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
        return "https://ziv-lang.dev/diagnostics/" + error_code;
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
}

// Helper to generate error codes based on diagnostic kind
inline std::string generate_error_code(const DiagnosticKind& kind) {
    static const std::unordered_map<DiagnosticKind, std::string> code_map {
        {DiagnosticKind::InvalidCharacter(), "ZIV-1001"},
        {DiagnosticKind::UnterminatedString(), "ZIV-1002"},
        {DiagnosticKind::TypeMismatch(), "ZIV-2001"},
        {DiagnosticKind::UndeclaredIdentifier(), "ZIV-2002"},
        {DiagnosticKind::RedeclaredIdentifier(), "ZIV-2003"},
        // Add more mappings as needed...
    };

    auto it = code_map.find(kind);
    if (it != code_map.end()) {
        return it->second;
    }
    return "ZIV-0000"; // Unknown error code
}

inline std::string format_source_excerpt(const std::string& line, size_t col_start, size_t length) {
    if (line.empty()) return "";

    std::string result;
    result += "    " + line + "\n";
    result += "    " + std::string(col_start - 1, ' ') + color::RED;
    result += std::string(length > 0 ? length : 1, '^');
    result += std::string(color::RESET) + "\n";
    return result;
}

} // namespace ziv::toolchain::diagnostic

#endif // ZIV_TOOLCHAIN_DIAGNOSTIC_MESSAGE_HPP
