// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP

#pragma once
#include "diagnostic_kind.hpp"
#include "source_location.hpp"
#include "diagnostic_consumer.hpp"
#include <string>
#include <memory>
#include <vector>


namespace ziv::toolchain::diagnostic {
    class DiagnosticEmitter {
        public:
            DiagnosticEmitter(std::shared_ptr<DiagnosticConsumer> consumer): consumer_(consumer) {}

            template<typename... Args>
            // Emit a diagnostic with a message and a location.
            void emit(DiagnosticKind kind, SourceLocation location, Args&&... args) {
                std::string message = format(std::forward<Args>(args)...);
                consumer_->consume({kind, location, message});
            }

        private:
            std::shared_ptr<DiagnosticConsumer> consumer_;

            template<typename... Args>
            std::string format(Args&&... args) {
                std::string result;
                (result += ... += std::forward<Args>(args));
                return result;
            }
    };
} // namespace ziv::toolchain::diagnostic

#endif // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
