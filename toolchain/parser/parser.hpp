// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once
#ifndef ZIV_TOOLCHAIN_PARSER_PARSER_HPP
    #define ZIV_TOOLCHAIN_PARSER_PARSER_HPP

    #include <iostream>
    #include <memory>
    #include <vector>

    #include "llvm/ADT/StringRef.h"
    #include "llvm/Support/raw_ostream.h"
    #include "toolchain/ast/tree.hpp"
    #include "toolchain/diagnostics/diagnostic_consumer.hpp"
    #include "toolchain/lex/lexer.hpp"
    #include "toolchain/lex/token_buffer.hpp"
    #include "toolchain/source/source_buffer.hpp"

namespace ziv::toolchain::parser {
class Parser {
public:
    Parser(std::vector<ziv::toolchain::lex::TokenBuffer::Token> tokens,
           ziv::toolchain::ast::AST& ast,
           std::shared_ptr<diagnostics::DiagnosticConsumer> consumer,
           const source::SourceBuffer& source)
        : tokens_(std::move(tokens)), ast_(ast), current_(0), emitter_(consumer, source) {}

    void parse();

    ziv::toolchain::ast::AST& get_ast() {
        return ast_;
    }

    bool is_eof() const {
        return current_ >= tokens_.size();
    }

    bool match(ziv::toolchain::lex::TokenKind kind) const;

    bool is_keyword() const;

    const ziv::toolchain::lex::TokenBuffer::Token& consume();

    const ziv::toolchain::lex::TokenBuffer::Token& peek() const;

    const ziv::toolchain::lex::TokenBuffer::Token& previous() const;

    bool consume_match(ziv::toolchain::lex::TokenKind kind);

    ziv::toolchain::ast::AST::Node parse_binary_expression(int min_precedence = 0);
    bool is_binary_operator(ziv::toolchain::lex::TokenKind kind) const;
    bool is_unary_operator(ziv::toolchain::lex::TokenKind kind) const;
    int get_operator_precedence(ziv::toolchain::lex::TokenKind op) const;
    bool should_take_operator(ziv::toolchain::lex::TokenKind op, int min_precedence) const;

private:
    // Top-level parsing
    ziv::toolchain::ast::AST::Node parse_node();
    ziv::toolchain::ast::AST::Node parse_module_declaration();
    ziv::toolchain::ast::AST::Node parse_module_import();

    // Function parsing
    ziv::toolchain::ast::AST::Node parse_function_declaration();
    ziv::toolchain::ast::AST::Node parse_generic_parameters();
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
    ziv::toolchain::ast::AST::Node parse_match_statement();
    ziv::toolchain::ast::AST::Node parse_match_case();
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
    ziv::toolchain::ast::AST::Node parse_addition();
    ziv::toolchain::ast::AST::Node parse_multiplication();
    ziv::toolchain::ast::AST::Node parse_precedence_expression();

    // Type parsing
    ziv::toolchain::ast::AST::Node parse_type_specifier();
    ziv::toolchain::ast::AST::Node parse_primitive_type();


    // Utility functions
    void synchronize();  // Error recovery
    void expect(lex::TokenKind kind, const llvm::StringRef& message);
    ziv::toolchain::ast::AST::Node parse_identifier();

    // Class members
    std::vector<ziv::toolchain::lex::TokenBuffer::Token> tokens_;
    ziv::toolchain::ast::AST& ast_;
    size_t current_;
    diagnostics::DiagnosticEmitter emitter_;
};

}  // namespace ziv::toolchain::parser

#endif  // ZIV_TOOLCHAIN_PARSER_PARSER_HPP
