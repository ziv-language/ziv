#include <zivlexer.hpp>

char ziv::InputProcessor::current_char() const {
	if (pos >= input.size()) return '\0';
	return input[pos];
}

void ziv::InputProcessor::advance() {
	pos++;
}

bool ziv::InputProcessor::end_of_input() const {
	return pos >= input.size();
}
