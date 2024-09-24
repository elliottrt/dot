#ifndef _DOT_BUILTINS
#define _DOT_BUILTINS

#include "object.h"

#include <unordered_map>

namespace dot {
namespace builtins {

	typedef const std::unordered_map<const char *, function_type::call_type> builtin_list_type;

	// TODO: instead of adding each function to each object,
	// consider checking each object against its groups
	// and then return a function -> would save lots of memory
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

	bool has_function(object_ptr parent, const std::string &tag);
	object_ptr call_function(object_ptr parent, object_ptr argument, const std::string &tag, const location &loc);

}
}

#endif
