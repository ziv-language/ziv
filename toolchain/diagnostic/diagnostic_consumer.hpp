// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP

#pragma once
#include "diagnostic_kind.hpp"
#include "source_location.hpp"
#include <string>

namespace ziv::toolchain::diagnostic {

    struct Diagnostic {
        DiagnosticKind kind;
        SourceLocation location;
        std::string message;
    };

    class DiagnosticConsumer {
        public:
            virtual ~DiagnosticConsumer() = default;
            virtual void consume(const Diagnostic &diagnostic) = 0;
            bool has_errors() const { return error_count_ > 0; }
        protected:
            virtual void print_diagnostic(const Diagnostic &diagnostic);
            size_t error_count_ {0};
    };


    // ConsoleDiagnosticConsumer is a diagnostic consumer that prints diagnostics to the console.
    class ConsoleDiagnosticConsumer: public DiagnosticConsumer {
        public:
            void consume(const Diagnostic &diagnostic) override;
    };

} // namespace ziv::toolchain::diagnostic

#endif // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP
