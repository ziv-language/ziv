<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# The Ziv Programming Language

[![Discord](https://img.shields.io/discord/1310665530763513927?color=7289DA&label=Discord&logo=discord&logoColor=white)](https://discord.gg/jNxqNSdyQH)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](LICENSE)
[![Documentation](https://img.shields.io/badge/docs-website-blue)](https://ziv-language.github.io/book/)

> A modern programming language focused on simplicity, safety, and performance.

## Overview

Welcome to the Ziv programming language project! Ziv is designed to combine the best features of various programming languages while maintaining simplicity and ease of use. Our goal is to create a language that is not only powerful but also accessible to developers of all backgrounds.

> **Note**: Ziv is still in its early stages of development. We appreciate your patience and support as we work to bring you a powerful and user-friendly programming language.

## Key Features

- 🚀 **High Performance**: Built on LLVM for optimized native code generation
- 🛡️ **Memory Safety**: Strong compile-time guarantees without runtime overhead
- 📦 **Zero-Cost Abstractions**: High-level features with predictable performance
- 🔧 **Modern Tooling**: Integrated build system and package management
- 📚 **Rich Standard Library**: Comprehensive built-in functionality
- 🌟 **Clear Syntax**: Familiar, expressive, and readable code

## Quick Start

### Prerequisites

- C++20 compatible compiler
- CMake 3.20+
- LLVM 19.0+
- Python 3.8+ (for development tools)

### Installation

1. **Clone the Repository**
```bash
git clone git@github.com:ziv-language/ziv.git
cd ziv
```

2. **Set Up Development Environment**
```bash
# Install dependencies using Mamba
mamba env create -f ziv.yml
mamba activate ziv
```

3. **Build the Project**
```bash
cmake -G Ninja -B build -S .
cmake --build build
```

4. **Verify Installation**
```bash
./build/zivc --version
```

### Hello World

```ziv
fn main() -> unit:
    println!("Hello, Ziv!")
```

## Project Structure

```
ziv/
├── examples/       # Example programs
├── toolchain/      # Compiler implementation
├── tests/          # Test suite
└── zivc/           # Command-line interface
```

## Documentation

- [Language Guide](https://ziv-language.github.io/book/)
- [Example Collection](examples/)
- [Contributing Guide](CONTRIBUTING.md)

## Community & Support

- 💬 [Discord Community](https://discord.gg/jNxqNSdyQH)
- 📝 [GitHub Discussions](https://github.com/ziv-language/ziv/discussions)
- 🐛 [Issue Tracker](https://github.com/ziv-language/ziv/issues)
- 📰 [Release Notes](CHANGELOG.md)

### Getting Help

1. Check the [documentation](https://ziv-language.github.io/book/)
2. Search [existing issues](https://github.com/ziv-language/ziv/issues)
3. Ask in [Discord](https://discord.gg/jNxqNSdyQH) or [Discussions](https://github.com/ziv-language/ziv/discussions)
4. [Open an issue](https://github.com/ziv-language/ziv/issues/new/choose)

## Contributing

We welcome contributions of all kinds! Here's how you can help:

- 🐛 Report bugs and suggest features
- 📝 Improve documentation
- 🔧 Submit pull requests
- 💬 Help others in the community

See our [Contributing Guide](CONTRIBUTING.md) for details.

## Security

For security concerns, please see our [Security Policy](SECURITY.md). We take all security reports seriously and appreciate responsible disclosure.

## License

Ziv is licensed under the Apache License 2.0 with LLVM Exception - see [LICENSE](LICENSE) for details.

---

<div align="center">

**[Website](https://ziv-language.org)** • **[Documentation](https://ziv-language.github.io/book/)** • **[Discord](https://discord.gg/jNxqNSdyQH)**

Built with ❤️ by the Ziv community

</div>
