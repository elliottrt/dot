#include "builtins.h"

using namespace dot;

DOT_DECLARE(set) {
	(void) loc;
	dot::object::set(self, arg);
	return self;
}

builtins::builtin_list_type builtins::OBJECT_FUNCTIONS = {
	{"set", set}
};
