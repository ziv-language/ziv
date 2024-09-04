#ifndef LEXER_HPP
#define LEXER_HPP

#include "llvm/ADT/StringRef.h"
#include "tokens.hpp"

namespace ziv {
    class Lexer {
        public:
            Lexer(std::string filename)
                : last_location(
                        {std::make_shared<std::string>(std::move(filename)), 0, 0}) {}
            virtual ~Lexer() = default;

            Token get_current_token();
            Token get_next_token();
            void consume_token(Token tok);
            llvm::StringRef get_id();
            double get_value();
            Location get_last_location();
            int get_line();
            int get_column();
            
        private:
            virtual llvm::StringRef read_next_line() = 0;
            int get_next_char();
            Token get_token();

            Token current_token = tok_eof;
            Location last_location;
            std::string identifier_str;
            double numeric_value = 0;
            Token last_char = Token(' ');
            int current_line = 0;
            int current_column = 0;
            llvm::StringRef current_line_buffer = "\n";
    };
}

#endif // LEXER_HPP
