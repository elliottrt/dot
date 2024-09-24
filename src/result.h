#ifndef _RESULT
#define _RESULT

#include <functional>

template<typename V, typename E> 
class Result {
	static_assert(
		!std::is_same<V, E>::value, 
		"Result ValueType and ErrorType may not be the same"
	);

	const bool _is_ok;
	union {
		V _ok;
		E _err;
	};

public:
	using value_type = V;
	using error_type = E;

	constexpr Result(const value_type &ok): _is_ok(true), _ok(ok) {}
	constexpr Result(const error_type &err): _is_ok(false), _err(err) {}
	Result(const Result &other): _is_ok(other._is_ok) {
		if (_is_ok) _ok = other._ok;
		else _err = other._err;
	}
	~Result() {
		if (_is_ok) _ok.~value_type();
		else _err.~error_type();
	}

	constexpr bool is_ok() const { return _is_ok; }
	constexpr bool is_err() const { return !_is_ok; }

	const value_type &ok() const {
		if (is_err())
			throw std::logic_error("tried to get ok from err");
		return _ok;
	}

	const error_type &err() const {
		if (is_ok())
			throw std::logic_error("tried to get err from ok");
		return _err;
	}

	value_type ok_or(const value_type &default_) const {
		if (_is_ok) return ok;
		else return default_;
	}
	error_type err_or(const error_type &default_) const {
		if (!_is_ok) return _err;
		else return default_;
	}
	
	template<typename T>
	Result<T, error_type> map(const std::function<T(value_type)> &mapper) const {
		if (_is_ok) return mapper(_ok);
		else return _err;
	}
	template<typename T>
	T map_or(const std::function<T(value_type)> &mapper, T default_) const {
		if (_is_ok) return mapper(_ok);
		else return default_;
	}

	operator bool() const {
		return _is_ok;
	}
};

#endif
