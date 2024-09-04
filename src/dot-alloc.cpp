#include "dot-alloc.h"

#include "dot-builtins.h"

dot::ObjectRef dot::ObjectAllocator::global_null = new Object();

dot::ObjectRef dot::ObjectAllocator::create(dot::ObjectRef parent, const std::string &name) {
	if (parent == nullptr)
		parent = global_null;
		
	dot::ObjectRef new_object = new dot::Object();

	set_tag(parent, name, new_object);

	return new_object;
}

dot::ObjectRef dot::ObjectAllocator::create(dot::ObjectRef parent, const std::string &name, const dot::integer_type &value) {
	if (parent == nullptr)
		parent = global_null;
		
	dot::ObjectRef new_object = new dot::Object(value);

	set_tag(parent, name, new_object);

	return new_object;
}

dot::ObjectRef dot::ObjectAllocator::create(dot::ObjectRef parent, const std::string &name, const std::string &value) {
	if (parent == nullptr)
		parent = global_null;
		
	dot::ObjectRef new_object = new dot::Object(value);

	set_tag(parent, name, new_object);

	return new_object;
}

dot::ObjectRef dot::ObjectAllocator::create(dot::ObjectRef parent, const std::string &name, const dot::array_type &value) {
	if (parent == nullptr)
		parent = global_null;
		
	dot::ObjectRef new_object = new dot::Object(value);

	set_tag(parent, name, new_object);

	return new_object;
}

dot::ObjectRef dot::ObjectAllocator::create(dot::ObjectRef parent, const std::string &name, const dot::function_type &value) {
	if (parent == nullptr)
		parent = global_null;
		
	dot::ObjectRef new_object = new dot::Object(value);

	set_tag(parent, name, new_object);

	return new_object;
}

void dot::ObjectAllocator::set_tag(dot::ObjectRef parent, const std::string &tag, dot::ObjectRef object) {
	if (parent == nullptr)
		parent = global_null;

	// TODO: what should we do here?
	if (object == nullptr)
		return;

	parent->tags[tag] = object;
}

dot::result::Result<dot::ObjectRef, dot::error::NoSuchObjectError> dot::ObjectAllocator::get_tag(dot::ObjectRef parent, const std::string &tag) {
	if (parent == nullptr)
		parent = global_null;

	if (parent->tags.count(tag) > 0)
		return parent->tags[tag];

	return dot::error::NoSuchObjectError(tag);
}

bool dot::ObjectAllocator::has_tag(dot::ObjectRef parent, const std::string &tag) {
	if (parent == nullptr)
		parent = global_null;

	return parent->tags.count(tag) > 0;
}

void dot::ObjectAllocator::remove_tag(dot::ObjectRef parent, const std::string &tag) {
	if (parent == nullptr)
		parent = global_null;

	// TODO: does this destroy the ObjectRef?
	parent->tags.erase(tag);
}
