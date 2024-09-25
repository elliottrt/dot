#include "builtins.h"

using namespace dot;

const function_type::call_type *find_function(object_ptr caller, const std::string &tag) {
	builtins::builtin_list_type::const_iterator it;

#define CHECK_HAS_FUNC(FL) it = FL.find(tag); if (it != FL.end()) return &it->second;
	switch (caller->type()) {
		case object_type::function: /* functions don't have children */ return nullptr;
		case object_type::integer: CHECK_HAS_FUNC(builtins::INTEGER_FUNCTIONS) break;
		case object_type::string: CHECK_HAS_FUNC(builtins::STRING_FUNCTIONS) break;
		case object_type::array: CHECK_HAS_FUNC(builtins::ARRAY_FUNCTIONS) break;
		default: break;
	}
	CHECK_HAS_FUNC(builtins::OBJECT_FUNCTIONS)

#undef CHECK_HAS_FUNC
	return nullptr;
}

object_ptr builtins::get_function(object_ptr caller, const std::string &tag) {
	const function_type::call_type *function = find_function(caller, tag);

	return function != nullptr ?
		dot::object::create(function_type(*function)) :
		nullptr;
}
