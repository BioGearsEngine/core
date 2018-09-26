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
/// @file CompoundUnitElement.h
/// @author Chris Volpe
///
/// This class contains a representation of a single element in a Compound Unit.
/// A Compound Unit is therefore a product of these.
//----------------------------------------------------------------------------
#pragma once

#include <biogears/cdm/utils/unitconversion/SnapValue.h>

namespace biogears {
class CCompoundUnitElement {
public:
  // The use of CSnapValue as the exponent type allows for fractional exponents
  // without resulting in "almost integer" exponents. Why do we want fractional
  // exponents? Well, it allows you to take roots of quantities.
  // For example, if I have a square area given in units of acres and I want
  // to know the length of the square, I can compute the square root with the result
  // being expressed in units of acres^[1/2]. This is a perfectly legitimate unit of
  // distance and can be converted to feet, meters, etc by our existing algorithm. The
  // obvious point that one might make in response is that since any "square-rootable"
  // unit must ultimately have dimensions with even exponents, why not just convert the
  // unit to its expansion into fundamental units and divide those exponents, which must
  // be even, by two? Well, that would either require collusion with client code to do
  // the value conversion, or would require maintaining a separate fudge-factor within
  // the CompoundUnit structure itself. This seems like a hack. After all, since any
  // unit can be squared and cubed, it should be possible to un-square or un-cube any
  // unit whose dimensions are perfect squares and cubes, respectively.
  typedef CSnapValue ExponentType;

  // Need a parameterless constructor to be used in a vector
  CCompoundUnitElement()
  {
    // nothing
  }

  CCompoundUnitElement(int unitId, ExponentType exponent = 1.0, int prefixID = -1)
    : m_iUnitID(unitId)
    , m_CExponent(exponent)
    , m_iPrefixID(prefixID){
      // nothing
    };

  CCompoundUnitElement(const CCompoundUnitElement& src)
    : m_iUnitID(src.m_iUnitID)
    , m_CExponent(src.m_CExponent)
    , m_iPrefixID(src.m_iPrefixID){
      // nothing
    };

  CCompoundUnitElement& operator=(const CCompoundUnitElement& rhs)
  {
    if (this != &rhs) {
      m_iUnitID = rhs.m_iUnitID;
      m_CExponent = rhs.m_CExponent;
      m_iPrefixID = rhs.m_iPrefixID;
    }
    return *this;
  };

  // Setters/Getters
  void SetUnitID(int unitID)
  {
    m_iUnitID = unitID;
  };

  int GetUnitID() const
  {
    return m_iUnitID;
  }

  void SetExponent(const ExponentType& exponent)
  {
    m_CExponent = exponent;
  };

  void AddExponent(const ExponentType& exponent)
  {
    m_CExponent += exponent;
  }

  void MultExponent(const ExponentType& exppwr)
  {
    m_CExponent *= exppwr;
  }

  void SubtractExponent(const ExponentType& exponent)
  {
    m_CExponent -= exponent;
  }

  const ExponentType& GetExponent() const
  {
    return m_CExponent;
  }

  void SetPrefixID(int prefixID)
  {
    m_iPrefixID = prefixID;
  }

  int GetPrefixID() const
  {
    return m_iPrefixID;
  }

  // http://support.microsoft.com/kb/168958 says we need to define operators < and ==
  // for this if we want to export the vector of these contained in CompoundUnit. It
  // even says we can just return true if there's no sensible interpretation of
  // "operator<" for this class.
  bool operator<(const CCompoundUnitElement& ref) const
  {
    return true; // Dummy implementation
  }

  bool operator==(const CCompoundUnitElement& ref) const
  {
    return ((m_iUnitID == ref.m_iUnitID) && (m_CExponent == ref.m_CExponent) && (m_iPrefixID == ref.m_iPrefixID));
  }

  // Convenience method for moving a CompoundUnitElement from the numerator to
  // the denominator, or vice versa
  void Invert()
  {
    m_CExponent *= -1;
  }

  double GetBigness() const;
  double GetBias() const;
  bool IsDecibel() const;

private:
  int m_iUnitID;
  ExponentType m_CExponent;
  int m_iPrefixID;
};
}