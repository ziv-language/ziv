// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP

#pragma once
#include <string>
#include <vector>

#include "diagnostic_context.hpp"
#include "diagnostic_kind.hpp"
#include "diagnostic_message.hpp"
#include "toolchain/source/source_extractor.hpp"
#include "toolchain/source/source_location.hpp"

namespace ziv::toolchain::diagnostics {
struct Diagnostic {
    DiagnosticKind kind;
    source::SourceLocation location;
    std::string message;
    DiagnosticMessage details;
};

class DiagnosticConsumer {
public:
    virtual ~DiagnosticConsumer() = default;
    virtual void consume(const Diagnostic& diagnostic) = 0;

    void print_summary();
    bool has_errors() const {
        return error_count_ > 0;
    }
    size_t error_count() const {
        return error_count_;
    }
    const std::vector<Diagnostic>& diagnostics() const {
        return diagnostics_;
    }

protected:
    virtual void print_diagnostic(const DiagnosticMessage& msg);
    virtual void print_source_context(const DiagnosticMessage& msg);
    void print_context(const std::vector<std::string>& lines);

    size_t error_count_{0};
    size_t warning_count_{0};
    size_t error_sequence_{0};
    std::vector<Diagnostic> diagnostics_;
};

class ConsoleDiagnosticConsumer : public DiagnosticConsumer {
public:
    ConsoleDiagnosticConsumer(const source::SourceBuffer& source) : source_extractor_(source) {}
    void consume(const Diagnostic& diagnostic) override;

private:
    source::SourceExtractor source_extractor_;
};

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP
