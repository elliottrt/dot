#include "ast.h"

using namespace dot;

object_ptr ast::FunctionDefinition::call(object_ptr self, object_ptr arg, const location &loc) const {printf("in %s\n", __PRETTY_FUNCTION__);
	(void) loc;

	object_ptr last_object = nullptr;

	self->add_self_and_arg(self, arg);

	for (const node_ptr &child : children) {
		last_object = child->evaluate(self);
		std::cout << "completed function line, result = " << last_object->to_string() << std::endl;
	}

	self->remove_self_and_arg();

	return last_object;
}

object_ptr ast::FunctionDefinition::evaluate(object_ptr parent) const {printf("in %s\n", __PRETTY_FUNCTION__);
	(void) parent;

	auto call = std::bind(&ast::FunctionDefinition::call, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	return object::create(callwrapper<object_ptr>(call));
}

object_ptr ast::ArrayLiteral::evaluate(object_ptr parent) const {printf("in %s\n", __PRETTY_FUNCTION__);
	array_type array = array_type();

	for (const node_ptr &child : this->children) {
		array.push_back(child->evaluate(parent));
	}

	array.shrink_to_fit();
	return dot::object::create(array);
}

object_ptr ast::Application::evaluate(object_ptr parent) const {printf("in %s\n", __PRETTY_FUNCTION__);
	// for function calls, parent will always be the function
	// that the application was called in

	object_ptr target_obj = this->target->evaluate(parent);
	object_ptr argument_obj = nullptr;

	std::cout << "applying" << std::endl;
	std::cout << '\t' << target_obj->to_string() << std::endl;
	argument->print(1);

	if (target_obj->type() == dot::object_type::function) {
		function_type &function = target_obj->get_function(this->loc);

		// for anonymous functions
		if (!function.is_bound())
			function.bind_to(parent);

		argument_obj = argument->evaluate(parent);

		return function(argument_obj, this->loc);
	}

	// TODO: often, this being an integer/string/array makes
	// no sense, and should be an error. do that.
	return argument->evaluate(target_obj);
}

object_ptr ast::Identifier::evaluate(object_ptr parent) const {printf("in %s, tag=%s, parent=%p\n", __PRETTY_FUNCTION__, tag.c_str(), (void *) parent.get());
	object_ptr obj = object::get_child_of(parent, this->tag);
	printf("completed identifying %s -> %s\n", tag.c_str(), obj->to_string().c_str());
	return obj;
}

object_ptr ast::IntegerLiteral::evaluate(object_ptr parent) const {printf("in %s, val=%s\n", __PRETTY_FUNCTION__, integer.c_str());
	(void) parent;
	// TODO: array indexing?
	integer_type value;
	try { 
		value = std::stoll(this->integer);
	} catch (...) {
		throw error::SyntaxError(this->loc, "invalid integer literal " + this->integer);
	}
	return object::create(value);
}

object_ptr ast::StringLiteral::evaluate(object_ptr parent) const {printf("in %s, str=%s\n", __PRETTY_FUNCTION__, string.c_str());
	(void) parent;
	return object::create(this->string);
}
