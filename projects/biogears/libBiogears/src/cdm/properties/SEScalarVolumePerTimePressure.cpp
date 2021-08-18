/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.Licensed under the Apache License, Version 2.0(the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>

namespace biogears
{
  const VolumePerTimePressureUnit VolumePerTimePressureUnit::L_Per_s_mmHg("L/s mmHg");
  const VolumePerTimePressureUnit VolumePerTimePressureUnit::mL_Per_s_mmHg("mL/s mmHg");
  const VolumePerTimePressureUnit VolumePerTimePressureUnit::L_Per_min_mmHg("L/min mmHg");
  const VolumePerTimePressureUnit VolumePerTimePressureUnit::mL_Per_min_mmHg("mL/min mmHg");

  template class SEScalarQuantity<VolumePerTimePressureUnit>;

  VolumePerTimePressureUnit::~VolumePerTimePressureUnit() 
  {

  };
  //-------------------------------------------------------------------------------
  VolumePerTimePressureUnit::VolumePerTimePressureUnit(const char* u)
    : VolumePerTimePressureUnit(std::string { u })
  {
  }
  //-------------------------------------------------------------------------------
  VolumePerTimePressureUnit::VolumePerTimePressureUnit(const std::string& u)
    : CCompoundUnit(u)
  {
  }
  //-------------------------------------------------------------------------------
  SEScalarVolumePerTimePressure::SEScalarVolumePerTimePressure()
  {
  }
  //-------------------------------------------------------------------------------
  SEScalarVolumePerTimePressure::~SEScalarVolumePerTimePressure()
  {
  }
  //-------------------------------------------------------------------------------
  CDM::ScalarVolumePerTimePressureData* SEScalarVolumePerTimePressure::Unload() const
  {
    if (!IsValid())
      return nullptr;
    CDM::ScalarVolumePerTimePressureData* data(new CDM::ScalarVolumePerTimePressureData());
    SEScalarQuantity::Unload(*data);
    return data;
  }
  //-------------------------------------------------------------------------------
  bool VolumePerTimePressureUnit::IsValidUnit(const char* unit)
  {
    if (strcmp(L_Per_s_mmHg.GetString(), unit) == 0)
      return true;
    if (strcmp(mL_Per_s_mmHg.GetString(), unit) == 0)
      return true;
    if (strcmp(L_Per_min_mmHg.GetString(), unit) == 0)
      return true;
    if (strcmp(mL_Per_min_mmHg.GetString(), unit) == 0)
      return true;
    return false;
  }
  //-------------------------------------------------------------------------------
  bool VolumePerTimePressureUnit::IsValidUnit(const std::string& unit)
  {
    return IsValidUnit(unit.c_str());
  }
  //-------------------------------------------------------------------------------
  const VolumePerTimePressureUnit& VolumePerTimePressureUnit::GetCompoundUnit(const char* unit)
  {
    if (strcmp(L_Per_s_mmHg.GetString(), unit) == 0)
      return L_Per_s_mmHg;
    if (strcmp(mL_Per_s_mmHg.GetString(), unit) == 0)
      return mL_Per_s_mmHg;
    if (strcmp(L_Per_min_mmHg.GetString(), unit) == 0)
      return L_Per_min_mmHg;
    if (strcmp(mL_Per_min_mmHg.GetString(), unit) == 0)
      return mL_Per_min_mmHg;
    std::stringstream err;
    err << unit << " is not a valid VolumePerTimePressure unit";
    throw CommonDataModelException(err.str());
  }
  //-------------------------------------------------------------------------------
  const VolumePerTimePressureUnit& VolumePerTimePressureUnit::GetCompoundUnit(const std::string& unit)
  {
    return GetCompoundUnit(unit.c_str());
  }
  //-------------------------------------------------------------------------------
  bool VolumePerTimePressureUnit::operator==(const VolumePerTimePressureUnit& obj) const
  {
    return CCompoundUnit::operator==(obj);
  }
  //-------------------------------------------------------------------------------
  bool VolumePerTimePressureUnit::operator!=(const VolumePerTimePressureUnit& obj) const
  {
    return !(*this == obj);
  }
  //-------------------------------------------------------------------------------

  bool SEScalarVolumePerTimePressure::operator==(const SEScalarVolumePerTimePressure& obj) const
  {
    return m_unit == obj.m_unit
      && m_value == obj.m_value;
  }
  //-------------------------------------------------------------------------------
  bool SEScalarVolumePerTimePressure::operator!=(const SEScalarVolumePerTimePressure& obj) const
  {
    return !(*this == obj);
  }
}