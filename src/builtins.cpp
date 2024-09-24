#include "builtins.h"

using namespace dot;

void dot::builtins::give_functions(object_ptr object) {
#define GIVE(L) for (const auto &p : L) { object->add_system_function(p.first, callwrapper<dot::object_ptr>(p.second)); }
	switch (object->type()) {
		case object_type::function: /* functions don't have children */ return;
		case object_type::integer: GIVE(INTEGER_FUNCTIONS) break;
		case object_type::string: GIVE(STRING_FUNCTIONS) break;
		case object_type::array: GIVE(ARRAY_FUNCTIONS) break;
		default: break;
	}
	GIVE(OBJECT_FUNCTIONS)
#undef GIVE
}

const function_type::call_type *find_function(object_ptr parent, const std::string &tag) {
	const char *cstr = tag.c_str();
	builtins::builtin_list_type::const_iterator it;

#define CHECK_HAS_FUNC(FL) it = FL.find(cstr); if (it != FL.end()) return &it->second;
	switch (parent->type()) {
		case object_type::function: /* functions don't have children */ return nullptr;
		case object_type::integer: CHECK_HAS_FUNC(builtins::INTEGER_FUNCTIONS) break;
		case object_type::string: CHECK_HAS_FUNC(builtins::STRING_FUNCTIONS) break;
		case object_type::array: CHECK_HAS_FUNC(builtins::ARRAY_FUNCTIONS) break;
		default: break;
	}
	CHECK_HAS_FUNC(builtins::OBJECT_FUNCTIONS)
	CHECK_HAS_FUNC(builtins::GLOBAL_FUNCTIONS)

#undef CHECK_HAS_FUNC
	return nullptr;
}

bool builtins::has_function(object_ptr parent, const std::string &tag) {
	return find_function(parent, tag) != nullptr;
}

object_ptr builtins::call_function(object_ptr parent, object_ptr argument, const std::string &tag, const location &loc) {
	const function_type::call_type *function = find_function(parent, tag);
	return (*function)(parent, argument, loc);
}
