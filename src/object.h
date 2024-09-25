#ifndef _DOT_OBJECT
#define _DOT_OBJECT

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "location.h"
#include "callwrapper.h"

namespace dot {
	class object;
	typedef std::shared_ptr<object> object_ptr;
	
	typedef int integer_type;
	typedef std::string string_type;
	typedef std::vector<object_ptr> array_type;
	typedef callwrapper<object_ptr> function_type;

	// type of an object, can be only
	// one of these at once
	enum class object_type {
		none,
		integer,
		string,
		array,
		function,
	};

	class object_value {
		friend class object;

		object_type type;
		union {
			integer_type int_value;
			string_type string_value;
			array_type array_value;
			function_type function_value;
		};
	public:
		object_value(): type(object_type::none) {}
		object_value(const integer_type &value): type(object_type::integer), int_value(value) {}
		object_value(const string_type &value): type(object_type::string), string_value(value) {}
		object_value(const array_type &value): type(object_type::array), array_value(value) {}
		object_value(const function_type &value): type(object_type::function), function_value(value) {}

		void value_destructor();
		~object_value();

		object_value(const object_value &other);
		object_value &operator=(const object_value &other);
	};

	class object {

		std::unordered_map<std::string, object_ptr> child_map;
		object_ptr get(dot::object_ptr self, const std::string &name);
	public:
		object_value value;

		object(): value() {}
		object(const integer_type &value): value(value) {}
		object(const string_type &value): value(value) {}
		object(const array_type &value): value(value) {}
		object(const function_type &value): value(value) {}

		static object_ptr create();
		static object_ptr create(const integer_type &value);
		static object_ptr create(const string_type &value);
		static object_ptr create(const array_type &value);
		static object_ptr create(const function_type &value);

		static object_ptr from_argv(int argc, const char **argv);

		static object_ptr get_child_of(object_ptr self, const std::string &name);

		std::string to_string();

		const object_type &type() const;

		integer_type &get_int(const location &loc);
		string_type &get_str(const location &loc);
		array_type &get_arr(const location &loc);
		function_type &get_function(const location &loc);

		void add_self_and_arg(object_ptr self, object_ptr arg);
		void remove_self_and_arg();

		static void set(object_ptr dst, object_ptr src);
	};

	const char *object_type_name(const dot::object_type &type);
}

#endif
