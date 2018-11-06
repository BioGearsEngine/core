#ifndef BIOGEARS_FRAMEWORK_RESULT_TCI_H
#define BIOGEARS_FRAMEWORK_RESULT_TCI_H

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

#include <biogears/framework/result.h>

#include <biogears/framework/option.tci.h>

namespace biogears {

//------------------------------------------------------------------------------
//!
//! Constructs an "Error" variant wrapping type `E`
//!
//! @param args These arguments are forwarded to the constructor for type E
//!
template<typename T, typename E>
template <typename... Args>
Result<T, E> Result<T, E>::make_error(Args... args) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return Result<T, E>(ErrConstructor::UNIT, std::forward<Args>(args)...);
}

//------------------------------------------------------------------------------
//!
//! Constructs an "Ok" variant wrapping type `T`
//!
//! @param args These arguments are forwarded to the constructor for type T
//!
template<typename T, typename E>
template <typename... Args>
Result<T, E> Result<T, E>::make(Args... args) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return Result<T, E>(OkConstructor::UNIT, std::forward<Args>(args)...);
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Result<T, E>::Result(Result<T, E>&& other) : _variant(EMPTY), _empty(0) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  *this = std::move(other);
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(Result<T, E>&& other) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");

  if (this != &other) {
    if (is_ok()) {
      dump_ok();
    }
    else if (is_err()) {
      dump_err();
    }
    _variant = other._variant;
    if (is_ok()) {
      new (&_ok_val) T(other.dump_ok());
    }
    else if (is_err()) {
      new (&_err_val) E(other.dump_err());
    }
  }
  return *this;
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Result<T, E>::Result(const Result<T, E>& other) : _variant(EMPTY), _empty(0) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  *this = other;
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(const Result<T, E>& other) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  // beware of self-assignment
  if (this != &other) {
    _variant = other._variant;
    if (is_ok()) {
      new (&_ok_val) T(other._ok_val);
      //_ok_val = other._ok_val;
    }
    else if (is_err()) {
      new (&_err_val) E(other._err_val);
      //_err_val = other._err_val;
    }
  }
  return *this;
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Result<T, E> Result<T, E>::clone() const
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return Result<T, E>(*this);
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Result<T, E>::~Result() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  //! Unions do not call deconstructors of its members.
  if (is_ok()) { _ok_val.~T(); }
  else if (is_err()) { _err_val.~E(); }
  _variant = EMPTY;
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Option<T> Result<T, E>::ok() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { return Option<T>(dump_ok()); }
  else { return Option<T>(); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
Option<E> Result<T, E>::err() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_err()) { return Option<E>(dump_err()); }
  else { return Option<E>(); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
template <typename U>
U Result<T, E>::match(std::function<U(T&&)> func_ok, std::function<U()> func_err) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_err()) { dump_err(); }
  return (is_ok()) ? func_ok(dump_ok()) : func_err();
}

//------------------------------------------------------------------------------
template<typename T, typename E>
template <typename U>
U Result<T, E>::match(std::function<U(const T&)> func_ok, std::function<U()> func_err) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return (is_ok()) ? func_ok(_ok_val) : func_err();
}

//------------------------------------------------------------------------------
template<typename T, typename E>
void Result<T, E>::match_void(std::function<void(T&&)> func_ok, std::function<void(void)> func_err) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_err()) { dump_err(); }
  if (is_ok()) { func_ok(dump_ok()); }
  else { func_err(); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
void Result<T, E>::match_void(std::function<void(const T&)> func_ok, std::function<void(void)> func_err) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { func_ok(_ok_val); }
  else { func_err(); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
void Result<T, E>::if_ok(std::function<void(const T&)> func_ok) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { func_ok(_ok_val); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
void Result<T, E>::if_err(std::function<void(const E&)> func_err) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_err()) { func_err(_err_val); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
void Result<T, E>::map_void(std::function<void(T&&)> func) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { func(dump_ok()); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
template <typename U>
Result<U, E> Result<T, E>::map(std::function< Result<U, E>(T&&)> func) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
	assert(_variant != EMPTY);
	return is_ok() ? Result<U, E>::make(func(dump_ok()))
    : is_err() ?Result<U, E>::make_error(dump_err())
		: Result<U,E>::make_error();
}

//------------------------------------------------------------------------------
template<typename T, typename E>
void Result<T, E>::map_void(std::function< void(const T&)> func_ok) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { func_ok(_ok_val); }
}

//------------------------------------------------------------------------------
template<typename T, typename E>
template <typename U>
Result<U, E> Result<T, E>::map(std::function< Result<U, E>(const T&)> func) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
	assert(_variant != EMPTY);
  return is_ok() ? Result<U, E>::make(func(_ok_val))
		: is_err() ? Result<U, E>::make_error(_err_val)
		: Result<U, E>::make_error();
}

template<typename T, typename E>
void Result<T, E>::map_err(std::function<void(E&&)> func) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_err()) { func(dump_err()); }
}

template<typename T, typename E>
template <typename F>
Result<T, F> Result<T, E>::map_err(std::function<F(E&&)> func) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
	assert(_variant != EMPTY);
	return is_err() ? Result<T, F>::make_error(func(dump_err()))
	: is_ok() ? Result<T, F>::make(dump_ok())
	: Result<T, F>::make_error(); 

}

template<typename T, typename E>
void Result<T, E>::map_err(std::function<void(const E&)> func) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_err()) { func(_err_val); }
}

template<typename T, typename E>
template <typename F>
Result<T, F> Result<T, E>::map_err(std::function<F(const E&)> func) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
	assert(_variant != EMPTY);
	return  is_err()? Result<T, F>::make_error(func(_err_val))
	: is_ok() ? Result<T, F>::make(_ok_val)
	: Result<T,F>::make_error();
}

