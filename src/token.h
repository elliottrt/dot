#ifndef _DOT_TOKEN
#define _DOT_TOKEN

#include <string>
#include <vector>

#include "location.h"

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
		location loc;

		token(const location &loc, token_type type, const std::string &text)
		: type(type), text(text), loc(loc) {}

		// TODO: token to string for error messages
	};

	using ptr_t = const token *;
	using stack_t = std::vector<ptr_t>;
	using group_t = std::vector<ptr_t>;

	std::vector<token> tokenize(const std::string &file, const std::string &source);

}
}

#endif
