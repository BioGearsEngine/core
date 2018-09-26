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
/// @file CompoundUnitElement.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------
#include <biogears/cdm/utils/unitconversion/CompoundUnitElement.h>

#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>

namespace biogears {
//----------------------------------------------------------------------------
double CCompoundUnitElement::GetBigness() const
{
  static CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  double dPrefixFac = 1.0;
  if (m_iPrefixID != -1) {
    const CPrefixDescriptor& pd(uce.GetPrefixDescriptor(m_iPrefixID));
    dPrefixFac = pd.GetScaleFactor();
  }

  const CUnitDescriptor& ud = uce.GetUnitDescriptor(m_iUnitID);

  // Combine the unit's conversion factor and prefix, and then
  // the exponent
  double dBigness = (ud.GetConvFac() * dPrefixFac);
  return std::pow(dBigness, m_CExponent);
}

//----------------------------------------------------------------------------
double CCompoundUnitElement::GetBias() const
{
  static CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  const CUnitDescriptor& ud = uce.GetUnitDescriptor(m_iUnitID);
  return ud.GetBias();
}

//----------------------------------------------------------------------------
bool CCompoundUnitElement::IsDecibel() const
{
  static CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  const CUnitDescriptor& ud = uce.GetUnitDescriptor(m_iUnitID);
  return ud.IsDecibel();
}
}