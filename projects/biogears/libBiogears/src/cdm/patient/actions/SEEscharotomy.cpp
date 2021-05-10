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
#include <biogears/cdm/patient/actions/SEEscharotomy.h>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {

SEEscharotomy::SEEscharotomy()
  : SEPatientAction()
{
  m_Location = ""; //User input, location of escharotomy, check for compartment syndrome at location occurs later
}

SEEscharotomy::~SEEscharotomy()
{
  Clear();
}

void SEEscharotomy::Clear()
{
  SEPatientAction::Clear();
  m_Location = "";
}

bool SEEscharotomy::IsValid() const
{
  const std::vector<std::string> validCmpts { "LeftArm", "LeftLeg", "RightArm", "RightLeg", "Trunk" };
  return SEPatientAction::IsValid() && HasLocation() && std::find(validCmpts.begin(), validCmpts.end(), GetLocation()) != validCmpts.end();
}

bool SEEscharotomy::IsActive() const
{
  return IsValid();
}

bool SEEscharotomy::Load(const CDM::EscharotomyData& in)
{
  SEPatientAction::Load(in);
  m_Location = in.Location();
  return true;
}

CDM::EscharotomyData* SEEscharotomy::Unload() const
{
  CDM::EscharotomyData* data(new CDM::EscharotomyData());
  Unload(*data);
  return data;
}

void SEEscharotomy::Unload(CDM::EscharotomyData& data) const
{
  SEPatientAction::Unload(data);
  if (HasLocation())
    data.Location(m_Location);
}

//-------------------------------------------------------------------------------
std::string SEEscharotomy::GetLocation() const
{
  return m_Location;
}
//-------------------------------------------------------------------------------
bool SEEscharotomy::HasLocation() const
{
  return !m_Location.empty();
}
//-------------------------------------------------------------------------------
void SEEscharotomy::InvalidateLocation()
{
  m_Location = "";
}
//-------------------------------------------------------------------------------
void SEEscharotomy::SetLocation(const char* name)
{
  return SetLocation(std::string { name });
}
//-------------------------------------------------------------------------------
void SEEscharotomy::SetLocation(const std::string& name)
{
  m_Location = name;
}
//-------------------------------------------------------------------------------

void SEEscharotomy::ToString(std::ostream& str) const
{
  str << "Patient Action : Escharotomy";
  str << "\n\tLocation:  ";
  HasLocation() ? str << GetLocation() : str << "No Location Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEEscharotomy::operator==(const SEEscharotomy& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant = m_Location == rhs.m_Location;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEEscharotomy::operator!=(const SEEscharotomy& rhs) const
{
  return !(*this == rhs);
}
}