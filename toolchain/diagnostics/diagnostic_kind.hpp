// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP

#pragma once
#include <cstdint>
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::diagnostics {

    enum class Severity: uint8_t {
            #define ZIV_DIAGNOSTIC_SEVERITY(NAME) NAME,
            #include "diagnostic_kind_registry.def"
            #undef ZIV_DIAGNOSTIC_SEVERITY
    };

    class DiagnosticKind {
        enum class KindEnum: uint8_t {
            // Define enum values first
            #define ZIV_DIAGNOSTIC_KIND(NAME) NAME,
            #include "diagnostic_kind_registry.def"
            #undef ZIV_DIAGNOSTIC_KIND
        };

    public:
        // Then define static methods
        #define ZIV_DIAGNOSTIC_KIND(NAME) \
            static constexpr DiagnosticKind NAME() { return DiagnosticKind(KindEnum::NAME); }
        #include "diagnostic_kind_registry.def"
        #undef ZIV_DIAGNOSTIC_KIND

        DiagnosticKind() = delete;
        bool operator==(const DiagnosticKind&) const = default;

        constexpr operator KindEnum() const { return kind_; }
        KindEnum get_kind() const { return kind_; }
        llvm::StringRef get_name() const;
        Severity get_severity() const;
        friend struct std::hash<DiagnosticKind>;

    private:
        constexpr DiagnosticKind(KindEnum kind): kind_(kind) {}
        KindEnum kind_;
    };

    // Add severity enum
    enum class DiagnosticSeverity: uint8_t {
        #define ZIV_DIAGNOSTIC_SEVERITY(NAME) NAME,
        #include "diagnostic_kind_registry.def"
        #undef ZIV_DIAGNOSTIC_SEVERITY
    };
} // namespace ziv::toolchain::diagnostic

namespace std {
    template<>
    struct hash<ziv::toolchain::diagnostics::DiagnosticKind> {
        size_t operator()(const ziv::toolchain::diagnostics::DiagnosticKind& k) const {
            return std::hash<uint8_t>{}(static_cast<uint8_t>(k.get_kind()));
        }
    };
} // namespace std

#endif // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP
