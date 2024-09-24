#include <vector>

namespace ziv {

enum class TokenType {
  NUMBER,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LEFTPAREN,
  RIGHTPAREN,
  ENDOFFILE,
  INVALID,
}

struct Token {
	TokenType type;
	std::string value;

	inline Token(TokenType type, std::string value = "") : type(type), value(value) {}
};

class InputProcessor {
public:
	inline explicit InputProcessor(const std::string& input) : input(input), pos(0) {}

	char currentChar() const {
		if (pos >= input.size()) return '\0';
		return input[pos];
	}

	void advance() {
		pos++;
	}
	
private:
	std::string input;
	size_t pos;
};

class TokenMatcher {
	public:
		virtual Token match(InputProcessor& InputProcessor) const = 0;
};

class NumberMatcher : public TokenMatcher {
	public:
    Token match(InputProcessor& inputProcessor) const override {
        if (isdigit(inputProcessor.currentChar())) {
            std::string num;
            while (isdigit(inputProcessor.currentChar())) {
                num += inputProcessor.currentChar();
                inputProcessor.advance();
            }
            return Token(TokenType::Number, num);
        }
        return Token(TokenType::Invalid);
    }
};

class Lexer {
	public:
//		explicit Lexer(InputProcessor& inputProcessor) : inputProcessor(inputProcessor)
	private:
		InputProcessor& inputProcessor;
		std::vector<TokenMatcher*> matchers;
		Token getNextToken();
};
} // namespace ziv
