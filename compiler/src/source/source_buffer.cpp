#include "source/source_buffer.hpp"
#include <iostream>

namespace ziv {

auto SourceBuffer::from_stdin() -> std::optional<SourceBuffer> {
    // Create memory buffer from stdin
    return from_memory_buffer(llvm::MemoryBuffer::getSTDIN(), "<stdin>");
}

auto SourceBuffer::from_file(llvm::vfs::FileSystem& fs, llvm::StringRef filename) -> std::optional<SourceBuffer> {
    llvm::ErrorOr<std::unique_ptr<llvm::vfs::File>> file = fs.openFileForRead(filename);
    if (!file) {
        llvm::errs() << "Error opening file: " << filename << "\n";
        return std::nullopt;
    }

    llvm::ErrorOr<llvm::vfs::Status> status = (*file)->status();
    if (!status) {
        llvm::errs() << "Error getting file status: " << filename << "\n";
        return std::nullopt;
    }

    int64_t size = status->isRegularFile() ? status->getSize() : -1;

    if (size == -1) {
        llvm::errs() << "Invalid file size for: " << filename << "\n";
        return std::nullopt;
    }

    return from_memory_buffer((*file)->getBuffer(filename, size), filename);
}

auto SourceBuffer::from_memory_buffer(llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer,
                                      llvm::StringRef filename) -> std::optional<SourceBuffer> {
    if (!buffer) {
        llvm::errs() << "Error reading memory buffer from: " << filename << "\n";
        return std::nullopt;
    }

    // Buffer created successfully with sentinel (null terminator).
    return SourceBuffer(filename.str(), std::move(*buffer));
}

} // namespace ziv
