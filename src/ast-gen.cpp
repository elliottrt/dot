#include "ast.h"

using namespace dot;
using namespace dot::ast;
using namespace dot::error;

node_ptr generate_forward(const std::vector<token::group_t> &tokens);
node_ptr generate_application(const std::vector<token::group_t> &tokens);
node_ptr generate_function(const token::group_t &tokens, const location &beginLoc);

std::vector<token::group_t> generate_groups(const token::group_t &tokens, token::token_type delim = token::token_type::DOT) {
	// stack of (), [], {}
	token::stack_t stack;
	// groups generated by this function
	std::vector<token::group_t> groups;

	// start of group
	auto group_start = tokens.cbegin();
	// current position
	auto iter = tokens.cbegin();

	// for each token
	for (; iter != tokens.cend(); ++iter) {
		token::ptr_t tok = *iter;

		// if it's a DELIM and the stack is empty then add it to the pile
		if (tok->type == delim && stack.empty()) {
			groups.push_back(token::group_t(group_start, iter));
			group_start = iter + 1;
		// otherwise do stack operations or nothing
		} else switch (tok->type) {
			case token::token_type::OPEN_PAREN:
			case token::token_type::OPEN_CURLY_BRACKET:
			case token::token_type::OPEN_SQUARE_BRACKET:
				stack.push_back(tok);
				break;
			case token::token_type::CLOSE_PAREN:
				if (stack.empty())
					throw SyntaxError(tok->loc, "unexpected " + tok->text);
				else if (stack.back()->type == token::token_type::OPEN_PAREN)
					stack.pop_back();
				break;
			case token::token_type::CLOSE_CURLY_BRACKET:
				if (stack.empty())
					throw SyntaxError(tok->loc, "unexpected " + tok->text);
				else if (stack.back()->type == token::token_type::OPEN_CURLY_BRACKET)
					stack.pop_back();
				break;
			case token::token_type::CLOSE_SQUARE_BRACKET:
				if (stack.empty())
					throw SyntaxError(tok->loc, "unexpected " + tok->text);
				else if (stack.back()->type == token::token_type::OPEN_SQUARE_BRACKET)
					stack.pop_back();
				break;
			default:
				break;
		}
	
	}

	// add last group
	groups.push_back(token::group_t(group_start, iter));

	// if the stack isn't empty then 1 or more things weren't closed
	if (!stack.empty()) {
		token::ptr_t last = stack.back();
		throw SyntaxError(last->loc, "unclosed " + last->text);
	}

	return groups;
}

node_ptr generate_trivial(token::ptr_t token) {

	switch (token->type) {
		case token::token_type::IDENTIFIER:
		case token::token_type::ARGUMENT:
		case token::token_type::SELF:
			return std::make_shared<Identifier>(token->loc, token->text);
		case token::token_type::STRING:
			// TODO: unescape the text
			return std::make_shared<StringLiteral>(token->loc, token->text);
		case token::token_type::INTEGER:
			return std::make_shared<IntegerLiteral>(token->loc, token->text);
		default:
			throw SyntaxError(token->loc, "unexpected token '" + token->text + "'");
	}

}

node_ptr generate_array(const token::group_t &tokens, const location &beginLoc) {
	// an empty list []
	if (tokens.size() == 0)
		return std::make_shared<ArrayLiteral>(beginLoc, std::vector<node_ptr>());

	std::vector<token::group_t> groups = generate_groups(tokens, token::token_type::COMMA);

	std::vector<node_ptr> children;
	for (const token::group_t &group : groups) {
		node_ptr child = generate_forward(std::vector<token::group_t>{group});
		children.push_back(child);
	}

	return std::make_shared<ArrayLiteral>(beginLoc, children);
}

node_ptr generate_bracketed(const token::group_t &tokens) {

	// can be: (...) ()[] [...] []{} {...} {}{} {
	//	...
	// }
	// find the matching closer for the first token, and 
	// if there's elements left
	//	we error
	// else 
	//	if [] -> return generate_array(...);
	// 	if {} -> return generate_function(...);
	// 	if () -> return generate_forward(...);

	// how do we know what an array is?
	// array if: front=[, back=], front matches back -> generate_array() without front, back
	// function if front={, back=}, front matches back -> generate_tree() without front, back
	// application if front=(, back=), front matches back -> generate_application() without front, back

	if (tokens.empty())
		throw SyntaxError::LogicError(__FILE__, __LINE__);

	// token stack for () {} []
	token::stack_t stack = { tokens[0] };

	token::token_type open_type = tokens[0]->type;
	token::token_type close_type;

	// find the open and close types
	if (open_type == token::token_type::OPEN_PAREN)
		close_type = token::token_type::CLOSE_PAREN;
	else if (open_type == token::token_type::OPEN_CURLY_BRACKET)
		close_type = token::token_type::CLOSE_CURLY_BRACKET;
	else if (open_type == token::token_type::OPEN_SQUARE_BRACKET)
		close_type = token::token_type::CLOSE_SQUARE_BRACKET;
	else
		throw SyntaxError::LogicError(__FILE__, __LINE__);

	// iterator
	auto iter = tokens.cbegin() + 1;
	for (; iter != tokens.cend(); ++iter) {
		if (stack.empty())
			break;
		
		// if open type then push to stack
		if ((*iter)->type == open_type)
			stack.push_back(*iter);

		// if close type then try popping from stack
		else if ((*iter)->type == close_type) {
			if (stack.empty())
				throw SyntaxError((*iter)->loc, "unexpected " + (*iter)->text);
			else if (stack.back()->type == open_type)
				stack.pop_back();
		}
	}

	// tokens left in stack at the end of parsing
	if (!stack.empty()) {
		token::ptr_t last = stack.back();
		throw SyntaxError(last->loc, "unexpected " + last->text);
	}
	
	// exited bracketed expression early
	if (iter != tokens.cend())
		throw SyntaxError((*iter)->loc, "unexpected " + (*iter)->text);

	// remove front and back
	token::group_t trimmed_group = token::group_t(tokens.cbegin() + 1, tokens.cend() - 1);

	std::cout << "length of trimmed group = " << trimmed_group.size() << std::endl;
	for (const auto i : trimmed_group)
		std::cout << "\t" << i->text << std::endl;

	switch (open_type) {
		case token::token_type::OPEN_PAREN: return generate_forward(generate_groups(trimmed_group));
		case token::token_type::OPEN_CURLY_BRACKET: return generate_function(trimmed_group, tokens[0]->loc);
		case token::token_type::OPEN_SQUARE_BRACKET: return generate_array(trimmed_group, tokens[0]->loc);
		default: throw SyntaxError::LogicError(__FILE__, __LINE__);
	}
}

