#ifndef BIOGEARS_CONTAINER_TREE_TCI_H
#define BIOGEARS_CONTAINER_TREE_TCI_H

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

//! \author Steven A White
//! \date   Sept 7 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file
//!
//! Bounded Concurrent Queue using std::queue std::mutex.
//!

#include <biogears/container/Tree.h>

namespace biogears {
//!
//! Constructs a subtree with a root node value of the given
template <typename Data>
Tree<Data>::Tree(const Data& value)
  : _value(value)
{
}
//-------------------------------------------------------------------------------
//!
//! Constructs a subtree with a root node value of the given
template <typename Data>
Tree<Data>::Tree(Data&& value)
  : _value(std::move(value))
{
}
//-------------------------------------------------------------------------------
//!
//! Constructs a subtree with a root node value of the given
template <typename Data>
Tree<Data>::Tree(const Tree& obj)
  : _value(obj._value)
  , _children(obj._children)
{
}
//-------------------------------------------------------------------------------
//!
//! Constructs a subtree with a root node value of the given
template <typename Data>
Tree<Data>::Tree(Tree&& obj)
  : _value(std::move(obj._value))
  , _children(std::move(obj._children))
{
}
//-------------------------------------------------------------------------------
template <typename Data>
template< typename ... PARAMS >
Tree<Data>::Tree(const PARAMS&... params)
:_value(params...)
{
  
}
//-------------------------------------------------------------------------------
//!
//! \return const_refrence : A const Data& to the value of the current Node.
//!
//!  Accessor for the value of the current node
template <typename Data>
auto Tree<Data>::value() const -> const_reference
{
  return _value;
}
//-------------------------------------------------------------------------------
//!
//! \return const_refrence : A const Data& to the value of the current Node.
//!
//!  Accessor for the value of the current node
template <typename Data>
auto Tree<Data>::value() -> reference
{
  return _value;
}
//-------------------------------------------------------------------------------
//!
//! \return refrence : A Data& to the value of the current Node.
//!
//! Right hand side builder interface for setting value.
template <typename Data>
Tree<Data>& Tree<Data>::value(const Data& value) &&
{
  _value = value;
  return *this;
}
//-------------------------------------------------------------------------------
//!
//! \param  Data& [IN] : New value of the current node
//!.
//!.Sets the current nodes value
template <typename Data>
void Tree<Data>::value(const Data& value) &
{
  _value = value;
}
//-------------------------------------------------------------------------------
//!
//! \return std::vector<Tree<Data>> : Returns a const vector reference to the children of the node
//!
//! Access children of the current node directly
template <typename Data>
auto Tree<Data>::children() const -> const child_vector&
{
  return _children;
}
//-------------------------------------------------------------------------------
//!
//! \return std::vector<Tree<Data>> : Returns a vector reference to the children of the node
//!
//! Access children of the current node directly
template <typename Data>
auto Tree<Data>::children() -> child_vector&
{
  return _children;
}
//-------------------------------------------------------------------------------
//!
//! \param  const std::vector<Tree<Data>>& [IN] : New vector of children for this node.
//!
//! Replaces curent children with the ones provided.
template <typename Data>
void Tree<Data>::children(const std::vector<Tree>& children)
{
  _children = children;
}
//-------------------------------------------------------------------------------
//!
//! \param  const std::vector<Tree<Data>>&& [INOUT] : New vector of children for this node.
//!
//! Replaces curent children with the ones provided.
template <typename Data>
void Tree<Data>::children(std::vector<Tree>&& children)
{
  _children = std::move(children);
}
//-------------------------------------------------------------------------------
//!
//! \return child_vector::iterator : Iterator to the first element of children
//!
template <typename Data>
auto Tree<Data>::begin() noexcept -> typename child_vector::iterator
{
  return _children.begin();
}
//-------------------------------------------------------------------------------
//!
//! \return child_vector::const_iterator : Iterator to the end() of underlying child container
//!
template <typename Data>
auto Tree<Data>::end() const noexcept -> typename child_vector::const_iterator
{
  return _children.end();
}
//-------------------------------------------------------------------------------
//!
//! \return child_vector::const_iterator : Iterator to the first element of children
//!
template <typename Data>
auto Tree<Data>::begin() const noexcept -> typename child_vector::const_iterator
{
  return _children.begin();
}
//-------------------------------------------------------------------------------
//!
//! \return child_vector::const_iterator : Iterator to the end() of underlying child container
//!
template <typename Data>
auto Tree<Data>::cend() const noexcept -> typename child_vector::const_iterator
{
  return _children.cend();
}
//-------------------------------------------------------------------------------
//!
//! \return child_vector::const_iterator : Iterator to the first element of children
//!
template <typename Data>
auto Tree<Data>::cbegin() const noexcept -> typename child_vector::const_iterator
{
  return _children.cbegin();
}
//-------------------------------------------------------------------------------
//!
//! \return child_vector::iterator : Iterator to the end() of underlying child container
//!
template <typename Data>
auto Tree<Data>::end() noexcept -> typename child_vector::iterator
{
  return _children.end();
}
//-------------------------------------------------------------------------------
//! Returns a reference to the element at specified location pos. No bounds checking is performed.
template <typename Data>
auto Tree<Data>::operator[](size_t pos) -> reference
{
  return _children[pos];
}
//-------------------------------------------------------------------------------
//! Returns a const_reference to the element at specified location pos. No bounds checking is performed.
template <typename Data>
auto Tree<Data>::operator[](size_t pos) const -> const_reference
{
  return _children[pos];
}
//-------------------------------------------------------------------------------
//! Assignment Operator by copy
template <typename Data>
auto Tree<Data>::operator=(const Tree& rhs) -> Tree&
{
  if (&*this != &rhs)
  {
    _value = rhs._value;
    _children = rhs._children;
  }
  return *this;
}
//-------------------------------------------------------------------------------
//! assignment Operator by move
template <typename Data>
auto Tree<Data>::operator=(Tree&& rhs) -> Tree&
{
  if( &*this != &rhs)
  {
    _value = std::move(rhs._value);
    _children = std::move(rhs._children);
  }
  return *this;
}
}

#endif //BIOGEARS_CONTAINER_TREE_TCI_H