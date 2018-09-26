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

//----------------------------------------------------------------------------
/// @file UnitDimension.h
/// @author Chris Volpe
///
/// This class defines a representation of the dimensions of a Compound Unit
/// that is used for purposes of dimensional analysis
//----------------------------------------------------------------------------
#pragma once
#include <algorithm>
#include <vector>
#include <cmath>
#include <functional>

#include <biogears/cdm/Macros.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnitElement.h>

// The nth element in our array contains the total exponent of the nth fundamental
// quantity type in the transitive closure of the expansion of each unit in
// a compound unit structure. The purpose of this object is to determine whether
// two CompoundUnits are dimensionally compatible (i.e. whether one can be converted
// to the other and vice-versa).
namespace biogears {
class CUnitDimension {
public:
  using ExponentType =  CCompoundUnitElement::ExponentType;
  using ExponentList =  std::vector<ExponentType>;

  // Default ctor: size the array according to the number of fundamental quantities
  CUnitDimension();

  // Copy ctor.
  CUnitDimension(const CUnitDimension& src)
    : m_EList(src.m_EList){

    };

  size_t size() const
  {
    return m_EList.size();
  }

  // Vectors of ints are probably initialized to zero anyway upon creation,
  // but I'd rather not rely on that.
  void InitElems();

  // Operators that allow CCompoundUnit to construct our elements
  // Always a good idea to overload operator= if providing a copy constructor
  CUnitDimension& operator=(const CUnitDimension& rhs)
  {
    m_EList = rhs.m_EList;
    return *this;
  };

  // Provide interface to indexing to the underlying vector elements. Alternative
  // is exposing the vector pointer, which would be somewhat ugly.
  CCompoundUnitElement::ExponentType& operator[](size_t pos)
  {
    return m_EList[pos];
  };

  // Not sure if we'd ever need to index into a const CCompoundUnitElement, but
  // completeness is always a good thing.
  CCompoundUnitElement::ExponentType const& operator[](size_t pos) const
  {
    return m_EList[pos];
  };

  // Test two CUnitDimension objects for equality. Here's where we want behavior
  // that goes above and beyond what a vector does all by itself. It's conceivable,
  // though unlikely, that application code may add new fundamental quantities
  // after CUnitDimension objects have already been created. Thus, two CUnitDimension
  // objects can be "equal" even if they're not the same size, so long as the longer
  // one has zeros in its elements that are beyond the length of the shorter one.
  bool operator==(const CUnitDimension& rhs) const
  {
    if (m_EList.size() == rhs.m_EList.size()) {
      return (m_EList == rhs.m_EList);
    } else {
      size_t shorter = std::min<size_t>(m_EList.size(), rhs.m_EList.size());
      size_t longer = std::max<size_t>(m_EList.size(), rhs.m_EList.size());
      // First compare over elements they have in common
      for (size_t i = 0; i < shorter; ++i) {
        if (m_EList[i] != rhs.m_EList[i]) {
          return false;
        }
      }
      // Now figure out which vector is the longer one and check its extra
      // elements to make sure they're all zero
      const ExponentList& longerList = (m_EList.size() == longer ? m_EList : rhs.m_EList);
      for (size_t i = shorter; i < longer; ++i) {
        if (longerList[i] != 0.0) {
          return false;
        }
      }
      return true;
    }
  };

  // Implement operator!= for sake of completeness
  bool operator!=(const CUnitDimension& rhs) const
  {
    return !(*this == rhs);
  }

  // Adding and subtracting dimension objects allows us to take shortcuts
  // when multiplying and dividing CCompoundUnit objects whose CUnitDimension
  // objects have already been built
  CUnitDimension& operator+=(const CUnitDimension& rhs)
  {
    size_t shorter = std::min<size_t>(m_EList.size(), rhs.m_EList.size());
    size_t longer = std::max<size_t>(m_EList.size(), rhs.m_EList.size());
    // First combine over elements they have in common
    for (size_t i = 0; i < shorter; ++i) {
      m_EList[i] += rhs.m_EList[i];
    }

    // If the right hand side is longer than *this, then resize *this and copy
    // the extra elements
    if ((longer > shorter) && (rhs.m_EList.size() == longer)) {
      m_EList.resize(longer);
      for (size_t i = shorter; i < longer; ++i) {
        m_EList[i] = rhs.m_EList[i];
      }
    } // if we need to copy more
    return *this;
  }

  CUnitDimension& operator-=(const CUnitDimension& rhs)
  {
    size_t shorter = std::min<size_t>(m_EList.size(), rhs.m_EList.size());
    size_t longer = std::max<size_t>(m_EList.size(), rhs.m_EList.size());
    // First combine over elements they have in common
    for (size_t i = 0; i < shorter; ++i) {
      m_EList[i] -= rhs.m_EList[i];
    }

    // If the right hand side is longer than *this, then resize *this and copy
    // the extra elements
    if ((longer > shorter) && (rhs.m_EList.size() == longer)) {
      m_EList.resize(longer);
      for (size_t i = shorter; i < longer; ++i) {
        m_EList[i] = -rhs.m_EList[i];
      }
    } // if we need to copy more
    return *this;
  }

  const CUnitDimension operator+(const CUnitDimension& rhs) const
  {
    return CUnitDimension(*this) += rhs;
  }

