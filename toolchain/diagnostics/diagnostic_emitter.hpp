// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP

#pragma once
#include <memory>
#include <string>
#include <vector>

#include "diagnostic_consumer.hpp"
#include "diagnostic_kind.hpp"
#include "diagnostic_message.hpp"
#include "toolchain/source/source_location.hpp"


namespace ziv::toolchain::diagnostics {
class DiagnosticEmitter {
public:
    DiagnosticEmitter(std::shared_ptr<DiagnosticConsumer> consumer) : consumer_(consumer) {}

    template<typename... Args>
    // Emit a diagnostic message with the given kind, location, and arguments.
    void emit(DiagnosticKind kind, source::SourceLocation location, Args&&... args) {
        const auto& metadata = kind.get_metadata();

        DiagnosticMessage msg(kind);
        msg.error_code = std::string(metadata.code);
        msg.severity = metadata.severity;
        msg.message = std::vformat(std::string(metadata.message),
                                   std::make_format_args(std::forward<Args>(args)...));
        msg.hint = std::string(metadata.hint);
        msg.notes.assign(metadata.notes.begin(), metadata.notes.end());
        msg.location = location;

        consumer_->consume({kind, location, msg.message});
    }

private:
    std::shared_ptr<DiagnosticConsumer> consumer_;
};
}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
