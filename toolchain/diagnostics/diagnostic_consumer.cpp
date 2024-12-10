// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_consumer.hpp"

#include "llvm/Support/raw_ostream.h"

namespace ziv::toolchain::diagnostics {

void ConsoleDiagnosticConsumer::consume(const Diagnostic& diagnostic) {
    const auto& metadata = diagnostic.kind.get_metadata();

    DiagnosticMessage msg(diagnostic.kind);
    msg.error_code = std::string(metadata.code);
    msg.kind = diagnostic.kind;
    msg.severity = metadata.severity;
    msg.location = diagnostic.location;
    msg.message = diagnostic.message;
    msg.hint = std::string(metadata.hint);
    msg.notes.assign(metadata.notes.begin(), metadata.notes.end());

    // Get source context for the error
    auto context = source_extractor_.get_context(diagnostic.location);
    msg.source_line = context.line.str();
    msg.location.length = context.length;

    print_diagnostic(msg);

    exit(1);
}

void DiagnosticConsumer::print_diagnostic(const DiagnosticMessage& msg) {
    llvm::raw_ostream& os = llvm::errs();

    // Print location and error code
    os << msg.location.file << ":" << msg.location.line << ":" << msg.location.column << " ["
       << msg.error_code << "]: ";

    // Print severity and message
    switch (msg.severity) {
    case Severity::Error:
        os << color::RED << "error" << color::RESET;
        break;
    case Severity::Warning:
        os << color::YELLOW << "warning" << color::RESET;
        break;
    case Severity::Note:
        os << color::BLUE << "note" << color::RESET;
        break;
    default:
        break;
    }
    os << ": " << msg.message << "\n";

    // Print source code with caret
    if (!msg.source_line.empty()) {
        os << "     " << msg.source_line << "\n";
        os << "     " << std::string(msg.location.column - 1, ' ') << color::RED << "^"
           << color::RESET << "\n";
    }

    // Print hint if available
    if (!msg.hint.empty()) {
        os << color::GREEN << "help" << color::RESET << ": " << msg.hint << "\n";
    }

    // Print notes
    for (const auto& note : msg.notes) {
        if (!note.empty()) {
            os << color::BLUE << "note" << color::RESET << ": " << note << "\n";
        }
    }

    // Print learn more link
    os << msg.get_doc_url() << "\n\n";
}

}  // namespace ziv::toolchain::diagnostics
