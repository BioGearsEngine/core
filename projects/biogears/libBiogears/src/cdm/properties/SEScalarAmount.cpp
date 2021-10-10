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

#include <biogears/cdm/properties/SEScalarAmount.h>

namespace biogears {
const AmountUnit AmountUnit::mol("mol");
const AmountUnit AmountUnit::pmol("pmol");
template class SEScalarQuantity<AmountUnit>;

//-----------------------------------------------------------------------------
SEScalarAmount::SEScalarAmount()
  : SEScalarQuantity()
{
}
//-----------------------------------------------------------------------------
SEScalarAmount::~SEScalarAmount()  {

}
//-----------------------------------------------------------------------------
AmountUnit::AmountUnit(const char* u)
  : AmountUnit(std::string{ u })
{
}
//-----------------------------------------------------------------------------
AmountUnit::AmountUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-----------------------------------------------------------------------------
AmountUnit::~AmountUnit()
{
}
//-----------------------------------------------------------------------------
CDM::ScalarAmountData* SEScalarAmount::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarAmountData* data(new CDM::ScalarAmountData());
  SEScalarQuantity::Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
bool AmountUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mol.GetString(),unit) == 0)
    return true;
  if (strcmp(pmol.GetString(),unit) == 0)
    return true;
  return false;
}
//-----------------------------------------------------------------------------
bool AmountUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-----------------------------------------------------------------------------
const AmountUnit& AmountUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mol.GetString(),unit) == 0)
    return mol;
  if (strcmp(pmol.GetString(),unit) == 0)
    return pmol;
  std::stringstream err;
  err << unit << " is not a valid Amount unit";
  throw CommonDataModelException(err.str());
}
//-----------------------------------------------------------------------------
const AmountUnit& AmountUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(  unit.c_str() );
}
//-----------------------------------------------------------------------------
bool AmountUnit::operator==(const AmountUnit& obj) const
{
  return CCompoundUnit::operator==(obj);
}
//-------------------------------------------------------------------------------
bool AmountUnit::operator!=(const AmountUnit& obj) const
{
  return !(*this == obj);
}
//-------------------------------------------------------------------------------

bool SEScalarAmount::operator==(const SEScalarAmount& obj) const
{
  return m_unit == obj.m_unit
    && m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalarAmount::operator!=(const SEScalarAmount& obj) const
{
  return !(*this == obj);
}
}