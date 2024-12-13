// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_PARSER_OPERATOR_PRECEDENCE_HPP
#define ZIV_TOOLCHAIN_PARSER_OPERATOR_PRECEDENCE_HPP

#include "toolchain/lex/token_kind.hpp"

namespace ziv::toolchain::parser {

enum class Precedence {
    LeftBindsTighter,
    RightBindsTighter,
    Ambiguous,
};

class OperatorPrecedence {
public:
    static Precedence compare_precedence(ziv::toolchain::lex::TokenKind left,
                                         ziv::toolchain::lex::TokenKind right) {
        // Multiplication and division bind tighter than addition and subtraction
        if ((is_multiplicative(left) && is_additive(right))
            || (is_multiplicative(left) && is_bitwise(right))
            || (is_additive(left) && is_bitwise(right))) {
            return Precedence::LeftBindsTighter;
        }

        if ((is_additive(left) && is_multiplicative(right))
            || (is_bitwise(left) && is_multiplicative(right))
            || (is_bitwise(left) && is_additive(right))) {
            return Precedence::RightBindsTighter;
        }

        // Same precedence level operators
        if (is_multiplicative(left) && is_multiplicative(right)) {
            return Precedence::LeftBindsTighter;  // Left associative
        }
        if (is_additive(left) && is_additive(right)) {
            return Precedence::LeftBindsTighter;  // Left associative
        }

        // Bitwise operations with mixed arithmetic require explicit parentheses
        if ((is_bitwise(left) && is_bitwise(right)) || (is_bitwise(left) && is_arithmetic(right))
            || (is_arithmetic(left) && is_bitwise(right))) {
            return Precedence::Ambiguous;
        }

        // Comparison operators
        if (is_comparison(left) || is_comparison(right)) {
            return Precedence::Ambiguous;  // Always require parentheses for clarity
        }

        // Logical operators
        if (is_logical(left) && is_logical(right)) {
            if (left == right) {
                return Precedence::LeftBindsTighter;  // Left associative
            }
            // Different logical operators require explicit precedence
            return Precedence::Ambiguous;
        }

        return Precedence::Ambiguous;
    }

private:
    static bool is_multiplicative(ziv::toolchain::lex::TokenKind kind) {
        return kind == ziv::toolchain::lex::TokenKind::Star()
               || kind == ziv::toolchain::lex::TokenKind::Slash()
               || kind == ziv::toolchain::lex::TokenKind::Percent();
    }

    static bool is_additive(ziv::toolchain::lex::TokenKind kind) {
        return kind == ziv::toolchain::lex::TokenKind::Plus()
               || kind == ziv::toolchain::lex::TokenKind::Minus();
    }

    static bool is_bitwise(ziv::toolchain::lex::TokenKind kind) {
        return kind == ziv::toolchain::lex::TokenKind::Pipe()
               || kind == ziv::toolchain::lex::TokenKind::Ampersand()
               || kind == ziv::toolchain::lex::TokenKind::Caret();
    }

    static bool is_comparison(ziv::toolchain::lex::TokenKind kind) {
        return kind == ziv::toolchain::lex::TokenKind::Less()
               || kind == ziv::toolchain::lex::TokenKind::Greater()
               || kind == ziv::toolchain::lex::TokenKind::LessEquals()
               || kind == ziv::toolchain::lex::TokenKind::GreaterEquals()
               || kind == ziv::toolchain::lex::TokenKind::DoubleEquals()
               || kind == ziv::toolchain::lex::TokenKind::NotEquals();
    }

    static bool is_logical(ziv::toolchain::lex::TokenKind kind) {
        return kind == ziv::toolchain::lex::TokenKind::And()
               || kind == ziv::toolchain::lex::TokenKind::Or()
               || kind == ziv::toolchain::lex::TokenKind::Not();
    }

    static bool is_arithmetic(ziv::toolchain::lex::TokenKind kind) {
        return is_multiplicative(kind) || is_additive(kind);
    }
};

}  // namespace ziv::toolchain::parser

#endif  // ZIV_TOOLCHAIN_PARSER_OPERATOR_PRECEDENCE_HPP
