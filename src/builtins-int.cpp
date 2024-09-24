#include "builtins.h"

using namespace dot;

DOT_DECLARE(dot_add) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	self_int += arg_int;

	return self;
}

DOT_DECLARE(dot_sub) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	self_int -= arg_int;

	return self;
}

DOT_DECLARE(dot_mul) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	self_int *= arg_int;

	return self;
}

DOT_DECLARE(dot_div) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	self_int /= arg_int;

	return self;
}

DOT_DECLARE(dot_mod) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	self_int %= arg_int;

	return self;
}

builtins::builtin_list_type builtins::INTEGER_FUNCTIONS = {
	{"add", dot_add},
	{"sub", dot_sub},
	{"mul", dot_mul},
	{"div", dot_div},
	{"mod", dot_mod}
};
