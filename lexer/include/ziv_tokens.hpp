#include <string>
#include <vector>

namespace ziv {
enum TokenValue {
  // Identifier
  IDENTIFIER,
  // keyWord
  FOR,
  WHILE,
  BOOL,
  TRUE,
  FALSE,
  NUMBER,
  NUMBER_TYPE,
  STRING,
  STRING_TYPE,
  FUNCTION,
  RETURN,
  // Operator
  PLUS,
  MINUS,
  STAR,
  SLASH,
  EQUALS,
  DIV,
  MOD,
  // Delimiter
  SEMICOLON,
  COMMA,
  LPAREN,
  RPAREN,
  LBRANCE,
  RBRANCE,
};

struct Token {
  TokenValue token;
  std::string value;
  Token(const TokenValue&, const std::string&);
	Token(const TokenValue&);
};

class TokenAdministrator {
private:
  std::vector<Token> values_;

public:
  TokenAdministrator();
  void add(const Token &) noexcept;
  std::vector<Token> get_tokens() const;
	void clear() noexcept;
};
} // namespace ziv
