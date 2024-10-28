// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once
#ifndef ZIV_TOOLCHAIN_PARSER_PARSER_HPP
#define ZIV_TOOLCHAIN_PARSER_PARSER_HPP

#include <vector>
#include <memory>
#include "toolchain/lex/token_buffer.hpp"
#include "toolchain/lex/lexer.hpp"
#include "toolchain/ast/tree.hpp"
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::parser {
    class Parser {
        public:
            Parser(std::vector<ziv::toolchain::lex::TokenBuffer::Token> tokens, ziv::toolchain::ast::AST &ast):
                tokens_(std::move(tokens)), ast_(ast), current_(0) {}

            void parse();

            ziv::toolchain::ast::AST &get_ast() { return ast_; }

            bool is_eof() const { return current_ >= tokens_.size(); }

            bool match(ziv::toolchain::lex::TokenKind kind) const;

            bool check(ziv::toolchain::lex::TokenKind kind) const;

            const ziv::toolchain::lex::TokenBuffer::Token& consume();

            const ziv::toolchain::lex::TokenBuffer::Token& peek() const;

            const ziv::toolchain::lex::TokenBuffer::Token& previous() const;

            bool consume_match(ziv::toolchain::lex::TokenKind kind);

        private:
            // Top-level parsing
            ziv::toolchain::ast::AST::Node parse_top_level();
            ziv::toolchain::ast::AST::Node parse_module_declaration();
            ziv::toolchain::ast::AST::Node parse_module_import();


            // Function parsing
            ziv::toolchain::ast::AST::Node parse_function_declaration();
            ziv::toolchain::ast::AST::Node parse_function_signature();
            ziv::toolchain::ast::AST::Node parse_parameter_list();
            ziv::toolchain::ast::AST::Node parse_function_body();
            ziv::toolchain::ast::AST::Node parse_function_call();

            // Statement parsing
            ziv::toolchain::ast::AST::Node parse_statement();
            ziv::toolchain::ast::AST::Node parse_block();
            ziv::toolchain::ast::AST::Node parse_variable_declaration();
            // Control flow parsing
            ziv::toolchain::ast::AST::Node parse_if_statement();
            ziv::toolchain::ast::AST::Node parse_if_else();
            ziv::toolchain::ast::AST::Node parse_else_statement();
            ziv::toolchain::ast::AST::Node parse_else_if_statement();
            // Loop parsing
            ziv::toolchain::ast::AST::Node parse_for_statement();
            ziv::toolchain::ast::AST::Node parse_while_statement();
            ziv::toolchain::ast::AST::Node parse_do_while_statement();
            ziv::toolchain::ast::AST::Node parse_try_handle_statement();
            ziv::toolchain::ast::AST::Node parse_return_statement();
            ziv::toolchain::ast::AST::Node parse_break_statement();
            ziv::toolchain::ast::AST::Node parse_continue_statement();

            // Expression parsing
            ziv::toolchain::ast::AST::Node parse_delimiter();
            ziv::toolchain::ast::AST::Node parse_expression();
            ziv::toolchain::ast::AST::Node parse_assignment();
            ziv::toolchain::ast::AST::Node parse_logical_or();
            ziv::toolchain::ast::AST::Node parse_logical_and();
            ziv::toolchain::ast::AST::Node parse_logical_not();
            ziv::toolchain::ast::AST::Node parse_equality();
            ziv::toolchain::ast::AST::Node parse_comparison();
            ziv::toolchain::ast::AST::Node parse_term();
            ziv::toolchain::ast::AST::Node parse_factor();
            ziv::toolchain::ast::AST::Node parse_unary();
            ziv::toolchain::ast::AST::Node parse_primary();


            // Type parsing
            ziv::toolchain::ast::AST::Node parse_type_specifier();
            ziv::toolchain::ast::AST::Node parse_primitive_type();


            // Utility functions
            void synchronize(); // Error recovery
            void expect(ziv::toolchain::lex::TokenKind kind, const llvm::StringRef &message);
            void parse_error(const llvm::StringRef &message);
            void parse_code_block();
            ziv::toolchain::ast::AST::Node parse_identifier();

            // Class members
            std::vector<ziv::toolchain::lex::TokenBuffer::Token> tokens_;
            ziv::toolchain::ast::AST &ast_;
            size_t current_;

    };


} // namespace ziv::toolchain::parser

#endif // ZIV_TOOLCHAIN_PARSER_PARSER_HPP
