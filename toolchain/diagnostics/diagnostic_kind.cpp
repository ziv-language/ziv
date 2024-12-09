// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_kind.hpp"

namespace ziv::toolchain::diagnostic {
    llvm::StringRef DiagnosticKind::get_name() const {
        static constexpr llvm::StringLiteral names[] = {
            #define ZIV_DIAGNOSTIC(NAME) #NAME,
            #include "diagnostic_kind_registry.def"
        };
        return names[static_cast<int>(kind_)];
    }

    Severity DiagnosticKind::get_severity() const {
        // Define the severity of each diagnostic kind
        switch (kind_) {
            case KindEnum::InvalidCharacter:
            case KindEnum::UnterminatedString:
            case KindEnum::UnterminatedComment:
            case KindEnum::TabInIndentation:
            case KindEnum::InvalidIndentation:
            case KindEnum::InvalidEscapeSequence:
            case KindEnum::UnexpectedToken:
                return Severity::Error;
            default:
                return Severity::Warning;
        }
    }



} // namespace ziv::toolchain::diagnostic
