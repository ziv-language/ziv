<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# Contributing to Ziv

Thank you for considering contributing to Ziv! Your efforts to enhance the language and its ecosystem are greatly appreciated. It’s dedicated individuals like you who make Ziv a fantastic programming language.

Below are the guidelines we follow to ensure contributions are streamlined, organized, and high-quality.

## Table of Contents

- [Core Philosophy](#core-philosophy)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
  - [Branching Strategy](#branching-strategy)
  - [Commit Messages](#commit-messages)
  - [Pull Requests](#pull-requests)
- [Code Standards](#code-standards)
  - [Naming and Style](#naming-and-style)
  - [Code Organization](#code-organization)
  - [Documentation](#documentation)
- [Build and Test](#build-and-test)
- [Developer Setup](#developer-setup)

## Core Philosophy

- **Incremental Changes**: Prefer small, focused improvements over sweeping changes
- **Quality First**: Prioritize clean, maintainable code over quick fixes
- **Collaboration**: Discuss significant changes early through issues and discussions
- **Main Branch Stability**: All changes must pass tests before merging

## Getting Started

1. Fork and clone the repository
2. Set up your development environment (see [Developer Setup](#developer-setup))
3. Find or create an issue for your contribution
4. Create a feature branch
5. Make your changes
6. Submit a pull request

## Development Workflow

### Branching Strategy

- Branch naming format: `<type>/<description>`
  - Example: `feat/add-parser`, `fix/lexer-bug`, `docs/readme-update`
- Types: `feat`, `fix`, `docs`, `test`, `ci`, `refac`, `perf`, `chore`, `revert`

### Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/):

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

Example:
```
feat(parser): add error recovery for invalid tokens

- Implemented error recovery for the lexer/parser
- Ensures the compilation process continues even after encountering invalid tokens

Fixes #123
```

### Pull Requests

- Request assignment to an issue before starting work
- Ensure all checks pass
- Get at least one review
- PRs are squashed when merging
- Link related issues in the description

## Code Standards

### Naming and Style

#### File Structure
```
ziv/
├── toolchain/
│   ├── lexer/
│   ├── parser/
│   └── ...
├── zivc/
└── tests/
```

#### Naming Conventions
| Type | Convention | Example |
|------|------------|---------|
| Variables/Functions | `snake_case` | `parse_token()` |
| Classes/Structs | `PascalCase` | `TokenParser` |
| Files | `snake_case.{hpp,cpp}` | `token_parser.hpp` |
| Constants/Enums | `SCREAMING_SNAKE_CASE` | `MAX_BUFFER_SIZE` |
| Member Variables | `snake_case_` | `current_token_` |

#### Required File Header
```cpp
/*
 * Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
 * See /LICENSE for license details.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */
```

### Code Organization

- Line length: 100 characters maximum
- Indentation: 4 spaces
- Function length: 100 lines maximum
- Class member order: public → protected → private
- Header guards: Use `#pragma once`
- Include order: system → third-party → project

### Best Practices

- Use smart pointers over raw pointers
- Prefer references to pointers
- Use `const` wherever possible
- Use `[[nodiscard]]` for functions returning values
- Use `llvm::StringRef` for string views
- Always initialize variables
- Avoid global variables and macros
- Use C++-style casts

### Documentation

- Document all public APIs
- Use `///` for documentation comments
- Explain "why" not "what" in implementation comments
- Keep comments up-to-date with code changes
- Include `TODO(#issue)` for planned work

## Build and Test

### Requirements
- CMake 3.20+
- LLVM/MLIR/Clang 19+
- Google Test 1.15+
- LLDB 19+

### Basic Commands

#### Configure
```bash
cmake -G Ninja -B build -S .
```

#### Build
```bash
cmake --build build
```

#### Test
```bash
cd build && ctest --output-on-failure
```

#### Run

```bash
./build/zivc --help
```

## Developer Setup

1. Install Mamba: Follow instructions at https://mamba.readthedocs.io/en/latest/installation

2. Clone and setup:
    ```bash
    git clone git@github.com:your-username/ziv.git
    ```

    ```bash
    cd ziv/
    ```

    ```bash
    mamba env create -f ziv.yml
    conda activate ziv
    ```

   ```bash
   pre-commit install
   ```

3. Build and verify:
    ```bash
    cmake -G Ninja -B build -S .
    cmake --build build
    ```
    ```bash
    cd build && ctest --output-on-failure
    ```

---

Please review our [Code of Conduct](CODE_OF_CONDUCT.md) to understand the standards of behavior we expect from contributors.

> Thank you for following these guidelines and helping make Ziv a great language!
