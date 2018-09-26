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
/// @file QuantityConversionKey.h
/// @author Chris Volpe

#include <functional>

#include <biogears/cdm/utils/unitconversion/QuantityConversionKey.h>

#include <biogears/cdm/utils/unitconversion/QuantityTypeDescriptor.h>
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>

namespace biogears {
CQuantityConversionKey::CQuantityConversionKey(int fromID, int toID)
{
  // Get the dimensions from the associated quantity type IDs
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();

  const CQuantityTypeDescriptor& fromType = uce.GetQuantityTypeDescriptor(fromID);
  const CQuantityTypeDescriptor& toType = uce.GetQuantityTypeDescriptor(toID);

  m_CUDfromDim = fromType.GetDimension();
  m_CUDtoDim = toType.GetDimension();
}

// Compute a hash value from an array of the two hash values of the From and
// To Dimension objects
size_t CQuantityConversionKey::hash_value() const
{
  size_t hashes[2] = { m_CUDfromDim->hash_value(), m_CUDtoDim->hash_value() };
  // Use the templated method in <xhash> to compute the hash value for
  // the array
  return _Hash_value(hashes, hashes + 2);
}

// override less-than operator to facilitate default hash comparison function
bool CQuantityConversionKey::operator<(const CQuantityConversionKey& rhs) const
{
  // Perform lexicographic comparison, treating From Dimension and
  // To Dimension as a two-letter word

  if (*m_CUDfromDim == *rhs.m_CUDfromDim) {
    // let the second half rule
    bool rv1 = *m_CUDtoDim < *rhs.m_CUDtoDim;
    return rv1;
  }
  bool rv2 = *m_CUDfromDim < *rhs.m_CUDfromDim;
  return rv2;
}

bool CQuantityConversionKey::operator==(const CQuantityConversionKey& rhs) const
{
  return ((*m_CUDfromDim == *rhs.m_CUDfromDim) && (*m_CUDtoDim == *rhs.m_CUDtoDim));
}
}