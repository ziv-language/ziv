// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <iostream>
#include "parser.hpp"
#include "llvm/Support/raw_ostream.h"

namespace ziv::toolchain::parser {

    bool Parser::match(lex::TokenKind kind) const {
        if (is_eof()) {
            return false;
        }
        return peek().kind == kind;
    }

    bool Parser::check(lex::TokenKind kind) const {
        if (is_eof()) {
            return false;
        }
        return peek().kind == kind;
    }

    const lex::TokenBuffer::Token& Parser::consume() {
        if (!is_eof()) {
            return tokens_[current_++];
        }
        return tokens_[current_];
    }

    const lex::TokenBuffer::Token& Parser::peek() const {
        if (!is_eof()) {
            return tokens_[current_];
        }
        return tokens_[current_];
    }

    const lex::TokenBuffer::Token& Parser::previous() const {
        if (current_ > 0) {
            return tokens_[current_ - 1];
        }
        return tokens_[current_];
    }

    bool Parser::consume_match(lex::TokenKind kind) {
        if (match(kind)) {
            consume();
            return true;
        }
        return false;
    }

    void Parser::expect(lex::TokenKind kind, const llvm::StringRef &message) {
        if (!consume_match(kind)) {
            llvm::errs() << message << "\n"
                            << "Expected token: " << kind.get_name()
                            << "Got token: " << peek().kind.get_name() << "\n";
        }
    }

    void Parser::parse_error(const llvm::StringRef &message) {
        llvm::errs() << "Parse error: " << message << " at line: " << peek().line << " column: " << peek().column << "\n";

    }

    void Parser::parse() {
        auto root = ast_.add_node(ast::NodeKind::FileStart(),consume());
        while (!is_eof()) {
            auto node = parse_top_level();
            if (node.is_valid()) {
                ast_.add_child(root, node);
            }
            if (is_eof()) {
                break;
            }
        }
    }

    ziv::toolchain::ast::AST::Node Parser::parse_top_level() {
        if (Parser::match(ziv::toolchain::lex::TokenKind::Module())) {
            return parse_module_declaration();
        }
        else if (Parser::match(ziv::toolchain::lex::TokenKind::Import())) {
            return parse_module_import();
        }
        else if (Parser::match(ziv::toolchain::lex::TokenKind::Fn())) {
            return parse_function_declaration();
        }
        else if (consume_match(ziv::toolchain::lex::TokenKind::Eof())) {
            return ast_.add_node(ast::NodeKind::FileEnd(), previous());
        }
        consume();
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
        /*
            Import is done in three ways:
            1. Importing a module
            2. Importing a module with a specific name
            3. Importing a module with a specific name and alias

            Examples:
            1. import std
            2. import std as s
            3. import std { print }
        */
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
                parse_error("Expected identifier after 'as' keyword in module import");
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
                    parse_error("Expected identifier in module import");
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
                parse_error("Expected type after '->' in function signature");
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
                parse_error("Expected identifier in parameter list");
                return parameter_list_node;
            }
            auto parameter = ast_.add_node(ast::NodeKind::Parameter(), consume());
            ast_.add_child(parameter_list_node, parameter);

            if (!consume_match(ziv::toolchain::lex::TokenKind::Colon())) {
                parse_error("Expected ':' in parameter list");
                return parameter_list_node;
            }

