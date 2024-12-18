// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "parser.hpp"

namespace ziv::toolchain::parser {

void Parser::synchronize() {
    consume();

    while (!is_eof()) {
        if (previous().kind == lex::TokenKind::Semicolon())
            return;

        switch (peek().kind) {
        case lex::TokenKind::Fn():
        case lex::TokenKind::Let():
        case lex::TokenKind::Mut():
        case lex::TokenKind::Class():
        case lex::TokenKind::If():
        case lex::TokenKind::While():
        case lex::TokenKind::Return():
            return;
        default:
            consume();
        }
    }
}

bool Parser::match(lex::TokenKind kind) const {
    if (is_eof()) {
        return false;
    }
    return peek().kind == kind;
}

bool Parser::is_keyword() const {
    return peek().get_kind().is_keyword();
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

void Parser::expect(lex::TokenKind kind, const llvm::StringRef& message) {
    if (!consume_match(kind)) {
        emitter_.emit(diagnostics::DiagnosticKind::UnexpectedToken(),
                      peek().get_location(),
                      peek().get_spelling(),
                      message);
    }
}

void Parser::parse_error(const ast::AST::Node& node, const llvm::StringRef& message) {
    ast_.mark_error(node);
    llvm::outs() << "Error: " << message << "\n";
}

}  // namespace ziv::toolchain::parser
