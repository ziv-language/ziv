// source/source_buffer.hpp

#ifndef SOURCE_BUFFER_HPP
#define SOURCE_BUFFER_HPP

#include <memory>
#include <string>
#include <optional>

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/raw_ostream.h"

namespace ziv {

class SourceBuffer {
public:
    
    static auto from_stdin() -> std::optional<SourceBuffer>;

    static auto from_file(
        llvm::vfs::FileSystem &fs, llvm::StringRef filename
    ) -> std::optional<SourceBuffer>;

    SourceBuffer() = delete;

    auto get_filename() const -> llvm::StringRef { return filename_; };
    auto get_text() const -> llvm::StringRef { return buffer_->getBuffer(); };

private:

    // Constructor used by factory functions
    explicit SourceBuffer(
        std::string filename,
        std::unique_ptr<llvm::MemoryBuffer> buffer
    ): filename_(std::move(filename)), buffer_(std::move(buffer)) {}

    // Filename of the source code
    std::string filename_;
    // Buffer to hold the source code
    std::unique_ptr<llvm::MemoryBuffer> buffer_;

    // Private helper function to create the buffer
    static auto from_memory_buffer(
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer,
        llvm::StringRef filename
    ) -> std::optional<SourceBuffer>;
};

} // namespace ziv

#endif // SOURCE_BUFFER_HPP
