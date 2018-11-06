#ifndef BIOGEARS_FRAMEWORK_OPTION_H
#define BIOGEARS_FRAMEWORK_OPTION_H

//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//!
//! \author Alex Pronschinske
//! \date 2017 Mar 20
//!

#include <functional>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <cassert>

namespace biogears {

template <typename T, typename E> class Result;

//!
//! Option type
//!
//! This type is a tagged union that may or may not contain the templated
//! type T. This type is excellent for modeling the return type of functions
//! that can fail. This implementation is modeled off of the following:
//!   * Rust's Option type: https://doc.rust-lang.org/std/option/index.html
//!   * Boost's Optional class:
//!     http://www.boost.org/doc/libs/1_62_0/libs/optional/doc/html/index.html
//!   * C++17's experimental Optional class:
//!     http://en.cppreference.com/w/cpp/experimental/optional
//!
//! The key idea with this class is that you can either deal with the type-
//! level uncertainty it models by checking (`is_some`) and "unwrapping" or
//! by dealing with it monadically and passing a function to the `map` method
//! to operate on the potentially contained type.
//!
//! @note Some functions still use `std::function` to accept functions as
//!       arguments, be aware these function will incur a performance hit from
//!       indirection (and the subsequent prevention of inlining). I have been
//!       converting these to the templated style as I've been making use of
//!       them.
//!
template<typename T>
class Option {
protected:
	// Data members
	enum { NONE, SOME } _variant;
	union {
		uint8_t _empty;
		T _val;
	};

public:
	using SomeT = T;

	// Nested types
	class ConstIter;
	class Iter;

	// Constructors
	//!
	//! Default constructor is to the None value
	//!
	Option();
	Option(const T& val);
	Option(T&& val);
	template <typename ... Args>
	Option(Args&& ... args);
	template <typename ... Args>
	static Option<T> make(Args&& ... args);

	// move constructor from move assignment operator
	Option(Option<T>&& other);
	Option<T>& operator=(Option<T>&& other);

	// copy constructor from copy assignment operator
	Option(const Option<T>& other);
	Option<T>& operator=(const Option<T>& other);

	inline explicit operator bool() const
	{
		return is_some();
	}

	inline bool operator==(const T& rhs) const
	{
		if (is_some())
		{
			return _val == rhs;
		}
		return false;
	}

	Option<T> clone() const;

	// destructor
	~Option();

	// variant testing
	inline bool is_some() const
	{
		return _variant == SOME;
	}
	inline bool is_none() const
	{
		return _variant == NONE;
	}

	bool operator==(const Option<T>& other) const;
	bool operator!=(const Option<T>& other) const;

	//!
	//! The match method take a function for handling either variant and
	//! dispatches the appropriate function and returns the common type.
	//!
	template <typename U>
	U match(std::function<U(T&&)> func_some, std::function<U(void)> func_none);

	template <typename U>
	U match(std::function<U(const T&)> func_some, std::function<U(void)> func_none) const;

	void match_void(std::function<void(T&&)> func_some, std::function<void(void)> func_none);
	void match_void(std::function<void(const T&)> func_some, std::function<void(void)> func_none) const;


	template<typename E, typename ... Args>
	auto ok_or(Args&& ... args) &&->Result<T, E>;

	template<typename E, typename ErrFuncT>
	auto ok_or_else(ErrFuncT err_func) &&->Result<T, E>;

	template <typename U>
	Option<U> map(std::function<U(T&&)> func);
	template <typename U>
	Option<U> map(std::function<U(const T&)> func) const;

	void map_void(std::function<void(T&&)> func);
	void map_void(std::function<void(const T&)> func) const;


	template <typename U>
	U map_or(U&& default_val, std::function<U(T&&)> func);
	template <typename U>
	U map_or(U&& default_val, std::function<U(const T&)> func) const;

	template <typename U>
	U map_or_else(std::function<U()> default_func, std::function<U(T&&)> func);
	template <typename U>
	U map_or_else(std::function<U()> default_func, std::function<U(const T&)> func) const;

	//!
	//! Returns a "None" variant if the Option object is "None", otherwise
	//! it calls the input function with the contained value of type T and
	//! returns an Option<U> with the result.
	//!
	template <typename U>
	Option<U> and_then( std::function<Option<U>(T&&)> func);

	template <typename U>
	Option<U> and_then( std::function<Option<U>(const T&)> func ) const;

	template <typename U>
	Option<U> mand(Option<U>&& other_opt) const;

	Option<T> mor(Option<T>&& other_opt);

	Option<T> or_else(std::function<Option<T>()> alt_func);

	Option<T>& emplace(T&& obj);

	Option<T>& emplace(const T& obj);

	template <typename ... Args>
	Option<T>& emplace(Args&& ... args);

	//!
	//! Returns the "Some" value, and sets it's self to "None"
	//! @note This function will segfault if it is called on a "None" Option
	//!
	T unwrap();

	T expect(const char* err_msg);

	T unwrap_or(T&& alt);
	T unwrap_or(const T& alt);
	T unwrap_or_default();
	T unwrap_or_else(std::function<T()> alt_func);

protected:
	T dump();

};

template<typename T>
Option<T> make_some(T& x);
template<typename T>
Option<T> make_some(const T& x);
template<typename T>
Option<T> make_some(T&& x);

} // end namespace biogears

#endif
