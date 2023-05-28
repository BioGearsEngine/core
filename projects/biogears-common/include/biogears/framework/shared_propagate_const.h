#ifndef BIOGEARS_FRAMEWORK_SHARED_PROPAGATE_CONST_H
#define BIOGEARS_FRAMEWORK_SHARED_PROPAGATE_CONST_H

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
//! \file
//! \author Steven A White
//! \date   November 1st 2017
//! \brief  The goal of this class is to allow
//!         unique_ptr and shared_ptr to properly propagate constness
//!         and prevent the use of non const functions from unique_ptr
//!         In general the class should never point to a null object

#include <memory>

#include <biogears/common-exports.h>

OPEN_BIOGEARS_NAMESPACE

template <class Implementation>
class shared_propagate_const {
public:
  template <typename... PARAMS>
  shared_propagate_const(PARAMS...);
  shared_propagate_const(std::nullptr_t);

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
  std::shared_ptr<Implementation> _impl;
};

template <class Implementation>
template <typename... PARAMS>
shared_propagate_const<Implementation>::shared_propagate_const(PARAMS... params)
  : _impl(std::make_unique<Implementation>(std::forward<PARAMS>(params)...))
{
}
//-------------------------------------------------------------------------------
template <class Implementation>
shared_propagate_const<Implementation>::shared_propagate_const(std::nullptr_t)
  : _impl(std::unique_ptr<Implementation>(nullptr))
{
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr Implementation& shared_propagate_const<Implementation>::operator*()
{
  return *_impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr const Implementation& shared_propagate_const<Implementation>::operator*() const
{
  return *_impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr Implementation* const shared_propagate_const<Implementation>::operator->()
{
  return _impl.operator->();
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr Implementation const* const shared_propagate_const<Implementation>::operator->() const
{
  return _impl.operator->();
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr shared_propagate_const<Implementation>::operator bool() const
{
  return _impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr shared_propagate_const<Implementation>::operator Implementation*()
{
  return _impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
constexpr shared_propagate_const<Implementation>::operator const Implementation*() const
{
  return _impl;
}
//-------------------------------------------------------------------------------
template <class Implementation>
Implementation* const shared_propagate_const<Implementation>::get()
{
  return _impl.get();
}
//-------------------------------------------------------------------------------
template <class Implementation>
Implementation const* const shared_propagate_const<Implementation>::get() const
{
  return _impl.get();
}
//-------------------------------------------------------------------------------
CLOSE_BIOGEARS_NAMESPACE

#endif //BIOGEARS_FRAMEWORK_SHARED_PROPAGATE_CONST_H
