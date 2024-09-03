#ifndef _DOT_AST
#define _DOT_AST

#include <vector>
#include <iostream>

#include "dot-object.h"
#include "dot-alloc.h"

#include "token.h"

namespace dot {
namespace ast {

	// generic node type
	struct Node {

		virtual ~Node() = default;
	
		void print() const { print(0); }
		virtual void print(const size_t &depth) const = 0;

		virtual Object *eval(Object *parent) const = 0;
	};

	struct None : public Node {
		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "None" << std::endl;
		}
	};

	// a group (chain) of nodes
	struct Group : public Node {

		std::vector<Node *> children;

		Group(const std::vector<Node *> &children): children(children) {}

		~Group() override {
			for (Node *child : children) 
				delete child;
		}

	};

	// function definition
	struct FunctionDefinition : public Group { 

		FunctionDefinition(const std::vector<Node *> &children): Group(children) {}

		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "Function Definition" << std::endl;
			for (const Node *child : children)
				child->print(depth + 1);
		}

		Object *eval(Object *parent) const override;
	};

	// array literal
	struct ArrayLiteral : public Group {

		ArrayLiteral(const std::vector<Node *> &children): Group(children) {}
		
		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "Array Literal" << std::endl;
			for (const Node *child : children)
				child->print(depth + 1);
		}

		Object *eval(Object *parent) const override;
	};

	// (a).(b)
	struct Application : public Node {

		Node *target;
		Node *argument;

		Application(Node *target, Node *argument): target(target), argument(argument) {}

		~Application() override {
			delete target;
			delete argument;
		}

		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "Application: " << std::endl;
			target->print(depth + 1);
			argument->print(depth + 1);
		}

		Object *eval(Object *parent) const override;
	};

	// name of variable or function
	struct Identifier : public Node {

		const std::string tag;

		Identifier(const std::string &tag): tag(tag) {}

		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "Identifier: " << tag << std::endl;
		}

		Object *eval(Object *parent) const override;
	};

	// integer literal
	struct IntegerLiteral : public Node {

		const integer_type integer;

		IntegerLiteral(const integer_type &integer): integer(integer) {}

		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "Integer Literal: " << integer << std::endl;
		}

		Object *eval(Object *parent) const override;
	};

	// string literal
	struct StringLiteral : public Node {

		const string_type string;

		StringLiteral(const string_type &string): string(string) {}

		void print(const size_t &depth) const override {
			std::cout << std::string(depth, '\t') << "String Literal: " << string << std::endl;
		}

		Object *eval(Object *parent) const override;
	};

	result::Result<Node *, error::SyntaxError> generate_tree(const std::vector<token::token> &tokens);

}
}

#endif
