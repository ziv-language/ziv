// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_consumer.hpp"

#include "llvm/Support/raw_ostream.h"

namespace ziv::toolchain::diagnostics {

void DiagnosticConsumer::print_diagnostic(const DiagnosticMessage& msg) {
    llvm::raw_ostream& os = llvm::errs();
    size_t error_num = ++error_sequence_;

    os << color::BOLD << '[' << error_num << ']' << color::RESET << ' ' << msg.location.file << ':'
       << msg.location.line << ':' << msg.location.column << " <" << msg.error_code << ">: ";

    switch (msg.severity) {
    case Severity::Error:
        os << color::RED << "error" << color::RESET;
        DiagnosticContext::instance().increment_error_count();
        break;
    case Severity::Warning:
        os << color::YELLOW << "warning" << color::RESET;
        DiagnosticContext::instance().increment_warning_count();
        break;
    case Severity::Note:
        os << color::BLUE << "note" << color::RESET;
        break;
    default:
        break;
    }

    os << ": " << msg.message << "\n";
    print_source_context(msg);

    if (!msg.hint.empty()) {
        os << color::GREEN << "\u2192 help" << color::RESET << ": " << msg.hint << "\n";
    }

    for (const auto& note : msg.notes) {
        if (!note.empty()) {
            os << color::BLUE << "\u2022 note" << color::RESET << ": " << note << "\n";
        }
    }

    os << msg.get_doc_url() << "\n\n";
}

void DiagnosticConsumer::print_source_context(const DiagnosticMessage& msg) {
    if (!msg.source_line.empty()) {
        llvm::errs() << "     " << msg.source_line << "\n"
                     << "     " << std::string(msg.location.column - 1, ' ') << color::RED << "^"
                     << color::RESET << "\n";
    }
}

void DiagnosticConsumer::print_context(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        llvm::errs() << "     " << line << "\n";
    }
}

void DiagnosticConsumer::print_summary() {
    auto& context = DiagnosticContext::instance();
    if (context.get_error_count() > 0 || context.get_warning_count() > 0) {
        llvm::errs() << color::BOLD << "\u2514 " << context.get_error_count() << " error"
                     << (context.get_error_count() != 1 ? "s" : "");

        if (context.get_warning_count() > 0) {
            llvm::errs() << " and " << context.get_warning_count() << " warning"
                         << (context.get_warning_count() != 1 ? "s" : "");
        }

        llvm::errs() << " generated." << color::RESET << "\n\n";

        if (context.get_error_count() > 0) {
            std::exit(1);
        }
    }
}

void ConsoleDiagnosticConsumer::consume(const Diagnostic& diagnostic) {
    const auto& metadata = diagnostic.kind.get_metadata();
    if (metadata.severity == Severity::Error)
        error_count_++;

    diagnostics_.push_back(diagnostic);
    print_diagnostic(diagnostic.details);
}

}  // namespace ziv::toolchain::diagnostics
