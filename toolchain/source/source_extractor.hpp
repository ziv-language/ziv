// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SOURCE_EXTRACTOR_HPP
#define ZIV_TOOLCHAIN_SOURCE_EXTRACTOR_HPP

#include "source_buffer.hpp"
#include "source_location.hpp"
#include <vector>

namespace ziv::toolchain::source {

class SourceExtractor {
public:
    struct SourceContext {
        llvm::StringRef line;        // The source line
        size_t start_col;            // Start column of the error
        size_t length;               // Length of the error span
        std::vector<llvm::StringRef> context_before;  // Lines before
        std::vector<llvm::StringRef> context_after;   // Lines after
    };

    explicit SourceExtractor(const SourceBuffer& buffer): buffer_(buffer) {
        build_line_table();
    }

    // Get a specific line
    llvm::StringRef get_line(size_t line_number) const;

    // Get source context for a location
    SourceContext get_context(const SourceLocation& loc, size_t context_lines = 2) const;

private:
    const SourceBuffer& buffer_;
    std::vector<size_t> line_offsets_;

    void build_line_table();
};

} // namespace ziv::toolchain::source

#endif