node_ptr generate_forward(const std::vector<token::group_t> &tokens) {

	//printf("forwarding: group count: %lu\n", tokens.size());

	if (tokens.size() == 0)
		throw SyntaxError::LogicError(__FILE__, __LINE__);

	if (tokens.size() == 1) {
		const token::group_t &group = tokens.front();
		//printf("\tsingle group; size = %lu\n", group.size());

		if (group.size() == 1)
			return generate_trivial(group.front());
		else 
			return generate_bracketed(group);
	}

	return generate_application(tokens);
}

node_ptr generate_application(const std::vector<token::group_t> &tokens) {

	if (tokens.size() < 2)
		throw SyntaxError::LogicError(__FILE__, __LINE__);

	std::vector<token::group_t> target = std::vector<token::group_t>(tokens.cbegin(), tokens.cend() - 1);
	std::vector<token::group_t> argument = { tokens.back() };

	node_ptr target_node = generate_forward(target);
	node_ptr argument_node = generate_forward(argument);

	return std::make_shared<Application>(
		target.front().front()->loc,
		target_node,
		argument_node
	);
}

node_ptr generate_function(const token::group_t &tokens, const location &beginLoc) {
	// empty function {}
	if (tokens.size() == 0) 
		return std::make_shared<FunctionDefinition>(beginLoc, std::vector<node_ptr>());

	// elements of the root
	std::vector<node_ptr> children;
	// function declaration stack
	token::stack_t stack; 
	// current group
	token::group_t child_group;

	std::cout << "length of func tokens = " << tokens.size() << std::endl;
	for (const auto i : tokens)
		std::cout << "\t" << i->text << std::endl;

	// groups are separated by token::token_type::LINE_END unless within function

	for (auto iter = tokens.cbegin(); iter != tokens.cend(); ++iter) {
		// if we've reached the end of a line not in a function
		if (((*iter)->type == token::token_type::LINE_END || (*iter)->type == token::token_type::TOKEN_END) && stack.empty()) {
			if (child_group.size() > 0) {
				std::cout << "length of child tokens = " << child_group.size() << std::endl;
				for (const auto i : child_group)
					std::cout << "\t" << i->text << std::endl;
				children.push_back(generate_forward(generate_groups(child_group)));
				child_group.clear();
			}
		} else if ((*iter)->type == token::OPEN_CURLY_BRACKET) {
			stack.push_back(*iter);
			child_group.push_back(*iter);
		} else if ((*iter)->type == token::CLOSE_CURLY_BRACKET) {
			if (stack.empty()) throw SyntaxError((*iter)->loc, "unexpected }");
	
			stack.pop_back();
			child_group.push_back(*iter);
		} else {
			std::cout << "pushed " << (*iter)->text << " to child group" << std::endl;
			child_group.push_back(*iter);
		}
	}

	if (!stack.empty()) {
		token::ptr_t last = stack.back();
		throw SyntaxError(last->loc, "unclosed {");
	}

	if (child_group.size() > 0) {
		std::cout << "length of LAST child group = " << child_group.size() << std::endl;
		for (const auto i : child_group)
			std::cout << "\t" << i->text << std::endl;
		children.push_back(generate_forward(generate_groups(child_group)));
	}

	std::cout << "length of children = " << children.size() << std::endl;
	for (const auto &i : children)
		i->print(1);
	return std::make_shared<FunctionDefinition>(beginLoc, children);
}

node_ptr dot::ast::generate_tree(const std::vector<token::token> &_tokens) {
	token::group_t tokens = token::group_t(_tokens.size(), nullptr);

	for (size_t i = 0; i < tokens.size(); ++i)
		tokens[i] = &_tokens[i];

	// TODO: will this error if _tokens is empty?
	return generate_function(tokens, tokens[0]->loc);
}
