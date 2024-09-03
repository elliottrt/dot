#ifndef _DOT_ERROR
#define _DOT_ERROR

#include <exception>

#include "dot-defs.h"

namespace dot {
namespace error {

	class Error : public std::runtime_error {
	public:
		Error(const std::string &what)
			: std::runtime_error(what) {}
	};

	class TypeError : Error {
	public:
		dot::object_type expected;
		dot::object_type received;

		TypeError(const dot::object_type &expected, const dot::object_type &received)
			: Error("type error"), expected(expected), received(received) {}
		TypeError(const TypeError &other): TypeError(other.expected, other.received) {}
		TypeError &operator=(const TypeError &other) {
			expected = other.expected;
			received = other.received;
			return *this;
		}
	};

	class NoSuchObjectError : Error {
	public:
		std::string name;

		NoSuchObjectError(const std::string &name)
			: Error("no such object error"), name(name) {}
		NoSuchObjectError(const NoSuchObjectError &other): NoSuchObjectError(other.name) {}
		NoSuchObjectError &operator=(const NoSuchObjectError &other) {
			name = other.name;
			return *this;
		}
	};

	class SyntaxError : Error {
	public:
		std::string what;
		size_t row;
		size_t col;

		SyntaxError(const std::string &what, size_t row, size_t col)
			: Error("syntax error"), what(what), row(row), col(col) {}

		SyntaxError(const SyntaxError &other): SyntaxError(other.what, other.row, other.col) {}

		static SyntaxError LogicError(const char *file, int line) {
			return SyntaxError(std::string(file), line, 0);
		}

		SyntaxError &operator=(const SyntaxError &other) {
			what = other.what;
			row = other.row;
			col = other.col;
			return *this;
		}
	};

}
}

#endif
