/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <algorithm>
#include <concepts>
#include <map>
#include <type_traits>
#include <vector>

namespace biogears {
#define BLIM(input, min, max) (input = (input > max) ? max : ((input < min) ? min : input))
#define LLIM(input, min) (input = (input < min) ? min : input)
#define ULIM(input, max) (input = (input > max) ? max : input)
#define LIMIT(A, B, C) (((A) < (B)) ? (B) : (((A) > (C)) ? (C) : (A)))

template <class _InIt>
inline size_t _Hash_value(_InIt _Begin, _InIt _End)
{ // hash range of elements
  size_t _Val = 2166136261U;

  while (_Begin != _End)
    _Val = 16777619U * _Val ^ (size_t)*_Begin++;
  return (_Val);
}

#define DEFINE_STATIC_STRING(name) static constexpr char const* name = #name;
#define DEFINE_STATIC_STRING_EX(name, value) static constexpr char const* name = #value;

#define SAFE_DELETE(obj) \
  {                      \
    delete obj;          \
    obj = nullptr;       \
  }

#define SAFE_DELETE_ARRAY(ary) \
  {                            \
    delete[] ary;              \
    *ary = nullptr;            \
  }

#define INVALIDATE_PROPERTY(name) \
  if (name != nullptr)            \
    name->Invalidate();

#define COPY_PROPERTY(name) \
  if (from.Has##name())     \
    Get##name().Set(*from.m_##name);

#define MERGE_CHILD(name) \
  if (from.Has##name())   \
    Get##name().Merge(*from.m_##name);

template <class ContainerType>
concept Container = requires(ContainerType a) {
  requires std::regular<ContainerType>;
  requires std::swappable<ContainerType>;
  requires std::destructible<typename ContainerType::value_type>;
  requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type&>;
  requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type&>;
  requires std::forward_iterator<typename ContainerType::iterator>;
  requires std::forward_iterator<typename ContainerType::const_iterator>;
  requires std::signed_integral<typename ContainerType::difference_type>;
  requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::iterator>::difference_type>;
  requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
  {
    a.begin()
  } -> std::same_as<typename ContainerType::iterator>;
  {
    a.end()
  } -> std::same_as<typename ContainerType::iterator>;
  {
    a.cbegin()
  } -> std::same_as<typename ContainerType::const_iterator>;
  {
    a.cend()
  } -> std::same_as<typename ContainerType::const_iterator>;
  {
    a.size()
  } -> std::same_as<typename ContainerType::size_type>;
  {
    a.max_size()
  } -> std::same_as<typename ContainerType::size_type>;
  {
    a.empty()
  } -> std::same_as<bool>;
};

template <class T>
inline void Copy(const std::vector<T*>& from, std::vector<T*>& to)
{
  to.clear();
  to.resize(from.size());
  std::copy(from.begin(), from.end(), to.begin());
}

template <class T>
inline void Copy(const std::vector<T>& from, std::vector<T>& to)
{
  to.clear();
  to.resize(from.size());
  std::copy(from.begin(), from.end(), to.begin());
}

template <class T>
inline bool Contains(const std::vector<T*>& v, T& item)
{
  return std::find(v.begin(), v.end(), &item) != v.end();
}

template <class T>
inline void Remove(std::vector<T*>& v, T* item)
{
  v.erase(std::remove(v.begin(), v.end(), item), v.end());
}

template <class T>
inline void Copy(const std::vector<T*>& from, std::vector<const T*>& to)
{
  to.clear();
  for (auto i : from)
    to.push_back(i);
}

template <class T>
inline void DELETE_VECTOR(std::vector<T>& vec)
{
  for (unsigned int i = 0; i < vec.size(); i++) {
    delete vec.at(i);
  }
  vec.clear();
}

template <typename ContainerType>
  requires Container<ContainerType> && std::is_pointer_v<typename ContainerType::value_type>
inline void DELETE_CONTAINER_OF_POINTERS(ContainerType& container)
{
  for (auto& element : container) {
    delete element;
    element = nullptr;
  }
  container.clear();
}
// Th
// This will delete all of the second items in a map and then clear the map to destroy all dangling pointers
template <class T, class K>
inline void DELETE_MAP_OF_POINTERS(std::map<T, K>& map)
{
  for (auto& [key,value] : map) {
    delete value;
    value = nullptr;
  }
  map.clear();
}

template <typename valueType>
bool SameSign(valueType x, valueType y)
{
  return (x >= 0) ^ (y < 0);
}
}