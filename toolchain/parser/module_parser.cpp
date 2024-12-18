// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {

ziv::toolchain::ast::AST::Node Parser::parse_module_declaration() {
    auto module_node = ast_.add_node(ast::NodeKind::ModuleDecl(), consume());

    // Parse module name
    if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
        return module_node;
    }

    auto module_name = ast_.add_node(ast::NodeKind::ModuleName(), consume());
    ast_.add_child(module_node, module_name);

    // Parse module body
    while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::End())) {
        auto node = parse_node();
        if (node.is_valid()) {
            ast_.add_child(module_node, node);
        }
    }
    expect(ziv::toolchain::lex::TokenKind::End(),
           "Expected 'end' keyword at end of module declaration");
    expect(ziv::toolchain::lex::TokenKind::Module(),
           "Expected 'module' keyword at end of module declaration");
    return module_node;
}

ziv::toolchain::ast::AST::Node Parser::parse_module_import() {
    auto import_node = ast_.add_node(ast::NodeKind::ModuleImport(), consume());

    // Parse module name
    if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
        return import_node;
    }

    auto module_name = ast_.add_node(ast::NodeKind::ModuleName(), consume());
    ast_.add_child(import_node, module_name);

    // Check for alias
    if (consume_match(ziv::toolchain::lex::TokenKind::As())) {
        expect(ziv::toolchain::lex::TokenKind::Identifier(), "Expected identifier after 'as'");
        auto alias = ast_.add_node(ast::NodeKind::ModuleAlias(), consume());
        ast_.add_child(import_node, alias);
    }

    // Check for specific imports
    if (consume_match(ziv::toolchain::lex::TokenKind::LBrace())) {
        auto import_list = ast_.add_node(ast::NodeKind::ModuleImportList(), previous());

        while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::RBrace())) {
            expect(ziv::toolchain::lex::TokenKind::Identifier(),
                   "Expected identifier in import list");
            auto import = ast_.add_node(ast::NodeKind::ModuleImportItem(), consume());
            ast_.add_child(import_list, import);
            if (!consume_match(ziv::toolchain::lex::TokenKind::Comma())) {
                break;
            }
        }
        expect(ziv::toolchain::lex::TokenKind::RBrace(), "Expected '}' at end of module import");
        ast_.add_child(import_node, import_list);
    }
    return import_node;
}

}  // namespace ziv::toolchain::parser
