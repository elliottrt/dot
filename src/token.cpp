#include "token.h"
#include "error.h"

#include <sstream>
#include <functional>

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

std::vector<token> dot::token::tokenize(const std::string &file, const std::string &source) {

	const size_t source_size = source.size();
	size_t token_start = 0, token_size = 0;
	size_t pos = 0, skipped = 0;

	location loc = location(file, 1, 1);

	std::vector<token> tokens;
	std::istringstream ss = std::istringstream(source);

	while (pos < source_size) {

		// skip whitespace
		while (!ss.eof() && isspace_(ss.peek())) {
			char ignored = ss.get();
			pos++;
			if (ignored == '\n') {
				tokens.push_back(token(loc, token_type::LINE_END, "\n"));
				loc.row += 1;
				loc.col = 1;
			} else {
				loc.col += 1;
			}
		}
		if (ss.eof()) break;

		char next = ss.get();
		token_size = 1;
		token_start = pos;

		switch (next) {
			case '.': tokens.push_back(token(loc, token_type::DOT, ".")); loc.col++; break;
			case ',': tokens.push_back(token(loc, token_type::COMMA, ",")); loc.col++; break;
			case '(': tokens.push_back(token(loc, token_type::OPEN_PAREN, "(")); loc.col++; break;
			case ')': tokens.push_back(token(loc, token_type::CLOSE_PAREN, ")")); loc.col++; break;
			case '{': tokens.push_back(token(loc, token_type::OPEN_CURLY_BRACKET, "{")); loc.col++; break;
			case '}': tokens.push_back(token(loc, token_type::CLOSE_CURLY_BRACKET, "}")); loc.col++; break;
			case '[': tokens.push_back(token(loc, token_type::OPEN_SQUARE_BRACKET, "[")); loc.col++; break;
			case ']': tokens.push_back(token(loc, token_type::CLOSE_SQUARE_BRACKET, "]")); loc.col++; break;
			case '$': tokens.push_back(token(loc, token_type::ARGUMENT, "$")); loc.col++; break;
			case '@': tokens.push_back(token(loc, token_type::SELF, "@")); loc.col++; break;
			case '\n': tokens.push_back(token(loc, token_type::LINE_END, "\n")); loc.row++; loc.col = 1; break;
			default: {

				if (isidstart_(next)) {
					skipped = skip_while(ss, isid_);
					token_size += skipped;

					tokens.push_back(token(loc, token_type::IDENTIFIER, source.substr(token_start, token_size)));
				} else if (isdigitstart_(next)) {
					skipped = skip_while(ss, isdigit_);
					token_size += skipped;

					tokens.push_back(token(loc, token_type::INTEGER, source.substr(token_start, token_size)));
				} else if (next == '"') {
					bool next_escaped = false;
					while (!ss.eof() && (ss.peek() != '"' || next_escaped)) {
						next = ss.get();
						token_size++;
						next_escaped = next == '\\';
						if (next == '\n')
							throw dot::error::SyntaxError(loc, "multiline strings are not permitted");
					}
					ss.get(); // ending "
					token_size++;
					// skip beginning and ending quotation mark
					tokens.push_back(token(loc, token_type::STRING, source.substr(token_start + 1, token_size - 2)));
				} else {
					throw dot::error::SyntaxError(loc, "invalid beginning of token " + std::string(1, next));
				}

				loc.col += token_size;
			} break;
		}

		pos += token_size;
	}

	tokens.push_back(token(loc, token_type::END, ""));
	tokens.shrink_to_fit();
	return tokens;
}

std::string dot::token::token::to_string() const {
	static const char *TOKEN_NAMES[] = {
		"end of input",
		"identifier",
		"integer literal",
		"string literal",
		"dot",
		"comma",
		"open parenthesis",
		"close parenthesis",
		"open brace",
		"close brace",
		"open bracket",
		"close bracket",
		"argument",
		"self",
		"line end"
	};

	return std::string(TOKEN_NAMES[(size_t) type]) + " at " + loc.to_string();
}
