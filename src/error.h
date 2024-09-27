#ifndef _DOT_ERROR
#define _DOT_ERROR

#include <exception>
#include <string>

#include "location.h"

#include <iostream>

namespace dot {
namespace error {

	struct Error : public std::exception {
		const std::string info;
		Error(const location &loc, const std::string &error_type, const std::string &message = "")
			: info(loc.to_string() + ": " + error_type + ": " + message) {}
	};

	struct RangeError : public Error {
		RangeError(const location &loc, const ssize_t &requested, const ssize_t &max)
			: Error(loc, "range error", std::to_string(requested) + " out of bounds, max is " + std::to_string(max)) {}
	};

	struct SyntaxError : public Error {
		SyntaxError(const location &loc, const std::string &description)
			: Error(loc, "syntax error", description) {}

		static SyntaxError LogicError(const char *file, int line) {
			return SyntaxError(location(file, line, -1), "logic error");
		}
	};

	struct TypeError : public Error {
		TypeError(const location &loc, const std::string &requested, const std::string &actual)
			: Error(loc, "type error", "cannot get " + requested + " from " + actual) {}
	};

	struct BindError : public Error {
		BindError(const location &loc)
			: Error(loc, "bind error", "function was not bound to object") {}
	};

	struct DivisonByZeroError : public Error {
		DivisonByZeroError(const location &loc)
			: Error(loc, "division by zero error", "divison by zero") {}
	};

}
}

#endif
