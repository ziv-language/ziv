// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "lexer.hpp"

#include "toolchain/diagnostics/compilation_phase.hpp"

namespace ziv::toolchain::lex {

void Lexer::update_location(char c) {
    if (c == '\n') {
        current_loc_.line++;
        current_loc_.column = 1;
    } else {
        current_loc_.column++;
    }
    current_loc_.offset++;
    current_loc_.length = 1;
}

void Lexer::save_location() {
    start_loc_ = current_loc_;
}


void Lexer::initialize_handlers() {
    // Single line comments
    handlers_['#'] = &Lexer::consume_comment;

    // Strings and characters
    handlers_['"'] = &Lexer::consume_string;
    handlers_['\''] = &Lexer::consume_char;

    // Operators and punctuation
    handlers_['+'] = &Lexer::consume_operator;
    handlers_['-'] = &Lexer::consume_operator;
    handlers_['*'] = &Lexer::consume_operator;
    handlers_['/'] = &Lexer::consume_operator;
    handlers_['%'] = &Lexer::consume_operator;
    handlers_['='] = &Lexer::consume_operator;
    handlers_['!'] = &Lexer::consume_operator;
    handlers_['<'] = &Lexer::consume_operator;
    handlers_['>'] = &Lexer::consume_operator;
    handlers_['&'] = &Lexer::consume_operator;
    handlers_['|'] = &Lexer::consume_operator;
    handlers_['^'] = &Lexer::consume_operator;
    handlers_['~'] = &Lexer::consume_operator;

    // Delimiters
    handlers_['('] = &Lexer::consume_punctuation;
    handlers_[')'] = &Lexer::consume_punctuation;
    handlers_['{'] = &Lexer::consume_punctuation;
    handlers_['}'] = &Lexer::consume_punctuation;
    handlers_['['] = &Lexer::consume_punctuation;
    handlers_[']'] = &Lexer::consume_punctuation;
    handlers_[','] = &Lexer::consume_punctuation;
    handlers_[';'] = &Lexer::consume_punctuation;
    handlers_['.'] = &Lexer::consume_operator;
    handlers_[':'] = &Lexer::consume_operator;
    handlers_['@'] = &Lexer::consume_operator;
}

void Lexer::track_indentation() {
    size_t spaces = 0;
    size_t tab_width = 4;  // Standard tab width

    while (peek() == ' ' || peek() == '\t') {
        if (peek() == ' ') {
            spaces++;
        } else {  // tab
            spaces += tab_width - (spaces % tab_width);
        }
        consume();
    }

    // Each indentation level must be exactly indent_width_ spaces
    if (spaces % indent_width_ != 0) {
        emitter_.emit(diagnostics::DiagnosticKind::InvalidIndentation(),
                      current_loc_,
                      indent_width_);
        return;
    }

    size_t level = spaces / indent_width_;

    if (level > indent_level_) {
        // Only allow single level increases
        if (level != indent_level_ + 1) {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidIndentation(),
                          current_loc_,
                          "Invalid indentation level");
            return;
        }
        indent_stack_.push_back(indent_level_);
        indent_level_ = level;
        add_token(TokenKind::Indent(), "");
    } else if (level < indent_level_) {
        while (!indent_stack_.empty() && level < indent_level_) {
            indent_level_ = indent_stack_.back();
            indent_stack_.pop_back();
            add_token(TokenKind::Dedent(), "");
        }
        if (level != indent_level_) {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidIndentation(),
                          current_loc_,
                          "Invalid indentation level");
        }
    }
}

void Lexer::lex() {
    toolchain::diagnostics::PhaseGuard guard(toolchain::diagnostics::CompilationPhase::Lexing);
    add_token(TokenKind::Sof(), "");  // Start of file

    auto last_token = TokenKind::Sof();
    bool new_line = true;  // Start assuming we're at beginning of line

    while (!is_eof()) {
        if (is_line_terminator()) {
            new_line = true;
            consume();
            continue;
        }

        // Handle indentation at start of line
        if (new_line) {
            // Add implicit semicolon if needed
            if (can_terminate_expression(last_token)) {
                add_token(TokenKind::Semicolon(), ";");
            }
            track_indentation();
        }

        char current = peek();
        if (handlers_.count(current)) {
            (this->*handlers_[current])();
        } else if (std::isspace(current)) {
            consume_whitespace();
        } else if (std::isalpha(current) || current == '_') {
            consume_identifier();
        } else if (std::isdigit(current)) {
            consume_number();
        } else {
            consume_unknown();
        }

        new_line = false;
        last_token = buffer_.get_last_token();
    }

    // Handle final semicolon if needed
    if (can_terminate_expression(last_token)) {
        add_token(TokenKind::Semicolon(), ";");
    }

    // Add remaining dedents at EOF
    while (!indent_stack_.empty()) {
        indent_level_ = indent_stack_.back();
        indent_stack_.pop_back();
        add_token(TokenKind::Dedent(), "");
    }

    add_token(TokenKind::Eof(), "");
}

