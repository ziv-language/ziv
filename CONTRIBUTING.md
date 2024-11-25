<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# Contributing to Ziv

Thank you for considering contributing to Ziv! Your efforts to enhance the language and its ecosystem are greatly appreciated. It’s dedicated individuals like you who make Ziv a fantastic programming language.

Below are the guidelines we follow to ensure contributions are streamlined, organized, and high-quality.

## Table of Contents

- [Philosophy and Guidelines](#philosophy-and-guidelines)
- [Branching and Issues](#branching-and-issues)
- [Commit Messages](#commit-messages)
- [Pull Requests](#pull-requests)
- [Coding Guidelines](#coding-guidelines)
    - [C++ Code Style](#c-code-style)
    - [Comments](#comments)
    - [File Formatting](#file-formatting)
    - [Building and Testing](#building-and-testing)
- [Style Guidelines for Documentation](#style-guidelines-for-documentation)
- [Contribution Philosophy](#contribution-philosophy)
- [Setting Up Your Development Environment](#setting-up-your-development-environment)
- [Code of Conduct](#code-of-conduct)

## Philosophy and Guidelines

- **Incremental Changes**: We prefer small, incremental changes. Avoid large, sweeping changes unless absolutely necessary.

- **Main Branch Health**: The `main` branch must always remain stable. This means all tests and checks must pass before any PR is merged.

- **Collaboration**: Encourage collaboration by asking questions and discussing your approach before diving into significant tasks.

- **Code Quality**: Always prioritize writing clean, maintainable, and well-documented code over quick fixes.


## Branching and Issues

- **Branch Names**: Each feature or fix should be created on its own branch, named after the feature or type of contribution (e.g., `feat/add-parser`, `fix/lexer-bug`, `docs/readme-update`).

- **Issues**: Every contribution should have an associated issue. If the issue does not exist, please create one before starting your work. Issues are categorized by type:
    - `feat`: New features
    - `fix`: Bug fixes
    - `docs`: Documentation updates
    - `test`: Adding or improving tests
    - `ci`: Continuous Integration-related changes
    - `refac`: Code refactoring
    - `perf`: Performance improvements
    - `chore`: Maintenance tasks
    - `revert`: Reverting changes

- **Pull Requests (PRs)**: Each branch should be merged through a pull request (PR) into the `main` branch. Every PR must have a description of what it adds or changes and reference the associated issue. Trunk-based development means that all changes are reviewed, tested, and pass checks before being merged.

## Commit Messages

All commit messages must follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) standard. The format is:

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

Example:

```
feat(parser): add error recovery for invalid tokens

Implemented error recovery for the lexer/parser. This ensures that the compilation process continues even after encountering invalid tokens.

Fixes #123
```

### Accepted Commit Types:

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation-only changes
- `style`: Code style changes (whitespace, formatting, etc.)
- `refactor`: Code changes that neither fix a bug nor add a feature
- `test`: Adding or updating tests
- `ci`: Changes to CI configuration files
- `perf`: Performance improvements
- `chore`: Other changes that don't modify source code
- `revert`: Revert a previous commit


## Pull Requests

- **Requesting Issues**: Before starting work on an issue, please comment on the issue asking to be assigned to it. This prevents duplicate work and informs others that you're working on it.

- **Checklist**: Ensure your PR meets all the requirements in the PR template.

- **Reviews**: Every PR needs at least one review and must pass all the required checks (e.g., CI, linters, tests) before being merged into `main`.

- **Squashing**: We use squash commits to preserve a clean, linear commit history. Please ensure your PR is squashed into a single commit before merging.

- **Merging**: PRs are merged using the "Squash and Merge" option to maintain a clean commit history.

## Coding Guidelines

### C++ Code Style:

- **Naming Conventions**:
    - Variables, functions, and namespaces use snake_case.
    - Classes and structs use PascalCase.
    - Filenames use underscores (`snake_case`).

- **Avoid `typedef` and `auto`**: Use explicit type declarations whenever possible to improve code readability.

- **Namespace Use**:
    - Place your code inside a namespace whenever possible.
    - Do not use a `using-directive` to import entire namespaces globally.

- **Headers**:
    - List all project's headers as descendants of the source directory without using UNIX directory aliases like `..` or `./`.

- **Code Organization**:
    - In classes, list `public` members first, followed by `protected`, and then `private`.
    - Avoid large files; files must not contain more than 1000 lines unless necessary.
    - Write small, focused functions with clear purposes.

- **Brackets**: Always use brackets, even for single-line conditionals or loops. For example, this is invalid:

    ```cpp
    if (a == true)
            return false;
    ```

- **Exception Handling**: Avoid using C++ exceptions; use error handling mechanisms that don't rely on exceptions.

- **Type Definitions**: Use types like `int64_t` instead of `int` for numbers.

- **Avoid `NULL`**: Use `nullptr` instead of `NULL`.

### Comments:

- Comment your code when necessary but avoid redundant comments.
- Use `TODO` comments only when necessary.

### File Formatting:

- Each file, especially Markdown files, must include the following header or a variation of it:

    ```markdown
    <!--
    Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
    See /LICENSE for license details.
    SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
    -->
    ```

- Use `UTF-8` encoding for all files.

- Remove trailing whitespace and ensure consistent line endings.

- Run `pre-commit` hooks before committing changes.

### Building and Testing:

- **Compiler**: We will use Clang as the default compiler for Ziv.
- **Build System**: Currently, we are using **CMake** but may switch to **Bazel** in the future. Ensure your contributions are compatible with the build system.
- **Tools**: The project uses `LLVM`, `MLIR`, `omp`, `lld`, `pipx` for pre-commit hooks, and **clang-format** for code style.

## Style Guidelines for Documentation

- Ensure clarity, conciseness, and consistency when writing documentation.
- Use meaningful titles for each section and organize information logically.

## Contribution Philosophy

- **Simplicity**: Ziv's goal is simplicity. Every contribution should aim to keep the language, its codebase, and its ecosystem as simple and understandable as possible.

- **Collaboration**: Open dialogue and teamwork are the keys to success. Don’t hesitate to reach out to maintainers or the community with questions or ideas.

- **Quality over Quantity**: It's better to make fewer, higher-quality contributions than to contribute large amounts of low-quality code. Each small, focused change should improve the project in a meaningful way.

## Setting Up Your Development Environment

To set up your development environment, follow these steps:

1. Make a fork of the repository.

2. Clone your fork to your local machine:

```bash
    git clone git@github.com:your-username/ziv.git
```

3. Navigate to the project directory:

```bash
    cd ziv/
```

4. Install the build dependencies:

```bash
    mamba env create -f ziv.yml
```

5. Activate the environment:

```bash
    conda activate ziv
```

6. Install the pre-commit hooks:

```bash
    pre-commit install
```

7. Build the project:

```bash
    cmake -G Ninja -B build -S .
    cmake --build build
```

8. Run the compiler:

```bash
    ./build/zivc --help
```

9. You're all set! Now you can start contributing to Ziv.


## Code of Conduct

Please review our [Code of Conduct](CODE_OF_CONDUCT.md) to understand the standards of behavior we expect from contributors.

> Thank you for following these guidelines and helping make Ziv a great language!
