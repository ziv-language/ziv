// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {

ziv::toolchain::ast::AST::Node Parser::parse_node() {
    switch (peek().kind) {
    case ziv::toolchain::lex::TokenKind::Fn():
        return parse_function_declaration();
    default:
        auto invalid_node = ast_.add_node(ast::NodeKind::Invalid(), consume());
        return invalid_node;
    }
}

ziv::toolchain::ast::AST::Node Parser::parse_identifier() {
    // we don't have a keyword for variables so we need to check if it's a variable declaration or
    // expression variables are defined as: <identifier> : <type> = <expression>

    // Fall back to statement parsing
    if (is_keyword()) {
        return parse_statement();
    }

    // Check next token for function call
    if (tokens_[current_ + 1].kind == ziv::toolchain::lex::TokenKind::LParen()) {
        return parse_function_call();
    }
    auto identifier_expr = ast_.add_node(ast::NodeKind::IdentifierExpr(), consume());
    return identifier_expr;
}

}  // namespace ziv::toolchain::parser
