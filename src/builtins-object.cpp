#include "builtins.h"

using namespace dot;

DOT_DECLARE(dot_set) {
	(void) loc;
	dot::object::set(self, arg);
	return self;
}

DOT_DECLARE(dot_length) {
	(void) self;

	const object_type &type = arg->type();

	if (type == object_type::array)
		return dot::object::create(arg->get_arr(loc).size());
	else if (type == object_type::string)
		return dot::object::create(arg->get_str(loc).size());
	else
		throw error::TypeError(loc, "array or string", object_type_name(type));
}

DOT_DECLARE(dot_print) {
	(void) self;
	(void) loc;

	fputs(arg->to_string().c_str(), stdout);

	return arg;
}

builtins::builtin_list_type builtins::OBJECT_FUNCTIONS = {
	{"set", dot_set},
	{"length", dot_length},
	{"print", dot_print}
};
