// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SEMANTICS_SYMBOL_TABLE_HPP
#define ZIV_TOOLCHAIN_SEMANTICS_SYMBOL_TABLE_HPP

#pragma once
#include <memory>
#include <vector>

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringRef.h"
#include "symbol.hpp"

namespace ziv::toolchain::semantics {
class SymbolTable {
public:
    void enter_scope();
    void exit_scope();
    bool define(Symbol symbol);
    Symbol* lookup(llvm::StringRef name);

private:
    struct Scope {
        llvm::DenseMap<llvm::StringRef, Symbol> symbols;
    };
    std::vector<std::unique_ptr<Scope>> scopes_;
};

}  // namespace ziv::toolchain::semantics

#endif  // ZIV_TOOLCHAIN_SEMANTICS_SYMBOL_TABLE_HPP
