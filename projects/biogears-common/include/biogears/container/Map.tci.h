#ifndef BIOGEARS_CONTAINER_MAP_TCI_H
#define BIOGEARS_CONTAINER_MAP_TCI_H

#include <map>

#include "Map.h"
#include "Pair.tci.h"

namespace biogears {

template <typename KeyType, typename ValueType>
struct Map<KeyType, ValueType>::Iterator::IteratorImplementation {

  friend class Map<KeyType, ValueType>;
  friend class Map<KeyType, ValueType>::Iterator;

  IteratorImplementation()
    : iterator()
    , cache()
  {
  }

  IteratorImplementation(typename std::map<KeyType, ValueType>::iterator m)
    : iterator(m)
    , cache(nullptr)
  {
  }

  IteratorImplementation(IteratorImplementation const& obj)
    : iterator(obj.iterator)
    , cache(nullptr)
  {
  }

  template <typename KeyType, typename ValueType>
  auto operator=(Iterator const& rhs) -> IteratorImplementation&
  {
    iterator = rhs->iterator;
    if (cache != nullptr) {
      cache = std::make_unique<Pair<KeyType const, ValueType&>>(rhs->cache);
    }
    return *this;
  }

  typename std::map<KeyType, ValueType>::iterator iterator;
  std::unique_ptr<Pair<KeyType const, ValueType&>> cache;
};

template <typename KeyType, typename ValueType>
struct Map<KeyType, ValueType>::Implementation {

  friend Map<KeyType, ValueType>;

  Implementation()
    : map()
  {
  }

  Implementation(std::map<KeyType, ValueType>&& m)
    : map(std::move(m))
  {
  }

  std::map<typename KeyType, typename ValueType> map;
};

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Iterator::Iterator()
  : impl(std::make_unique<IteratorImplementation>())
{
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Iterator::Iterator(const IteratorImplementation& obj)
  : impl(std::make_unique<IteratorImplementation>(obj))
{
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Iterator::Iterator(const Iterator& obj)
  : impl(std::make_unique<IteratorImplementation>(*obj.impl))
{
}
template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Iterator::Iterator(Iterator&& obj)
  : impl(std::move(obj.impl))
{
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Iterator::~Iterator()
{
}

template <typename KeyType, typename ValueType>
ValueType const& Map<KeyType, ValueType>::Iterator::value() const
{
  return impl->iterator->second;
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::Iterator::operator=(Iterator const& rhs) -> Iterator&
{
  impl->iterator = rhs.impl->iterator;
  return *this;
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::Iterator::operator=(Iterator&& rhs) -> Iterator&
{
  impl->iterator = std::move(rhs.impl->iterator);
  return *this;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Iterator::operator==(Iterator const& rhs) const
{
  return impl->iterator == rhs.impl->iterator;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Iterator::operator!=(Iterator const& rhs) const
{
  return !(*this == rhs);
}

template <typename KeyType, typename ValueType>
Pair<KeyType const, ValueType&> const& Map<KeyType, ValueType>::Iterator::operator()() const
{
  if (!impl->cache) {
    impl->cache = std::make_unique<Pair<KeyType const, ValueType&>>(impl->iterator->first, impl->iterator->second);
  }
  return *impl->cache;
}

template <typename KeyType, typename ValueType>
Pair<KeyType const, ValueType&> const* Map<KeyType, ValueType>::Iterator::operator->()
{
  if (!impl->cache) {
    impl->cache = std::make_unique<Pair<KeyType const, ValueType&>>(impl->iterator->first, impl->iterator->second);
  }
  return impl->cache.get();
}

template <typename KeyType, typename ValueType>
Pair<const KeyType, ValueType&> const& Map<KeyType, ValueType>::Iterator::operator*()
{
  if (!impl->cache) {
    impl->cache = std::make_unique<Pair<KeyType const, ValueType&>>(impl->iterator->first, impl->iterator->second);
  }
  return *impl->cache;
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::Iterator::operator++() -> Iterator&
{
  ++impl->iterator;
  this->impl->cache = std::make_unique<Pair<KeyType const, ValueType&>>(impl->iterator->first, impl->iterator->second);
  return *this;
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::Iterator::operator++(int) -> Iterator
{
  Iterator& v = *this;
  ++impl->iterator;
  impl->cache = std::make_unique<Pair<KeyType const, ValueType&>>(impl->iterator->first, impl->iterator->second);
  return v;
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map()
  : impl(std::make_unique<Implementation>())
{
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map(const Map& obj)
  : impl(std::make_unique<Implementation>(*obj.impl))
{
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map(Map&& obj)
  : impl(std::move(obj.impl))
{
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::~Map()
{
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::begin() noexcept -> typename iterator
{
  return impl->map.begin();
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::end() noexcept -> typename iterator
{
  return impl->map.begin();
}

//!
//! We need to create an actual const_iterator class instead of
//! depending on cost versions of Iterator to meet the stl requirments
//!
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::begin() const noexcept -> typename const_iterator
{
  return Iterator::IteratorImplementation(std::move(impl->map.begin()));
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::end() const noexcept -> typename const_iterator
{
  return Iterator::IteratorImplementation(std::move(impl->map.end()));
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::cbegin() const noexcept -> typename const_iterator
{
  return Iterator::IteratorImplementation(impl->map.begin());
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::cend() const noexcept -> typename const_iterator
{
  return Iterator::IteratorImplementation(impl->map.end());
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::find(const KeyType& key) -> iterator
{
  return Iterator(Iterator::implementation(impl->map.find(key)));
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::find(const KeyType& key) const -> const_iterator
{
  return Iterator(Iterator::implementation(impl->map.find(key)));
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::size() const noexcept -> size_type
{
  return impl->map.size();
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::count(const KeyType& key) const -> size_type
{
  return impl->map.count(key);
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::clear() noexcept
{
  return impl->map.clear();
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::empty() const noexcept
{
  return impl->map.empty();
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::erase(const_iterator pos) -> iterator
{
  return impl->map.erase(pos.impl->iterator);
}
template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::erase(const KeyType& key) -> size_type
{
  return impl->map.erase(key);
}

template <typename KeyType, typename ValueType>
ValueType& Map<KeyType, ValueType>::operator[](const KeyType& index) noexcept
{
  return impl->map[index];
}

template <typename KeyType, typename ValueType>
ValueType const& Map<KeyType, ValueType>::at(const KeyType& key) const
{
  return impl->map.at(key);
}

template <typename KeyType, typename ValueType>
ValueType& Map<KeyType, ValueType>::at(const KeyType& key)
{
  return impl->map.at(key);
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::operator=(const Map& rhs) -> Map&
{
  *this->impl = *rhs.impl;
  return *this;
}

template <typename KeyType, typename ValueType>
auto Map<KeyType, ValueType>::operator=(Map&& rhs) -> Map&
{
  this->impl = std::move(rhs.impl);
  return *this;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator==(const Map& rhs)
{
  return impl == rhs.impl;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator!=(const Map& rhs)
{
  return !(impl == rhs.impl);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator<(const Map& rhs)
{
  return impl < rhs.impl;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator<=(const Map& rhs)
{
  return impl <= rhs.impl;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator>(const Map& rhs)
{
  return impl > rhs.impl;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator>=(const Map& rhs)
{
  return impl >= rhs.impl;
}

}

#endif
