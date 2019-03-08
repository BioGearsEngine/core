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
/// @file UnitDimension.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------

#include <biogears/cdm/utils/unitconversion/UnitDimension.h>

#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>

namespace biogears {
CUnitDimension::CUnitDimension()
  : m_EList(CUnitConversionEngine::GetEngine().GetNumFundamentalQuantities())
{
  InitElems();
};

//----------------------------------------------------------------------------
// Vectors of ints are probably initialized to zero anyway upon creation,
// but I'd rather not rely on that.
void CUnitDimension::InitElems()
{
  // **** REMOVE THIS CODE ****
  // This is here temporarily to test my belief that the casting away
  // of the constness is correct, and that the constness is not there
  // because the ref points to a temporary copy. That would be bad.
  // Note: Ok, I tested this and it works. I'm just leaving it here,
  // commented out, to document what I did.
  //for each(ExponentType const &ExpRef in m_EList)
  //{
  //  const_cast<ExponentType&>(ExpRef) = 12345;
  //}
  //for each(ExponentType const &ExpRef in m_EList)
  //{
  //  assert(const_cast<ExponentType&>(ExpRef) == 12345);
  //}
  //cerr << "*** Assertion passed in CUnitDimension::InitElemt(). Safe to remove code\n";
  // **** END OF SECTION TO REMOVE ****

  // This code should work without the const cast and without ExpRef being
  // declared as a ref to const. But there appears to be a bug in the
  // compiler that causes the initializing expression for ExpRef to be
  // a const ExponentType. This requires declaring ExpRef as a ref to const,
  // but since we want to modify it, we need to cast away the const.
  // Yeah, I could have gotten the size, and used a conventional for-loop
  // to iterate over an array index variable, but I was being stubborn.
  for (auto ExpRef = m_EList.begin(); ExpRef != m_EList.end(); ExpRef++) {
    *ExpRef = 0.0;
  }
};
}