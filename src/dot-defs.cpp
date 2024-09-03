#include "dot-defs.h"

const char *dot::object_type_name(const dot::object_type &type) {
	switch (type) {
		case dot::object_type::null: return "null";
		case dot::object_type::integer: return "integer";
		case dot::object_type::function: return "function";
		case dot::object_type::array: return "array";
	}
}
