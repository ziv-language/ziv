// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "printer.hpp"
#include <map>

namespace ziv::toolchain::ast {

    const llvm::StringRef Printer::get_kind_name(NodeKind kind) {
        static const std::map<NodeKind, llvm::StringRef> kind_names = {
            {NodeKind::ArgumentList(), "ArgumentList"},
            {NodeKind::FileStart(), "FileStart"},
            {NodeKind::FileEnd(), "FileEnd"},
            {NodeKind::Invalid(), "Invalid"},
            {NodeKind::Placeholder(), "Placeholder"},
            {NodeKind::VariableDecl(), "VariableDecl"},
            {NodeKind::VariableInit(), "VariableInit"},
            {NodeKind::TypeSpec(), "TypeSpec"},
            {NodeKind::FunctionDecl(), "FunctionDecl"},
            {NodeKind::FunctionBody(), "FunctionBody"},
            {NodeKind::ParameterList(), "ParameterList"},
            {NodeKind::ReturnType(), "ReturnType"},
            {NodeKind::FunctionCall(), "FunctionCall"},
            {NodeKind::FunctionName(), "FunctionName"},
            {NodeKind::ArgumentList(), "ArgumentList"},
            {NodeKind::IfStatement(), "IfStatement"},
            {NodeKind::IfCondition(), "IfCondition"},
            {NodeKind::ElseStatement(), "ElseStatement"},
            {NodeKind::ElseIfStatement(), "ElseIfStatement"},
            {NodeKind::ForLoop(), "ForLoop"},
            {NodeKind::ForHeader(), "ForHeader"},
            {NodeKind::WhileLoop(), "WhileLoop"},
            {NodeKind::DoWhileLoop(), "DoWhileLoop"},
            {NodeKind::BreakStatement(), "BreakStatement"},
            {NodeKind::ContinueStatement(), "ContinueStatement"},
            {NodeKind::ReturnStatement(), "ReturnStatement"},
            {NodeKind::ReturnValue(), "ReturnValue"},
            {NodeKind::StatementList(), "StatementList"},
            {NodeKind::CodeBlock(), "CodeBlock"},
            {NodeKind::PrimitiveType(), "PrimitiveType"},
            {NodeKind::ArithmeticOp(), "ArithmeticOp"},
            {NodeKind::ComparisonOp(), "ComparisonOp"},
            {NodeKind::LogicalOp(), "LogicalOp"},
            {NodeKind::AssignmentOp(), "AssignmentOp"},

            {NodeKind::ModuleDecl(), "ModuleDecl"},
            {NodeKind::ModuleName(), "ModuleName"},
            {NodeKind::ModuleImport(), "ModuleImport"},
            {NodeKind::ModuleImportName(), "ModuleImportName"},
            {NodeKind::ModuleImportList(), "ModuleImportList"},
            {NodeKind::ModuleAlias(), "ModuleAlias"},
            {NodeKind::ModuleImportItem(), "ModuleImportItem"},
            {NodeKind::TryBlock(), "TryBlock"},
            {NodeKind::BinaryExpression(), "BinaryExpression"},
            {NodeKind::UnaryExpression(), "UnaryExpression"},
            {NodeKind::LiteralExpression(), "LiteralExpression"},
            {NodeKind::FileEnd(), "FileEnd"},
            {NodeKind::Invalid(), "Invalid"},
        };

        auto it = kind_names.find(kind);
        if (it != kind_names.end()) {
            return it->second;
        }
        return "Unknown";
    }

} // namespace ziv::toolchain::ast
