#include "builtins.h"

using namespace dot;

DOT_DECLARE(dot_get) {
	array_type &array = self->get_arr(loc);
	integer_type &index = arg->get_int(loc);

	if (index < 0 || index >= (ssize_t) array.size())
		throw error::RangeError(loc, index, array.size() - 1);

	return array[index];
}

DOT_DECLARE(dot_push) {
	self->get_arr(loc).push_back(arg);

	return self;
}

DOT_DECLARE(dot_remove) {
	array_type &array = self->get_arr(loc);
	integer_type &index = arg->get_int(loc);

	if (index < 0 || index >= (ssize_t) array.size())
		throw error::RangeError(loc, index, array.size() - 1);

	object_ptr removed_value = array[index];

	array.erase(array.begin() + index);

	return removed_value;
}

builtins::builtin_list_type builtins::ARRAY_FUNCTIONS = {
	{"get", dot_get},
	{"push", dot_push},
	{"remove", dot_remove},
};
