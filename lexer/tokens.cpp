#include <ziv_tokens.hpp>

ziv::Token::Token(const ziv::TokenValue& token, const std::string& value)
    : token(token), value(value) {}

ziv::Token::Token(const TokenValue& token) : token(token), value(std::string()) {}

ziv::TokenAdministrator::TokenAdministrator() : values_() {}

void ziv::TokenAdministrator::add(const Token &token) noexcept {
  values_.push_back(token);
}

std::vector<ziv::Token> ziv::TokenAdministrator::get_tokens() const {
  return values_;
}

void ziv::TokenAdministrator::clear() noexcept {
	values_.clear();
}
