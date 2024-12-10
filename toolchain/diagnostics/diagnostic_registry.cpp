// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "diagnostic_registry.hpp"

#include "diagnostic_kind.hpp"

namespace ziv::toolchain::diagnostics {

const std::array<DiagnosticMetadata, 1024>& DiagnosticRegistry::get_metadata_table() {
    static const auto table = []() {
        std::array<DiagnosticMetadata, 1024> metadata{};
#define ZIV_DIAGNOSTIC_INFO(NAME, CODE, SEVERITY, MESSAGE, HINT, NOTES, PATH) \
    metadata[DiagnosticKind::NAME().get_index()] =                            \
        DiagnosticMetadata{"ZIV-" #CODE, Severity::SEVERITY, MESSAGE, HINT, {NOTES}, PATH};
#include "diagnostic_kind_registry.def"
#undef ZIV_DIAGNOSTIC_INFO
        return metadata;
    }();
    return table;
}

const DiagnosticMetadata& DiagnosticRegistry::get_metadata(const DiagnosticKind& kind) {
    return get_metadata_table()[kind.get_index()];
}

}  // namespace ziv::toolchain::diagnostics