char Lexer::peek() const {
    return cursor_ < source_.get_contents().size() ? source_.get_contents()[cursor_] : '\0';
}

char Lexer::peek_next() const {
    if (cursor_ + 1 >= source_.get_contents().size())
        return '\0';
    return source_.get_contents()[cursor_ + 1];
}

char Lexer::consume() {
    char current = peek();
    update_location(current);
    cursor_++;
    return current;
}

bool Lexer::is_eof() const {
    return cursor_ >= source_.get_contents().size();
}

bool Lexer::is_line_terminator() const {
    return peek() == '\n' || peek() == '\r';
}

void Lexer::add_token(TokenKind kind, llvm::StringRef spelling) {
    buffer_.add_token(kind, spelling, current_loc_.line, current_loc_.column);
}

void Lexer::consume_whitespace() {
    while (std::isspace(peek())) {
        consume();
    }
}

void Lexer::consume_comment() {
    save_location();
    consume();  // Initial '#'

    if (peek() == '-' && source_.get_contents()[cursor_ + 1] == '-') {
        // Multi-line comment
        consume();  // First '-'
        consume();  // Second '-'

        while (!is_eof()) {
            if (peek() == '-' && cursor_ + 2 < source_.get_contents().size()
                && source_.get_contents()[cursor_ + 1] == '-'
                && source_.get_contents()[cursor_ + 2] == '#') {
                consume();  // First '-'
                consume();  // Second '-'
                consume();  // '#'
                return;
            }
            consume();
        }

        emitter_.emit(diagnostics::DiagnosticKind::UnterminatedComment(),
                      current_loc_,
                      "EOF in multi-line comment");
    } else {
        // Single-line comment
        while (!is_eof() && peek() != '\n') {
            consume();
        }
    }
}

void Lexer::consume_identifier() {
    std::string spelling;
    while (is_identifier_char(peek())) {
        spelling += consume();
    }
    TokenKind kind = lookup_keyword(spelling);
    add_token(kind, spelling);
}

void Lexer::consume_number() {
    save_location();
    std::string spelling;
    bool is_float = false;
    bool has_exponent = false;

    // Handle hex numbers
    if (peek() == '0' && (peek_next() == 'x' || peek_next() == 'X')) {
        spelling += consume();  // '0'
        spelling += consume();  // 'x'
        if (!std::isxdigit(peek())) {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidNumber(),
                          start_loc_,
                          "Expected hexadecimal digit after '0x'");
            return;
        }
        while (std::isxdigit(peek())) {
            spelling += consume();
        }
        add_token(TokenKind::IntLiteral(), spelling);
        return;
    }

    // Handle binary numbers
    if (peek() == '0' && (peek_next() == 'b' || peek_next() == 'B')) {
        spelling += consume();  // '0'
        spelling += consume();  // 'b'
        if (peek() != '0' && peek() != '1') {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidNumber(),
                          start_loc_,
                          "Expected binary digit after '0b'");
            return;
        }
        while (peek() == '0' || peek() == '1') {
            spelling += consume();
        }
        add_token(TokenKind::IntLiteral(), spelling);
        return;
    }

    // Handle leading sign
    if (peek() == '-' || peek() == '+') {
        spelling += consume();
    }

    // Consume integer part
    while (std::isdigit(peek())) {
        spelling += consume();
    }

    // Handle decimal point
    if (peek() == '.') {
        is_float = true;
        spelling += consume();

        // Must have at least one digit after decimal
        if (!std::isdigit(peek())) {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidNumber(),
                          start_loc_,
                          "Expected digit after decimal point");
            return;
        }

        while (std::isdigit(peek())) {
            spelling += consume();
        }
    }

    // Handle exponent
    if (peek() == 'e' || peek() == 'E') {
        is_float = true;
        has_exponent = true;
        spelling += consume();

        // Handle exponent sign
        if (peek() == '-' || peek() == '+') {
            spelling += consume();
        }

        if (!std::isdigit(peek())) {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidNumber(),
                          start_loc_,
                          "Expected digit in exponent");
            return;
        }

        while (std::isdigit(peek())) {
            spelling += consume();
        }
    }

    // Check for invalid suffixes
    if (std::isalpha(peek()) || peek() == '_') {
        emitter_.emit(diagnostics::DiagnosticKind::InvalidNumber(),
                      start_loc_,
                      "Invalid number suffix");
        return;
    }

    add_token(is_float ? TokenKind::FloatLiteral() : TokenKind::IntLiteral(), spelling);
}