            if (!match(ziv::toolchain::lex::TokenKind::Identifier())) {
                parse_error("Expected identifier after ':' in parameter list");
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
        // Function call is done as follows:
        // <identifier> ( <argument_list> )
        // Example: print("Hello, World!")


        // the function call node has function name and argument list as children
        auto function_call_node = ast_.add_node(ast::NodeKind::FunctionCall(), consume());

        auto argument_list = ast_.add_node(ast::NodeKind::ArgumentList(), consume());

        while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::RParen())) {
            auto argument = parse_statement();
            if (argument.is_valid()) {
                ast_.add_child(argument_list, argument);
            }
            if (!consume_match(ziv::toolchain::lex::TokenKind::Comma())) {
                break;
            }

        }
        expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of function call");

        ast_.add_child(function_call_node, argument_list);
        return function_call_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_statement() {
        /*
            as we don't have keywords for variable declaration, first check
            if it's a variable declaration or expression
        */

        if (match(ziv::toolchain::lex::TokenKind::Identifier())) {
            return parse_identifier();
        }
        else if (match(ziv::toolchain::lex::TokenKind::If())) {
            return parse_if_statement();
        }
        // else if (match(ziv::toolchain::lex::TokenKind::For())) {
        //     return parse_for_statement();
        // }
        // else if (match(ziv::toolchain::lex::TokenKind::While())) {
        //     return parse_while_statement();
        // }
        // else if (match(ziv::toolchain::lex::TokenKind::Do())) {
        //     return parse_do_while_statement();
        // }
        // else if (match(ziv::toolchain::lex::TokenKind::Try())) {
        //     return parse_try_handle_statement();
        // }
        // else if (match(ziv::toolchain::lex::TokenKind::Return())) {
        //     return parse_return_statement();
        // }
        // else if (match(ziv::toolchain::lex::TokenKind::Break())) {
        //     return parse_break_statement();
        // }
        // else if (match(ziv::toolchain::lex::TokenKind::Continue())) {
        //     return parse_continue_statement();
        // }
        return parse_primary();
    }

    ziv::toolchain::ast::AST::Node Parser::parse_block() {
        auto block_node = ast_.add_node(ast::NodeKind::CodeBlock(), consume());

        if (match(ziv::toolchain::lex::TokenKind::LBrace())) {
            return block_node;
        }

        while (!is_eof() && !match(ziv::toolchain::lex::TokenKind::RBrace())) {
            auto statement = parse_statement();
            if (statement.is_valid()) {
                ast_.add_child(block_node, statement);
            }
        }
        expect(ziv::toolchain::lex::TokenKind::RBrace(), "Expected '}' at end of block");
        return block_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_identifier() {
        // we don't have a keyword for variables so we need to check if it's a variable declaration or expression
        // variables are defined as: <identifier> : <type> = <expression>

        // look ahead to see if it's a variable declaration
        auto lookahead = current_;
        while (lookahead < tokens_.size() && tokens_[lookahead].kind != ziv::toolchain::lex::TokenKind::LBrace()) {
            if (tokens_[lookahead].kind == ziv::toolchain::lex::TokenKind::Colon()) {
                return parse_variable_declaration();
            }
            else if (tokens_[lookahead].kind == ziv::toolchain::lex::TokenKind::LParen()) {
                return parse_function_call();
            }
            lookahead++;
        }
        return ast_.add_node(ast::NodeKind::IdentifierExpr(), consume());
    }


    ziv::toolchain::ast::AST::Node Parser::parse_variable_declaration() {

        auto variable_node = ast_.add_node(ast::NodeKind::VariableDecl(), consume());

        if (!consume_match(ziv::toolchain::lex::TokenKind::Colon())) {
            return variable_node;
        }

        auto type = parse_type_specifier();
        if (type.is_valid()) {
            ast_.add_child(variable_node, type);
        }

        if (!consume_match(ziv::toolchain::lex::TokenKind::Equals())) {
            parse_error("Expected '=' in variable declaration");
            return variable_node;
        }

        auto expression = parse_primary();
        if (expression.is_valid()) {
            ast_.add_child(variable_node, expression);
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
            parse_error("Expected type specifier");
            return {};
        }
    }

    ziv::toolchain::ast::AST::Node Parser::parse_primary() {
        // Handle literals
        if (match(ziv::toolchain::lex::TokenKind::StringLiteral()) ||
            match(ziv::toolchain::lex::TokenKind::IntLiteral()) ||
            match(ziv::toolchain::lex::TokenKind::FloatLiteral()) ||
            // Boolean literals
            match(ziv::toolchain::lex::TokenKind::True()) ||
            match(ziv::toolchain::lex::TokenKind::False())) {
            return ast_.add_node(ast::NodeKind::LiteralExpr(), consume());
        }

        // Variable references
        if (match(ziv::toolchain::lex::TokenKind::Identifier())) {
            return parse_identifier();
        }

        parse_error("Expected primary expression");
        consume();
        return {};
    }


    ziv::toolchain::ast::AST::Node Parser::parse_delimiter() {
        if (consume_match(ziv::toolchain::lex::TokenKind::LParen())) {
            auto expression = parse_expression();
            expect(ziv::toolchain::lex::TokenKind::RParen(), "Expected ')' at end of expression");
            return expression;
        }

        // If there's no opening parenthesis, directly parse the condition
        return parse_expression(); // This should handle identifiers and literals
}

    ziv::toolchain::ast::AST::Node Parser::parse_expression() {

        return parse_comparison();
    }

    ziv::toolchain::ast::AST::Node Parser::parse_if_statement() {
        auto if_node = ast_.add_node(ast::NodeKind::IfStatement(), consume());

        auto condition = parse_delimiter();
        if (condition.is_valid()) {
            ast_.add_child(if_node, condition);
        }

        auto block = parse_block();
        if (block.is_valid()) {
            ast_.add_child(if_node, block);
        }

        if (match(ziv::toolchain::lex::TokenKind::Else())) {
            auto then_node = parse_if_else();
            ast_.add_child(if_node, then_node);
        }

        return if_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_if_else() {
        // check if else is an else if or else
        // look ahead to see if it's an else if
        auto lookahead = current_;
        while (lookahead < tokens_.size() && tokens_[lookahead].kind != ziv::toolchain::lex::TokenKind::Semicolon()) {
            if (tokens_[lookahead].kind == ziv::toolchain::lex::TokenKind::If()) {
                return parse_else_if_statement();
            }
            lookahead++;
        }
        return parse_else_statement();
    }

    ziv::toolchain::ast::AST::Node Parser::parse_else_statement() {

        auto else_node = ast_.add_node(ast::NodeKind::ElseStatement(), consume());

        auto block = parse_block();
        if (block.is_valid()) {
            ast_.add_child(else_node, block);
        }

        return else_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_else_if_statement() {
        auto else_if_node = ast_.add_node(ast::NodeKind::ElseIfStatement(), consume()); // consume 'else'

        if (!consume_match(ziv::toolchain::lex::TokenKind::If())) {
            parse_error("Expected 'if' in else if statement");
            return else_if_node;
        }


        auto condition = parse_delimiter();
        if (condition.is_valid()) {
            ast_.add_child(else_if_node, condition);
        }

        auto block = parse_block();
        if (block.is_valid()) {
            ast_.add_child(else_if_node, block);
        }

        if (match(ziv::toolchain::lex::TokenKind::Else())) {
            auto then_node = parse_if_else();
            ast_.add_child(else_if_node, then_node);
        }

        return else_if_node;
    }

    ziv::toolchain::ast::AST::Node Parser::parse_comparison() {
        auto left = parse_primary();

        while (match(ziv::toolchain::lex::TokenKind::Less()) ||
               match(ziv::toolchain::lex::TokenKind::LessEquals()) ||
               match(ziv::toolchain::lex::TokenKind::Greater()) ||
               match(ziv::toolchain::lex::TokenKind::GreaterEquals()) ||
               match(ziv::toolchain::lex::TokenKind::DoubleEquals()) ||
               match(ziv::toolchain::lex::TokenKind::NotEquals())) {
            auto op = consume();
            auto comparison_node = ast_.add_node(ast::NodeKind::ComparisonOp(), op);

            ast_.add_child(comparison_node, left);

            auto right = parse_primary();
            if (right.is_valid()) {
                ast_.add_child(comparison_node, right);
            }

            left = comparison_node;
        }
        return left;
    }

} // namespace ziv::toolchain::parser
