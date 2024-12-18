// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {

ziv::toolchain::ast::AST::Node Parser::parse_variable_declaration() {
    // Consume let/var
    auto var_decl = ast_.add_node(ast::NodeKind::VarDecl(), consume());

    // Handle let/mut/const
    expect(lex::TokenKind::Identifier(), "Expected variable declaration type");

    auto var_name = ast_.add_node(ast::NodeKind::VariableName(), consume());
    ast_.add_child(var_decl, var_name);

    // Parse type annotation
    expect(lex::TokenKind::Colon(), "Expected ':' after variable name");

    auto type_spec = parse_type_specifier();
    ast_.add_child(var_decl, type_spec);

    // Parse initialization
    expect(lex::TokenKind::Equals(), "=");

    auto init_expr = ast_.add_node(ast::NodeKind::VariableInit(), peek());
    auto expr = parse_expression();
    if (expr.is_valid()) {
        ast_.add_child(init_expr, expr);
    }
    ast_.add_child(var_decl, init_expr);

    expect(lex::TokenKind::Semicolon(), "Expected ';' after variable declaration");
    return var_decl;
}

ziv::toolchain::ast::AST::Node Parser::parse_type_specifier() {
    if (!match(lex::TokenKind::Int())) {
        emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                      peek().get_location(),
                      peek().get_name());
        return ast_.add_node(ast::NodeKind::Error(), peek());
    }

    auto base_type = ast_.add_node(ast::NodeKind::TypeSpec(), consume());
    // Handle generic type arguments if present (e.g., Vec<T>)
    if (match(lex::TokenKind::Less())) {
        consume();  // Consume '<'

        while (!is_eof() && !match(lex::TokenKind::Greater())) {
            auto type_arg = parse_type_specifier();
            ast_.add_child(base_type, type_arg);

            if (!match(lex::TokenKind::Greater())) {
                if (!consume_match(lex::TokenKind::Comma())) {
                    break;
                }
            }
        }

        expect(lex::TokenKind::Greater(), "Expected '>' after generic type arguments");
    }

    return base_type;
}


ziv::toolchain::ast::AST::Node Parser::parse_delimiter() {
    bool has_parens = consume_match(ziv::toolchain::lex::TokenKind::LParen());

    auto expr = parse_expression();

    if (has_parens) {
        expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of expression");
    }

    // Check for delimiter
    expect(ziv::toolchain::lex::TokenKind::Semicolon(), "Expected ';' at end of expression");

    return expr;
}


ziv::toolchain::ast::AST::Node Parser::parse_assignment() {
    auto left = parse_logical_or();

    if (match(ziv::toolchain::lex::TokenKind::Equals())) {
        auto op = consume();
        auto assignment_node = ast_.add_node(ast::NodeKind::AssignmentOp(), op);

        ast_.add_child(assignment_node, left);

        auto right = parse_assignment();  // Recurse to handle chained assignments
        if (right.is_valid()) {
            ast_.add_child(assignment_node, right);
        }
        return assignment_node;
    }
    return left;
}

}  // namespace ziv::toolchain::parser
