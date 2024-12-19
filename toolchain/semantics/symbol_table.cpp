// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "symbol_table.hpp"

namespace ziv::toolchain::semantics {

void SymbolTable::enter_scope() {
    scopes_.emplace_back(std::make_unique<Scope>());
}

void SymbolTable::exit_scope() {
    scopes_.pop_back();
}

bool SymbolTable::define(Symbol symbol) {
    auto& current_scope = scopes_.back();
    auto [it, inserted] = current_scope->symbols.try_emplace(symbol.get_name(), symbol);
    return inserted;
}

Symbol* SymbolTable::lookup(llvm::StringRef name) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto& scope = **it;
        auto symbol = scope.symbols.find(name);
        if (symbol != scope.symbols.end()) {
            return &symbol->second;
        }
    }
    return nullptr;
}


}  // namespace ziv::toolchain::semantics
