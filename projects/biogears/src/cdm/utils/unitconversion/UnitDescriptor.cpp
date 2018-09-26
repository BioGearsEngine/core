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
/// @file UnitDescriptor.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------
#include <biogears/cdm/utils/unitconversion/UnitDescriptor.h>

//Project Includes
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>
//----------------------------------------------------------------------------

namespace biogears {
CUnitDescriptor::CUnitDescriptor(const std::string& name, const std::string& symbol, unsigned int quantityTypeId,
  const std::string& targetCompoundUnit, const double& convFac, const double& bias,
  PrefixModeType mode, const std::string& prefixSet)
  : m_strName(name)
  , m_strSymbol(symbol)
  , m_uiQuantityTypeId(quantityTypeId)
  , m_uiPrefixMode(mode)
  , m_strPrefixSet(prefixSet)
{

  // Set relative convFac and bias to identity values by default
  double relConvFac = 1.0;
  double relBias = 0.0;
  //double prefixScaleFac = 1.0;
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();

  // Check if we are defining in terms of the base unit or some other unit
  if (targetCompoundUnit != "*" && targetCompoundUnit != "") {
    // We've got a target unit, so compose our conversion info with the target's.
    CCompoundUnit TargetCU(targetCompoundUnit);

    // Make sure the dimensions of the target are correct for this unit's
    // quantity type
    const CQuantityTypeDescriptor& qtDesc = uce.GetQuantityTypeDescriptor(quantityTypeId);

    const CUnitDimension* qtDim = qtDesc.GetDimension();
    const CUnitDimension* tgtDim = TargetCU.GetDimension();

    if (*qtDim != *tgtDim) {
      std::cerr << "Target unit " << targetCompoundUnit << " is dimensionaly incompatible with quantity type " << qtDesc.GetName() << std::endl;
    }
    relConvFac = TargetCU.GetBigness();
    relBias = TargetCU.GetBias();
    m_bDBFlag = TargetCU.IsDecibel();
  } else {
    // We're defining relative to the base unit, so get the bigness from the
    // Quantity Type Descriptor
    const CQuantityTypeDescriptor& qtDesc = uce.GetQuantityTypeDescriptor(quantityTypeId);
    relConvFac = qtDesc.GetBigness();
    m_bDBFlag = false;
  }

  // multiply conversion factors and add scaled biases
  // Note that a bias is the value of the current unit when the target unit
  // or base unit is zero, and it's expressed in intervals of the current unit.
  // In other words, the bias is the amount one must *subtract* from the current
  // unit before applying any scaling for converting to another unit. E.g., if defining
  // Fahrenheit in terms of Celsius, Fahrenheit's bias would be 32.
  m_dConvFac = relConvFac * convFac;
  m_dBias = relBias / convFac + bias;
  DEBUGOUT(cerr << "Creating unit descriptor for " << name << " with convfac = " << m_dConvFac << " and bias = " << m_dBias << endl;)
}

//----------------------------------------------------------------------------
bool CUnitDescriptor::IsPrefixAllowed(char prefix) const
{
  bool allowedIfFound = false;
  // Don't know why both size_type and size_t exist, but comparison of
  // find result with npos might not work unless value compared is of
  // type size_type, according to MSVC++8 basic_string documentation in
  // MSDN Library.
  std::string::allocator_type::size_type findResult;

  switch (m_uiPrefixMode) {
  case NONE:
    return false;
    break;

  case ALL:
    return true;
    break;

  case SOME:
    allowedIfFound = true;
    // Don't break!
  case ALLBUT:
    findResult = m_strPrefixSet.find(prefix);
    return (findResult != std::string::npos) == allowedIfFound;
  default:
    // Can't get here
    return false;
  }
}
}