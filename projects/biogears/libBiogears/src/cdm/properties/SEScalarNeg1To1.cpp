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

#include <biogears/cdm/properties/SEScalarNeg1To1.h>

namespace biogears {
SEScalarNeg1To1::SEScalarNeg1To1()
  : SEScalar()
{
}
//-------------------------------------------------------------------------------
SEScalarNeg1To1::~SEScalarNeg1To1()
{
}
//-------------------------------------------------------------------------------
CDM::ScalarNeg1To1Data* SEScalarNeg1To1::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarNeg1To1Data* data(new CDM::ScalarNeg1To1Data());
  SEScalar::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
double SEScalarNeg1To1::GetValue(const NoUnit& unitless) const
{
  return SEScalar::GetValue();
}
//-------------------------------------------------------------------------------
void SEScalarNeg1To1::SetValue(double d)
{
  if (d > 1 || d < -1)
    throw CommonDataModelException("SEScalarNeg1To1 must be between [-1,1]");
  SEScalar::SetValue(d);
}
//-------------------------------------------------------------------------------
void SEScalarNeg1To1::SetValue(double d, const NoUnit& unitless)
{
  SEScalarNeg1To1::SetValue(d);
}
//-------------------------------------------------------------------------------
bool SEScalarNeg1To1::operator==(const SEScalarNeg1To1& obj) const
{
  return m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarNeg1To1::operator!=(const SEScalarNeg1To1& obj) const
{
  return !(*this == obj);
}
}