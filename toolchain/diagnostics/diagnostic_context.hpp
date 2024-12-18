// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception


#ifndef ZIV_TOOLCHAIN_DIAGNOSTICS_DIAGNOSTIC_CONTEXT_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTICS_DIAGNOSTIC_CONTEXT_HPP
#pragma once
#include <atomic>
#include <memory>
#include <vector>

#include "compilation_phase.hpp"

namespace ziv::toolchain::diagnostics {

class DiagnosticContext {
public:
    static DiagnosticContext& instance() {
        static DiagnosticContext context;
        return context;
    }

    void increment_error_count() {
        error_count_++;
    }
    size_t get_error_count() const {
        return error_count_;
    }

    void increment_warning_count() {
        warning_count_++;
    }
    size_t get_warning_count() const {
        return warning_count_;
    }

    void set_phase(CompilationPhase phase) {
        current_phase_ = phase;
    }
    CompilationPhase get_phase() const {
        return current_phase_;
    }

    bool should_continue() const {
        return error_count_ == 0 || current_phase_ == CompilationPhase::Parsing;
    }

    void reset() {
        error_count_ = 0;
        warning_count_ = 0;
        current_phase_ = CompilationPhase::Lexing;
    }

private:
    std::atomic<size_t> error_count_{0};
    std::atomic<size_t> warning_count_{0};
    CompilationPhase current_phase_{CompilationPhase::Lexing};
};

class PhaseGuard {
public:
    PhaseGuard(CompilationPhase phase) {
        DiagnosticContext::instance().set_phase(phase);
    }

    ~PhaseGuard() {
        if (!DiagnosticContext::instance().should_continue()) {
            std::exit(1);
        }
    }
};

}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTICS_DIAGNOSTIC_CONTEXT_HPP
