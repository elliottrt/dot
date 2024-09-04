#ifndef _DOT_GC_PTR
#define _DOT_GC_PTR

// A smart pointer that counts how many references have been made
// upon deconstruction, if this is the last pointer to the object,
// destroy the object.

// See https://ootips.org/yonat/4dev/smart-pointers.html
// See https://ootips.org/yonat/4dev/counted_ptr.h
template <typename T>
struct gc_ptr {
	typedef T element_type;
	typedef size_t size_type;

	gc_ptr(element_type *ptr = nullptr): inner_counter(nullptr) {
		if (ptr != nullptr)
			inner_counter = new counter(ptr);
	}

	gc_ptr(const gc_ptr &other) {
		acquire(other.inner_counter);
	}

	~gc_ptr() {
		release();
	}

	gc_ptr &operator=(const gc_ptr &other) {
		if (this != &other) {
			release();
			acquire(other.inner_counter);
		}
		return *this;
	}

	element_type &operator*()  const {
		return *inner_counter->ptr;
	}

    element_type *operator->() const {
		return inner_counter->ptr;
	}

	size_type count() const {
		return inner_counter->count;
	}

	bool operator==(const std::nullptr_t &null) const {
		return inner_counter == nullptr || inner_counter->ptr == null;
	}

	bool operator==(const element_type *other) const {
		return inner_counter && inner_counter->ptr == other;
	}

	bool operator==(const gc_ptr<element_type> &other) const {
		return other.inner_counter && 
			   inner_counter &&
			   inner_counter->ptr == other.inner_counter->ptr;
	}

private:
	struct counter {
		element_type *ptr;
		size_type count;

		counter(element_type *ptr, size_type count = 1): ptr(ptr), count(count) {}
	} *inner_counter;

	void acquire(counter *c) {
		inner_counter = c;
		if (inner_counter != nullptr)
			inner_counter->count += 1;
	}

	void release() {
		if (inner_counter != nullptr) {
			inner_counter->count -= 1;
			if (inner_counter->count == 0) {
				delete inner_counter->ptr;
				delete inner_counter;
				inner_counter = nullptr;
			}
		}
	}

};

#endif
