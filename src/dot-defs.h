#ifndef _DOT_DEFS
#define _DOT_DEFS

#include <string>
#include <functional>
#include <unordered_map>

namespace dot {
	class Object;

	using integer_type = int;
	// return value: object
	// first object: self or a null object
	// second object: argument to function or a null object
	using function_type = std::function<Object *(Object *, Object *)>;
	using array_type = std::vector<Object *>;

	enum object_type {
		null,
		integer,
		function,
		array
	};

	const char *object_type_name(const object_type &type);

	using object_tags = std::unordered_map<std::string, Object *>;

}

#include "dot-result.h"
#include "dot-error.h"

#endif
