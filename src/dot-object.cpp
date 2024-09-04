#include "dot-object.h"
#include "dot-alloc.h"

dot::Object::~Object() {
	if (type == object_type::array) {
		array.~array_type();
	}
}

void dot::Object::set(const dot::ObjectRef other) {
	// clean up anything we're leaving behind
	if (type == object_type::array) {
		array.~array_type();
	}

	type = other->type;
	switch (type) {
		case object_type::null: integer = 0; break;
		case object_type::integer: integer = other->integer; break;
		case object_type::function: function = other->function; break;
		case object_type::array: array = other->array; break;
	}
}
