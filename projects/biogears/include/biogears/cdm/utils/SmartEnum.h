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

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>

// Not sure why I have such a facination with enums, but..
// This is a bit of a pet project, I have been curious to see how one can make
// and enum that both, has string mappings and is iterable
// It would really be nice to have unbound wildcard templates in C++ some how...
// I really want to be able to have a compartment manager be populated with
// a set of compartments defined in a custom enum definition
// and have the compartment manager getCompartment method take in the custom type
// And allow for support of multiple custom types.. 
// At anyrate, I am only using this in unit testing
// Found these articles that helped come up with this junk
// http://www.cprogramming.com/c++11/c++11-nullptr-strongly-typed-enum-class.html
// http://www.cprogramming.com/tutorial/template_specialization.html
// http://codereview.stackexchange.com/questions/57626/iterable-enum-class-in-c11
// http://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header?rq=1
// http://stackoverflow.com/questions/6288812/template-in-c

// This is the type that will hold all the strings.
// Each enumerate type will declare its own specialization.
// Any enum that does not have a specialization will generate a compiler error
// indicating that there is no definition of this variable (as there should be
// be no definition of a generic version).
template<typename T>
struct enumStrings
{
  static char const* values[];
};

// Here is a base class that works via an index and string
struct SmartEnum
{
  virtual int index() const = 0;
  virtual const std::string& string() const = 0;
};

// Here are is a macro to fill out the contents of a SmartEnum struct
// You only need to provide the struct, the enum associated (and how many)
// and the strings mapped to those enums

#define SMART_ENUM(Clazz, Type, Length) \
 static constexpr std::size_t _size = Length; \
\
  Clazz() : _idx(0) {} \
  Clazz(Type t) { _idx = static_cast<Type>(t); } \
  virtual ~Clazz() {} \
\
  int index() const { return _idx; } \
  const Type value() const { return Type(_idx); } \
  const char* string() const { return Clazz::Value(_idx); } \
  void set(const Type& t) { _idx = static_cast<int>(t); } \
\
  Type        operator++() { if (_idx != (_size - 1)) _idx++;  return Type(_idx); } \
  Type        operator++(int) { if (_idx != (_size - 1)) _idx++;  return Type(_idx); } \
  bool        operator==(Clazz const& rhs) { return _idx == rhs._idx; } \
  bool        operator!=(Clazz const& rhs) { return _idx != rhs._idx; } \
protected: \
  int    _idx; \
public:   

// Here is an example (You don't need to derive from SmartEnum if you don't want to)
//struct ExampleEnum : public SmartEnum
//{
//  // NOTE, THESE ENUMS MUST BE OF THE DEFAULT INDEXING (You can't say enum Type {A=10, B=30 }
//  enum Type { A, B, C };
//  SMART_ENUM(ExampleEnum, Type, 3);
//  static char const* Value(size_t idx);
//};// Length of enum and values[] must be the same, and reflected in the input to SMART_ENUM
// Put this in your cpp file
//template<> char const* enumStrings<ExampleEnum>::values[] = { "Alpha", "Bravo", "Charlie" };
//char const* ExampleEnum::Value(size_t idx)
//{
//  return enumStrings<ExampleEnum>::values[idx];
//}
//NOTE: You can also templitize this class with the enum type if you want, but how to you set the size?
//I will figure that out when/if I need it