void Lexer::consume_string() {
    save_location();
    consume();  // Initial quote
    std::string spelling;
    bool escaped = false;

    while (!is_eof()) {
        char c = peek();
        if (c == '\n') {
            emitter_.emit(diagnostics::DiagnosticKind::UnterminatedString(),
                          start_loc_,
                          "unterminated string literal");
            return;
        }

        if (!escaped) {
            if (c == '"') {
                consume();
                add_token(TokenKind::StringLiteral(), spelling);
                return;
            }
            if (c == '\\') {
                escaped = true;
                auto escape_loc = current_loc_;
                if (peek_next() == '\0') {
                    emitter_.emit(diagnostics::DiagnosticKind::InvalidEscapeSequence(),
                                  escape_loc,
                                  "incomplete escape sequence");
                    return;
                }
                consume();
                continue;
            }
        } else {
            escaped = false;
            switch (c) {
            case 'n':
                spelling += '\n';
                break;
            case 't':
                spelling += '\t';
                break;
            case 'r':
                spelling += '\r';
                break;
            case '\\':
                spelling += '\\';
                break;
            case '"':
                spelling += '"';
                break;
            default:
                emitter_.emit(diagnostics::DiagnosticKind::InvalidEscapeSequence(),
                              current_loc_,
                              "invalid escape sequence '\\{0}'",
                              c);
                spelling += '\\';
                spelling += c;
            }
            consume();
            continue;
        }
        spelling += consume();
    }

    emitter_.emit(diagnostics::DiagnosticKind::UnterminatedString(),
                  start_loc_,
                  "EOF in string literal");
}

void Lexer::consume_char() {
    save_location();
    consume();  // Initial quote
    std::string spelling;

    if (is_eof()) {
        emitter_.emit(diagnostics::DiagnosticKind::UnterminatedCharacter(),
                      start_loc_,
                      "Empty character literal");
        return;
    }

    char c = peek();
    if (c == '\\') {
        consume();
        if (is_eof()) {
            emitter_.emit(diagnostics::DiagnosticKind::InvalidEscapeSequence(),
                          start_loc_,
                          "Incomplete escape sequence");
            return;
        }
        c = peek();
        switch (c) {
        case 'n':
            spelling = "\n";
            break;
        case 't':
            spelling = "\t";
            break;
        case 'r':
            spelling = "\r";
            break;
        case '\\':
            spelling = "\\";
            break;
        case '\'':
            spelling = "'";
            break;
        default:
            emitter_.emit(diagnostics::DiagnosticKind::InvalidEscapeSequence(),
                          current_loc_,
                          "Invalid escape sequence '\\{0}'",
                          c);
            return;
        }
        consume();
    } else {
        spelling = consume();
    }

    if (peek() != '\'') {
        emitter_.emit(diagnostics::DiagnosticKind::UnterminatedCharacter(),
                      start_loc_,
                      "Multi-character char literal or unterminated char literal");
        return;
    }
    consume();  // Closing quote

    add_token(TokenKind::CharLiteral(), spelling);
}

void Lexer::consume_punctuation() {
    std::string spelling;
    spelling += consume();

    // Handle special cases like '..'
    if (spelling[0] == '.' && peek() == '.') {
        spelling += consume();
    }

    TokenKind kind = lookup_symbol(spelling);
    add_token(kind, spelling);
}

void Lexer::consume_operator() {
    std::string spelling;
    spelling += consume();

    // Check for two-character operators
    char next = peek();
    if (next == '=' || (spelling[0] == '-' && next == '>') || (spelling[0] == '.' && next == '.')
        || ((spelling[0] == '+' || spelling[0] == '-') && next == spelling[0])) {
        spelling += consume();
    }

    TokenKind kind = lookup_symbol(spelling);
    add_token(kind, spelling);
}

void Lexer::consume_unknown() {
    save_location();
    char c = consume();
    emitter_.emit(diagnostics::DiagnosticKind::InvalidCharacter(),
                  start_loc_,
                  "Invalid character '{0}'",
                  c);
}

bool Lexer::skip_whitespace() {
    bool skipped = false;
    while (std::isspace(peek())) {
        consume();
        skipped = true;
    }
    return skipped;
}

