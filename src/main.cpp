
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

	// read all of the contents of the file and tokenize it
	std::vector<token> tokens = dot::token::tokenize("test.dot", slurp("test.dot"));

	// display all tokens read from the file
	// for (const token &tok : tokens)
		// printf("%s\n", tok.to_string().c_str());

	node_ptr ast_tree = dot::ast::generate_tree(tokens);

	// print the full ast tree
	// ast_tree->print(0);

	dot::object_ptr global = dot::object::create();

	dot::object_ptr main_function_obj = ast_tree->evaluate(global);
	dot::function_type main_function = main_function_obj->get_function(ast_tree->loc);
	main_function.bind_to(global);

	//dot::object_ptr result = 
	main_function(
		dot::object::from_argv(argc, argv), // argv array
		ast_tree->loc // location of the main function
	);

	//printf("final result = %s\n", result->to_string().c_str());

	printf("process exited successfully\n");
	return EXIT_SUCCESS;
}

