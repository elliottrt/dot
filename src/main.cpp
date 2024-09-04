
#include <iostream>
#include <fstream>
#include <sstream>

#include "token.h"
#include "ast.h"

#include "dot-alloc.h"

// from https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c
std::string slurp(const std::string &path) {
	std::ifstream f = std::ifstream(path);
    std::ostringstream sstr;
    sstr << f.rdbuf();
    return sstr.str();
}

int main(int argc, char const *argv[]) {

	(void) argc;
	(void) argv;

	using namespace dot::error;
	using namespace dot::result;
	using namespace dot::token;
	using namespace dot::ast;

	// read all of the contents of the file and tokenize it
	Result<std::vector<token>, SyntaxError> tokens = dot::token::tokenize(slurp("test.dot"));

	// if tokenization encounters an error, display it and exit
	if (tokens.is_err()) {
		const SyntaxError &error = tokens.err();
		fprintf(stderr, "%s:%lu:%lu: error: %s\n", "test.dot", error.row, error.col, error.what.c_str());
		return EXIT_FAILURE;
	}

	// display all tokens read from the file
	// for (token &tok : tokens.ok())
		// printf("%s:%lu:%lu: '%c' (%d): \"%s\"\n", "test.dot", tok.row, tok.col, (char) tok.type, (int) tok.type, tok.text.c_str());

	// TODO: ast tree should use gc_ptr
	Result<Node *, SyntaxError> ast_tree = generate_tree(tokens.ok());

	// if ast generation encounters an error, display it and exit
	if (ast_tree.is_err()) {
		const SyntaxError &error = ast_tree.err();
		fprintf(stderr, "%s:%lu:%lu: error: %s\n", "test.dot", error.row, error.col, error.what.c_str());
		return EXIT_FAILURE;
	}

	// print the full ast tree
	// ast_tree.ok()->Node::print();

	// TODO: make sure the gc_ptr is working

	auto main = ast_tree.ok()->eval(nullptr);

	if (main.is_err()) {
		const RuntimeError *error = main.err();
		throw error;
		return EXIT_FAILURE;
	}

	// arg is null (probably bad idea)
	auto retval = main.ok()->as_function().ok()->operator()(nullptr);

	if (retval.is_err()) {
		const RuntimeError *error = retval.err();
		throw error;
		return EXIT_FAILURE;
	}

	std::cout << retval.ok()->to_string() << std::endl;

	delete ast_tree.ok();
	std::cout << "process exited successfully" << std::endl;
	return EXIT_SUCCESS;
}

