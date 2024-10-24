<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# Ziv Toolchain - Source Buffer

This directory contains the source buffer implementation for the Ziv programming language. The source buffer is responsible for reading and managing the source code of a Ziv program.

## Overview

The source buffer is responsible for:

- Reading the source code from standard input or a file.
- Managing the content and filename of the source code.
- Providing utility functions to determine the nature of the source.

### Files

- `source_buffer.h`: The header file for the source buffer.
- `source_buffer.cpp`: The implementation file for the source buffer.

### `SourceBuffer` Class

The `SourceBuffer` class includes the following key functionalities:

- **Static Methods:**
  - `from_stdin()`: Creates a `SourceBuffer` from standard input.
  - `from_file(llvm::vfs::FileSystem &fs, llvm::StringRef filename)`: Creates a `SourceBuffer` from a specified file, validating the file extension.

- **Member Functions:**
  - `get_filename()`: Returns the name of the source file.
  - `get_content()`: Returns the content of the source file.
  - `is_regular_file()`: Indicates if the source buffer was loaded from a regular file.

### Private Methods

- `is_valid_extension(llvm::StringRef filename)`: Validates the file extension of the source file.
- `from_memory_buffer(...)`: A helper function to create a `SourceBuffer` from a `MemoryBuffer`.

### Example Usage

To create a source buffer from standard input:
```cpp
auto buffer = ziv::toolchain::SourceBuffer::from_stdin();
```

To create a source buffer from a file:
```cpp
llvm::vfs::FileSystem &fs = ...; // File system instance
auto buffer = ziv::toolchain::SourceBuffer::from_file(fs, "my_program.ziv");
```
