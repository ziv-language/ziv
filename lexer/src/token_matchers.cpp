#include <cctype>
#include <zivlexer.hpp>

ziv::Token ziv::NumberMatcher::match(ziv::IInputProcessor& input_processor) const {
	if (std::isdigit(input_processor.current_char())) {
		std::string num;
		while (std::isdigit(input_processor.current_char())) {
			num += input_processor.current_char();
			input_processor.advance();
		}
		return ziv::Token(ziv::TokenType::NUMBER, num);
	}
	return Token(ziv::TokenType::INVALID);
}
