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
/// @file QuantityTypeDescriptor.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------
#include <biogears/cdm/utils/unitconversion/QuantityTypeDescriptor.h>

#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/utils/unitconversion/UnitDimension.h>
//----------------------------------------------------------------------------
namespace biogears {
CQuantityTypeDescriptor::CQuantityTypeDescriptor(const char* name, int fundIdx, bool twentyLog)
  : CQuantityTypeDescriptor(std::string{ name }, fundIdx, twentyLog)
{
}
//----------------------------------------------------------------------------
CQuantityTypeDescriptor::CQuantityTypeDescriptor(const std::string& name, int fundIdx, bool twentyLog)
  : m_sName(name)
  , m_iFundIdx(fundIdx)
  , m_CUExpansion(nullptr)
  , m_bTwentyLogRule(twentyLog)
{
  // This ctor is for fundamental quantities, and the bigness is 1.0 by definition.
  m_dBigness = 1.0;
  m_CUD = new CUnitDimension();
  (*m_CUD)[m_iFundIdx] = 1.0;
}
//----------------------------------------------------------------------------
CQuantityTypeDescriptor::CQuantityTypeDescriptor(const char* name, CCompoundUnit* expansion, bool twentyLog)
  : CQuantityTypeDescriptor(std::string{ name }, expansion, twentyLog)
{
}
//----------------------------------------------------------------------------
CQuantityTypeDescriptor::CQuantityTypeDescriptor(const std::string& name, CCompoundUnit* expansion, bool twentyLog)
  : m_sName(name)
  , m_iFundIdx(-1)
  , m_CUExpansion(expansion)
  , m_bTwentyLogRule(twentyLog)
{
  // By convention, when the engine hands expansion to us, we own it. So we'll free it
  // in our dtor
  // This ctor is for derived quantities, and the bigness is that of the expansion.
  m_dBigness = m_CUExpansion->GetBigness();
  // Copy our expansion's dimension. Since the other constructors allocate their
  // own, we can't simply copy the pointer from our expansion
  m_CUD = new CUnitDimension(*m_CUExpansion->GetDimension());
}

//----------------------------------------------------------------------------
CQuantityTypeDescriptor::~CQuantityTypeDescriptor()
{
  if (m_CUExpansion) {
    delete m_CUExpansion;
  }
  delete m_CUD;
}
//----------------------------------------------------------------------------
// Return the name of this quantity type
std::string CQuantityTypeDescriptor::GetName() const
{
  return m_sName;
}
//----------------------------------------------------------------------------
// Return the name of this quantity type
const char* CQuantityTypeDescriptor::GetName_cStr() const
{
  return m_sName.c_str();
}
}