template<typename T, typename E>
template <typename U>
Result<U, E> Result<T, E>::and_then(std::function<Result<U, E>(T&&)> func) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
	assert(_variant != EMPTY);
  return is_ok() ? func(dump_ok())
	: is_err() ? Result<U, E>::make_error(dump_err())
	:  Result<U, E>::make_error();
}

template<typename T, typename E>
template <typename U>
Result<U, E> Result<T, E>::and_then(std::function<Result<U, E>(const T&)> func) const {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
	assert(_variant != EMPTY);
  return is_ok() ? func(_ok_val)
	: is_err() ? Result<U, E>::make_error(_err_val)
	: Result<U,E>::make_error();
}

template<typename T, typename E>
template <typename U>
Result<U, E> Result<T, E>::mand(Result<U, E>&& other_res) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return is_ok() ? other_res
	: is_err() ? Result<U, E>::make_error(dump_err())
	: Result<U,E>::make_error();
}

template<typename T, typename E>
Result<T, E> Result<T, E>::mor(Result<T, E>&& other_res) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return is_ok() ? Result<T, E>::make(dump_ok()) : other_res;
}

template<typename T, typename E>
template <typename F>
Result<T, F>  Result<T, E>::or_else(std::function<Result<T, F>(E&&)> alt_func, F&& final_error) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return is_ok() ? Result<T, F>::make(dump_ok())
	: is_err() ? alt_func(dump_err())
	: Result<T,F>::make_error(final_error);
}

template<typename T, typename E>
T Result<T, E>::unwrap() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  assert(is_ok());
  return dump_ok();
};

template<typename T, typename E>
E Result<T, E>::unwrap_err() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  assert(is_err());
  return dump_err();
};

template<typename T, typename E>
T Result<T, E>::expect(const char* what_arg) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { return dump_err(); }
  else { throw std::runtime_error(what_arg); }
};

template<typename T, typename E>
T Result<T, E>::unwrap_or(T&& alt) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return is_ok() ? dump_ok() : alt;
}

template<typename T, typename E>
E Result<T, E>::unwrap_err_or(E&& alt) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  return is_err() ? dump_err() : alt;
}

template<typename T, typename E>
T Result<T, E>::unwrap_or_else(std::function<T(void)> alt_func) {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok()) { return dump_ok(); }
  return alt_func();
}

template<typename T, typename E>
Result<T, E>::Result() : _variant(EMPTY), _empty(0)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
}

template<typename T, typename E>
template <typename... Args>
Result<T, E>::Result(OkConstructor, Args&&... args)
  : _variant(OK), _ok_val(std::forward<Args>(args)...)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
}

template<typename T, typename E>
template <typename... Args>
Result<T, E>::Result(ErrConstructor, Args&&... args)
  : _variant(ERR), _err_val(std::forward<Args>(args)...)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
}

template<typename T, typename E>
T Result<T, E>::dump_ok() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  auto temp = std::move(_ok_val);
  _ok_val.~T();
  _variant = EMPTY;
  return temp;
}

template<typename T, typename E>
E Result<T, E>::dump_err() {
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  auto temp = std::move(_err_val);
  _err_val.~E();
  _variant = EMPTY;
  return temp;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::emplace(T&& obj)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok())
  {
    dump_ok();
  }
  if (is_err())
  {
    dump_err();
  }
  new (&_ok_val)T(std::move(obj));
  _variant = OK;
  return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::emplace(const T& obj)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok())
  {
    dump_ok();
  }
  if (is_err())
  {
    dump_err();
  }
  new (&_ok_val)T(obj);
  _variant = OK;

  return *this;
}

template<typename T, typename E>
template<typename SAME>
Result<T, E>& Result<T, E>::emplace(typename std::enable_if<!std::is_same<T, SAME>::value, E>::type&& obj)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok())
  {
    dump_ok();
  }
  if (is_err())
  {
    dump_err();
  }
  new (&_err_val)T(std::move(obj));;
  _variant = ERR;
  return *this;
}

template<typename T, typename E>
template<typename SAME>
Result<T, E>& Result<T, E>::emplace(const typename std::enable_if<!std::is_same<T, SAME>::value, E>::type& obj)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok())
  {
    dump_ok();
  }
  if (is_err())
  {
    dump_err();
  }
  new (&_err_val)T(obj);
  _variant = ERR;
  return *this;
}

template<typename T, typename E>
template <typename ... Args>
Result<T, E>& Result<T, E>::emplace_ok(Args&& ... args)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok())
  {
    dump_ok();
  }
  if (is_err())
  {
    dump_err();
  }
  _ok_val = T(std::forward<Args>(args) ...);
  _variant = OK;
  return *this;
}

template<typename T, typename E>
template <typename ... Args>
Result<T, E>& Result<T, E>::emplace_err(Args&& ... args)
{
  static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
  if (is_ok())
  {
    dump_ok();
  }
  if (is_err())
  {
    dump_err();
  }
  _err_val = E(std::forward<Args>(args) ...);
  _variant = ERR;
  return *this;
}

} // end namespace biogears
// Macros
//======================================================================
#define TRY_DECLARE(var, expr)                              \
  auto res_##var = expr;                                    \
  if (res_##var.is_err()) {                                 \
    return ReturnT::make_error(std::move(res_##var).unwrap_err()); \
  }                                                         \
  decltype(expr)::OkT var(std::move(res_##var).unwrap());   \

#define TRY_ASSIGN(var, expr)                               \
  auto res_##var = expr;                                    \
  if (res_##var.is_err()) {                                 \
    return ReturnT::make_error(std::move(res_##var).unwrap_err()); \
  }                                                         \
  var = std::move(res_##var).unwrap();                      \

#endif
