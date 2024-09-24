#include "ast.h"

using namespace dot;
using namespace dot::ast;

void FunctionDefinition::print(const size_t &depth) const {
	std::cout << std::string(depth, '\t') << "Function Definition" << std::endl;
	for (const node_ptr &child : children)
		child->print(depth + 1);
}

void ArrayLiteral::print(const size_t &depth) const {
	std::cout << std::string(depth, '\t') << "Array Literal" << std::endl;
	for (const node_ptr &child : children)
		child->print(depth + 1);
}

void Application::print(const size_t &depth) const {
	std::cout << std::string(depth, '\t') << "Application: " << std::endl;
	target->print(depth + 1);
	argument->print(depth + 1);
}

void Identifier::print(const size_t &depth) const {
	std::cout << std::string(depth, '\t') << "Identifier: " << tag << std::endl;
}

void IntegerLiteral::print(const size_t &depth) const {
	std::cout << std::string(depth, '\t') << "Integer Literal: " << integer << std::endl;
}

void StringLiteral::print(const size_t &depth) const {
	std::cout << std::string(depth, '\t') << "String Literal: " << string << std::endl;
}
