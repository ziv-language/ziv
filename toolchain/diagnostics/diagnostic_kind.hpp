// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP
#define ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP

#pragma once
#include <cstdint>

#include "diagnostic_base.hpp"
#include "diagnostic_registry.hpp"

namespace ziv::toolchain::diagnostics {
class DiagnosticKind {
    enum class KindEnum : uint8_t {
#define ZIV_DIAGNOSTIC_KIND(NAME) NAME,
#include "diagnostic_kind_registry.def"
#undef ZIV_DIAGNOSTIC_KIND
    };

public:
#define ZIV_DIAGNOSTIC_KIND(NAME)              \
    static constexpr DiagnosticKind NAME() {   \
        return DiagnosticKind(KindEnum::NAME); \
    }
#include "diagnostic_kind_registry.def"
#undef ZIV_DIAGNOSTIC_KIND

    DiagnosticKind() = delete;
    bool operator==(const DiagnosticKind&) const = default;

    constexpr operator KindEnum() const {
        return kind_;
    }
    KindEnum get_kind() const {
        return kind_;
    }
    size_t get_index() const {
        return static_cast<size_t>(kind_);
    }
    llvm::StringRef get_name() const;

    const DiagnosticMetadata& get_metadata() const {
        return DiagnosticRegistry::get_metadata(*this);
    }

    friend struct std::hash<DiagnosticKind>;

private:
    constexpr DiagnosticKind(KindEnum kind) : kind_(kind) {}
    KindEnum kind_;
};
}  // namespace ziv::toolchain::diagnostics

#endif  // ZIV_TOOLCHAIN_DIAGNOSTIC_DIAGNOSTIC_KIND_HPP
