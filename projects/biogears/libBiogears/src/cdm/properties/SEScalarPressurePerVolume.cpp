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

#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>

// Private Includes
#include <io/cdm/Property.h>

namespace biogears {
const PressurePerVolumeUnit PressurePerVolumeUnit::mmHg_Per_mL("mmHg/mL");
const PressurePerVolumeUnit PressurePerVolumeUnit::cmH2O_Per_mL("cmH2O/mL");

template class SEScalarQuantity<PressurePerVolumeUnit>;

PressurePerVolumeUnit::PressurePerVolumeUnit(const char* u)
  : PressurePerVolumeUnit(std::string { u })
{
}
//-------------------------------------------------------------------------------
PressurePerVolumeUnit::PressurePerVolumeUnit(const std::string& u)
  : CCompoundUnit(u)
{
}
//-------------------------------------------------------------------------------
PressurePerVolumeUnit::~PressurePerVolumeUnit()
{
}
//-------------------------------------------------------------------------------
SEScalarPressurePerVolume::SEScalarPressurePerVolume()
{
}
//-------------------------------------------------------------------------------
SEScalarPressurePerVolume::~SEScalarPressurePerVolume(){

}
//-------------------------------------------------------------------------------
bool SEScalarPressurePerVolume::Load(const CDM::ScalarPressurePerVolumeData& in)
{
  io::Property::UnMarshall(in, *this);
  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::ScalarPressurePerVolumeData* SEScalarPressurePerVolume::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarPressurePerVolumeData* data(new CDM::ScalarPressurePerVolumeData());
  Unload(*data);
  return data;
} //-------------------------------------------------------------------------------
void SEScalarPressurePerVolume::Unload(CDM::ScalarPressurePerVolumeData& data) const
{
  io::Property::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool PressurePerVolumeUnit::IsValidUnit(const char* unit)
{
  if (strcmp(mmHg_Per_mL.GetString(), unit) == 0)
    return true;
  if (strcmp(cmH2O_Per_mL.GetString(), unit) == 0)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool PressurePerVolumeUnit::IsValidUnit(const std::string& unit)
{
  return IsValidUnit(unit.c_str());
}
//-------------------------------------------------------------------------------
const PressurePerVolumeUnit& PressurePerVolumeUnit::GetCompoundUnit(const char* unit)
{
  if (strcmp(mmHg_Per_mL.GetString(), unit) == 0)
    return mmHg_Per_mL;
  if (strcmp(cmH2O_Per_mL.GetString(), unit) == 0)
    return cmH2O_Per_mL;
  std::stringstream err;
  err << unit << " is not a valid PressurePerVolume unit";
  throw CommonDataModelException(err.str());
}
//-------------------------------------------------------------------------------
const PressurePerVolumeUnit& PressurePerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  return GetCompoundUnit(unit.c_str());
}


//-------------------------------------------------------------------------------
}