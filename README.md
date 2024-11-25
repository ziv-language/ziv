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
- [Join Our Community! 🚀](#join-our-community)

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

Navigate to the project directory:

```bash
cd ziv/
```

**2. Install Build Dependencies**

Install [Mamba](https://mamba.readthedocs.io/en/latest/installation/mamba-installation.html) to manage the dependencies, and then create a the environment environment:


```bash
mamba env create -f ziv.yml
```

Activate the environment:

```bash
mamba activate ziv
```

Now you can proceed to build the project.

**3. Build the Project**

Build the project and compile it:

```bash
cmake -G Ninja -B build -S .
cmake --build build
```

**4. Run the Compiler**

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

## Join Our Community

[![Discord](https://img.shields.io/discord/1310665530763513927?color=7289DA&label=Join%20our%20Discord&logo=discord&logoColor=white)](https://discord.gg/jNxqNSdyQH)

We have an active and friendly community on Discord where you can:
- Get help with Ziv development
- Share your projects and ideas
- Participate in language design discussions
- Connect with other developers
- Stay updated on the latest changes

Don't hesitate to join us and be part of shaping Ziv's future! 🚀
