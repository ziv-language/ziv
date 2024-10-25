// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SOURCE_BUFFER_HPP
#define ZIV_TOOLCHAIN_SOURCE_BUFFER_HPP

#include <memory>
#include <string>
#include <optional>

#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/VirtualFileSystem.h"

namespace ziv::toolchain::source {
    class SourceBuffer {
    public:
        static std::optional<SourceBuffer> from_stdin();
        static std::optional<SourceBuffer> from_file(
            llvm::vfs::FileSystem &fs,
            llvm::StringRef filename
        );

        SourceBuffer() = delete; // No default constructor

        llvm::StringRef get_filename() const { return filename_; };
        llvm::StringRef get_contents() const { return buffer_->getBuffer(); };
        // Returns true if the source buffer was loaded from a regular file
        bool is_regular_file() const { return is_regular_file_; };

    private:

        llvm::StringRef filename_;
        std::unique_ptr<llvm::MemoryBuffer> buffer_;
        bool is_regular_file_;

        explicit SourceBuffer(
            llvm::StringRef filename,
            std::unique_ptr<llvm::MemoryBuffer> buffer,
            bool is_regular_file
        ):
            filename_(filename),
            buffer_(std::move(buffer)),
            is_regular_file_(is_regular_file) {};

        static bool is_valid_extension(llvm::StringRef filename);

        // Helper function to create a SourceBuffer from a MemoryBuffer
        static std::optional<SourceBuffer> from_memory_buffer(
            llvm::StringRef filename,
            llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer,
            bool is_regular_file
        );
    };
} // namespace ziv::toolchain::source

#endif // ZIV_TOOLCHAIN_SOURCE_BUFFER_HPP
