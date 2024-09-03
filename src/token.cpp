#include "token.h"

#include <sstream>

inline size_t skip_while(std::istringstream &ss, std::function<bool(char)> predicate) {
	size_t count = 0;
	while (!ss.eof() && predicate(ss.peek())) {
		ss.get();
		++count;
	}
	return count;
}

bool isspace_(char ch) {
	return std::isspace(static_cast<unsigned char>(ch));
}

bool isalpha_(char ch) {
	return std::isalpha(static_cast<unsigned char>(ch));
}

bool isalnum_(char ch) {
	return std::isalnum(static_cast<unsigned char>(ch));
}

bool isdigit_(char ch) {
	return std::isdigit(static_cast<unsigned char>(ch));
}

bool isdigitstart_(char ch) {
	return ch == '-' || isdigit_(ch);
}

bool isidstart_(char ch) {
	return ch == '_' || isalpha_(ch);
}

bool isid_(char ch) {
	return ch == '_' || isalnum_(ch);
}

using namespace dot::token;

dot::result::Result<std::vector<token>, dot::error::SyntaxError> dot::token::tokenize(const std::string &source) {

	const size_t source_size = source.size();
	size_t token_start = 0, token_size = 0;
	size_t row = 1, col = 1, pos = 0, skipped = 0;

	std::vector<token> tokens;
	std::istringstream ss = std::istringstream(source);

	while (pos < source_size) {

		// skip whitespace
		while (!ss.eof() && isspace_(ss.peek())) {
			char ignored = ss.get();
			pos++;
			if (ignored == '\n') {
				tokens.push_back(token(token_type::LINE_END, "\n", row, col));
				row++;
				col = 1;
			} else {
				col++;
			}
		}

		char next = ss.get();
		token_size = 1;
		token_start = pos;

		switch (next) {
			case '.': tokens.push_back(token(token_type::DOT, ".", row, col)); col++; break;
			case ',': tokens.push_back(token(token_type::COMMA, ",", row, col)); col++; break;
			case '(': tokens.push_back(token(token_type::OPEN_PAREN, "(", row, col)); col++; break;
			case ')': tokens.push_back(token(token_type::CLOSE_PAREN, ")", row, col)); col++; break;
			case '{': tokens.push_back(token(token_type::OPEN_CURLY_BRACKET, "{", row, col)); col++; break;
			case '}': tokens.push_back(token(token_type::CLOSE_CURLY_BRACKET, "}", row, col)); col++; break;
			case '[': tokens.push_back(token(token_type::OPEN_SQUARE_BRACKET, "[", row, col)); col++; break;
			case ']': tokens.push_back(token(token_type::CLOSE_SQUARE_BRACKET, "]", row, col)); col++; break;
			case '$': tokens.push_back(token(token_type::ARGUMENT, "$", row, col)); col++; break;
			case '@': tokens.push_back(token(token_type::SELF, "@", row, col)); col++; break;
			case '\n': tokens.push_back(token(token_type::LINE_END, "\n", row, col)); row++; col = 1; break;
			default: {

				if (isidstart_(next)) {
					skipped = skip_while(ss, isid_);
					token_size += skipped;

					tokens.push_back(token(token_type::IDENTIFIER, source.substr(token_start, token_size), row, col));
				} else if (isdigitstart_(next)) {
					skipped = skip_while(ss, isdigit_);
					token_size += skipped;

					tokens.push_back(token(token_type::INTEGER, source.substr(token_start, token_size), row, col));
				} else if (next == '"') {
					bool next_escaped = false;
					while (!ss.eof() && (ss.peek() != '"' || next_escaped)) {
						next = ss.get();
						token_size++;
						next_escaped = next == '\\';
						if (next == '\n')
							return dot::error::SyntaxError("multiline strings are not permitted", row, col);
					}
					ss.get(); // ending "
					token_size++;
					// skip beginning and ending quotation mark
					tokens.push_back(token(token_type::STRING, source.substr(token_start + 1, token_size - 2), row, col));
				} else {
					return dot::error::SyntaxError("invalid beginning of token " + std::string(1, next), row, col);
				}

				col += token_size;
			} break;
		}

		pos += token_size;
	}

	tokens.push_back(token(token_type::TOKEN_END, "", row, col));
	tokens.shrink_to_fit();
	return tokens;
}
