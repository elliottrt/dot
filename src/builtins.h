#ifndef _DOT_BUILTINS
#define _DOT_BUILTINS

#include "object.h"

#include <unordered_map>

namespace dot {
namespace builtins {

	typedef const std::unordered_map<std::string, function_type::call_type> builtin_list_type;

	extern builtin_list_type OBJECT_FUNCTIONS;
	extern builtin_list_type INTEGER_FUNCTIONS;
	extern builtin_list_type STRING_FUNCTIONS;
	extern builtin_list_type ARRAY_FUNCTIONS;

	// object_ptr <function>(object_ptr self, object_ptr other) { ... }
	#define DOT_DECLARE(N) object_ptr N(object_ptr self, object_ptr arg, const location &loc)

	object_ptr get_function(object_ptr caller, const std::string &tag);

}
}

#endif
