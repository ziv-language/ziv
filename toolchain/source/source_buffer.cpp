// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "source_buffer.hpp"

namespace ziv::toolchain::source {

    std::optional<SourceBuffer> SourceBuffer::from_stdin() {
        return from_memory_buffer(
            "<stdin>",
            llvm::MemoryBuffer::getSTDIN(),
            false // Not a regular file
        );
    };

    std::optional<SourceBuffer> SourceBuffer::from_file(
        llvm::vfs::FileSystem &fs,
        llvm::StringRef filename
    ) {
        if (!is_valid_extension(filename)) {
            return std::nullopt; // Invalid file extension
        }

        llvm::ErrorOr<std::unique_ptr<llvm::vfs::File>> file = fs.openFileForRead(filename);

        if (!file) {
            return std::nullopt; // File not found
        }

        llvm::ErrorOr<llvm::vfs::Status> status = (*file)->status();

        if (!status) {
            return std::nullopt; // Could not get file status
        }

        // Use -1 to indicate that the file is not a regular file
        int64_t size = status->isRegularFile() ? status->getSize() : -1;

        auto buffer = llvm::MemoryBuffer::getFile(filename);

        if (!buffer) {
            return std::nullopt; // Could not read file into memory buffer
        }

        return from_memory_buffer(filename, std::move(*buffer), status->isRegularFile());
    };

    std::optional<SourceBuffer> SourceBuffer::from_memory_buffer(
        llvm::StringRef filename,
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer,
        bool is_regular_file
    ) {
        if (!buffer) {
            return std::nullopt; // Could not read file into memory buffer
        }

        if ((*buffer)->getBufferSize() == 0) {
            return std::nullopt; // Empty file
        }

        if ((*buffer)->getBufferSize() > std::numeric_limits<int64_t>::max()) {
            return std::nullopt; // File too large
        }

        return SourceBuffer(
            filename,
            std::move(*buffer),
            is_regular_file
        );
    };

    bool SourceBuffer::is_valid_extension(llvm::StringRef filename) {
        // Valid extensions for source files
        const std::vector<llvm::StringRef> valid_extensions = {
            ".z",
            ".ziv"
        };

        for (const llvm::StringRef &ext : valid_extensions) {
            if (filename.ends_with(ext)) {
                return true;
            }
        }
        return false;
    };

} // namespace ziv::toolchain::source
