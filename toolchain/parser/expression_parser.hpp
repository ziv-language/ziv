// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {
    ziv::toolchain::ast::AST::Node Parser::parse_expression() {
        return parse_precedence_expression();
    }

    ziv::toolchain::ast::AST::Node Parser::parse_precedence_expression() {
        // This is a recursive method which starts with the highest precedence
        // Each precedence level calls the next lowest precedence level

        // Start with the highest precedence level
        return parse_logical_or();
    }

    ziv::toolchain::ast::AST::Node Parser::parse_logical_or() {
        auto left = parse_logical_and();
        while(consume_match(ziv::toolchain::lex::TokenKind::Or())) {
            auto operator_token = previous();
            auto right = parse_logical_and();
            auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), operator_token);
            ast_.add_child(binary_node, left);
            ast_.add_child(binary_node, right);
            left = binary_node;
        }
        return left;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_logical_and() {
        auto left = parse_equality();
        while(consume_match(ziv::toolchain::lex::TokenKind::And())) {
            auto operator_token = previous();
            auto right = parse_equality();
            auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), operator_token);
            ast_.add_child(binary_node, left);
            ast_.add_child(binary_node, right);
            left = binary_node;
        }
        return left;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_equality() {
        auto left = parse_comparison();
        while(consume_match(ziv::toolchain::lex::TokenKind::DoubleEquals()) || consume_match(ziv::toolchain::lex::TokenKind::NotEquals())) {
            auto operator_token = previous();
            auto right = parse_comparison();
            auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), operator_token);
            ast_.add_child(binary_node, left);
            ast_.add_child(binary_node, right);
            left = binary_node;
        }
        return left;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_comparison() {
        auto left = parse_addition();
        while(consume_match(ziv::toolchain::lex::TokenKind::Less()) || consume_match(ziv::toolchain::lex::TokenKind::LessEquals()) ||
              consume_match(ziv::toolchain::lex::TokenKind::Greater()) || consume_match(ziv::toolchain::lex::TokenKind::GreaterEquals())) {
            auto operator_token = previous();
            auto right = parse_addition();
            auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), operator_token);
            ast_.add_child(binary_node, left);
            ast_.add_child(binary_node, right);
            left = binary_node;
        }
        return left;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_addition() {
        auto left = parse_multiplication();
        while(consume_match(ziv::toolchain::lex::TokenKind::Plus()) || consume_match(ziv::toolchain::lex::TokenKind::Minus())) {
            auto operator_token = previous();
            auto right = parse_multiplication();
            auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), operator_token);
            ast_.add_child(binary_node, left);
            ast_.add_child(binary_node, right);
            left = binary_node;
        }
        return left;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_multiplication() {
        auto left = parse_unary();
        while(consume_match(ziv::toolchain::lex::TokenKind::Star()) || consume_match(ziv::toolchain::lex::TokenKind::Slash()) ||
                consume_match(ziv::toolchain::lex::TokenKind::Percent())) {
                auto operator_token = previous();
                auto right = parse_unary();
                auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), operator_token);
                ast_.add_child(binary_node, left);
                ast_.add_child(binary_node, right);
                left = binary_node;
        }
        return left;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_unary() {
        if (consume_match(ziv::toolchain::lex::TokenKind::Minus()) || consume_match(ziv::toolchain::lex::TokenKind::Not())) {
            auto operator_token = previous();
            auto unary_node = ast_.add_node(ast::NodeKind::UnaryExpr(), operator_token);
            auto right = parse_unary();
            ast_.add_child(unary_node, right);
            return unary_node;
        }
        return parse_primary();
    }

    ziv::toolchain::ast::AST::Node Parser::parse_primary() {
        if (match(ziv::toolchain::lex::TokenKind::IntLiteral())) {
            return ast_.add_node(ast::NodeKind::LiteralExpr(), consume());
        } else if (match(ziv::toolchain::lex::TokenKind::StringLiteral())) {
            return ast_.add_node(ast::NodeKind::LiteralExpr(), consume());
        } else if (match(ziv::toolchain::lex::TokenKind::True()) || match(ziv::toolchain::lex::TokenKind::False())) {
            return ast_.add_node(ast::NodeKind::LiteralExpr(), consume());
        } else if (match(ziv::toolchain::lex::TokenKind::Identifier())) {
            return parse_identifier();
        } else if (consume_match(ziv::toolchain::lex::TokenKind::LParen())) {
            auto expr = parse_expression();
            expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of expression");
            return expr;
        } else {
            auto invalid_node = ast_.add_node(ast::NodeKind::Invalid(), consume());
            parse_error(invalid_node, "Expected primary expression");
            return {};
        }
    }

} // namespace ziv::toolchain::parser
