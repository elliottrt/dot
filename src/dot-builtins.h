#ifndef _DOT_BUILTINS
#define _DOT_BUILTINS

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
// TODO: this won't print strings properly
ObjectRef print(ObjectRef self, ObjectRef arg) {
	std::cout << arg->to_string();
	return arg;
}

// return.arg
ObjectRef ret(ObjectRef self, ObjectRef arg) { 
	return arg; 
}

// --------------Object Functions-------------------------------------

// a.set.arg
ObjectRef set(ObjectRef self, ObjectRef arg) {
	self->set(arg);
	return self;
}

// --------------Integer Functions------------------------------------

// self.add.arg
ObjectRef add(ObjectRef self, ObjectRef arg) {
	*self->as_integer().ok() += *arg->as_integer().ok();

	return self;
}

// --------------Array Functions--------------------------------------

ObjectRef get(ObjectRef self, ObjectRef arg) {
	
}

}
}

#endif
