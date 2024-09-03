#include "ast.h"

const std::string LITERAL_TAG = "~literal~";

const std::string SELF_TAG = "@";
const std::string ARG_TAG = "$";

// TODO: special cases $ and @

dot::Object *dot::ast::IntegerLiteral::eval(dot::Object *parent) const {
	return dot::ObjectAllocator::create(parent, LITERAL_TAG, integer);
}

dot::Object *dot::ast::StringLiteral::eval(dot::Object *parent) const {
	return dot::ObjectAllocator::create(parent, LITERAL_TAG, string);
}

dot::Object *dot::ast::Identifier::eval(dot::Object *parent) const {
	// check if object exists
	dot::result::Result<Object *, dot::error::NoSuchObjectError> exists = dot::ObjectAllocator::get_tag(parent, tag);
	
	// if it does, return it
	if (exists.is_ok())
		return exists.ok();

	// else create new null
	return dot::ObjectAllocator::create(parent, tag);
}

dot::Object *dot::ast::ArrayLiteral::eval(dot::Object *parent) const {
	dot::Object *this_object = dot::ObjectAllocator::create(parent, LITERAL_TAG, array_type());
	array_type *this_array = this_object->as_array().ok();

	for (dot::ast::Node *child : children)
		this_array->push_back(child->eval(this_object));

	return this_object;
}

dot::Object *function_binding(const dot::ast::FunctionDefinition *function, dot::Object *self, dot::Object *argument) {
	dot::Object *last_object = nullptr;

	// TODO: $ (argument) and @ (self) for functions

	for (const dot::ast::Node *child : function->children) {
		// TODO: we are ignoring the return results (which i think is the right thing to do), check this
		last_object = child->eval(self);
	}

	return last_object;
}

dot::Object *dot::ast::FunctionDefinition::eval(dot::Object *parent) const {
	// TODO: after binding a function, we should keep that binding so we don't rebind
	function_type this_function = std::bind(function_binding, this, std::placeholders::_1, std::placeholders::_2);
	return dot::ObjectAllocator::create(parent, LITERAL_TAG, this_function);
}

dot::Object *dot::ast::Application::eval(dot::Object *parent) const {
	Object *target_object = target->eval(parent);
	Object *argument_object = nullptr;

	std::cout << "applying" << std::endl;
	target->print(1);
	argument->print(1);

	// target is function
	if (target_object->get_type() == dot::object_type::function) {
		const function_type *function = target_object->as_function().ok();

		// TODO: is null for parent the right thing to do here? I think so
		argument_object = argument->eval(nullptr);

		// the self argument of a function is the creator of the function
		// so a.set would have the creator a
		return (*function)(target_object->get_creator(), argument_object);
	}

	// else try getting
	return argument->eval(target_object);
}