bool Lexer::is_identifier_start(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::is_identifier_char(char c) const {
    return std::isalnum(c) || c == '_';
}

bool Lexer::can_terminate_expression(const TokenKind& kind) const {
    return kind == TokenKind::Identifier() || kind == TokenKind::IntLiteral()
           || kind == TokenKind::FloatLiteral() || kind == TokenKind::StringLiteral()
           || kind == TokenKind::Break() || kind == TokenKind::Continue()
           || kind == TokenKind::Return() || kind == TokenKind::RBrace()
           || kind == TokenKind::RParen() || kind == TokenKind::RBracket()
           || kind == TokenKind::Increment() || kind == TokenKind::Decrement();
}

TokenKind Lexer::lookup_keyword(const std::string& spelling) {
    static const std::unordered_map<std::string, TokenKind> keyword_map = {
        // General Keywords
        {"let", TokenKind::Let()},
        {"var", TokenKind::Var()},
        {"mut", TokenKind::Mut()},
        {"ref", TokenKind::Ref()},
        {"take", TokenKind::Take()},
        {"fn", TokenKind::Fn()},
        {"class", TokenKind::Class()},
        {"interface", TokenKind::Interface()},
        {"module", TokenKind::Module()},
        {"end", TokenKind::End()},
        {"import", TokenKind::Import()},
        {"as", TokenKind::As()},
        {"if", TokenKind::If()},
        {"else", TokenKind::Else()},
        {"for", TokenKind::For()},
        {"in", TokenKind::In()},
        {"while", TokenKind::While()},
        {"break", TokenKind::Break()},
        {"continue", TokenKind::Continue()},
        {"do", TokenKind::Do()},
        {"ret", TokenKind::Return()},
        // Logical Keywords
        {"and", TokenKind::And()},
        {"or", TokenKind::Or()},
        {"not", TokenKind::Not()},
        {"xor", TokenKind::Xor()},
        // Type Keywords
        {"int", TokenKind::Int()},
        {"float", TokenKind::Float()},
        {"bool", TokenKind::Bool()},
        {"char", TokenKind::Char()},
        {"string", TokenKind::String()},
        {"void", TokenKind::Type()},
        {"null", TokenKind::Null()},
        // Boolean Keywords
        {"true", TokenKind::True()},
        {"false", TokenKind::False()},
        {"thread", TokenKind::Thread()},
    };

    auto it = keyword_map.find(spelling);
    return it != keyword_map.end() ? it->second : TokenKind::Identifier();
};

TokenKind Lexer::lookup_symbol(const std::string& spelling) {
    static const std::unordered_map<std::string, TokenKind> symbol_map = {
        // Symbols
        {"->", TokenKind::Arrow()},
        {"..", TokenKind::DoubleDot()},
        {"==", TokenKind::DoubleEquals()},
        {"!=", TokenKind::NotEquals()},
        {"<=", TokenKind::LessEquals()},
        {">=", TokenKind::GreaterEquals()},
        {"+=", TokenKind::PlusEquals()},
        {"-=", TokenKind::MinusEquals()},
        {"*=", TokenKind::StarEquals()},
        {"/=", TokenKind::SlashEquals()},
        {"++", TokenKind::Increment()},
        {"--", TokenKind::Decrement()},
        {"<", TokenKind::Less()},
        {">", TokenKind::Greater()},
        {"+", TokenKind::Plus()},
        {"-", TokenKind::Minus()},
        {"*", TokenKind::Star()},
        {"/", TokenKind::Slash()},
        {"%", TokenKind::Percent()},
        {"=", TokenKind::Equals()},
        {"!", TokenKind::Bang()},
        {"&", TokenKind::Ampersand()},
        {"|", TokenKind::Pipe()},
        {"^", TokenKind::Caret()},
        {"~", TokenKind::Tilde()},
        {".", TokenKind::Dot()},
        {"@", TokenKind::At()},

        // Delimiters
        {"(", TokenKind::LParen()},
        {")", TokenKind::RParen()},
        {"{", TokenKind::LBrace()},
        {"}", TokenKind::RBrace()},
        {"[", TokenKind::LBracket()},
        {"]", TokenKind::RBracket()},
        {",", TokenKind::Comma()},
        {":", TokenKind::Colon()},
        {";", TokenKind::Semicolon()},
    };

    auto it = symbol_map.find(spelling);
    return it != symbol_map.end() ? it->second : TokenKind::Unknown();
};
}  // namespace ziv::toolchain::lex
