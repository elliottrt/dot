#ifndef _DOT_ALLOCATOR
#define _DOT_ALLOCATOR

#include "dot-object.h"

#include <iostream>

namespace dot {

	class ObjectAllocator {

		static Object *global_null;
		static std::unordered_map<Object *, size_t> objects;

	public:

		static void gc();

		static void display() {
			for (const auto &pair : objects)
				std::cout << "\t" << pair.first->get_type() << " " << pair.second << std::endl;
		}

		static void dec_ref(Object *obj) {
			objects[obj] -= 1;
		}

		static void inc_ref(Object *obj) {
			objects[obj] += 1;
		}

		template<typename T>
		static Object *create(Object *parent, const std::string &name, const T &value) {
			if (parent == nullptr)
				parent = global_null;
				
			Object *new_object = new Object(value);
			objects[new_object] = 1;

			new_object->creator = parent;
			set_tag(parent, name, new_object);

			return new_object;
		}

		static Object *create(Object *parent, const std::string &name);

		static void set_tag(Object *parent, const std::string &tag, Object *object);

		static result::Result<Object *, error::NoSuchObjectError> get_tag(Object *parent, const std::string &tag);

		static bool has_tag(Object *parent, const std::string &tag);

	};

}

#endif
