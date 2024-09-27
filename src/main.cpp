
#include <fstream>
#include <sstream>

#include "token.h"
#include "ast.h"
#include "error.h"

#include "object.h"

// from https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c
std::string slurp(const std::string &path) {
	std::ifstream f = std::ifstream(path);
    std::ostringstream sstr;
    sstr << f.rdbuf();
    return sstr.str();
}

int main(int argc, char const *argv[]) {

	using namespace dot::token;
	using namespace dot::error;
	using namespace dot::ast;

	if (argc != 2) {
		printf("Usage:\n%s <filepath>\n", argv[0]);
		return EXIT_FAILURE;
	}

	const std::string filepath = std::string(argv[1]);

	// read all of the contents of the file and tokenize it
	std::vector<token> tokens = dot::token::tokenize(filepath, slurp(filepath));

	// display all tokens read from the file
	// for (const token &tok : tokens)
		// printf("%s\n", tok.to_string().c_str());

	
	try {
		node_ptr ast_tree = dot::ast::generate_tree(tokens);

		// print the full ast tree
		// ast_tree->print(0);

		dot::object_ptr global = dot::object::create();

		dot::object_ptr main_function_obj = ast_tree->evaluate(global);
		dot::function_type main_function = main_function_obj->get_function(ast_tree->loc);
		main_function.bind_to(global);

		main_function(
		dot::object::from_argv(argc, argv), // argv array
		ast_tree->loc // location of the main function
	);
	} catch (dot::error::Error e) {
		printf("%s\n", e.info.c_str());
	}

	//printf("final result = %s\n", result->to_string().c_str());

	return EXIT_SUCCESS;
}

