#ifndef _DOT_OBJECT
#define _DOT_OBJECT

#include "dot-defs.h"
#include "dot-builtins.h"

namespace dot {
	class ObjectAllocator;

	class Object {
	friend class ObjectAllocator;
		object_type type;
		union {
			integer_type integer;
			string_type string;
			function_type function;
			array_type array;
		};
		object_tags tags;
		// where this object was created
		Object *creator;

		// null object
		Object(): type(object_type::null), integer(0), tags{DOT_OBJECT_FUNCTIONS} {}
		Object(const integer_type &value): type(object_type::integer), integer(value), tags{DOT_OBJECT_FUNCTIONS, DOT_INTEGER_FUNCTIONS} {}
		Object(const string_type &value): type(object_type::string), string(value), tags{DOT_OBJECT_FUNCTIONS, DOT_STRING_FUNCTIONS} {}
		Object(const function_type &value): type(object_type::function), function(value) {}
		Object(const array_type &value): type(object_type::array), array(value), tags{DOT_OBJECT_FUNCTIONS, DOT_ARRAY_FUNCTIONS} {}

	public:

		~Object();

		void set(const Object *other);

		const object_type &get_type() {
			return type;
		}

		Object *get_creator() {
			return creator;
		}
	
		result::Result<integer_type *, error::TypeError> as_integer() {
			if (type == object_type::integer) return &integer;
			else return error::TypeError(object_type::integer, type);
		}
		result::Result<string_type *, error::TypeError> as_string() {
			if (type == object_type::string) return &string;
			else return error::TypeError(object_type::string, type);
		}
		result::Result<function_type *, error::TypeError> as_function() {
			if (type == object_type::function) return &function;
			else return error::TypeError(object_type::function, type);
		}
		result::Result<array_type *, error::TypeError> as_array() {
			if (type == object_type::array) return &array;
			else return error::TypeError(object_type::array, type);
		}
	};

}


#endif
