#ifndef _DOT_LOCATION
#define _DOT_LOCATION

#include <string>

struct location {
	const std::string &file;
	size_t row;
	size_t col;

	location(const std::string &file, const size_t &row, const size_t &col)
	: file(file), row(row), col(col) {}

	std::string to_string() const {
		return file + ":" + std::to_string(row) + ":" + std::to_string(col) + ":";
	}
};

#endif
