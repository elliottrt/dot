#include "ast.h"

const std::string LITERAL_TAG = "~literal~";

const std::string SELF_TAG = "@";
const std::string ARG_TAG = "$";

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> dot::ast::IntegerLiteral::eval(dot::ObjectRef parent) const {
	return dot::ObjectAllocator::create(parent, LITERAL_TAG, integer);
}

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> dot::ast::StringLiteral::eval(dot::ObjectRef parent) const {
	return dot::ObjectAllocator::create(parent, LITERAL_TAG, string);
}

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> dot::ast::Identifier::eval(dot::ObjectRef parent) const {
	// check if object exists
	dot::result::Result<ObjectRef , dot::error::NoSuchObjectError> exists = dot::ObjectAllocator::get_tag(parent, tag);
	
	// if it does, return it
	// no need to inc ref as we maintain the same ref as is returned
	if (exists.is_ok())
		return exists.ok();

	// else create new null
	return dot::ObjectAllocator::create(parent, tag);
}

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> dot::ast::ArrayLiteral::eval(dot::ObjectRef parent) const {
	dot::ObjectRef this_object = dot::ObjectAllocator::create(parent, LITERAL_TAG, array_type());
	array_type *this_array = this_object->as_array().ok();

	for (dot::ast::Node *child : children) {
		dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> child_result = child->eval(this_object);

		if (child_result.is_err())
			return child_result.err();

		this_array->push_back(child_result.ok());
	}

	return this_object;
}

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> function_binding(const dot::ast::FunctionDefinition *function, dot::ObjectRef self, dot::ObjectRef argument) {
	dot::ObjectRef last_object = nullptr;

	dot::ObjectAllocator::set_tag(self, "@", self);
	dot::ObjectAllocator::set_tag(self, "$", argument);

	for (const dot::ast::Node *child : function->children) {
		dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> child_result = child->eval(self);
		
		if (child_result.is_err())
			return child_result.err();

		last_object = child_result.ok();
	}

	dot::ObjectAllocator::remove_tag(self, "@");
	dot::ObjectAllocator::remove_tag(self, "$");

	return last_object;
}

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> dot::ast::FunctionDefinition::eval(dot::ObjectRef parent) const {
	// TODO: after binding a function, we should keep that binding so we don't rebind
	// TODO: couldn't we pass parent instead of placeholder 1?
	function_type this_function = std::bind(function_binding, this, parent, std::placeholders::_1);
	// TODO: is this the right parent?
	return dot::ObjectAllocator::create(parent, LITERAL_TAG, this_function);
}

dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> dot::ast::Application::eval(dot::ObjectRef parent) const {

	dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> target_result = target->eval(parent);

	if (target_result.is_err())
		return target_result.err();

	ObjectRef target_object = target_result.ok();
	ObjectRef argument_object = nullptr;

	std::cout << "applying" << std::endl;
	target->print(1);
	argument->print(1);

	// target is function
	if (target_object->get_type() == dot::object_type::function) {
		function_type *function = target_object->as_function().ok();

		// TODO: is null for parent the right thing to do here? I think so
		dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> argument_result = argument->eval(nullptr);

		if (argument_result.is_err())
			return argument_result.err();

		argument_object = argument_result.ok();

		// the self argument of a function is the owner of the function
		// so a.set would have the owner a
		dot::result::Result<ObjectRef, dot::error::RuntimeError *> result = (*function)(argument_object);

		if (result.is_ok()) 
			std::cout << "function produced " << result.ok()->to_string() << std::endl;
			
		return result;
	}

	// else try getting
	dot::result::Result<dot::ObjectRef, dot::error::RuntimeError *> result = argument->eval(target_object);

	if (result.is_ok()) 
		std::cout << "apply produced " << result.ok()->to_string() << std::endl;

	return result;
}
