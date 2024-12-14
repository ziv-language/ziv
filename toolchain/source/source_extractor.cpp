// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "source_extractor.hpp"

namespace ziv::toolchain::source {

void SourceExtractor::build_line_table() {
    line_offsets_.clear();
    line_offsets_.push_back(0);  // First line starts at offset 0

    llvm::StringRef content = buffer_.get_contents();
    size_t pos = 0;
    while ((pos = content.find('\n', pos)) != llvm::StringRef::npos) {
        pos++;  // Move past the newline
        line_offsets_.push_back(pos);
    }
}

llvm::StringRef SourceExtractor::get_line(size_t line_number) const {
    if (line_number == 0 || line_number > line_offsets_.size()) {
        return llvm::StringRef();
    }

    llvm::StringRef content = buffer_.get_contents();
    size_t start = line_offsets_[line_number - 1];
    size_t length;

    if (line_number < line_offsets_.size()) {
        length = line_offsets_[line_number] - start - 1;  // Exclude the newline
    } else {
        length = content.size() - start;
    }

    return content.substr(start, length);
}

SourceExtractor::SourceContext SourceExtractor::get_context(const SourceLocation& loc,
                                                            size_t context_lines) const {
    SourceContext context;

    // Get the main error line
    context.line = get_line(loc.line);
    context.start_col = loc.column;
    context.length = loc.length > 0 ? loc.length : 1;

    // Get context lines before
    for (size_t i = context_lines; i > 0 && loc.line > i; --i) {
        llvm::StringRef before_line = get_line(loc.line - i);
        if (!before_line.empty()) {
            context.context_before.push_back(before_line);
        }
    }

    // Get context lines after
    for (size_t i = 1; i <= context_lines; ++i) {
        llvm::StringRef after_line = get_line(loc.line + i);
        if (!after_line.empty()) {
            context.context_after.push_back(after_line);
        }
    }

    return context;
}

}  // namespace ziv::toolchain::source
