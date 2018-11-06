#ifndef BIOGEARS_FRAMEWORK_OPTION_TCI_H
#define BIOGEARS_FRAMEWORK_OPTION_TCI_H

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

#include <biogears/framework/option.h>

namespace biogears {

//==============================================================================
// Option definitions
//------------------------------------------------------------------------------
template<typename T>
Option<T>::Option() : _variant(NONE), _empty(0)
{
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>::Option(const T& val) : _variant(SOME), _val(val)
{
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>::Option(T&& val) : _variant(SOME), _val(std::move(val))
{
}
//------------------------------------------------------------------------------
template<typename T>
template <typename ... Args>
Option<T>::Option(Args&& ...args) : _variant(SOME), _val(std::forward<Args>(args) ...)
{}
//------------------------------------------------------------------------------
template<typename T>
template <typename ... Args>
Option<T> Option<T>::make(Args&& ... args)
{
	return Option<T>(std::forward<Args>(args) ...);
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>::Option(Option<T>&& other) : _variant(NONE), _empty(0)
{
	*this = std::move(other);
}
template<typename T>
Option<T>& Option<T>::operator=(Option<T>&& other)
{
	// beware of self-assignment
	if (this != &other)
	{
		if (_variant == SOME)
		{
			dump();
		}
		_variant = other._variant;
		if (_variant == NONE)
		{
			_empty = 0;
		}
		else
		{
			new (&_val)T(other.dump());
		}
	}
	return *this;
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>::Option(const Option<T>& other) : _variant(NONE), _empty(0)
{
	*this = other;
}
template<typename T>
Option<T>& Option<T>::operator=(const Option<T>& other)
{
	// beware of self-assignment
	if (this != &other)
	{
		_variant = other._variant;
		if (_variant == NONE)
		{
			_empty = 0;
		}
		else
		{
			new (&_val)T(other._val);
			//_val = other._val;
		}
	}
	return *this;
}

//------------------------------------------------------------------------------
template<typename T>
Option<T> Option<T>::clone() const
{
	return Option<T>(*this);
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>::~Option()
{
	if (is_some())
	{
		_val.~T();
	}
	_variant = NONE;
}

template<typename T>
bool Option<T>::operator==(const Option<T>& other) const
{
	if ( is_some() && other.is_some() )
	{
		  return _val == other._val;
	}
	return is_none() && other.is_none();
}
template<typename T>
bool Option<T>::operator!=(const Option<T>& other) const
{
	return !(*this == other);
}

//------------------------------------------------------------------------------
//!
//! The match method take a function for handling either variant and
//! dispatches the appropriate function and returns the common type.
//!
template<typename T>
template <typename U>
U Option<T>::match(std::function<U(T&&)> func_some, std::function<U(void)> func_none)
{
	return is_some() ? func_some(dump()) : func_none();
}

//------------------------------------------------------------------------------
template<typename T>
template <typename U>
U Option<T>::match(std::function<U(const T&)> func_some, std::function<U(void)> func_none) const
{
	return is_some() ? func_some(_val) : func_none();
}

//------------------------------------------------------------------------------
template<typename T>
void Option<T>::match_void(std::function<void(T&&)> func_some, std::function<void(void)> func_none)
{
	if (is_some())
	{
		func_some(dump());
	}
	else
	{
		func_none();
	}
}

//------------------------------------------------------------------------------
template<typename T>
void Option<T>::match_void(std::function<void(const T&)> func_some, std::function<void(void)> func_none) const
{
	if (is_some())
	{
		func_some(_val);
	}
	else
	{
		func_none();
	}
}

//------------------------------------------------------------------------------
template <typename T>
template <typename E, typename ... Args>
auto Option<T>::ok_or(Args&& ... args) &&->Result<T, E>
{
	return is_some() ? Result<T, E>::Ok(dump())
				 : Result<T, E>::Err(std::forward<Args>(args) ...);
}

//------------------------------------------------------------------------------
template <typename T>
template<typename E, typename ErrFuncT>
auto Option<T>::ok_or_else(ErrFuncT err_func) &&->Result<T, E>
{
	static_assert(
		std::is_same<typename std::result_of<ErrFuncT()>::type, E>::value
		, "function passed to `Option::ok_or_else` does not have correct return type, E"
		);
	return is_some()
	       ? Result<T, E>::Ok(dump())
				 : Result<T, E>::Err(err_func());
}

//------------------------------------------------------------------------------
//!
//! Map Option<T> to Option<U> by applying the argument `func` (of type
//! U func(T) or T -> U) to the contained data if there is some.
//!
template<typename T>
template <typename U>
Option<U> Option<T>::map(std::function<U(T&&)> func)
{
	if (is_some())
	{
		return Option<U>(func(dump()));
	}
	else
	{
		return Option<U>();
	}
}

//------------------------------------------------------------------------------
// !
// ! Map Option<T> to Option<U> by applying the argument `func` (of type
// ! U func(T) or T -> U) to the contained data if there is some.
// !
template<typename T>
template <typename U>
Option<U> Option<T>::map(std::function<U(const T&)> func) const
{
	if (is_some())
	{
		return Option<U>(func(_val));
	}
	else
	{
		return Option<U>();
	}
}

//------------------------------------------------------------------------------
// !
// ! Usually modifies a memory address by reference based on
// ! the value of the option
// !
template<typename T>
void Option<T>::map_void(std::function<void(T&&)> func)
{
	if (is_some())
	{
		func(dump());
	}
}

//------------------------------------------------------------------------------
// !
// ! Usually modifies a memory address by reference based on
// ! the value of the option
// !
template<typename T>
void Option<T>::map_void(std::function<void(const T&)> func) const
{
	if (is_some())
	{
		func(_val);
	}
}

//------------------------------------------------------------------------------
// !
// ! Returns the first param when the option is a NONE variant
// ! else executes the second
template<typename T>
template <typename U>
U Option<T>::map_or(U&& default_val, std::function<U(T&&)> func)
{
	if (is_some())
	{
		return func(dump());
	}
	else
	{
		return default_val;
	}
}

//------------------------------------------------------------------------------
// !
// ! Returns the first param when the option is a NONE variant
// ! else executes the second
template<typename T>
template <typename U>
U Option<T>::map_or(U&& default_val, std::function<U(const T&)> func) const
{
	if (is_some())
	{
		return func(_val);
	}
	else
	{
		return default_val;
	}
}

//------------------------------------------------------------------------------
// !
// ! Executes the first param when the option is a NONE variant
// ! else the second
template<typename T>
template <typename U>
U Option<T>::map_or_else(std::function<U()> default_func, std::function<U(T&&)> func)
{
	if (is_some())
	{
		return func(dump());
	}
	else
	{
		return default_func();
	}
}

//------------------------------------------------------------------------------
// !
// ! Executes the first param when the option is a NONE variant
// ! else the second
template<typename T>
template <typename U>
U Option<T>::map_or_else(std::function<U()> default_func, std::function<U(const T&)> func) const
{
	if (is_some())
	{
		return func(_val);
	}
	else
	{
		return default_func();
	}
}

//------------------------------------------------------------------------------
//!
//! Returns a "None" variant if the Option object is "None", otherwise
//! it calls the input function with the contained value of type T and
//! returns an Option<U> with the result.
//!
template<typename T>
template <typename U>
Option<U> Option<T>::and_then(std::function<Option<U>(T&&)> func)
{
	if (is_some())
	{
		return func(dump());
	}
	else
	{
		return Option<U>();
	}
}

//------------------------------------------------------------------------------
template<typename T>
template <typename U>
Option<U> Option<T>::and_then(std::function<Option<U>(const T&)> func) const
{
	if (is_some())
	{
		return func(_val);
	}
	else
	{
		return Option<U>();
	}
}

//------------------------------------------------------------------------------
template<typename T>
template <typename U>
Option<U> Option<T>::mand(Option<U>&& other_opt) const
{
	if (is_some())
	{
		return std::move(other_opt);
	}
	else
	{
		return Option<U>();
	}
}

//------------------------------------------------------------------------------
template<typename T>
Option<T> Option<T>::mor(Option<T>&& other_opt)
{
	if (is_some())
	{
		return Option<T>(dump());
	}
	else
	{
		return std::move(other_opt);
	}
}

//------------------------------------------------------------------------------
template<typename T>
Option<T> Option<T>::or_else(std::function<Option<T>()> alt_func)
{
	if (is_some())
	{
		return Option<T>(dump());
	}
	else
	{
		return alt_func();
	}
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>& Option<T>::emplace(T&& obj)
{
	if (is_some())
	{
		dump();
	}
	new (&_val)T(std::move(obj));
	_variant = SOME;
	return *this;
}

//------------------------------------------------------------------------------
template<typename T>
Option<T>& Option<T>::emplace(const T& obj)
{
	if (is_some())
	{
		dump();
	}
	new (&_val)T(obj);

	_variant = SOME;
	return *this;
}

//------------------------------------------------------------------------------
template<typename T>
template <typename ... Args>
Option<T>& Option<T>::emplace(Args&& ... args)
{
	if (is_some())
	{
		dump();
	}
	new (&_val)T(std::forward<Args>(args) ...);
	
	_variant = SOME;
	return *this;
}

//------------------------------------------------------------------------------
//!
//! Returns the "Some" value, and sets it's self to "None"
//! @note This function will segfault if it is called on a "None" Option
//!
template<typename T>
T Option<T>::unwrap()
{
	return dump();
}

//------------------------------------------------------------------------------
template<typename T>
T Option<T>::expect(const char* err_msg)
{
	if (is_some())
	{
		return dump();
	}
	else
	{
		throw std::runtime_error(err_msg);
	}
}

//------------------------------------------------------------------------------
template<typename T>
T Option<T>::unwrap_or(T&& alt)
{
	if (is_some())
	{
		return dump();
	}
	else
	{
		return alt;
	}
}

//------------------------------------------------------------------------------
template<typename T>
T Option<T>::unwrap_or(const T& alt)
{
	if (is_some())
	{
		return dump();
	}
	else
	{
		return alt;
	}
}

//------------------------------------------------------------------------------
template<typename T>
T Option<T>::unwrap_or_default()
{
	if (is_some())
	{
		return dump();
	}
	else
	{
		return T();
	}
}

//------------------------------------------------------------------------------
template<typename T>
T Option<T>::unwrap_or_else(std::function<T()> alt_func)
{
	if (is_some())
	{
		return dump();
	}
	else
	{
		return alt_func();
	}
}

//------------------------------------------------------------------------------
template<typename T>
T Option<T>::dump()
{
	assert( _variant == SOME);
	auto temp = std::move(_val);
	_val.~T();
	_empty   = 0;
	_variant = NONE;
	return temp;
}

//------------------------------------------------------------------------------
template<typename T>
Option<T> make_some(T& x)
{
	return Option<T>(x);
}

//------------------------------------------------------------------------------
template<typename T>
Option<T> make_some(const T& x)
{
	return Option<T>(x);
}

//------------------------------------------------------------------------------
template<typename T>
Option<T> make_some(T&& x)
{
	return Option<T>(std::move(x));
}

} // end namespace biogears

#endif
