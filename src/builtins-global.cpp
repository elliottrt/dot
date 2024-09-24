#include "builtins.h"

using namespace dot;

#define GIVE(L) for (const auto &p : L) { object->add_system_function(p.first, callwrapper<dot::object_ptr>(p.second)); }

	void dot::builtins::give_functions(object_ptr object) {
		switch (object->type()) {
			case object_type::function: /* functions don't have children */ return;
			case object_type::integer: GIVE(INTEGER_FUNCTIONS) break;
			case object_type::string: GIVE(STRING_FUNCTIONS) break;
			case object_type::array: GIVE(ARRAY_FUNCTIONS) break;
			default: break;
		}
		GIVE(OBJECT_FUNCTIONS)
	}
#undef GIVE

builtins::builtin_list_type builtins::GLOBAL_FUNCTIONS = {};
