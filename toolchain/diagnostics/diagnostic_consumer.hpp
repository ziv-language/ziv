// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP

#pragma once
#include <string>

#include "diagnostic_kind.hpp"
#include "diagnostic_message.hpp"
#include "toolchain/source/source_extractor.hpp"
#include "toolchain/source/source_location.hpp"

namespace ziv::toolchain::diagnostics {

struct Diagnostic {
    DiagnosticKind kind;
    source::SourceLocation location;
    std::string message;
};

class DiagnosticConsumer {
public:
    virtual ~DiagnosticConsumer() = default;
    virtual void consume(const Diagnostic& diagnostic) = 0;
    bool has_errors() const {
        return error_count_ > 0;
    }
    size_t error_count() const {
        return error_count_;
    }

protected:
    virtual void print_diagnostic(const DiagnosticMessage& msg);
    size_t error_count_{0};
};


// ConsoleDiagnosticConsumer is a diagnostic consumer that prints diagnostics to the console.
class ConsoleDiagnosticConsumer : public DiagnosticConsumer {
public:
    ConsoleDiagnosticConsumer(const source::SourceBuffer& source) : source_extractor_(source) {}
    void consume(const Diagnostic& diagnostic) override;

private:
    source::SourceExtractor source_extractor_;
};

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_CONSUMER_HPP
