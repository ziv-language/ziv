// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP

#pragma once
#include <cstdint>
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::diagnostic {

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

        Severity get_severity() const;

        DiagnosticKind() = delete;
        bool operator==(const DiagnosticKind&) const = default;
        llvm::StringRef get_name() const;

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

#endif // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP
