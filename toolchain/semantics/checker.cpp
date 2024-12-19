// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "checker.hpp"

#include "toolchain/diagnostics/compilation_phase.hpp"

namespace ziv::toolchain::semantics {

bool SemanticChecker::check() {
    diagnostics::PhaseGuard guard(diagnostics::CompilationPhase::SemanticAnalysis);
    symbols_.enter_scope();

    for (const auto& node : ast_.nodes()) {
        if (!check_node(node)) {
            return false;
        }
    }

    symbols_.exit_scope();
    return true;
}

bool SemanticChecker::check_node(ast::AST::Node node) {
    if (node.get_kind() == ast::NodeKind::VarDecl()) {
        return check_variable_declaration(node);
    } else if (node.get_kind() == ast::NodeKind::FunctionDecl()) {
        return check_function_declaration(node);
    }

    return true;
}

bool SemanticChecker::check_variable_declaration(ast::AST::Node node) {
    auto name_node = ast_.children(node).begin();
    auto type_node = std::next(name_node);

    if (name_node == ast_.children(node).end() || type_node == ast_.children(node).end()) {
        emitter_.emit(diagnostics::DiagnosticKind::VariableMissingType(),
                      node.get_location(),
                      "variable declaration must have a name and a type");
        return false;
    }
    llvm::StringRef name = (*name_node).get_spelling();
    Type* type = Type::get_Int_type();  // Using correct casing

    if (symbols_.lookup(name)) {
        emitter_.emit(diagnostics::DiagnosticKind::VariableRedeclaration(),
                      node.get_location(),
                      "variable '{}' is already declared",
                      name);
        return false;
    }
    symbols_.define(Symbol(Symbol::Kind::KVariable, name, type));
    return true;
}

Type* SemanticChecker::check_expression(ast::AST::Node node) {
    if (node.get_kind() == ast::NodeKind::IntegerType()) {
        return Type::get_Int_type();  // Using correct casing
    }

    // Handle other types...
    return Type::get_Error_type();  // Note: Also using correct casing for Error
}

bool SemanticChecker::check_function_declaration(ast::AST::Node node) {
    // Find function name node
    auto name_node = ast_.children(node).begin();
    if (name_node == ast_.children(node).end()) {
        emitter_.emit(diagnostics::DiagnosticKind::FunctionMissingName(),
                      node.get_location(),
                      "function declaration must have a name");
        return false;
    }

    llvm::StringRef name = (*name_node).get_spelling();

    // Check if function already declared
    if (symbols_.lookup(name)) {
        emitter_.emit(diagnostics::DiagnosticKind::FunctionMissingName(),
                      node.get_location(),
                      "function '{}' is already declared",
                      name);
        return false;
    }

    // Enter new scope for function parameters
    symbols_.enter_scope();

    // Find parameter list and return type
    auto params_node = std::next(name_node);
    if (params_node != ast_.children(node).end()
        && (*params_node).get_kind() == ast::NodeKind::ParameterList()) {
        // Check parameters...
        for (const auto& param : ast_.children(*params_node)) {
            auto param_name = ast_.children(param).begin();
            if (param_name != ast_.children(param).end()) {
                // For now, assume all parameters are int type
                symbols_.define(Symbol(Symbol::Kind::KVariable,
                                       (*param_name).get_spelling(),
                                       Type::get_Int_type()));
            }
        }
    }

    // Create and add function symbol
    // For now, assume all functions return int
    Type* return_type = Type::get_Int_type();
    symbols_.define(Symbol(Symbol::Kind::KFunction, name, return_type));

    // Check function body if it exists
    auto body_node = std::find_if(ast_.children(node).begin(),
                                  ast_.children(node).end(),
                                  [](const ast::AST::Node& n) {
                                      return n.get_kind() == ast::NodeKind::CodeBlock();
                                  });

    if (body_node != ast_.children(node).end()) {
        for (const auto& stmt : ast_.children(*body_node)) {
            if (!check_node(stmt)) {
                symbols_.exit_scope();
                return false;
            }
        }
    }

    // Exit function scope
    symbols_.exit_scope();
    return true;
}

}  // namespace ziv::toolchain::semantics
