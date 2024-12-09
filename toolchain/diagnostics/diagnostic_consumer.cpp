// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_consumer.hpp"
#include "llvm/Support/raw_ostream.h"

namespace ziv::toolchain::diagnostics {

void ConsoleDiagnosticConsumer::consume(const Diagnostic &diagnostic) {
    DiagnosticMessage msg;
    msg.error_code = generate_error_code(diagnostic.kind);
    msg.kind = diagnostic.kind;
    msg.severity = diagnostic.kind.get_severity();
    msg.location = diagnostic.location;
    msg.message = diagnostic.message;

    // Get source context for the error
    auto context = source_extractor_.get_context(diagnostic.location);
    msg.source_line = context.line.str();
    msg.location.length = context.length;

    if (diagnostic.kind == DiagnosticKind::UnterminatedString()) {
        msg.hint = "Add a closing quotation mark";
        msg.notes.push_back("String literals must be closed with a matching quotation mark");
    }

    print_diagnostic(msg);

    exit(1);
}

void DiagnosticConsumer::print_diagnostic(const DiagnosticMessage& msg) {
    llvm::raw_ostream& os = llvm::errs();

    // Print location and error code
    os << color::BOLD;
    os << msg.location.file << ":" << msg.location.line << ":" << msg.location.column;
    os << " [" << msg.error_code << "]: ";

    // Print severity with appropriate color
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

    // Print main message
    os << color::RESET << ": " << msg.message << "\n";

    // Print source code excerpt with error highlighting
    if (!msg.source_line.empty()) {
        os << format_source_excerpt(msg.source_line, msg.location.column, msg.location.length);
    }

    // Print quick fix hint if available
    if (!msg.hint.empty()) {
        os << color::GREEN << "help" << color::RESET << ": " << msg.hint << "\n";
    }

    // Print additional notes
    for (const auto& note : msg.notes) {
        os << color::BLUE << "note" << color::RESET << ": " << note << "\n";
    }

    #ifdef DEBUG
    if (!msg.stack_trace.empty()) {
        os << color::MAGENTA << "stack trace:" << color::RESET << "\n";
        os << msg.stack_trace << "\n";
    }
    #endif

    os << color::CYAN << "learn more: " << msg.get_doc_url() << color::RESET << "\n\n";
}

} // namespace ziv::toolchain::diagnostic