  const CUnitDimension operator-(const CUnitDimension& rhs) const
  {
    return CUnitDimension(*this) -= rhs;
  }

  // Multiplying CUnitDimension by a scalar allows us to take shortcuts when
  // raising to a power a CCompoundUnit object whose CUnitDimension object
  // has already been computed
  CUnitDimension& operator*=(const double& rhs)
  {
    size_t len = m_EList.size();
    for (size_t i = 0; i < len; ++i) {
      m_EList[i] *= rhs;
    }
    return *this;
  }

  const CUnitDimension operator*(const double& rhs) const
  {
    return CUnitDimension(*this) *= rhs;
  }

  // Determine if this UnitDimension object corresponds to a single
  // specified fundamental quantity
  bool IsFundamentalQuantity(size_t fundIdx) const
  {
    size_t len = m_EList.size();

    // Rule out invalid params
    if (fundIdx >= len) {
      return false;
    }

    int requiredVal;
    for (size_t i = 0; i < len; ++i) {
      //Must be 1 at the fundamental quantity index location, and zero everywhere else
      requiredVal = ((i == fundIdx) ? 1 : 0);
      if (m_EList[i] != requiredVal) {
        return false;
      }
    }
    return true;
  }

  // Determine if this UnitDimension object is dimensionless
  bool IsDimensionless() const
  {
    size_t len = m_EList.size();
    for (size_t i = 0; i < len; ++i) {
      if (m_EList[i] != 0) {
        return false;
      }
    }
    return true;
  }

  // Since we use CUnitDimension objects as the basis for a hashmap key in
  // CQuantityConversionKey, we need to implement a couple of methods for
  // use by the CQuantityConversionKey object to help it implement the
  // methods it needs in order for the default hash_map templated hash_compare
  // object to work properly. Specifically, we need "operator<" to be defined
  // properly, and we need a hash_value function.

  bool operator<(const CUnitDimension& rhs) const
  {
    size_t shorter = std::min<size_t>(m_EList.size(), rhs.m_EList.size());
    size_t longer = std::max<size_t>(m_EList.size(), rhs.m_EList.size());
    // First compare over elements they have in common
    for (size_t i = 0; i < shorter; ++i) {
      if (m_EList[i] > rhs.m_EList[i]) {
        return false;
      } else if (m_EList[i] < rhs.m_EList[i]) {
        return true;
      }
    }

    // If we got through the above loop without exiting, then all the
    // elements in common are equal. If the vector lengths are the same,
    // then *all* the elements are equal, and the less-than relationship
    // is false
    if (shorter == longer) {
      return false;
    }

    // The shorter vector is considered to be padded with zeros.
    // Thus, compare the longer vector's elements to the imaginary
    // zeros on the shorter
    if (m_EList.size() > rhs.m_EList.size()) {
      // ok, the left side is longer. Compare the excess left-side elements
      // to the imaginary padded zeros
      for (size_t i = shorter; i < longer; ++i) {
        if (m_EList[i] > 0.0) {
          return false;
        } else if (m_EList[i] < 0.0) {
          return true;
        }
      }
    } // left side is longer
    else {
      // ok, the right side is longer. Compare the excess right-side elements
      // to the imaginary padded zeros
      for (size_t i = shorter; i < longer; ++i) {
        if (rhs.m_EList[i] > 0.0) {
          return true;
        } else if (rhs.m_EList[i] < 0.0) {
          return false;
        }
      }
    } // right side is longer

    // If we get to this point, then the excess elements are all zeros,
    // which means that the two CUnitDimension objects are equal, which
    // means the less-than relationship is false.
    return false;
  }

  // For the sake of completeness only, implement the other relational operators
  bool operator>(const CUnitDimension& rhs) const
  {
    return rhs < *this;
  }

  bool operator<=(const CUnitDimension& rhs) const
  {
    return !(*this > rhs);
  }

  bool operator>=(const CUnitDimension& rhs) const
  {
    return !(*this < rhs);
  }

  // This hash value must be invariant with respect to trailing zeros
  // at the end of the vector. See comment above operator==.
  size_t hash_value() const
  {
    ExponentList::const_iterator iend = m_EList.end();
    ExponentList::const_iterator ibgn = m_EList.begin();
    // Find first trailing zero as new "end"
    while ((iend != ibgn) && *--iend == 0)
      ;
    iend++;

    // Now invoke the collection hash routine provided in <xhash>
    return _Hash_value(ibgn, iend);
  }

private:
  ExponentList m_EList;
};

// The member function operators work when the right operand is a double, but not
// when the left operand is a double. So write a non-member operator* that
// computes the result by invoking the member operator* with operands in the
// reverse order. This is fine because multiplication of a CUnitDimension and
// a scalar double val is an abelian operation.
inline const CUnitDimension operator*(const double& lhs, const CUnitDimension& rhs)
{
  return rhs * lhs;
}

// Overload non_member hash_value on CUnitDimension so that the
// templated hash_compare used by hash_map can call it.
inline size_t hash_value(const CUnitDimension& ref)
{
  return ref.hash_value();
}
}
namespace std {
template <>
struct hash<biogears::CUnitDimension> {
  size_t operator()(const biogears::CUnitDimension& ref) const
  {
    return ref.hash_value();
  }
};
}
