#include "dot-alloc.h"

dot::Object *dot::ObjectAllocator::global_null = new Object();
std::unordered_map<dot::Object *, size_t> dot::ObjectAllocator::objects = {{dot::ObjectAllocator::global_null, 1}};

void dot::ObjectAllocator::gc() {
	std::vector<dot::Object *> removals;
	
	for (std::unordered_map<dot::Object *, size_t>::value_type &obj : objects) {
		if (obj.second == 0) removals.push_back(obj.first);
	}

	for (Object *key : removals)
		objects.erase(key);
}

dot::Object *dot::ObjectAllocator::create(dot::Object *parent, const std::string &name) {
	if (parent == nullptr)
		parent = global_null;
		
	dot::Object *new_object = new dot::Object();
	objects[new_object] = 1;

	new_object->creator = parent;
	set_tag(parent, name, new_object);

	return new_object;
}

void dot::ObjectAllocator::set_tag(dot::Object *parent, const std::string &tag, dot::Object *object) {
	if (parent == nullptr)
		parent = global_null;

	if (parent->tags.count(tag) > 0)
		objects[parent->tags[tag]] -= 1;

	parent->tags[tag] = object;
	objects[object] += 1;
}

dot::result::Result<dot::Object *, dot::error::NoSuchObjectError> dot::ObjectAllocator::get_tag(dot::Object *parent, const std::string &tag) {
	if (parent == nullptr)
		parent = global_null;

	if (parent->tags.count(tag) > 0) {
		Object *child = parent->tags[tag];
		objects[child] += 1;
		return child;
	}

	return dot::error::NoSuchObjectError(tag);
}

bool dot::ObjectAllocator::has_tag(dot::Object *parent, const std::string &tag) {
	if (parent == nullptr)
		parent = global_null;

	return parent->tags.count(tag) > 0;
}
