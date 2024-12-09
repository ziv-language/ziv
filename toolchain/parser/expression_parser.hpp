// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"
#include "operator_precedence.hpp"

namespace ziv::toolchain::parser {

ast::AST::Node Parser::parse_expression() {
    return parse_binary_expression();
}

ast::AST::Node Parser::parse_binary_expression(int min_precedence) {
    auto left = parse_unary();

    while (true) {
        if (is_eof() || !is_binary_operator(peek().kind)) {
            break;
        }

        auto op = peek().kind;

        // Check if we should continue based on operator precedence
        if (!should_take_operator(op, min_precedence)) {
            break;
        }

        // Check next operator for precedence comparison
        size_t next_pos = current_ + 1;
        while (next_pos < tokens_.size() &&
               !is_binary_operator(tokens_[next_pos].kind)) {
            next_pos++;
        }

        if (next_pos < tokens_.size()) {
            auto next_op = tokens_[next_pos].kind;
            auto prec = OperatorPrecedence::compare_precedence(op, next_op);

            if (prec == Precedence::Ambiguous) {
                auto error_node = ast_.add_node(ast::NodeKind::Error(), peek());
                parse_error(error_node, "Ambiguous operator precedence. Please use parentheses to clarify.");
                return error_node;
            }

            if (prec == Precedence::RightBindsTighter) {
                break;
            }
        }

        consume(); // Consume the operator
        auto binary_node = ast_.add_node(ast::NodeKind::BinaryExpr(), previous());
        ast_.add_child(binary_node, left);

        auto right = parse_binary_expression(get_operator_precedence(op) + 1);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

ast::AST::Node Parser::parse_unary() {
    if (is_unary_operator(peek().kind)) {
        auto op = consume();
        auto unary_node = ast_.add_node(ast::NodeKind::UnaryExpr(), op);
        auto operand = parse_unary(); // Handle nested unary operators
        ast_.add_child(unary_node, operand);
        return unary_node;
    }

    return parse_primary();
}

ziv::toolchain::ast::AST::Node Parser::parse_primary() {
    // Handle literals
    if (match(lex::TokenKind::IntLiteral()) ||
        match(lex::TokenKind::FloatLiteral()) ||
        match(lex::TokenKind::StringLiteral()) ||
        match(lex::TokenKind::CharLiteral())) {
        return ast_.add_node(ast::NodeKind::LiteralExpr(), consume());
    }

    // Handle booleans
    if (match(lex::TokenKind::True()) || match(lex::TokenKind::False())) {
        return ast_.add_node(ast::NodeKind::LiteralExpr(), consume());
    }

    // Handle identifiers and function calls
    if (match(lex::TokenKind::Identifier())) {
        auto identifier = consume();

        // Check if it's a function call
        if (match(lex::TokenKind::LParen())) {
            return parse_function_call();
        }

        // Otherwise, it's a variable reference
        return ast_.add_node(ast::NodeKind::IdentifierExpr(), identifier);
    }

    // Handle parenthesized expressions
    if (consume_match(lex::TokenKind::LParen())) {
        auto expr = parse_expression();
        expect(lex::TokenKind::RParen(), "Expected ')' after expression");
        return expr;
    }

    // Handle errors
    auto error_node = ast_.add_node(ast::NodeKind::Error(), peek());
    parse_error(error_node, "Expected expression");
    return error_node;
}

ziv::toolchain::ast::AST::Node Parser::parse_logical_or() {
    auto left = parse_logical_and();

    while (match(lex::TokenKind::Or())) {
        auto op = consume();
        auto right = parse_logical_and();

        auto binary_node = ast_.add_node(ast::NodeKind::BinaryOp(), op);
        ast_.add_child(binary_node, left);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

ziv::toolchain::ast::AST::Node Parser::parse_logical_and() {
    auto left = parse_equality();

    while (match(lex::TokenKind::And())) {
        auto op = consume();
        auto right = parse_equality();

        auto binary_node = ast_.add_node(ast::NodeKind::BinaryOp(), op);
        ast_.add_child(binary_node, left);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

ziv::toolchain::ast::AST::Node Parser::parse_equality() {
    auto left = parse_comparison();

    while (match(lex::TokenKind::DoubleEquals()) ||
           match(lex::TokenKind::NotEquals())) {
        auto op = consume();
        auto right = parse_comparison();

        auto binary_node = ast_.add_node(ast::NodeKind::BinaryOp(), op);
        ast_.add_child(binary_node, left);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

ziv::toolchain::ast::AST::Node Parser::parse_comparison() {
    auto left = parse_addition();

    while (match(lex::TokenKind::Less()) ||
           match(lex::TokenKind::LessEquals()) ||
           match(lex::TokenKind::Greater()) ||
           match(lex::TokenKind::GreaterEquals())) {
        auto op = consume();
        auto right = parse_addition();

        auto binary_node = ast_.add_node(ast::NodeKind::BinaryOp(), op);
        ast_.add_child(binary_node, left);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

ziv::toolchain::ast::AST::Node Parser::parse_addition() {
    auto left = parse_multiplication();

    while (match(lex::TokenKind::Plus()) ||
           match(lex::TokenKind::Minus())) {
        auto op = consume();
        auto right = parse_multiplication();

        auto binary_node = ast_.add_node(ast::NodeKind::BinaryOp(), op);
        ast_.add_child(binary_node, left);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

ziv::toolchain::ast::AST::Node Parser::parse_multiplication() {
    auto left = parse_unary();

    while (match(lex::TokenKind::Star()) ||
           match(lex::TokenKind::Slash()) ||
           match(lex::TokenKind::Percent())) {
        auto op = consume();
        auto right = parse_unary();

        auto binary_node = ast_.add_node(ast::NodeKind::BinaryOp(), op);
        ast_.add_child(binary_node, left);
        ast_.add_child(binary_node, right);

        left = binary_node;
    }

    return left;
}

bool Parser::is_binary_operator(lex::TokenKind kind) const {
    return kind == lex::TokenKind::Plus() ||
           kind == lex::TokenKind::Minus() ||
           kind == lex::TokenKind::Star() ||
           kind == lex::TokenKind::Slash() ||
           kind == lex::TokenKind::Percent() ||
           kind == lex::TokenKind::Pipe() ||
           kind == lex::TokenKind::Ampersand() ||
           kind == lex::TokenKind::Caret() ||
           kind == lex::TokenKind::And() ||
           kind == lex::TokenKind::Or();
}

bool Parser::is_unary_operator(lex::TokenKind kind) const {
    return kind == lex::TokenKind::Minus() ||
           kind == lex::TokenKind::Not() ||
           kind == lex::TokenKind::Tilde();
}

int Parser::get_operator_precedence(lex::TokenKind op) const {
    if (op == lex::TokenKind::Star() ||
        op == lex::TokenKind::Slash() ||
        op == lex::TokenKind::Percent()) {
        return 5;
    }
    if (op == lex::TokenKind::Plus() ||
        op == lex::TokenKind::Minus()) {
        return 4;
    }
    if (op == lex::TokenKind::Less() ||
        op == lex::TokenKind::Greater() ||
        op == lex::TokenKind::LessEquals() ||
        op == lex::TokenKind::GreaterEquals()) {
        return 3;
    }
    if (op == lex::TokenKind::DoubleEquals() ||
        op == lex::TokenKind::NotEquals()) {
        return 2;
    }
    return 1; // Lowest precedence for other operators
}

bool Parser::should_take_operator(lex::TokenKind op, int min_precedence) const {
    return get_operator_precedence(op) >= min_precedence;
}

} // namespace ziv::toolchain::parser
