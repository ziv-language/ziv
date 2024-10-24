// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/driver/commands/source_command.hpp"

namespace ziv::toolchain::driver {

    void SourceCommand::execute(const std::string &filename) {

        llvm::vfs::FileSystem &fs = *llvm::vfs::getRealFileSystem();
        auto buffer = ziv::toolchain::source::SourceBuffer::from_file(fs, filename);

        if (!buffer) {
            llvm::errs() << "Error reading file: " << filename << "\n";
            return;
        }

        llvm::outs() << "Source buffer: " << buffer->get_filename() << "\n";
        llvm::outs() << "Size: " << buffer->get_content().size() << " bytes\n";
        llvm::outs() << "Is regular file: " << (buffer->is_regular_file() ? "yes" : "no") << "\n";
    }
} // namespace ziv::toolchain::driver
