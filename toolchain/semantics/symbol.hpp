// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SEMANTICS_SYMBOL_HPP
#define ZIV_TOOLCHAIN_SEMANTICS_SYMBOL_HPP

#include "llvm/ADT/StringRef.h"
#include "toolchain/ast/node_kind.hpp"
#include "types.hpp"

namespace ziv::toolchain::semantics {

class Symbol {
public:
    enum class Kind : uint8_t {
#define ZIV_SYMBOL_KIND(NAME) K##NAME,
#include "symbol_kind_registry.def"
    };

    Symbol(Kind kind, llvm::StringRef name, Type* type) : kind_(kind), name_(name), type_(type) {}

    Kind get_kind() const {
        return kind_;
    }
    llvm::StringRef get_name() const {
        return name_;
    }
    Type* get_type() const {
        return type_;
    }

    static llvm::StringRef get_kind_name(Kind kind);

private:
    Kind kind_;
    llvm::StringRef name_;
    Type* type_;
};

}  // namespace ziv::toolchain::semantics

#endif  // ZIV_TOOLCHAIN_SEMANTICS_SYMBOL_HPP
