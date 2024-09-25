#ifndef ZIVLEXER_HPP
#define ZIVLEXER_HPP
#include <string>
#include <memory>
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
};

struct Token {
  const TokenType type;
  const std::string value;

  inline Token(const TokenType& type, const std::string& value = "")
      : type(type), value(value) {}
};

class IInputProcessor {
public:
  virtual char current_char() const = 0;
  virtual void advance() = 0;
  virtual bool end_of_input() const = 0;
};

class InputProcessor : public IInputProcessor {
public:
  inline explicit InputProcessor(const std::string &input)
      : input(input), pos(0) {}

  virtual char current_char() const override;
  virtual void advance() override;
  virtual bool end_of_input() const override;

private:
  const std::string input;
  size_t pos;
};

class TokenMatcher {
public:
  virtual Token match(IInputProcessor &input_processor) const = 0;
};

class NumberMatcher : public TokenMatcher {
public:
  Token match(IInputProcessor &input_processor) const override;
};

class Lexer {
public:
  explicit Lexer(IInputProcessor &input_processor);
  std::vector<Token> tokenize();

private:
  IInputProcessor &input_processor;
  std::vector<std::unique_ptr<TokenMatcher>> matchers;
  Token get_next_token();
};
} // namespace ziv
#endif
