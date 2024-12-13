// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

#include "expression_parser.hpp"
#include "parser_utils.hpp"
#include "statement_parser.hpp"
#include "top_level_parser.hpp"

namespace ziv::toolchain::parser {

void Parser::parse() {
    // Create TranslationUnit as root
    auto root = ast_.add_node(ast::NodeKind::FileStart(), consume());

    // Parse all declarations
    while (!is_eof()) {
        auto node = parse_top_level();
        if (node.is_valid()) {
            ast_.add_child(root, node);
        }
        // Skip any extra semicolons
        while (match(lex::TokenKind::Semicolon())) {
            consume();
        }
    }
    auto eof = ast_.add_node(ast::NodeKind::FileEnd(),
                             lex::TokenBuffer::Token(lex::TokenKind::Eof(), "", 0, 0));
    ast_.add_child(root, eof);
}

}  // namespace ziv::toolchain::parser
