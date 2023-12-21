#ifndef BIOGEARS_CONTAINER_MAP_H
#define BIOGEARS_CONTAINER_MAP_H

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

#include <memory>

#include "Pair.h"

//! \author Steven A White
//! \date   Dec 21 2023
//! \copyright Copyright 2023 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file
//!
//! Map implementation with no inline functions
//! This is intended to be used for EXPORTING interfaces
//! to make sure allocations and deletions happen on our side of the DLL

namespace biogears {

template <typename KeyType, typename ValueType>
class Map {
public:
  class Iterator {
    friend class Map<KeyType, ValueType>;
    friend struct Map<KeyType, ValueType>::Implementation;

  public:
    Iterator();
    Iterator(const Iterator&);
    Iterator(Iterator&&);
    ~Iterator();

    ValueType const& value() const;

    Iterator& operator=(Iterator const& rhs);
    Iterator& operator=(Iterator&& rhs);

    bool operator==(Iterator const& rhs) const;
    bool operator!=(Iterator const& rhs) const;

    Pair<KeyType const, ValueType&> const& operator()() const;
    Pair<KeyType const, ValueType&> const* operator->();
    Pair<KeyType const, ValueType&> const& operator*();

    Iterator& operator++(); // Prefix  Operator
    Iterator operator++(int); // Postfix Operator
  private:
    struct IteratorImplementation;
    using implementation = IteratorImplementation;
    Iterator(const implementation&);
    std::unique_ptr<implementation> impl;
  };

  using size_type = typename size_t;
  using value_type = ValueType;
  using reference = value_type&;
  using const_reference = const value_type&;

  using iterator = typename Iterator;
  using const_iterator = typename Iterator const;

  Map();
  Map(const Map&);
  Map(Map&&);

  virtual ~Map();

  auto begin() noexcept -> typename iterator;
  auto end() noexcept -> typename iterator;
  auto begin() const noexcept -> typename const_iterator;
  auto end() const noexcept -> typename const_iterator;
  auto cbegin() const noexcept -> typename const_iterator;
  auto cend() const noexcept -> typename const_iterator;

  iterator find(const KeyType& key);
  const_iterator find(const KeyType& key) const;

  size_type size() const noexcept;
  size_type count(const KeyType& key) const;

  void clear() noexcept;
  bool empty() const noexcept;

  iterator erase(const_iterator pos);
  size_type erase(const KeyType& key);

  ValueType& operator[](const KeyType&) noexcept;
  ValueType const& at(const KeyType&) const;
  ValueType& at(const KeyType&);

  auto operator=(const Map&) -> Map&;
  auto operator=(Map&&) -> Map&;

  bool operator==(const Map&);
  bool operator!=(const Map&);
  bool operator<(const Map&);
  bool operator<=(const Map&);
  bool operator>(const Map&);
  bool operator>=(const Map&);

public:
  struct Implementation;
  using implementation = Implementation;
  Map(const implementation&);
  std::unique_ptr<implementation> impl;
};
}

#endif // BIOGEARS_CONTAINER_GRAPH_H