// main.cpp
#include "source_buffer.hpp"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <optional>

int main(int argc, char** argv) {
    llvm::cl::opt<std::string> InputFilename(
        llvm::cl::Positional, llvm::cl::desc("<input file>"), llvm::cl::Required);

    llvm::cl::ParseCommandLineOptions(argc, argv, "Ziv Compiler - Source Buffer Test\n");

    // Create an instance of the virtual file system
    llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FS = llvm::vfs::getRealFileSystem();

    // Try to load the file into a SourceBuffer
    if (auto buffer = ziv::SourceBuffer::from_file(*FS, InputFilename)) {
        llvm::outs() << "Successfully loaded file: " << buffer->get_filename() << "\n";
        llvm::outs() << "File content:\n";
        llvm::outs() << buffer->get_text() << "\n";
    } else {
        llvm::errs() << "Failed to load file: " << InputFilename << "\n";
        return 1; // Return error code
    }

    return 0; // Success
}
