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
#include <biogears/cdm/utils/unitconversion/QuantityConversionDescriptor.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>
//----------------------------------------------------------------------------
/// @file QuantityConversionDescriptor.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------

#include <iostream>

//#include <biogears/cdm/utils/unitconversion/UCCommon.h>

//----------------------------------------------------------------------------
namespace biogears {
CQuantityConversionDescriptor::CQuantityConversionDescriptor(double fromExp, const char* mappingUnit)
  :CQuantityConversionDescriptor(fromExp, std::string{ mappingUnit })
{
    
}
CQuantityConversionDescriptor::CQuantityConversionDescriptor(double fromExp, const std::string& mappingUnit)
  : m_dFromExponent(fromExp)
{
  m_CUMappingUnit = new CCompoundUnit(mappingUnit);
}

//----------------------------------------------------------------------------
CQuantityConversionDescriptor::~CQuantityConversionDescriptor()
{
  delete m_CUMappingUnit;
}

//----------------------------------------------------------------------------
// Make sure that the specified conversion passes
// dimensional analysis. This is called by whoever creates us, who knows
// what types we're being defined to convert from/to. There's no reason for us to
// store the from/to types, because that's the information used to look us up
// in the hash table
void CQuantityConversionDescriptor::Validate(int fromQTID, int toQTID)
{
  // Get the "from" dimension and the mapping unit's dimension, and compute
  // the resulting dimension (taking into account the power-raising) and
  // make sure it's equal to the "to" dimension. Note that dimension values are
  // exponents, so exponentiation becomes multiplication, and multiplication
  // becomes addition.
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  const CQuantityTypeDescriptor& fromType = uce.GetQuantityTypeDescriptor(fromQTID);
  const CQuantityTypeDescriptor& toType = uce.GetQuantityTypeDescriptor(toQTID);
  const CUnitDimension& fromDim = *fromType.GetDimension();
  const CUnitDimension& toDim = *toType.GetDimension();
  const CUnitDimension& mapUnitDim = *m_CUMappingUnit->GetDimension();

  CUnitDimension resultDim = fromDim * m_dFromExponent + mapUnitDim;

  if (resultDim != toDim) {
    std::cerr << "Error: Invalid quantity type conversion from " << fromType.GetName() << " to " << toType.GetName() << std::endl;
  }
}

const double& CQuantityConversionDescriptor::GetFromExponent() const
{
  return m_dFromExponent;
}

const CCompoundUnit* CQuantityConversionDescriptor::GetMappingUnit() const
{
  return m_CUMappingUnit;
}
}