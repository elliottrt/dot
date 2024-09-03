#ifndef _DOT_BUILTINS
#define _DOT_BUILTINS

#include "dot-defs.h"
#include "dot-object.h"

#include <iostream>

// #define DOT_GLOBAL_FUNCTIONS {"print", new Object(builtins::print)}, {"return", new Object(builtins::ret)}, {"len", new Object(builtins::len)}

// #define DOT_OBJECT_FUNCTIONS {"set", new Object(builtins::set)}

// #define DOT_INTEGER_FUNCTIONS {"add", new Object(builtins::add)}

// #define DOT_STRING_FUNCTIONS {"len", new Object(builtins::strlen)}

// #define DOT_ARRAY_FUNCTIONS {"get", new Object(builtins::get)}

namespace dot {
namespace builtins {

// --------------Global Functions-------------------------------------

// print.arg
Object *print(Object *self, Object *arg) {
	switch (arg->get_type()) {
		case object_type::integer: std::cout << arg->as_integer().ok(); break;
		case object_type::string: std::cout << arg->as_string().ok(); break;
		case object_type::function: std::cout << "<function>"; break;
		case object_type::null: std::cout << "<null>"; break;
		case object_type::array: std::cout << "<array>"; break;
	}
	return arg;
}

// return.arg
Object *ret(Object *self, Object *arg) { 
	return arg; 
}

// --------------Object Functions-------------------------------------

// a.set.arg
Object *set(Object *self, Object *arg) {
	self->set(arg);
	return self;
}

// --------------Integer Functions------------------------------------

// self.add.arg
Object *add(Object *self, Object *arg) {
	*self->as_integer().ok() += *arg->as_integer().ok();

	return self;
}

// --------------String Functions-------------------------------------

Object *strlen(Object *self, Object *arg) {
	
}

// --------------Array Functions--------------------------------------



}
}

#endif
