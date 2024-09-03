#ifndef _DOT_TOKEN
#define _DOT_TOKEN

#include <string>
#include <vector>

#include "dot-error.h"

namespace dot {
namespace token {

	enum token_type {
		TOKEN_END = 0,
		IDENTIFIER = 'i',
		INTEGER = '0',
		STRING = '"',
		DOT = '.',
		COMMA = ',',
		OPEN_PAREN = '(',
		CLOSE_PAREN = ')',
		OPEN_CURLY_BRACKET = '{',
		CLOSE_CURLY_BRACKET = '}',
		OPEN_SQUARE_BRACKET = '[',
		CLOSE_SQUARE_BRACKET = ']',
		ARGUMENT = '$',
		SELF = '@',
		LINE_END = '\n',
	};

	struct token {
		token_type type;
		std::string text;
		size_t row, col;

		token(token_type type, const std::string &text, size_t row, size_t col): type(type), text(text), row(row), col(col) {

		}

		// TODO: token to string for error messages
	};

	result::Result<std::vector<token>, error::SyntaxError> tokenize(const std::string &source);

}
}

#endif
