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

#include <biogears/cdm/properties/SEScalarFraction.h>

namespace biogears {
SEScalarFraction::SEScalarFraction()
  : SEScalar()
{
}
//-------------------------------------------------------------------------------
SEScalarFraction::~SEScalarFraction() {
}
//-------------------------------------------------------------------------------
CDM::ScalarFractionData* SEScalarFraction::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFractionData* data(new CDM::ScalarFractionData());
  SEScalar::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
//double SEScalarFraction::GetValue() const {
//  return SEScalar::GetValue();
//}
//-------------------------------------------------------------------------------
double SEScalarFraction::GetValue(const NoUnit& unitless) const
{
  return SEScalar::GetValue();
}
//-------------------------------------------------------------------------------
//void SEScalarFraction::SetValue(double d)
//{
//  SEScalar::SetValue(d);
//}
//-------------------------------------------------------------------------------
void SEScalarFraction::SetValue(double d, const NoUnit& unitless)
{
  SEScalar::SetValue(d);
}
//-------------------------------------------------------------------------------
bool SEScalarFraction::operator==(const SEScalarFraction& obj) const
{
  return  m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarFraction::operator!=(const SEScalarFraction& obj) const
{
  return !(*this == obj);
}
}