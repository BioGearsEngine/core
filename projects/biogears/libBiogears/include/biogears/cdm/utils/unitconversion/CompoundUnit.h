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
/// @file CompoundUnit.h
/// @author Chris Volpe
///
/// This class defines an internal representation of a Compound Unit, which is the
/// product of individual unit symbols, each with a scalar prefix and raised to
/// some exponent.
//----------------------------------------------------------------------------
#pragma once

// Let's try to make this as lightweight as possible so that CCompoundUnit
// objects can be efficiently manipulated within the context of an enclosing
// physical quantity object (name TBD: PhysicsQuantity? DimensionedQuantity?
// How about just Quantity?) when arithmetic with them needs to be done. This
// means maximizing use of inline functions, even those that manipulate the
// CUEVecType elements, which in turn requires dealing with Microsoft's BS for
// exporting STL classes and classes containing STL data objects as data members.
// (See http://support.microsoft.com/kb/168958) The only STL class that you can do
// this with is vector<>, but fortunately that's all we need here. Usually, I get
// around this by declaring the data objects as pointers to STL template
// instantiations that I dynamically allocate and manipulate in non-inline functions,
// but since we're going for inline methods here, doing so wouldn't buy me anything,
// so I may as well make them first class data members. That may have the added
// benefit of coaxing out any helpful warning messages that the compiler might
// mistakenly think are not relevant when the data members are merely pointers to
// STL classes. -CRV

// Note: The ordering of the following two declarations is vital. The above-mentioned
// knowledge base article only talks about exporting the vector class that I'm
// explicitly instantiating, but that results in an error message about the allocator
// needing to be exported. Adding the allocator export after the vector export didn't
// solve the problem. But the paragraph about nested classes in the KB article alludes to
// a circular dependency that manifests itself as different error messages depending
// on which thing you export first. So, on a hunch, I reversed the order, and voila!

//Standard includes

#include <vector>
#include <fstream>
#include <string>
//Project Includes
#include <biogears/exports.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnitElement.h>
#include <biogears/cdm/utils/unitconversion/UnitDimension.h>

namespace biogears {
class BIOGEARS_API CCompoundUnit {
  // Define the vector type that holds our individual components of a CompoundUnit
  typedef std::vector<CCompoundUnitElement> CUEVecType;
public:
  // Default ctor
  CCompoundUnit();
  // Construct directly from a unit string specification
  CCompoundUnit(const char* unitString);
  // Construct directly from a unit string specification
  CCompoundUnit(const std::string& unitString);
  // Copy ctor
  CCompoundUnit(const CCompoundUnit& src);

  // dtor
  virtual ~CCompoundUnit()
  {
    if (m_CUD) {
      delete m_CUD;
    }
  };

  // Obtain a dimension object that uniquely represents the dimensions of this
  // compound unit in terms of fundamental quantities
  const CUnitDimension* GetDimension() const;

  // Obtain the relative magnitude of this compound unit in relation to
  // an equivalently-dimensioned compound unit comprised of the base units
  // of each fundamental quantity type. The bigness is the key to unit conversion.
  // For any compound unit of a given dimension, the value of a quantity in that
  // unit times the bigness of that unit is an invariant quantity.
  const double& GetBigness() const;

  // Obtain the bias of this compound unit. Biases denote additive offsets of one unit
  // with respect to another unit of the same type. I am currently operating under the
  // assumption that it is nonsensical to process biases in a compound unit consisting
  // of more than one fundamental quantity type raised to the power of 1.0. If I come
  // across a counter-example to this, I may have to re-think things, but for now, I am
  // assuming that when multiplying a biased unit by other quantities, the bias must be
  // irrelevant. Currently, the only quantity type that has biased units is temperature,
  // and this principle seems to hold. E.g., J/degC means the same thing as J/degK even
  // though there's a bias in converting from degC to degK.
  double GetBias() const;

  // We can do arithmetic with these, so overload a few operators
  // But do *NOT* define operator+, operator-, or the assignment
  // versions of these operators. You cannot "add" two CompoundUnit
  // objects. You can add two *values* if they have equivalent
  // compound unit objects, but the only operations that
  // can be performed on a compound unit object itself are multiplication,
  // division, and raising to a power.
  CCompoundUnit& operator=(const CCompoundUnit& rhs)
  {
    if (this != &rhs) {
      m_dBigness = rhs.m_dBigness;
      m_bDBFlag = rhs.m_bDBFlag;
      m_bExplicitDBFlag = rhs.m_bExplicitDBFlag;
      m_bExplicitNonDBFlag = rhs.m_bExplicitNonDBFlag;

      // Vector objects should know how to deep-copy themselves
      m_CUEVec = rhs.m_CUEVec;
      // Free my dimension if I have one, whether it's stale or now
      if (m_CUD) {
        delete m_CUD;
        m_CUD = nullptr;
      }
      // We need to deep-copy the CUnitDimension, not copy pointers
      // But only copy if RHS is not stale. No sense duplicating
      // something that's stale. Also, if it's nullptr, it's stale, so
      // no need for a separate nullptr check.
      if (!rhs.m_bStaleDimension) {
        m_CUD = new CUnitDimension(*rhs.m_CUD);
      }
      m_bStaleBigness = rhs.m_bStaleBigness;
      m_bStaleDimension = rhs.m_bStaleDimension;
    }
    return *this;
  };

  CCompoundUnit& operator*=(const CCompoundUnit& rhs);
  CCompoundUnit& operator/=(const CCompoundUnit& rhs);

