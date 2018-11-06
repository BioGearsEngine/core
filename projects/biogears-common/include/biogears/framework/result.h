#ifndef BIOGEARS_FRAMEWORK_RESULT_H
#define BIOGEARS_FRAMEWORK_RESULT_H

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
//! A functional style return type which can be either a 
//! type T or an Error type E
//! 
//! Your Error type E should be a default constructible type
//! This avoids several issues

#include <functional>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <cassert>

namespace biogears {

template <typename T> class Option;

//==============================================================================
//!
//! Result type
//!
//! This type is a tagged union that will contain the templated type `T` or
//! the type `E`.
//!
template<typename T, typename E>
class Result {
protected:
    // Data Members
    enum { ERR, OK, EMPTY } _variant;
    union {
        E _err_val;
        T _ok_val;
        uint8_t _empty;
    };

public:
    // Type aliases
    using OkT = T;
    using ErrT = E;

    // Constructors
    //!
    //! Constructs an "Error" variant wrapping type `E`
    //!
    //! @param args These arguments are forwarded to the constructor for type E
    //!
    template <typename... Args>
    static Result<T, E> make_error(Args... args);

    //!
    //! Constructs an "Ok" variant wrapping type `T`
    //!
    //! @param args These arguments are forwarded to the constructor for type T
    //!
    template <typename... Args>
    static Result<T, E> make(Args... args);

    // move assignment
    Result(Result<T, E>&& other);
    Result<T, E>& operator=(Result<T, E>&& other);
    
    inline explicit operator bool() const
    {
      static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
      return is_ok();
    }

    inline bool operator==( const T& rhs) const
    {
      static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
      if (is_ok())
      {
        return _ok_val == rhs;
      }
      return false;
    }
    template<typename SAME = E>
    inline bool operator==( const typename std::enable_if<!std::is_same<T,SAME>::value, E>::type& rhs ) const
    {
      static_assert(!std::is_same<T, E>::value, "Result and Error can not be the same type.");
      if (is_err())
      {
        return _err_val == rhs;
      }
      return false;
    }

    // copy assignment operator
    Result(const Result<T, E>& other);
    Result<T, E>& operator=(const Result<T, E>& other);

    Result<T, E> clone() const;

    ~Result();

    // variant checking
    inline bool is_ok() const { return _variant == OK; }
    inline bool is_err() const { return _variant == ERR; }

    Option<T> ok();
    Option<E> err();

    //!
    //! Match functions must handle even empty result types
    //! Your basically choosing to swallow the err if present
    //! Use Maps if you want to propagate the error

    template <typename U>
    U match(std::function<U (T&&)> func_ok, std::function<U (void)> func_err);

    template <typename U>
    U match(std::function<U (const T&)> func_ok, std::function<U (void)> func_err) const;

    void match_void(std::function<void (T&&)> func_ok, std::function<void (void)> func_err);
    void match_void(std::function<void (const T&)> func_ok, std::function<void (void)> func_err) const;

    void if_ok(std::function<void (const T&)> func_ok) const;
    void if_err(std::function<void (const E&)> func_err) const;
    
    //!
    //! Map functions propagate Errors
    //! An assert is thrown in DEBUG if Result is empty
    //! Else in release Error is assumed default constructible
    //! and meaningful. This would be easier if we assumed
    //! TODO: is_copyassignable check is_defaultconstructable
    void map_void(std::function<void (T&&)> func) ;

    template <typename U>
    Result<U, E> map(std::function< Result<U, E> (T&&)> func) ;

    void map_void(std::function< void (const T&)> func_ok) const;

    template <typename U>
    Result<U, E> map(std::function< Result<U, E> (const T&)> func) const;

    void map_err(std::function<void(E&&)> func) ;

    template <typename F>
    Result<T, F> map_err(std::function<F(E&&)> func) ;

    void map_err(std::function<void(const E&)> func) const;

    template <typename F>
    Result<T, F> map_err(std::function<F(const E&)> func) const;

    template <typename U>
    Result<U, E> and_then(std::function< Result<U, E> (T&&)> func) ;

    template <typename U>
    Result<U, E> and_then(std::function< Result<U, E> (const T&)> func) const;

    template <typename U>
    Result<U, E> mand(Result<U, E>&& other_res) ;

    Result<T, E> mor(Result<T, E>&& other_res) ;

    template <typename F>
    Result<T, F> or_else(std::function<Result<T, F>(E&&)> alt_func, F&& final_error) ;

    T unwrap() ;
    E unwrap_err() ;

    T expect(const char* what_arg) ;

    T unwrap_or(T&& alt) ;
    E unwrap_err_or(E&& alt) ;
    T unwrap_or_else(std::function<T(void)> alt_func) ;

    Result<T, E>& emplace(T&& obj);

    Result<T, E>& emplace(const T& obj);

    template<typename SAME = E>
    Result<T, E>& emplace(typename std::enable_if<!std::is_same<T,SAME>::value,E>::type&& obj);

    template<typename SAME = E>
    Result<T, E>& emplace(const typename std::enable_if<!std::is_same<T,SAME>::value,E>::type& obj);

    template <typename ... Args>
    Result<T, E>& emplace_ok(Args&& ... args);

    template <typename ... Args>
    Result<T, E>& emplace_err(Args&& ... args);

protected:
    Result();

    enum class OkConstructor { UNIT };

    template <typename... Args>
    explicit Result(OkConstructor, Args&&... args);

    enum class ErrConstructor { UNIT };

    template <typename... Args>
    explicit Result(ErrConstructor, Args&&... args);

    T dump_ok();

    E dump_err();
};

}

#endif
