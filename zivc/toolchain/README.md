<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# Ziv Toolchain - Driver

This directory contains the driver implementation for the Ziv programming language. The driver orchestrates the compilation process and manages different commands, including the lexer, parser, and code generation.

## Overview

The main components of the driver include:

- **Command Interface**: Defines a base interface for commands that can be executed.
- **Source Command**: Implements the command to read and process source files.
- **Driver Class**: Responsible for registering and executing commands based on user input.

## File Structure

- **`command.hpp`**: Declaration of the base `Command` class.
- **`commands/source_command.cpp`**: Implementation of the `SourceCommand`, which reads a source file and outputs its details.
- **`commands/source_command.hpp`**: Header for the `SourceCommand` class.
- **`driver.cpp`**: Implementation of the `Driver` class, which manages command execution.
- **`driver.hpp`**: Header for the `Driver` class.
- **`README.md`**: Documentation for the driver directory.

## Usage

To use the driver, create an instance of the `Driver` class and call the `run` method with the desired command and arguments. For example:

```cpp
ziv::toolchain::driver::Driver driver;
driver.run("source", "path/to/source/file.ziv");
```

This will execute the `SourceCommand` with the specified source file.
