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

	if (arg_int == 0)
		throw error::DivisonByZeroError(loc);

	self_int /= arg_int;

	return self;
}

DOT_DECLARE(dot_mod) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	if (arg_int == 0)
		throw error::DivisonByZeroError(loc);

	self_int %= arg_int;

	return self;
}

DOT_DECLARE(dot_if) {
	integer_type &cond = self->get_int(loc);
	function_type &then = arg->get_function(loc);

	bool is_true = cond != 0;

	if (is_true) {
		object_ptr then_self = std::make_shared<object>();
		object_ptr then_arg = std::make_shared<object>();

		then.bind_to(then_self);
		then(then_arg, loc);
	}

	return self;
}

DOT_DECLARE(dot_else) {
	integer_type &cond = self->get_int(loc);
	function_type &then = arg->get_function(loc);

	bool is_true = cond != 0;

	if (is_true == false) {
		object_ptr then_self = std::make_shared<object>();
		object_ptr then_arg = std::make_shared<object>();

		then.bind_to(then_self);
		then(then_arg, loc);
	}

	return self;
}

DOT_DECLARE(dot_while) {
	integer_type &cond = self->get_int(loc);
	function_type &then = arg->get_function(loc);

	// because self is considered the parent here, changing value in the loop
	// changes self.value
	object_ptr then_arg = std::make_shared<object>();

	// note: cond is updated because it's a reference, so basically a pointer
	while (cond != 0) {
		then.bind_to(self);
		then(then_arg, loc);
	}

	return self;
}

DOT_DECLARE(dot_bool_not) {
	(void) self;

	integer_type &value = arg->get_int(loc);

	return dot::object::create(!value);
}

DOT_DECLARE(dot_bool_and) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	return dot::object::create(self_int && arg_int);
}

DOT_DECLARE(dot_bool_or) {
	integer_type &self_int = self->get_int(loc);
	integer_type &arg_int = arg->get_int(loc);

	return dot::object::create(self_int || arg_int);
}

builtins::builtin_list_type builtins::INTEGER_FUNCTIONS = {
	{"add", dot_add},
	{"sub", dot_sub},
	{"mul", dot_mul},
	{"div", dot_div},
	{"mod", dot_mod},
	{"if", dot_if},
	{"else", dot_else},
	{"while", dot_while},
	{"not", dot_bool_not},
	{"and", dot_bool_and},
	{"or", dot_bool_or}
};
