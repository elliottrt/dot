#ifndef _DOT_CALLWRAPPER
#define _DOT_CALLWRAPPER

#include <functional>

#include "location.h"
#include "error.h"

// TODO: consider source string to know where the function came from,
// either a location or <system function>

template <typename T>
class callwrapper {
public:
	typedef T item_type;
	typedef std::function<item_type(item_type, item_type, const location &)> call_type;

private:
	call_type call;
	item_type saved;
	bool bound;
public:
	callwrapper(call_type call): call(call), saved(nullptr), bound(false) {}

	void bind_to(item_type item) {
		saved = item;
		bound = true;
	}

	bool is_bound() const {
		return bound;
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