  CCompoundUnit operator*(const CCompoundUnit& rhs) const
  {
    return CCompoundUnit(*this) *= rhs;
  }

  CCompoundUnit operator/(const CCompoundUnit& rhs) const
  {
    return CCompoundUnit(*this) /= rhs;
  }

  // There's no "raise to a power" operator, but this is the next best thing.
  CCompoundUnit& Raise(CCompoundUnitElement::ExponentType);

  // Compare two CompoundUnits
  // We don't care about how the units are precisely represented, whether
  // one contains "J" and the other contains "kg m^2 s^-2", or "s^-2 m^2 kg"
  // for that matter. We care about whether they are equivalent. And they are
  // equivalent if and only if they are equivalent in both dimension and bigness.
  bool operator==(const CCompoundUnit& rhs) const
  {
    return ((*this->GetDimension() == *rhs.GetDimension()) && (this->GetBigness() == rhs.GetBigness()) && (this->GetBias() == rhs.GetBias()));
  }

  bool operator!=(const CCompoundUnit& rhs) const
  {
    return !(*this == rhs);
  }

  // Is this CompoundUnit dimensionally-compatible with a pre-defined
  // quantity type?
  bool IsOfType(int quantityTypeID);
  bool IsOfType(const char* quantityName);
  bool IsOfType(const std::string& quantityName);

  // Is this CompoundUnit dimensionless
  bool IsDimensionless() const
  {
    return this->GetDimension()->IsDimensionless();
  }

  // Is this CompoundUnit in "decibel" mode
  bool IsDecibel() const;

  bool IsUnitEmpty() const
  {
    return this->m_strUnit.empty();
  }

  // Returns either 10 or 20, depending on whether this compound unit is of a
  // Quantity Type that obeys the 20-Log-Rule
  double GetDecibelLogScaleFactor() const;

  void DecibelModeOn()
  {
    // Make this explicitly decibel mode, in case we need to do a conversion to/from dB.
    // We need explicit flags for each condition, so that with both of them off, the
    // dB state is driven by the presence of a dB mode in the Compound Unit Elements
    // (through the CompoundUnit expansion of a Unit Descriptor).
    m_bExplicitDBFlag = true;
    m_bExplicitNonDBFlag = false;
    m_bDBFlag = true;
  }

  void DecibelModeOff()
  {
    // Make this explicitly not-decibel mode
    m_bExplicitDBFlag = false;
    m_bExplicitNonDBFlag = true;
    m_bDBFlag = false;
  }

  // Used for incrementally building up a CompoundUnit
  CCompoundUnitElement& AddElement(const CCompoundUnitElement&);

  // Erase everything
  void clear()
  {
    if (m_CUD) {
      delete m_CUD;
      m_CUD = nullptr;
    }
    m_dBigness = 1.0;
    m_bStaleBigness = true;
    m_bStaleDimension = true;
    // The explicit dB flag is true if the CompoundUnit explicitly contained a "dB" token.
    // The regular dB flag is true if either the explicit dB flag is true or if any of the
    // Compound Unit Elements refer to Unit Descriptors whose Compound Unit expansion has
    // its dB flag set
    m_bExplicitDBFlag = false;
    m_bDBFlag = false;
    m_CUEVec.clear();
    m_strUnit.clear();
  }

  // Build up my internals from a string specification.
  void ParseString(const char* unitString);
  void ParseString(const std::string& unitString);

  const char* GetString() const;

  operator std::string() {
    return GetString();
  }
  // Auxiliary output routine
  std::ostream& PrintSelf(std::ostream& output) const;

protected:
  void BuildDimension() const;
  void ComputeBigness() const;

private:
  // Many of these are declared "mutable" so that they can be changed on a "const"
  // object. Ordinarily we don't let const objects be modified, but this allows us to
  // declare certain member function "const" when the function is supposed to be "read-only",
  // but in reality the "read" causes the update of cached values, and those must be
  // declared "mutable"
  mutable std::string m_strUnit;
  mutable double m_dBigness;
#pragma warning(push, 0)
  CUEVecType m_CUEVec;
#pragma warning(pop)
  mutable CUnitDimension* m_CUD;
  mutable bool m_bStaleBigness;
  mutable bool m_bStaleDimension;
  mutable bool m_bExplicitNonDBFlag;
  mutable bool m_bExplicitDBFlag;
  mutable bool m_bDBFlag;
};

inline CCompoundUnit pow(const CCompoundUnit& baseref, CCompoundUnitElement::ExponentType exp)
{
  // Construct a "C++ temporary" that we modify by raising to the power.
  // It's the same principle that guides the implementation of operator*
  // (see above) as described by Myers, except that there is no operator
  // that corresponds to raising to a power, so we use an ordinary
  // function instead.

  return (CCompoundUnit(baseref)).Raise(exp);
}

inline CCompoundUnit sqrt(const CCompoundUnit& argref)
{ return pow(argref,0.5);  }

inline std::ostream& operator<<(std::ostream& out, const CCompoundUnit& ccu)
{
  //return ccu.PrintSelf(output);
  out << ccu.GetString();
  return out;
}
inline std::ostream& operator<<(std::ostream& out, const CCompoundUnit* ccu)
{
  if (ccu == nullptr)
    out << "";
  else
    out << ccu->GetString();
  return out;
}
//-------------------------------------------------------------------------------
 double BIOGEARS_API Convert(double d, const CCompoundUnit& from, const CCompoundUnit& to);
 bool BIOGEARS_API CompatibleUnits(const CCompoundUnit& from, const CCompoundUnit& to);
}