#ifndef _DOT_DEFS
#define _DOT_DEFS

#include <string>
#include <functional>
#include <unordered_map>

#include "gc-ptr.h"
#include "dot-result.h"

namespace dot {
	class Object;
	namespace error { class RuntimeError; }

	using ObjectRef = gc_ptr<Object>;

	using integer_type = int;
	// return value: object or error
	// argument: argument to function or a null object
	using function_type = std::function<result::Result<ObjectRef, error::RuntimeError *>(ObjectRef)>;
	using array_type = std::vector<ObjectRef>;

	enum object_type {
		null,
		integer,
		function,
		array
	};

	const char *object_type_name(const object_type &type);

	using object_tags = std::unordered_map<std::string, ObjectRef>;

}

#include "dot-error.h"

#endif
