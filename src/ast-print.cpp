#include "ast.h"

using namespace dot;
using namespace dot::ast;

const int INDENT_WIDTH = 2;

void FunctionDefinition::print(const size_t &depth) const {
	printf("%*sFunction Definition\n", INDENT_WIDTH * (int) depth, ""); 
	for (const node_ptr &child : children)
		child->print(depth + 1);
}

void ArrayLiteral::print(const size_t &depth) const {
	printf("%*sArray Literal\n", INDENT_WIDTH * (int) depth, ""); 
	for (const node_ptr &child : children)
		child->print(depth + 1);
}

void Application::print(const size_t &depth) const {
	printf("%*sApplication\n", INDENT_WIDTH * (int) depth, ""); 
	target->print(depth + 1);
	argument->print(depth + 1);
}

void Identifier::print(const size_t &depth) const {
	printf("%*sIdentifier: %s\n", INDENT_WIDTH * (int) depth, "", tag.c_str()); 
}

void IntegerLiteral::print(const size_t &depth) const {
	printf("%*sInteger Literal: %s\n", INDENT_WIDTH * (int) depth, "", integer.c_str()); 
}

void StringLiteral::print(const size_t &depth) const {
	printf("%*sString Literal: %s\n", INDENT_WIDTH * (int) depth, "", string.c_str()); 
}
