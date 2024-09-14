%{
#include <string>
#include <ziv_tokens.hpp>
#include <utility>
ziv::TokenAdministrator* tokens;
%}

%%
"for"       {tokens->add(ziv::Token(ziv::FOR));}
"while"     {tokens->add(ziv::Token(ziv::WHILE));}
"num"       {tokens->add(ziv::Token(ziv ::NUMBER_TYPE));}
"bool"      {tokens->add(ziv::Token(ziv::BOOL));}
"str"       {tokens->add(ziv::Token(ziv::STRING_TYPE));}
fn       {tokens->add(ziv::Token(ziv::FUNCTION));}
return    {tokens->add(ziv::Token(ziv::RETURN));}
true      {tokens->add(ziv::Token(ziv::TRUE));}
false     {tokens->add(ziv::Token(ziv::FALSE));}
\/\/        {tokens->add(ziv::Token(ziv::DIV));}
%         {tokens->add(ziv::Token(ziv::MOD));}
"+"         {tokens->add(ziv::Token(ziv::PLUS));}
"-"         {tokens->add(ziv::Token(ziv::MINUS));}

[0-9]+ {
		tokens->add(ziv::Token(ziv::NUMBER, std::string(yytext)));
}

[a-zA-Z_][a-zA-Z0-9_]* {
	tokens->add(ziv::Token(ziv::IDENTIFIER, std::string(yytext)));
}

\"[^\"]* {
	tokens->add(ziv::Token(ziv::STRING, std::string(yytext)));
}
%%


int yywrap() {
	return 1;
}

namespace ziv {

std::vector<ziv::Token> analyze_string(const std::string& input) {
	tokens = new TokenAdministrator();

	YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
	yylex();

	yy_delete_buffer(buffer);
	auto solution = tokens->get_tokens();
	delete tokens; 
	return solution; 
}
}
