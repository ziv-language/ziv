// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_consumer.hpp"
#include "llvm/Support/raw_ostream.h"

namespace ziv::toolchain::diagnostic {

    void ConsoleDiagnosticConsumer::consume(const Diagnostic &diagnostic) {
        if (diagnostic.kind.get_severity() == Severity::Error) {
            error_count_++;
            print_diagnostic(diagnostic);
            // exit on error
            std::exit(1);
        }
    }

    void DiagnosticConsumer::print_diagnostic(const Diagnostic &diagnostic) {
        llvm::errs() << diagnostic.location.file << ":"
                    << diagnostic.location.line << ":"
                    << diagnostic.location.column << ": "
                    << diagnostic.message << "\n";
    }

} // namespace ziv::toolchain::diagnostic
