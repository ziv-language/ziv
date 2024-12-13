// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {

ziv::toolchain::ast::AST::Node Parser::parse_statement() {
    switch (peek().kind) {
    case lex::TokenKind::Let():
    case lex::TokenKind::Mut():
    case lex::TokenKind::Const():
        return parse_variable_declaration();
    case lex::TokenKind::If():
        return parse_if_statement();
    case lex::TokenKind::While():
        return parse_while_statement();
    case lex::TokenKind::Do():
        return parse_do_while_statement();
    case lex::TokenKind::For():
        return parse_for_statement();
    case lex::TokenKind::Match():
        return parse_match_statement();
    case lex::TokenKind::Return():
        return parse_return_statement();
    default:
        auto expr = parse_expression();
        expect(lex::TokenKind::Semicolon(), "Expected ';' after expression");
        return expr;
    }
}

ziv::toolchain::ast::AST::Node Parser::parse_block() {
    // First check if it's a block
    if (!match(ziv::toolchain::lex::TokenKind::LBrace())) {
        auto invalid_node = ast_.add_node(ast::NodeKind::Invalid(), peek());
        parse_error(invalid_node, "Expected '{' at start of block");
        return invalid_node;
    }

    auto block_node = ast_.add_node(ast::NodeKind::CodeBlock(), consume());

    while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::RBrace())) {
        auto statement = parse_statement();
        if (statement.is_valid()) {
            ast_.add_child(block_node, statement);
        }
    }
    expect(ziv::toolchain::lex::TokenKind::RBrace(), "Expected '}' at end of block");
    return block_node;
}


ziv::toolchain::ast::AST::Node Parser::parse_if_statement() {
    auto if_node = ast_.add_node(ast::NodeKind::IfStatement(), consume());

    // Parse condition
    bool has_parens = consume_match(ziv::toolchain::lex::TokenKind::LParen());

    auto condition = parse_expression();
    if (condition.is_valid()) {
        ast_.add_child(if_node, condition);
    }

    if (has_parens) {
        expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of condition");
    }

    // Parse block
    auto block = parse_block();
    if (block.is_valid()) {
        ast_.add_child(if_node, block);
    }

    // Handle else-if and else
    if (match(ziv::toolchain::lex::TokenKind::Else())) {
        consume();  // consume 'else'
        if (match(ziv::toolchain::lex::TokenKind::If())) {
            auto else_if = parse_if_statement();
            if (else_if.is_valid()) {
                ast_.add_child(if_node, else_if);
            }
        } else {
            auto else_block = parse_else_statement();
            ast_.add_child(if_node, else_block);
        }
    }
    return if_node;
}


ziv::toolchain::ast::AST::Node Parser::parse_else_statement() {
    auto else_node = ast_.add_node(ast::NodeKind::ElseStatement(), consume());

    auto block = parse_block();
    if (block.is_valid()) {
        ast_.add_child(else_node, block);
    }

    return else_node;
}


ziv::toolchain::ast::AST::Node Parser::parse_while_statement() {
    auto while_node = ast_.add_node(ast::NodeKind::WhileLoop(), consume());

    // Parse condition
    bool has_parens = consume_match(ziv::toolchain::lex::TokenKind::LParen());

    auto condition = parse_expression();
    if (condition.is_valid()) {
        ast_.add_child(while_node, condition);
    }

    if (has_parens) {
        expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of condition");
    }

    // Parse block
    auto block = parse_block();
    if (block.is_valid()) {
        ast_.add_child(while_node, block);
    }

    return while_node;
}


ziv::toolchain::ast::AST::Node Parser::parse_do_while_statement() {
    auto do_while_node = ast_.add_node(ast::NodeKind::DoWhileLoop(), consume());

    auto block = parse_block();
    if (block.is_valid()) {
        ast_.add_child(do_while_node, block);
    }

    auto while_node = ast_.add_node(ast::NodeKind::WhileLoop(), consume());
    ast_.add_child(do_while_node, while_node);

    // Parse condition
    bool has_parens = consume_match(ziv::toolchain::lex::TokenKind::LParen());

    auto condition = parse_expression();
    if (condition.is_valid()) {
        ast_.add_child(while_node, condition);
    }

    if (has_parens) {
        expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of condition");
    }

    return do_while_node;
}

// Match Statement Support
ziv::toolchain::ast::AST::Node Parser::parse_match_statement() {
    auto match_stmt = ast_.add_node(ast::NodeKind::MatchStmt(), consume());

    auto value = parse_expression();
    ast_.add_child(match_stmt, value);

    expect(lex::TokenKind::Colon(), "Expected ':' after match expression");

    while (!is_eof() && !match(lex::TokenKind::End())) {
        auto case_stmt = parse_match_case();
        ast_.add_child(match_stmt, case_stmt);
    }

    expect(lex::TokenKind::End(), "Expected 'end' after match cases");
    return match_stmt;
}

ziv::toolchain::ast::AST::Node Parser::parse_match_case() {
    auto case_stmt = ast_.add_node(ast::NodeKind::CaseStmt(), peek());

    auto pattern = parse_expression();
    ast_.add_child(case_stmt, pattern);

    expect(lex::TokenKind::Arrow(), "Expected '=>' after match pattern");

    auto body = parse_statement();
    ast_.add_child(case_stmt, body);

    return case_stmt;
}

ziv::toolchain::ast::AST::Node Parser::parse_for_statement() {
    auto for_node = ast_.add_node(ast::NodeKind::ForLoop(), consume());

    // Parse initialization
    auto init = parse_statement();
    if (init.is_valid()) {
        ast_.add_child(for_node, init);
    }

    expect(lex::TokenKind::Semicolon(), "Expected ';' after for loop initialization");

    // Parse condition
    auto condition = parse_expression();
    if (condition.is_valid()) {
        ast_.add_child(for_node, condition);
    }

    expect(lex::TokenKind::Semicolon(), "Expected ';' after for loop condition");

    // Parse increment
    auto increment = parse_expression();
    if (increment.is_valid()) {
        ast_.add_child(for_node, increment);
    }

    // Parse block
    auto block = parse_block();
    if (block.is_valid()) {
        ast_.add_child(for_node, block);
    }

    return for_node;
}

ziv::toolchain::ast::AST::Node Parser::parse_return_statement() {
    auto return_node = ast_.add_node(ast::NodeKind::ReturnStmt(), consume());

    // Parse return value
    if (!match(lex::TokenKind::Semicolon())) {
        auto value = parse_expression();
        if (value.is_valid()) {
            ast_.add_child(return_node, value);
        }
    }

    expect(lex::TokenKind::Semicolon(), "Expected ';' after return statement");
    return return_node;
}
}  // namespace ziv::toolchain::parser
