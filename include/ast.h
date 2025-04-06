#ifndef _AST
#define _AST

#include "error.h"
#include "token.h"
#include "result.h"
#include "object.h"

#include <vector>

namespace dot {
namespace ast {

	// generic node type
	struct Node {
		const location &loc;

		Node(const location &loc): loc(loc) {}

		virtual ~Node() = default;

		virtual void print(const size_t &depth) const = 0;

		virtual object_ptr evaluate(object_ptr parent) const = 0;
	};

	typedef std::shared_ptr<Node> node_ptr;

	// a group (chain) of nodes
	struct Group : public Node {
		std::vector<node_ptr> children;

		Group(const location &loc, const std::vector<node_ptr> &children): Node(loc), children(children) {}
	};

	// function definition
	struct FunctionDefinition : public Group { 
		FunctionDefinition(const location &loc, const std::vector<node_ptr> &children): Group(loc, children) {}

		void print(const size_t &depth) const override;

		object_ptr call(object_ptr self, object_ptr arg, const location &loc) const;
		object_ptr evaluate(object_ptr parent) const override;
	};

	// array literal
	struct ArrayLiteral : public Group {
		ArrayLiteral(const location &loc, const std::vector<node_ptr> &children): Group(loc, children) {}
		
		void print(const size_t &depth) const override;

		object_ptr evaluate(object_ptr parent) const override;
	};

	// (a).(b)
	struct Application : public Node {
		node_ptr target;
		node_ptr argument;

		Application(const location &loc, node_ptr target, node_ptr argument): Node(loc), target(target), argument(argument) {}

		void print(const size_t &depth) const override;

		object_ptr evaluate(object_ptr parent) const override;
	};

	// name of variable or function
	struct Identifier : public Node {
		const std::string tag;

		Identifier(const location &loc, const std::string &tag): Node(loc), tag(tag) {}

		void print(const size_t &depth) const override;

		object_ptr evaluate(object_ptr parent) const override;
	};

	// integer literal
	struct IntegerLiteral : public Node {
		const std::string integer;

		IntegerLiteral(const location &loc, const std::string &integer): Node(loc), integer(integer) {}

		void print(const size_t &depth) const override;

		object_ptr evaluate(object_ptr parent) const override;
	};

	// string literal
	struct StringLiteral : public Node {
		const std::string string;

		StringLiteral(const location &loc, const std::string &string): Node(loc), string(string) {}

		void print(const size_t &depth) const override;

		object_ptr evaluate(object_ptr parent) const override;
	};

	node_ptr generate_tree(const std::vector<token::token> &tokens);

}
}

#endif
