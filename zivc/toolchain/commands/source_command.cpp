// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "source_command.hpp"

namespace ziv::cli::toolchain {

void SourceCommand::execute(const std::string& args) {
    print_source(args);
}

void SourceCommand::print_source(const std::string& filename) {
    llvm::vfs::FileSystem& fs = *llvm::vfs::getRealFileSystem();
    auto buffer = ziv::toolchain::source::SourceBuffer::from_file(fs, filename);

    if (!buffer) {
        llvm::errs() << "Error: Could not open file " << filename << "\n";
        return;
    }

    llvm::outs() << "Source buffer for " << buffer->get_filename() << "\n";
    llvm::outs() << "Size: " << buffer->get_contents().size() << " bytes\n";
    llvm::outs() << "Is empty: " << (buffer->get_contents().empty() ? "true" : "false") << "\n";
    llvm::outs() << "Is regular file: " << (buffer->is_regular_file() ? "true" : "false") << "\n";
    llvm::outs() << "Contents:\n";
    llvm::outs() << buffer->get_contents() << "\n";
    llvm::outs().flush();
}

}  // namespace ziv::cli::toolchain
