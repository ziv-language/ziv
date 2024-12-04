<!--
Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
See /LICENSE for license details.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# CHANGELOG

All notable changes to this project will be documented in this file.  
This project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

### November 2024

#### Added
- updated dependencies manager [12-11-2024]
- #5 add do while expression recognition at parser (#6) [25-11-2024]
- #2 automatic semicolon insertion (#4) [25-11-2024]
- updated pre commits to check message [29-11-2024]
- #3 initial test implementation with gtest [29-11-2024]
- #3 initial test implementation with gtest (#8) [30-11-2024]
- #7 updated lexer to include indentation levels (#9) [30-11-2024]

#### Changed
- included support for semicolon delimiter [22-11-2024]
- updated contribution information [25-11-2024]


### October 2024

#### Added
- created source buffer with driver [24-10-2024]
- enhance symbol support and improve token management [25-10-2024]
- added basic lexer implementation [25-10-2024]
- implemented lex kind [25-10-2024]
- implemented abstract syntax tree [27-10-2024]
- added parser with basic support [28-10-2024]

#### Fixed
- improved cli logic and made input file mandatory [25-10-2024]

#### Changed
- created default community health files [11-10-2024]
- added prettier, PR checklist, pre-commit, and workflows [23-10-2024]
- add to ppal readme step-by-step ziv installation (#1) [28-10-2024]
- included error messages for each node [30-10-2024]
- implemented parser into different files [31-10-2024]

---

## Releases

### [x.x.x] - YYYY-MM-DD
*First release*

#### Added
- Initial release of the project

---

### Conventions

Changes are categorized using the following labels:

- **Added**: New features or capabilities
- **Fixed**: Bug fixes and corrections
- **Changed**: Updates or improvements to existing features
- **Security**: Security-related changes or fixes
- **Deprecated**: Features marked for future removal
- **Removed**: Features removed in this version

> This changelog is automatically updated on the first day of each month.
> New releases are manually tagged and documented.
