// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception


#include "parser.hpp"

namespace ziv::toolchain::parser {

    ziv::toolchain::ast::AST::Node Parser::parse_statement() {
        switch (peek().kind) {
            case ziv::toolchain::lex::TokenKind::If():
                return parse_if_statement();
            case ziv::toolchain::lex::TokenKind::While():
                return parse_while_statement();
            // case ziv::toolchain::lex::TokenKind::Return():
            //     return parse_return_statement();
            case ziv::toolchain::lex::TokenKind::Identifier():
                return parse_variable_declaration();
            default:
                return parse_expression();
        }
    }

    ziv::toolchain::ast::AST::Node Parser::parse_block() {
        // First check if it's a block
        if(!match(ziv::toolchain::lex::TokenKind::LBrace())) {
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
            consume(); // consume 'else'
            if (match(ziv::toolchain::lex::TokenKind::If())) {
                auto else_if = parse_if_statement();
                if (else_if.is_valid()) {
                    ast_.add_child(if_node, else_if);
                }
            }
            else {
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




}
