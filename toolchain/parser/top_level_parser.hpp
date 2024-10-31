// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception


#include "parser.hpp"

namespace ziv::toolchain::parser {
    ziv::toolchain::ast::AST::Node Parser::parse_top_level() {
        switch (peek().kind) {
            case ziv::toolchain::lex::TokenKind::Module():
                return parse_module_declaration();
            case ziv::toolchain::lex::TokenKind::Import():
                return parse_module_import();
            case ziv::toolchain::lex::TokenKind::If():
                return parse_if_statement();
            case ziv::toolchain::lex::TokenKind::Fn():
                return parse_function_declaration();
            default:
                return parse_identifier();
        }
        // consume();
        return ast_.add_node(ast::NodeKind::Invalid(), previous());
    }

    ziv::toolchain::ast::AST::Node Parser::parse_module_declaration() {
        auto module_node = ast_.add_node(ast::NodeKind::ModuleDecl(), consume());

        // Parse module name
        if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
            return module_node;
        }

        auto module_name = ast_.add_node(ast::NodeKind::ModuleName(), consume());
        ast_.add_child(module_node, module_name);

        // Parse module body
        while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::End())) {
            auto node = parse_top_level();
            if (node.is_valid()) {
                ast_.add_child(module_node, node);
            }
        }
        expect(ziv::toolchain::lex::TokenKind::End(), "Expected 'end' keyword at end of module declaration");
        expect(ziv::toolchain::lex::TokenKind::Module(), "Expected 'module' keyword at end of module declaration");
        return module_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_module_import() {
        auto import_node = ast_.add_node(ast::NodeKind::ModuleImport(), consume());

        // Parse module name
        if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
            return import_node;
        }

        auto module_name = ast_.add_node(ast::NodeKind::ModuleName(), consume());
        ast_.add_child(import_node, module_name);

        // Check for alias
        if (consume_match(ziv::toolchain::lex::TokenKind::As())) {
            if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
                // error node
                parse_error(import_node, "Expected identifier after 'as' in module import");
                return import_node;
            }
            auto alias = ast_.add_node(ast::NodeKind::ModuleAlias(), consume());
            ast_.add_child(import_node, alias);
        }

        // Check for specific imports
        if (consume_match(ziv::toolchain::lex::TokenKind::LBrace())) {
            auto import_list = ast_.add_node(ast::NodeKind::ModuleImportList(), previous());

            while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::RBrace())) {
                if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
                    parse_error(import_node, "Expected identifier in module import");
                    return import_node;
                }
                auto import = ast_.add_node(ast::NodeKind::ModuleImportItem(), consume());
                ast_.add_child(import_list, import);
                if (!consume_match(ziv::toolchain::lex::TokenKind::Comma())) {
                    break;
                }
            }
            expect(ziv::toolchain::lex::TokenKind::RBrace(), "Expected '}' at end of module import");
            ast_.add_child(import_node, import_list);
        }
        return import_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_function_declaration() {
        //  (param1: type, param2: type) -> return_type { body }
        auto function_node = ast_.add_node(ast::NodeKind::FunctionDecl(), consume());

        // Parse function name
        if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
            return function_node;
        }

        auto function_name = ast_.add_node(ast::NodeKind::FunctionName(), consume());
        ast_.add_child(function_node, function_name);

        // Parse function signature
        auto signature = parse_function_signature();
        if (signature.is_valid()) {
            ast_.add_child(function_node, signature);
        }

        // Parse function body
        auto body = parse_function_body();
        if (body.is_valid()) {
            ast_.add_child(function_node, body);
        }

        return function_node;
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
            if (!match(ziv::toolchain::lex::TokenKind::Type())) {
                parse_error(signature_node, "Expected type after '->' in function signature");
                return signature_node;
            }
            auto return_type = ast_.add_node(ast::NodeKind::ReturnType(), consume());
            ast_.add_child(signature_node, return_type);
        }

        return signature_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_parameter_list() {
        auto parameter_list_node = ast_.add_node(ast::NodeKind::ParameterList(), consume());

        if (!consume_match(ziv::toolchain::lex::TokenKind::LParen())) {
            return parameter_list_node;
        }

        while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::RParen())) {
            if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
                parse_error(parameter_list_node, "Expected identifier in parameter list");
                return parameter_list_node;
            }
            auto parameter = ast_.add_node(ast::NodeKind::Parameter(), consume());
            ast_.add_child(parameter_list_node, parameter);

            if (!consume_match(ziv::toolchain::lex::TokenKind::Colon())) {
                parse_error(parameter_list_node, "Expected ':' in parameter list");
                return parameter_list_node;
            }

            if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
                parse_error(parameter_list_node, "Expected identifier after ':' in parameter list");
                return parameter_list_node;
            }
            auto parameter_type = ast_.add_node(ast::NodeKind::ParameterType(), consume());
            ast_.add_child(parameter_list_node, parameter_type);

            if (!consume_match(ziv::toolchain::lex::TokenKind::Comma())) {
                break;
            }
        }
        expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of parameter list");
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

        return function_call_node;
    }


    ziv::toolchain::ast::AST::Node Parser::parse_identifier() {
        // we don't have a keyword for variables so we need to check if it's a variable declaration or expression
        // variables are defined as: <identifier> : <type> = <expression>

        if (is_keyword()) {
            return parse_statement();
        }

        // Check next token for function call
        if (tokens_[current_ + 1].kind == ziv::toolchain::lex::TokenKind::LParen()) {
            return parse_function_call();
        }
        // Look ahead to see if it's a variable declaration
        auto lookahead = current_;
        while (lookahead < tokens_.size() && tokens_[lookahead].kind != ziv::toolchain::lex::TokenKind::LBrace()) {
            if (tokens_[lookahead].kind == ziv::toolchain::lex::TokenKind::Colon()) {
                return parse_variable_declaration();
            }
            lookahead++;
        }
        return ast_.add_node(ast::NodeKind::IdentifierExpr(), consume());
    }


    ziv::toolchain::ast::AST::Node Parser::parse_variable_declaration() {
        // Store the identifier token
        auto identifier_token = consume();
        auto variable_node = ast_.add_node(ast::NodeKind::VariableDecl(), identifier_token);

        // Parse type specifier
        if (!consume_match(ziv::toolchain::lex::TokenKind::Colon())) {
            parse_error(variable_node, "Expected ':' in variable declaration");
            return variable_node;
        }

        // Parse the type
        auto type = parse_type_specifier();
        if (type.is_valid()) {
            ast_.add_child(variable_node, type);
        }

        // Parse the initial value
        if (consume_match(ziv::toolchain::lex::TokenKind::Equals())) {
            auto expression = parse_expression();
            if (expression.is_valid()) {
                ast_.add_child(variable_node, expression);
            }
        }

        return variable_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_type_specifier() {
        if (match(ziv::toolchain::lex::TokenKind::Int())) {
            return ast_.add_node(ast::NodeKind::IntegerType(), consume());
        }
        else if (match(ziv::toolchain::lex::TokenKind::Float())) {
            return ast_.add_node(ast::NodeKind::FloatType(), consume());
        }
        else if (match(ziv::toolchain::lex::TokenKind::String())) {
            return ast_.add_node(ast::NodeKind::StringType(), consume());
        }
        else if (match(ziv::toolchain::lex::TokenKind::Bool())) {
            return ast_.add_node(ast::NodeKind::BoolType(), consume());
        }
        else {
            auto invalid_node = ast_.add_node(ast::NodeKind::Invalid(), consume());
            parse_error(invalid_node, "Expected type specifier");
            return {};
        }
    }


    ziv::toolchain::ast::AST::Node Parser::parse_delimiter() {
        bool has_parens = consume_match(ziv::toolchain::lex::TokenKind::LParen());

        auto expr = parse_expression();

        if (has_parens) {
            expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of expression");
        }

        return expr;
    }


    ziv::toolchain::ast::AST::Node Parser::parse_assignment() {
        auto left = parse_logical_or();

        if (match(ziv::toolchain::lex::TokenKind::Equals())) {
            auto op = consume();
            auto assignment_node = ast_.add_node(ast::NodeKind::AssignmentOp(), op);

            ast_.add_child(assignment_node, left);

            auto right = parse_assignment(); // Recurse to handle chained assignments
            if (right.is_valid()) {
                ast_.add_child(assignment_node, right);
            }

            return assignment_node;
        }
        return left;
    }
} //
