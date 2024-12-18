// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP

#include <format>
#include <memory>
#include <string>

#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::diagnostics {
class DiagnosticEmitter {
public:
    DiagnosticEmitter(std::shared_ptr<DiagnosticConsumer> consumer,
                      const source::SourceBuffer& source_buffer)
        : consumer_(consumer), source_extractor_(source_buffer) {}

    template<typename... Args>
    void emit(DiagnosticKind kind, source::SourceLocation location, Args&&... args) {
        const auto& metadata = kind.get_metadata();
        auto context = source_extractor_.get_context(location);

        DiagnosticMessage msg(kind);
        msg.error_code = std::string(metadata.code);
        msg.severity = metadata.severity;

        std::vector<std::string> formatted_args;
        (formatted_args.push_back(to_string_helper(std::forward<Args>(args))), ...);

        msg.message = format_with_args(std::string(metadata.message), formatted_args);

        msg.hint = std::string(metadata.hint);
        msg.notes.assign(metadata.notes.begin(), metadata.notes.end());
        msg.location = location;
        msg.source_line = context.line.str();
        msg.location.length = context.length;
        msg.path = metadata.doc_path;

        consumer_->consume({kind, location, msg.message, msg});
    }

private:
    std::string format_with_args(const std::string& fmt, const std::vector<std::string>& args) {
        switch (args.size()) {
        case 0:
            return fmt;
        case 1:
            return std::vformat(fmt, std::make_format_args(args[0]));
        case 2:
            return std::vformat(fmt, std::make_format_args(args[0], args[1]));
        case 3:
            return std::vformat(fmt, std::make_format_args(args[0], args[1], args[2]));
        default:
            return fmt;
        }
    }

    std::string to_string_helper(const char* arg) {
        return std::string(arg);
    }

    std::string to_string_helper(char arg) {
        return std::string(1, arg);
    }

    std::string to_string_helper(const llvm::StringRef& arg) {
        return arg.str();
    }

    template<typename T>
    std::string to_string_helper(const T& arg) {
        return std::to_string(arg);
    }

    std::shared_ptr<DiagnosticConsumer> consumer_;
    source::SourceExtractor source_extractor_;
};
}  // namespace ziv::toolchain::diagnostics
#endif
