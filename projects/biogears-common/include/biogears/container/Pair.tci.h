#ifndef BIOGEARS_CONTAINER_PAIR_TCI_H
#define BIOGEARS_CONTAINER_PAIR_TCI_H

#include "Pair.h"

namespace biogears {

template <typename Type1, typename Type2>
Pair<Type1, Type2>::Pair()
  : first()
  , second()
{
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
Pair<Type1, Type2>::Pair(const Type1& f, const Type2& s)
  : first(f)
  , second(s)
{
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
Pair<Type1, Type2>::Pair(Type1&& f, Type2&& s)
  : first(std::move(f))
  , second(std::move(s))
{
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
Pair<Type1, Type2>::Pair(const Pair& p)
  : first(p.first)
  , second(p.second)
{
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
Pair<Type1, Type2>::Pair(Pair&& p)
  : first(std::move(p.first))
  , second(std::move(p.second))

{
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
Pair<Type1, Type2>::~Pair()
{
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
auto Pair<Type1, Type2>::operator=(const Pair& p) -> Pair&
{
  first = p.first;
  second = p.second;
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
auto Pair<Type1, Type2>::operator=(Pair&& p) -> Pair&
{
  first = std::move(p.first);
  second = std::move(p.second);
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator==(const Pair& p)
{
  return p.first == first && p.second == second;
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator!=(const Pair& p)
{
  return !(*this == p);
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator<(const Pair& p) 
{
  return p.first < first && p.second < second;
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator<=(const Pair& p) 
{
  return p.first <= first && p.second <= second;
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator>(const Pair& p)
{
  return p.first > first && p.second > second;
}
//-------------------------------------------------------------------------------
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator>=(const Pair& p) 
{
  return p.first > first && p.second > second;
}

}
#endif // BIOGEARS_CONTAINER_GRAPH_TCI_H