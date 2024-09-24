#include "builtins.h"

using namespace dot;

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

builtins::builtin_list_type builtins::GLOBAL_FUNCTIONS = {
	{"length", dot_length}
};
