#ifndef _DOT_OBJECT
#define _DOT_OBJECT

#include "dot-defs.h"

namespace dot {
	class ObjectAllocator;

	class Object {
	friend class ObjectAllocator;
		object_type type;
		union {
			integer_type integer;
			function_type function;
			array_type array;
		};
		object_tags tags;

		// null object
		Object(): type(object_type::null), integer(0) {}
		Object(const integer_type &value): type(object_type::integer), integer(value) {}
		Object(const std::string &value): type(object_type::array) {
			for (const char &c : value) {
				array.push_back(new Object(static_cast<integer_type>(c)));
			}
		}
		Object(const function_type &value): type(object_type::function), function(value) {}
		Object(const array_type &value): type(object_type::array), array(value) {}

	public:

		~Object();

		std::string to_string() {
			switch (type) {
				case object_type::integer: return std::to_string(integer);
				case object_type::array: {
					array_type *array = as_array().ok();
					std::string result;
					for (auto e : *array)
						result += e->to_string() + ", ";
					return "[" + result.substr(0, result.size() - 2) + "]";
				}
				case object_type::function: return "<function>";
				default: return "<null>";
			}
		}

		void set(const ObjectRef other);

		const object_type &get_type() {
			return type;
		}
	
		result::Result<integer_type *, error::TypeError> as_integer() {
			if (type == object_type::integer) return &integer;
			else return error::TypeError(object_type::integer, type);
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
