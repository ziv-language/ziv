<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# The Ziv Programming Language

Welcome to the Ziv programming language project! Ziv is designed to combine the best features of various programming languages while maintaining simplicity and ease of use. Our goal is to create a language that is not only powerful but also accessible to developers of all backgrounds.

> **Note**: Ziv is still in its early stages of development. We appreciate your patience and support as we work to bring you a powerful and user-friendly programming language.

## Table of Contents

- [Features](#features)
- [Documentation](#documentation)
- [Installation](#installation)
- [Contributing](#contributing)
- [Code of Conduct](#code-of-conduct)
- [Security](#security)
- [License](#license)

## Features

- **Simplicity**: Designed with a focus on readability and ease of use.

- **Versatility**: Incorporates features from various popular programming languages.

- **Performance**: Built on top of LLVM for optimized performance.

- **Modular**: Supports a modular architecture to allow easy extension and integration.

## Documentation

Comprehensive documentation will be provided as the project develops. For now, refer to the code comments and the [book](https://ziv-language.github.io/book/) for insights on language features and usage.

## Installation

**1. Clone the Repository using SSH**

Make sure your SSH keys are properly configured. Then, run:

```bash
git clone git@github.com:ziv-language/ziv.git
```

**2. Install Build Dependencies**

Install **CMake** and **Ninja** using:

```bash
sudo apt-get install cmake ninja-build
```

**3. Install LLVM and MLIR Dependencies**

Run the following command to install all required LLVM and MLIR packages:

```bash
sudo apt-get install llvm-18-dev llvm-18 llvm-18-tools clang-18 libmlir-18 libmlir-18-dev
```

**4. Set Up Environment Variables**

Add the following variables to your **~/.bashrc** or **~/.zshrc** file:

```bash
export CMAKE_PREFIX_PATH="/usr/lib/llvm-18:$CMAKE_PREFIX_PATH"
export MLIR_DIR="/usr/lib/llvm-18/lib/cmake/mlir"
```

Reload the shell configuration by running:

```bash
source ~/.bashrc  # If using bash
```

or

```bash
source ~/.zshrc  # If using zsh
```

**5. Build the Project**

Navigate into the project directory and compile it:

```bash
cd ziv
cmake -G Ninja -B build -S .
cmake --build build
```

**6. Run the Compiler**

Test the installation by running the following:

```bash
./build/zivc --help
```

## Contributing

We welcome contributions! Please see our [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to get involved.

## Code of Conduct

This project adheres to a [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this standard.

## Security

For reporting security vulnerabilities, please refer to our [SECURITY.md](SECURITY.md) for guidelines on responsible disclosure.

## License

This project is licensed under the Apache License 2.0. See the [LICENSE](LICENSE) file for more details.

---

Thank you for your interest in Ziv! We hope you find it as exciting as we do. Feel free to reach out with any questions or suggestions in the [Discussions](https://github.com/ziv-language/ziv/discussions) section.
