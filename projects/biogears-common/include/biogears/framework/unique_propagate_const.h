#ifndef BIOGEARS_FRAMEWORK_UNIQUE_PROPAGATE_CONST_H
#define BIOGEARS_FRAMEWORK_UNIQUE_PROPAGATE_CONST_H

//! Copyright (C) Applied Research Associates - All rights Reserved
//! Unauthorized copying of this file, via any medium is strictly prohibited
//! Proprietary and confidential
//!
//! This file is subject to the terms and conditions defined in file
//! 'LICENSE.txt', which is part of this source code package

//!
//! \file
//! \author Steven A White
//! \date   November 1st 2017
//! \brief  The goal of this class is to allow
//!         unique_ptr and shared_ptr to properly propagate constness
//!         and prevent the use of non const functions from unique_ptr
//!         In general the class should never point to a null object

#include <memory>

namespace biogears {

template <class Implementation>
class unique_propagate_const {
public:
  template <typename... PARAMS>
  unique_propagate_const(PARAMS... params);
  unique_propagate_const(std::nullptr_t);
  

  constexpr Implementation& operator*();
  constexpr const Implementation& operator*() const;
  constexpr Implementation* const operator->();
  constexpr Implementation const* const operator->() const;
  constexpr explicit operator bool() const;
  constexpr operator Implementation*();
  constexpr operator const Implementation*() const;

  Implementation* const get();
  Implementation const* const get() const;

private:
  std::unique_ptr<Implementation> _impl;
};

template <class Implementation>
template <typename... PARAMS>
unique_propagate_const<Implementation>::unique_propagate_const(PARAMS... params)
  : _impl(std::make_unique<Implementation>(std::forward<PARAMS>(params)...))
{
}
//-------------------------------------------------------------------------------
template <class Implementation>
unique_propagate_const<Implementation>::unique_propagate_const(std::nullptr_t)
  : _impl(std::unique_ptr<Implementation>(nullptr))
{
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr Implementation& unique_propagate_const<Implementation>::operator*()
{
  return *_impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr const Implementation& unique_propagate_const<Implementation>::operator*() const
{
  return *_impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr Implementation* const unique_propagate_const<Implementation>::operator->()
{
  return _impl.operator->();
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr Implementation const* const unique_propagate_const<Implementation>::operator->() const
{
  return _impl.operator->();
}
//-------------------------------------------------------------------------------
template <class Implementation>
Implementation* const unique_propagate_const<Implementation>::get()
{
  return _impl.get();
}
//-------------------------------------------------------------------------------
template <class Implementation>
Implementation const* const unique_propagate_const<Implementation>::get() const
{
  return _impl.get();
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr unique_propagate_const<Implementation>::operator bool() const
{
  return _impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr unique_propagate_const<Implementation>::operator Implementation*()
{
  return _impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr unique_propagate_const<Implementation>::operator const Implementation*() const
{
  return _impl;
}
//-------------------------------------------------------------------------------
}

#endif //BIOGEARS_FRAMEWORK_UNIQUE_PROPAGATE_CONST_H
