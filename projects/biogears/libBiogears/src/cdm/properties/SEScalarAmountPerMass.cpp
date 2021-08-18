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

#include <biogears/cdm/properties/SEScalarAmountPerMass.h>

namespace biogears {
AmountPerMassUnit AmountPerMassUnit::ct_Per_g("ct/g");
AmountPerMassUnit AmountPerMassUnit::ct_Per_ug("ct/ug");
template class SEScalarQuantity<AmountPerMassUnit>;
//-----------------------------------------------------------------------------
AmountPerMassUnit::AmountPerMassUnit(const char* u)
  : AmountPerMassUnit(std::string { u })
{
}
//-----------------------------------------------------------------------------
AmountPerMassUnit::AmountPerMassUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
AmountPerMassUnit::~AmountPerMassUnit(){
}
//-----------------------------------------------------------------------------
SEScalarAmountPerMass::SEScalarAmountPerMass()
  : SEScalarQuantity()
{
}
//-----------------------------------------------------------------------------
SEScalarAmountPerMass::~SEScalarAmountPerMass()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarAmountPerMassData* SEScalarAmountPerMass::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAmountPerMassData* data(new CDM::ScalarAmountPerMassData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool AmountPerMassUnit::IsValidUnit(const char* unit)
{
  return IsValidUnit(std::string { unit });
}
//-----------------------------------------------------------------------------
bool AmountPerMassUnit::IsValidUnit(const std::string& unit)
{
  std::string ct_Per_g_str = ct_Per_g.GetString();
  std::string ct_Per_ug_str = ct_Per_ug.GetString();
  if (ct_Per_g_str == unit)
    return true;
  if (ct_Per_ug_str == unit)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
const AmountPerMassUnit& AmountPerMassUnit::GetCompoundUnit(const char* unit)
{
  return GetCompoundUnit(std::string { unit });
}
//-----------------------------------------------------------------------------
const AmountPerMassUnit& AmountPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  std::string ct_Per_g_str = ct_Per_g.GetString();
  std::string ct_Per_ug_str = ct_Per_ug.GetString();
  if (ct_Per_g_str == unit)
    return ct_Per_g;
  if (ct_Per_ug_str == unit)
    return ct_Per_ug;
  std::stringstream err;
  err << unit << " is not a valid AmountPerMass unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
bool AmountPerMassUnit::operator==(const AmountPerMassUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool AmountPerMassUnit::operator!=(const AmountPerMassUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarAmountPerMass::operator==(const SEScalarAmountPerMass& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarAmountPerMass::operator!=(const SEScalarAmountPerMass& obj) const
{
  return !(*this == obj);
}
}