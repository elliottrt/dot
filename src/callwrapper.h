#ifndef _DOT_CALLWRAPPER
#define _DOT_CALLWRAPPER

#include <functional>

#include "location.h"
#include "error.h"

template <typename T>
class callwrapper {
public:
	typedef T item_type;
	typedef std::function<item_type(item_type, item_type, const location &)> call_type;

private:
	call_type call;
	item_type saved;
	bool bound;

	// where the function came from
	std::string source_string;
public:
	callwrapper(call_type call, const location &loc): call(call), saved(nullptr), bound(false), source_string("<function at " + loc.to_string() + ">") {}
	callwrapper(call_type call): call(call), saved(nullptr), bound(false), source_string("<system function>") {}

	void bind_to(item_type item) {
		saved = item;
		bound = true;
	}

	bool is_bound() const {
		return bound;
	}

	const std::string &get_source_string() const {
		return source_string;
	}

	item_type operator()(item_type arg, const location &loc) { 
		if (!bound)
			throw dot::error::BindError(loc);

		item_type retval = (call)(saved, arg, loc); 
		saved = nullptr;
		bound = false;
		return retval;
	}
};

#endif
