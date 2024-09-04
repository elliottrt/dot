#ifndef _DOT_ALLOCATOR
#define _DOT_ALLOCATOR

#include "dot-object.h"

#include <iostream>

namespace dot {

	class ObjectAllocator {

		static ObjectRef global_null;

	public:

		static ObjectRef create(ObjectRef parent, const std::string &name);
		static ObjectRef create(ObjectRef parent, const std::string &name, const integer_type &value);
		static ObjectRef create(ObjectRef parent, const std::string &name, const std::string &value);
		static ObjectRef create(ObjectRef parent, const std::string &name, const array_type &value);
		static ObjectRef create(ObjectRef parent, const std::string &name, const function_type &value);

		static void set_tag(ObjectRef parent, const std::string &tag, ObjectRef object);

		static result::Result<ObjectRef, error::NoSuchObjectError> get_tag(ObjectRef parent, const std::string &tag);

		static bool has_tag(ObjectRef parent, const std::string &tag);

		static void remove_tag(ObjectRef parent, const std::string &tag);

	};

}

#endif
