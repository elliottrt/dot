#include "ast.h"

using namespace dot;

object_ptr ast::FunctionDefinition::call(object_ptr self, object_ptr arg, const location &loc) const {
	DOT_PRINTF("in %s\n", __PRETTY_FUNCTION__);
	(void) loc;

	// create a null, in case the function is empty
	object_ptr last_object = std::make_shared<object>();

	self->add_self_and_arg(self, arg);

	for (const node_ptr &child : children) {
		last_object = child->evaluate(self);
		DOT_PRINTF("*** completed function line, result = %s\n", last_object->to_string().c_str());
	}

	self->remove_self_and_arg();

	return last_object;
}

object_ptr ast::FunctionDefinition::evaluate(object_ptr parent) const {
	DOT_PRINTF("in %s\n", __PRETTY_FUNCTION__);
	(void) parent;

	auto call = std::bind(&ast::FunctionDefinition::call, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	return object::create(callwrapper<object_ptr>(call, loc));
}

object_ptr ast::ArrayLiteral::evaluate(object_ptr parent) const {
	DOT_PRINTF("in %s\n", __PRETTY_FUNCTION__);
	array_type array = array_type();

	for (const node_ptr &child : this->children) {
		array.push_back(child->evaluate(parent));
	}

	array.shrink_to_fit();
	return dot::object::create(array);
}

object_ptr ast::Application::evaluate(object_ptr parent) const {
	DOT_PRINTF("in %s\n", __PRETTY_FUNCTION__);
	// for function calls, parent will always be the function
	// that the application was called in

	object_ptr target_obj = this->target->evaluate(parent);
	object_ptr argument_obj = nullptr;

	if (target_obj->type() == dot::object_type::function) {
		function_type &function = target_obj->get_function(this->loc);

		// for anonymous functions
		if (!function.is_bound())
			function.bind_to(parent);

		argument_obj = argument->evaluate(parent);

		return function(argument_obj, this->loc);
	}

	argument_obj = argument->evaluate(target_obj);

	// array indexing by integers instead of `get`
	if (target_obj->type() == dot::object_type::array && argument_obj->type() == dot::object_type::integer) {
		array_type &array = target_obj->get_arr(loc);
		integer_type &index = argument_obj->get_int(loc);

		if (index < 0 || index >= (ssize_t) array.size())
			throw error::RangeError(loc, index, array.size() - 1);

		return array[index];
	}

	return argument_obj;
}

object_ptr ast::Identifier::evaluate(object_ptr parent) const {
	DOT_PRINTF("in %s, tag=%s, parent=%p\n", __PRETTY_FUNCTION__, tag.c_str(), (void *) parent.get());
	object_ptr obj = object::get_child_of(parent, this->tag);
	DOT_PRINTF("completed identifying %s -> %s\n", tag.c_str(), obj->to_string().c_str());
	return obj;
}

object_ptr ast::IntegerLiteral::evaluate(object_ptr parent) const {
	DOT_PRINTF("in %s, val=%s\n", __PRETTY_FUNCTION__, integer.c_str());
	(void) parent;
	integer_type value;
	try { 
		value = std::stoll(this->integer);
	} catch (...) {
		throw error::SyntaxError(this->loc, "invalid integer literal " + this->integer);
	}
	return object::create(value);
}

std::string::value_type unescaped_character(std::string::value_type ch, const location &loc) {
	switch (ch) {
		case 'n': return '\n';
		case 'r': return '\r';
		case 'b': return '\b';
		case 't': return '\t';
		case 'f': return '\f';
		case 'v': return '\v';
		case '"': 
		case '\\':
			return ch;
		default: throw error::SyntaxError(loc, std::string("invalid escape character '") + ch + std::string("'"));
	}
}

std::string unescape_string(const std::string &source, const location &loc) {
	if (source.empty()) return source;

	std::string result = source;

	size_t pos;
	while ((pos = result.find('\\')) != std::string::npos) {
		if (pos == result.size() - 1)
			throw error::SyntaxError(loc, "string ends with invalid escape character");

		std::string::value_type next = result.at(pos + 1);

		std::string::value_type replacement = unescaped_character(next, loc);
		result.replace(pos, (size_t) 2, std::string(1, replacement));
	}
	
	return result;
}

object_ptr ast::StringLiteral::evaluate(object_ptr parent) const {
	DOT_PRINTF("in %s, str=%s\n", __PRETTY_FUNCTION__, string.c_str());
	(void) parent;
	return object::create(unescape_string(this->string, this->loc));
}
