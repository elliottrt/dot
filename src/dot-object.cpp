#include "dot-object.h"
#include "dot-alloc.h"

dot::Object::~Object() {
	if (type == object_type::string)
		string.~string_type();
	else if (type == object_type::array) {
		for (auto e : array)
			dot::ObjectAllocator::dec_ref(e);
		array.~array_type();
	}
		
	for (const object_tags::value_type &child : tags)
		dot::ObjectAllocator::dec_ref(child.second);
}

void dot::Object::set(const dot::Object *other) {
	// clean up anything we're leaving behind
	if (type == object_type::string)
		string.~string_type();
	if (type == object_type::array) {
		for (auto e : array)
			dot::ObjectAllocator::dec_ref(e);
		array.~array_type();
	}

	type = other->type;
	switch (type) {
		case object_type::null: integer = 0; break;
		case object_type::integer: integer = other->integer; break;
		case object_type::string: string = other->string; break;
		case object_type::function: function = other->function; break;
		case object_type::array: array = other->array; break;
	}
}
