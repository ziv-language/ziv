// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SEMANTICS_CHECKER_HPP
#define ZIV_TOOLCHAIN_SEMANTICS_CHECKER_HPP

#include "symbol_table.hpp"
#include "toolchain/ast/tree.hpp"
#include "toolchain/diagnostics/diagnostic_consumer.hpp"
#include "toolchain/diagnostics/diagnostic_emitter.hpp"

namespace ziv::toolchain::semantics {

class SemanticChecker {
public:
    SemanticChecker(ast::AST& ast,
                    std::shared_ptr<diagnostics::DiagnosticConsumer> consumer,
                    const source::SourceBuffer& source)
        : ast_(ast), emitter_(consumer, source) {}


    bool check();

private:
    bool check_node(ast::AST::Node node);
    bool check_variable_declaration(ast::AST::Node node);
    bool check_function_declaration(ast::AST::Node node);
    Type* check_expression(ast::AST::Node node);

    ast::AST& ast_;
    SymbolTable symbols_;
    diagnostics::DiagnosticEmitter emitter_;
};

}  // namespace ziv::toolchain::semantics

#endif  // ZIV_TOOLCHAIN_SEMANTICS_CHECKER_HPP
