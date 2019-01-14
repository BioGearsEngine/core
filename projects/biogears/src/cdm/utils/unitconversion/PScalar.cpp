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
/// @file PScalar.cpp
/// @author Chris Volpe
///
/// PScalar is short for Physics Scalar. It is an encapsulation of a
/// double precision floating point value and a CompoundUnit object.
/// It allows all the expected arithmetic operations, as well as the
/// functions pow() for raising to a power, and sqrt() for computing the
/// square root.
//----------------------------------------------------------------------------

#include <biogears/cdm/utils/unitconversion/PScalar.h>

namespace biogears {
// We need one translation unit that processes the header file in
// this DLL or else the methods won't be available in the Debug build,
// since inline function optimization isn't done

// Technically, we don't need any more than this, but let's insert some
// dummy code here to exercise some of the functionality at compile time.
// I've run into a few situations where enhancements I've made to the API,
// particularly for PScalars, resulted in overload-resolution issues when
// clients attempt to use them. So let's try to catch those issues here so that
// we don't have a situation where this library builds, but a client won't.

/*
static void TestMyAPI()
{
	CPScalar s1("g"), s2(s1);
	s1.ConvertTo(s2);
}
*/
// Construct from value and string unit spec
  CPScalar::CPScalar(double val, const char* unitSpec)
    : m_dValue(val)
    , m_CCU(unitSpec)
  {
    // Do nothing
  }
  // Construct from value and string unit spec
  CPScalar::CPScalar(double val, const std::string& unitSpec)
    : m_dValue(val)
    , m_CCU(unitSpec)
  {
    // Do nothing
  }

  // Construct from string unit. Initial value is 1.0 so that we can multiply by a pure
// unit specification.
  CPScalar::CPScalar(const char* unitSpec)
    : m_dValue(1.0)
    , m_CCU(unitSpec)
  {
    // Do nothing
  }
  // Construct from string unit. Initial value is 1.0 so that we can multiply by a pure
  // unit specification.
  CPScalar::CPScalar(const std::string& unitSpec)
    : m_dValue(1.0)
    , m_CCU(unitSpec)
  {
    // Do nothing
  }
  // Convert this object to a new set of units
  CPScalar& CPScalar::ConvertTo(const char* unitSpec)
  {
    return ConvertTo(std::string{ unitSpec });
  }
  // Convert this object to a new set of units
  CPScalar& CPScalar::ConvertTo(const std::string& unitSpec)
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    CCompoundUnit newUnit(unitSpec);
    double newval = uce.ConvertValue(m_dValue, m_CCU, newUnit);
    m_CCU = newUnit;
    m_dValue = newval;
    return *this;
  }
}