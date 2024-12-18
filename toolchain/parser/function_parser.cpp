// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {

ziv::toolchain::ast::AST::Node Parser::parse_function_declaration() {
    auto fn_decl = ast_.add_node(ast::NodeKind::FunctionDecl(), consume());  // Consume 'fn'

    // Parse function name
    expect(lex::TokenKind::Identifier(), "Expected function name");

    auto fn_name = ast_.add_node(ast::NodeKind::FunctionName(), previous());
    ast_.add_child(fn_decl, fn_name);

    // Parse generic parameters if present
    if (match(lex::TokenKind::LBracket())) {
        auto generic_params = parse_generic_parameters();
        if (generic_params.is_valid()) {
            ast_.add_child(fn_decl, generic_params);
        }
    }

    // Parse parameter list
    auto params = parse_parameter_list();
    if (params.is_valid()) {
        ast_.add_child(fn_decl, params);
    }

    // Parse return type
    if (consume_match(lex::TokenKind::Arrow())) {
        auto return_type = parse_type_specifier();
        if (return_type.is_valid()) {
            ast_.add_child(fn_decl, return_type);
        }
    }

    // Parse function body
    if (consume_match(lex::TokenKind::Colon())) {
        auto body = parse_block();
        if (body.is_valid()) {
            ast_.add_child(fn_decl, body);
        }
    }

    return fn_decl;
}

ziv::toolchain::ast::AST::Node Parser::parse_generic_parameters() {
    auto generic_params = ast_.add_node(ast::NodeKind::GenericParameters(),
                                        consume());  // Consume '['

    while (!is_eof() && !match(lex::TokenKind::RBracket())) {
        if (!match(lex::TokenKind::Identifier())) {
            emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                          peek().get_location(),
                          "Expected generic parameter name");
            break;
        }

        auto param = ast_.add_node(ast::NodeKind::GenericParameter(), consume());
        ast_.add_child(generic_params, param);

        // Handle type constraints if present (T: Trait)
        if (match(lex::TokenKind::Colon())) {
            consume();  // Consume ':'
            if (!match(lex::TokenKind::Identifier())) {
                emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                              peek().get_location(),
                              "Expected trait name after ':'");
                break;
            }
            auto constraint = ast_.add_node(ast::NodeKind::TypeConstraint(), consume());
            ast_.add_child(param, constraint);
        }

        // Handle comma separator for multiple generic parameters
        if (!match(lex::TokenKind::RBracket())) {
            if (!consume_match(lex::TokenKind::Comma())) {
                emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                              peek().get_location(),
                              "Expected ',' or ']' after generic parameter");
                break;
            }
        }
    }

    expect(lex::TokenKind::RBracket(), "Expected ']' after generic parameters");
    return generic_params;
}

ziv::toolchain::ast::AST::Node Parser::parse_function_signature() {
    auto signature_node = ast_.add_node(ast::NodeKind::FunctionSignature(), consume());

    // Parse parameter list
    auto parameter_list = parse_parameter_list();
    if (parameter_list.is_valid()) {
        ast_.add_child(signature_node, parameter_list);
    }

    // Parse return type
    if (consume_match(ziv::toolchain::lex::TokenKind::Arrow())) {
        expect(ziv::toolchain::lex::TokenKind::Type(), "Expected return type after '->'");
        auto return_type = ast_.add_node(ast::NodeKind::ReturnStmt(), consume());
        ast_.add_child(signature_node, return_type);
    }

    return signature_node;
}

ziv::toolchain::ast::AST::Node Parser::parse_parameter_list() {
    expect(lex::TokenKind::LParen(), "Expected '(' at start of parameter list");

    auto parameter_list_node = ast_.add_node(ast::NodeKind::ParameterList(), previous());

    while (!is_eof() && !match(lex::TokenKind::RParen())) {
        auto param = ast_.add_node(ast::NodeKind::Parameter(), peek());

        // Parse parameter modifiers
        if (match(lex::TokenKind::Take())) {
            // Handle 'take' modifier
            auto modifier = ast_.add_node(ast::NodeKind::ParameterModifier(), consume());
            ast_.add_child(param, modifier);
        } else if (match(lex::TokenKind::Mut())) {
            // Handle 'mut ref' case first
            auto mut_modifier = ast_.add_node(ast::NodeKind::ParameterModifier(), consume());
            if (match(lex::TokenKind::Ref())) {
                auto ref_modifier = ast_.add_node(ast::NodeKind::ParameterModifier(), consume());
                ast_.add_child(mut_modifier, ref_modifier);
                ast_.add_child(param, mut_modifier);
            } else {
                emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                              peek().get_location(),
                              "Expected 'ref' after 'mut'");
                break;
            }
        } else if (match(lex::TokenKind::Ref())) {
            // Handle 'ref' modifier
            auto modifier = ast_.add_node(ast::NodeKind::ParameterModifier(), consume());
            ast_.add_child(param, modifier);
        }

        // Parse parameter name
        if (!match(lex::TokenKind::Identifier())) {
            emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                          peek().get_location(),
                          "Expected parameter name");
            break;
        }
        auto param_name = ast_.add_node(ast::NodeKind::ParameterName(), consume());
        ast_.add_child(param, param_name);

        // Parse type annotation
        expect(lex::TokenKind::Colon(), "Expected ':' after parameter name");
        auto type = parse_type_specifier();
        if (type.is_valid()) {
            ast_.add_child(param, type);
        }

        ast_.add_child(parameter_list_node, param);

        if (!match(lex::TokenKind::RParen())) {
            if (!consume_match(lex::TokenKind::Comma())) {
                emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                              peek().get_location(),
                              "Expected ',' or ')' after parameter");
                break;
            }
        }
    }

    expect(lex::TokenKind::RParen(), "Expected ')' at end of parameter list");
    return parameter_list_node;
}

ziv::toolchain::ast::AST::Node Parser::parse_function_body() {
    auto body_node = ast_.add_node(ast::NodeKind::FunctionBody(), peek());

    // Parse block
    auto block = parse_block();
    if (block.is_valid()) {
        ast_.add_child(body_node, block);
    }

    return body_node;
}

ziv::toolchain::ast::AST::Node Parser::parse_function_call() {
    // the function call node has function name and argument list as children
    auto function_name = consume();
    auto function_call_node = ast_.add_node(ast::NodeKind::FunctionCall(), function_name);

    expect(ziv::toolchain::lex::TokenKind::LParen(), "Expected '(' at start of function call");

    // Parse argument list
    if (!match(ziv::toolchain::lex::TokenKind::RParen())) {
        do {
            auto argument = parse_expression();
            if (argument.is_valid()) {
                ast_.add_child(function_call_node, argument);
            }
        } while (consume_match(ziv::toolchain::lex::TokenKind::Comma()));
    }

    expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of function call");
    expect(ziv::toolchain::lex::TokenKind::Semicolon(), "Expected ';' at end of function call");

    return function_call_node;
}

}  // namespace ziv::toolchain::parser
