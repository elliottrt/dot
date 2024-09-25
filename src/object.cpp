#include "object.h"
#include "error.h"
#include "builtins.h"
#include "token.h"

#ifndef DOT_DEBUG
void DOT_PRINTF(const char *fmt...) { (void) fmt; }
#endif

const char *dot::object_type_name(const dot::object_type &type) {
	switch (type) {
		case dot::object_type::integer: return "integer";
		case dot::object_type::string: return "string";
		case dot::object_type::array: return "array";
		case dot::object_type::function: return "function";
		default: return "none";
	}
}

void dot::object_value::value_destructor() {
	switch (type) {
		case object_type::integer: int_value.~integer_type(); break;
		case object_type::string: string_value.~string_type(); break;
		case object_type::array: array_value.~array_type(); break;
		case object_type::function: function_value.~function_type(); break;
		default: break;
	}
}

dot::object_value::~object_value() {
	value_destructor();
}

dot::object_value::object_value(const dot::object_value &other) {
	type = other.type;
	switch (type) {
		case object_type::integer: int_value = other.int_value; break;
		case object_type::string: string_value = other.string_value; break;
		case object_type::array: array_value = other.array_value; break;
		case object_type::function: function_value = other.function_value; break;
		default: break;
	}
}

dot::object_value &dot::object_value::operator=(const dot::object_value &other) {
	if (&other == this) return *this;

	value_destructor();

	type = other.type;
	switch (type) {
		case object_type::integer: int_value = other.int_value; break;
		case object_type::string: string_value = other.string_value; break;
		case object_type::array: array_value = other.array_value; break;
		case object_type::function: function_value = other.function_value; break;
		default: break;
	}
	
	return *this;
}

dot::object_ptr dot::object::create() {
	dot::object_ptr obj = std::make_shared<dot::object>();
	// dot::builtins::give_functions(obj);
	return obj;
}

dot::object_ptr dot::object::create(const integer_type &value) {
	dot::object_ptr obj = std::make_shared<dot::object>(value);
	// dot::builtins::give_functions(obj);
	return obj;
}

dot::object_ptr dot::object::create(const string_type &value) {
	dot::object_ptr obj = std::make_shared<dot::object>(value);
	// dot::builtins::give_functions(obj);
	return obj;
}

dot::object_ptr dot::object::create(const array_type &value) {
	dot::object_ptr obj = std::make_shared<dot::object>(value);
	// dot::builtins::give_functions(obj);
	return obj;
}

dot::object_ptr dot::object::create(const dot::function_type &value) {
	dot::object_ptr obj = std::make_shared<dot::object>(value);
	// dot::builtins::give_functions(obj);
	return obj;
}

dot::object_ptr dot::object::from_argv(int argc, const char **argv) {
	std::vector<dot::object_ptr> args;

	for (int i = 0; i < argc; i++)
		args.push_back(dot::object::create(argv[i]));
	args.shrink_to_fit();

	return std::make_shared<dot::object>(args);
}

dot::object_ptr dot::object::get_child_of(dot::object_ptr self, const std::string &name) {
	object_ptr function_obj = builtins::get_function(self, name);
	if (function_obj != nullptr) {
		function_obj->get_function(location("logic error in " __FILE__, __LINE__, 0)).bind_to(self);
		return function_obj;
	}

	return self->get(self, name);
}

std::string dot::object::to_string() {
	switch (value.type) {
		case object_type::integer: return std::to_string(value.int_value);
		case object_type::string: return value.string_value;
		case object_type::array: {
			std::string result;
			for (const auto &e : value.array_value) {
				result += e->to_string() + ", ";
			}
			return "[" + result.substr(0, result.size() - 2) + "]";
		}
		case object_type::function: return "<function>";
		default: return "<null>";
	}
}

const dot::object_type &dot::object::type() const { 
	return value.type; 
}

dot::integer_type &dot::object::get_int(const location &loc) {
	if (value.type == dot::object_type::integer)
		return value.int_value;
	throw dot::error::TypeError(
		loc, 
		object_type_name(dot::object_type::integer), 
		object_type_name(value.type)
	);
}

dot::string_type &dot::object::get_str(const location &loc) {
	if (value.type == dot::object_type::string)
		return value.string_value;
	throw dot::error::TypeError(
		loc, 
		object_type_name(dot::object_type::string), 
		object_type_name(value.type)
	);
}

dot::array_type &dot::object::get_arr(const location &loc) {
	if (value.type == dot::object_type::array)
		return value.array_value;
	throw dot::error::TypeError(
		loc, 
		object_type_name(dot::object_type::array), 
		object_type_name(value.type)
	);
}

dot::function_type &dot::object::get_function(const location &loc) {
	if (value.type == dot::object_type::function)
		return value.function_value;
	throw dot::error::TypeError(
		loc, 
		object_type_name(dot::object_type::function), 
		object_type_name(value.type)
	);
}

dot::object_ptr dot::object::get(dot::object_ptr self, const std::string &name) {
	auto position = child_map.find(name);

	if (position != child_map.end()) {
		DOT_PRINTF("object %s exists in parent\n", name.c_str());
		dot::object_ptr child_obj = position->second;

		if (child_obj->type() == dot::object_type::function) {
			child_obj
				->get_function(location("logic error in " __FILE__, __LINE__, 0))
				.bind_to(dot::object_ptr(self));
		}

		return child_obj;
	}

	DOT_PRINTF("%s does not exist in parent, creating it...\n", name.c_str());
	dot::object_ptr new_object = dot::object::create();
	child_map[name] = new_object;
	return new_object;
}

void dot::object::add_self_and_arg(object_ptr self, object_ptr arg) {
	child_map[std::string(1, (char) dot::token::SELF)] = self;
	child_map[std::string(1, (char) dot::token::ARGUMENT)] = arg;
}

void dot::object::remove_self_and_arg() {
	child_map.erase(std::string(1, (char) dot::token::SELF));
	child_map.erase(std::string(1, (char) dot::token::ARGUMENT));
}

void dot::object::set(dot::object_ptr dst, dot::object_ptr src) {
	dst->value = src->value;
	dst->child_map = src->child_map;
}
