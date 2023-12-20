#ifndef BIOGEARS_CONTAINER_PAIR_H
#define BIOGEARS_CONTAINER_PAIR_H

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
//! \date   Dec 21 2023
//! \copyright Copyright 2023 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file
//!
//! Pair implementation with no inline functions
//! This is intended to be used for EXPORTING interfaces 
//! to make sure allocations and deletions happen on our side of the DLL

#include <vector>

namespace biogears {

template <typename Type1, typename Type2>
class Pair {
public:
  Pair();
  Pair(const Type1&, const Type2&);
  Pair(Type1&&, Type2&&);
  Pair(const Pair&);
  Pair(Pair&&);

  virtual ~Pair();

  auto operator=(const Pair&) -> Pair&;
  auto operator=(Pair&&) -> Pair&;

  bool operator==(const Pair&);
  bool operator!=(const Pair&);
  bool operator<(const Pair&);
  bool operator<=(const Pair&);
  bool operator>(const Pair&);
  bool operator>=(const Pair&);

public:
  Type1 first;
  Type2 second;

};
}

#endif // BIOGEARS_CONTAINER_GRAPH_H