#include <memory>
#include <zivlexer.hpp>

ziv::Lexer::Lexer(ziv::IInputProcessor &input_processor)
    : input_processor(input_processor) {
  matchers.push_back(std::make_unique<ziv::NumberMatcher>());
  // agregar más matchers
}

ziv::Token ziv::Lexer::get_next_token() {
  for (const auto &matcher : matchers) {
    ziv::Token token = matcher->match(input_processor);
    if (token.type != ziv::TokenType::INVALID) {
      return token;
    }
  }

  input_processor.advance();
  return ziv::Token(ziv::TokenType::INVALID,
                    std::string(1, input_processor.current_char()));
}

std::vector<ziv::Token> ziv::Lexer::tokenize() {
  std::vector<ziv::Token> tokens;

  while (!input_processor.end_of_input()) {
    ziv::Token token = get_next_token();

    if (token.type == ziv::TokenType::INVALID ||
        token.type == ziv::TokenType::ENDOFFILE) {
			tokens.push_back(token);
			break;
    }

		tokens.push_back(token);
  }

	return tokens;
}
