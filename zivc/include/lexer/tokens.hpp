// compiler/include/lexer/tokens.hpp

#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <memory>
#include <string>

namespace ziv {
    struct Location {
        std::shared_ptr<std::string> file;
        int line;
        int column;
    };

    enum Token : int {
        // Single-character tokens
        tok_colon = ':',
        tok_semicolon = ';',
        tok_lparen = '(',
        tok_rparen = ')',
        tok_lbrace = '{',
        tok_rbrace = '}',
        tok_lbracket = '[',
        tok_rbracket = ']',
        tok_comma = ',',
        tok_plus = '+',
        tok_minus = '-',
        tok_star = '*',
        tok_slash = '/',
        tok_percent = '%',
        tok_equal = '=',

        // EOF
        tok_eof = -1,

        // Keywords
        tok_let = -2,
        tok_mut = -3,
        tok_fn = -4,
        tok_return = -5,

        // Multi-character tokens
        tok_eqeq = -6,
        tok_noteq = -7,
        tok_gteq = -8,
        tok_lteq = -9,
        tok_arrow = -10,

        tok_if = -11,
        tok_else = -12,
        tok_match = -13,
        tok_for = -14,
        tok_while = -15,
        tok_async = -16,
        tok_await = -17,

        // Primary
        tok_identifier = -19,
        tok_number = -20,
        tok_string = -21,

        // Error
        tok_error = -100,
    };
}

#endif // TOKENS_HPP
