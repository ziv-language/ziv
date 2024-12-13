// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "printer.hpp"

#include <map>

namespace ziv::toolchain::ast {

const llvm::StringRef Printer::get_kind_name(NodeKind kind) {
    // Use a map for lookup since we can't convert NodeKind to size_t directly
    static const std::map<NodeKind, llvm::StringRef> kind_names = {
        {NodeKind::FileStart(), "FileStart"},
        {NodeKind::FileEnd(), "FileEnd"},
        {NodeKind::Invalid(), "Invalid"},
        {NodeKind::Placeholder(), "Placeholder"},
        {NodeKind::TranslationUnit(), "TranslationUnit"},
        {NodeKind::ModuleDecl(), "ModuleDecl"},
        {NodeKind::ModuleName(), "ModuleName"},
        {NodeKind::ModuleImport(), "ModuleImport"},
        {NodeKind::ModuleAlias(), "ModuleAlias"},
        {NodeKind::ModuleImportList(), "ModuleImportList"},
        {NodeKind::ModuleImportItem(), "ModuleImportItem"},
        {NodeKind::VarDecl(), "VariableDecl"},
        {NodeKind::VariableName(), "VariableName"},
        {NodeKind::VariableInit(), "VariableInit"},
        {NodeKind::TypeSpec(), "TypeSpec"},
        {NodeKind::FunctionDecl(), "FunctionDecl"},
        {NodeKind::FunctionName(), "FunctionName"},
        {NodeKind::FunctionSignature(), "FunctionSignature"},
        {NodeKind::FunctionBody(), "FunctionBody"},
        {NodeKind::StructDecl(), "StructDecl"},
        {NodeKind::ClassDecl(), "ClassDecl"},
        {NodeKind::EnumDecl(), "EnumDecl"},
        {NodeKind::MethodDecl(), "MethodDecl"},
        {NodeKind::ParameterList(), "ParameterList"},
        {NodeKind::Parameter(), "Parameter"},
        {NodeKind::ArgumentList(), "ArgumentList"},
        {NodeKind::Argument(), "Argument"},
        {NodeKind::StatementList(), "StatementList"},
        {NodeKind::CodeBlock(), "CodeBlock"},
        {NodeKind::ReturnStmt(), "ReturnStatement"},
        {NodeKind::IfStatement(), "IfStatement"},
        {NodeKind::ElseStatement(), "ElseStatement"},
        {NodeKind::WhileLoop(), "WhileLoop"},
        {NodeKind::DoWhileLoop(), "DoWhileLoop"},
        {NodeKind::ForLoop(), "ForLoop"},
        {NodeKind::MatchStmt(), "MatchStatement"},
        {NodeKind::CaseStmt(), "CaseStatement"},
        {NodeKind::BreakStmt(), "BreakStatement"},
        {NodeKind::ContinueStmt(), "ContinueStatement"},
        {NodeKind::ArrayExpr(), "ArrayExpr"},
        {NodeKind::LiteralExpr(), "LiteralExpr"},
        {NodeKind::BinaryExpr(), "BinaryExpr"},
        {NodeKind::UnaryExpr(), "UnaryExpr"},
        {NodeKind::CallExpr(), "CallExpr"},
        {NodeKind::IndexExpr(), "IndexExpr"},
        {NodeKind::IdentifierExpr(), "IdentifierExpr"},
        {NodeKind::FieldDecl(), "FieldDecl"},
        {NodeKind::MethodList(), "MethodList"},
        {NodeKind::FieldList(), "FieldList"},
        {NodeKind::Error(), "Error"},
        {NodeKind::Comment(), "Comment"},
    };

    auto it = kind_names.find(kind);
    return it != kind_names.end() ? it->second : "Unknown";
}

void Printer::print_indentation(llvm::raw_ostream& os,
                                size_t indent,
                                const std::vector<bool>& last_child) const {
    // We need to print indentation up to indent level
    for (size_t i = 0; i < indent; ++i) {
        // If we're within the last_child vector bounds
        if (i < last_child.size()) {
            // Use previous levels to determine if we print "│  " or "   "
            os << (last_child[i] ? "   " : "│  ");
        } else {
            // Default to "   " if we're beyond last_child bounds
            os << "   ";
        }
    }
}

void Printer::print_node_header(llvm::raw_ostream& os, AST::Node node) const {
    os << get_kind_name(ast_.get_kind(node));

    if (options_.show_metadata) {
        os << " (";
        if (options_.show_indices) {
            os << "idx:" << node.get_index() << ", ";
        }
        os << "ln:" << ast_.get_line(node) << ", "
           << "col:" << ast_.get_token(node).get_column();

        if (options_.show_error_state && ast_.has_error(node)) {
            os << ", ERROR";
        }
        os << ")";
    }
}

void Printer::print_node_details(llvm::raw_ostream& os,
                                 AST::Node node,
                                 size_t indent,
                                 const std::vector<bool>& last_child) const {
    if (!options_.show_token_info)
        return;

    const auto& token = ast_.get_token(node);
    auto token_kind = token.get_kind();

    std::vector<bool> child_prefix = last_child;
    child_prefix.push_back(true);  // Always true for details

    // First print the token kind
    os << "\n";
    print_indentation(os, indent + 1, child_prefix);
    os << "└─ Token: '" << token.get_name() << "'";

    // For literals and identifiers, print their actual value
    if (token_kind == lex::TokenKind::Identifier() || token_kind == lex::TokenKind::StringLiteral()
        || token_kind == lex::TokenKind::IntLiteral()
        || token_kind == lex::TokenKind::FloatLiteral()
        || token_kind == lex::TokenKind::CharLiteral()) {
        os << "\n";
        print_indentation(os, indent + 1, child_prefix);
        os << "└─ Value: '" << token.get_spelling() << "'";
    }
    // For keywords, only print the spelling if it differs from the name
    else if (token_kind.is_keyword()) {
        const auto spelling = token.get_spelling();
        if (spelling != token.get_name()) {
            os << "\n";
            print_indentation(os, indent + 1, child_prefix);
            os << "└─ Spelling: '" << spelling << "'";
        }
    }
}

void Printer::print_node(llvm::raw_ostream& os,
                         AST::Node node,
                         size_t indent,
                         std::vector<bool> last_child) const {
    if (!node.is_valid()) {
        print_indentation(os, indent, last_child);
        os << "└─ Invalid Node\n";
        return;
    }

    print_indentation(os, indent, last_child);

    // Determine connector based on position
    if (indent == 0) {
        os << "└─ ";  // Root node
    } else {
        os << (last_child.back() ? "└─ " : "├─ ");
    }

    print_node_header(os, node);

    auto children = ast_.children(node);
    auto child_count = std::distance(children.begin(), children.end());

    if (child_count > 0) {
        if (!options_.compact_mode) {
            print_node_details(os, node, indent, last_child);
        }
        os << "\n";

        size_t current_child = 0;
        for (const auto& child : children) {
            current_child++;
            bool is_last = (current_child == child_count);

            auto child_last = last_child;
            child_last.push_back(is_last);
            print_node(os, child, indent + 1, child_last);
        }
    } else {
        print_node_details(os, node, indent, last_child);
        os << "\n";
    }
}

}  // namespace ziv::toolchain::ast
