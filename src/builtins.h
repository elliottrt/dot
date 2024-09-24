#ifndef _DOT_BUILTINS
#define _DOT_BUILTINS

#include "object.h"

#include <unordered_map>

namespace dot {
namespace builtins {

	typedef const std::unordered_map<const char *, function_type::call_type> builtin_list_type;

	// TODO: add global functions
	extern builtin_list_type GLOBAL_FUNCTIONS;
	extern builtin_list_type OBJECT_FUNCTIONS;
	extern builtin_list_type INTEGER_FUNCTIONS;
	extern builtin_list_type STRING_FUNCTIONS;
	extern builtin_list_type ARRAY_FUNCTIONS;

/*
object_ptr <function>(object_ptr self, object_ptr other) { ... }
*/

	#define DOT_DECLARE(N) object_ptr N(object_ptr self, object_ptr arg, const location &loc)

	void give_functions(object_ptr object);

}
}

#endif
