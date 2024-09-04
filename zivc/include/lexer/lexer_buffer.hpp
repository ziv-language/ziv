// compiler/include/lexer/lexer_buffer.hpp

#ifndef LEXER_BUFFER_HPP
#define LEXER_BUFFER_HPP

#include "lexer.hpp"

namespace ziv {
    class LexerBuffer final : public Lexer {
        public:
            LexerBuffer(const char *begin, const char *end, std::string filename)
            : Lexer(std::move(filename)), current(begin), end(end) {}

        private:
            llvm::StringRef read_next_line() override {
            auto *begin = current;
            while (current <= end && *current && *current != '\n')
            ++current;
            if (current <= end && *current)
            ++current;
            llvm::StringRef result{begin, static_cast<size_t>(current - begin)};
            return result;
        }
        const char *current, *end;
    };
};

#endif // LEXER_BUFFER_HPP