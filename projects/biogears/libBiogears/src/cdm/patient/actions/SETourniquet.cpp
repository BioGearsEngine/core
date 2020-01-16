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

#include <biogears/cdm/patient/actions/SETourniquet.h>

namespace biogears {
SETourniquet::SETourniquet()
  : SEPatientAction()
{
  m_Compartment = ""; //User input, location of Tourniquet
  m_TourniquetLevel = (CDM::enumTourniquetApplicationLevel::value)-1;
}
//-----------------------------------------------------------------------------
SETourniquet::~SETourniquet()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SETourniquet::Clear()
{
  SEPatientAction::Clear();
  m_Compartment = "";
  m_TourniquetLevel = (CDM::enumTourniquetApplicationLevel::value)-1;
}
//-----------------------------------------------------------------------------
bool SETourniquet::IsValid() const
{
  const std::vector<std::string> validCmpts{ "LeftArm", "LeftLeg", "RightArm", "RightLeg" };
  return SEPatientAction::IsValid() && HasCompartment() && HasTourniquetLevel()
    && std::find(validCmpts.begin(), validCmpts.end(), GetCompartment()) != validCmpts.end();
}
//-----------------------------------------------------------------------------
bool SETourniquet::IsActive() const
{
  return IsValid() ? !(m_TourniquetLevel == (CDM::enumTourniquetApplicationLevel::None)) : false;
}//-----------------------------------------------------------------------------
bool SETourniquet::Load(const CDM::TourniquetData& in)
{
  SEPatientAction::Load(in);
  m_TourniquetLevel = in.TourniquetLevel();
  m_Compartment = in.Compartment();

  return true;
}
//-----------------------------------------------------------------------------
CDM::TourniquetData* SETourniquet::Unload() const
{
  CDM::TourniquetData* data(new CDM::TourniquetData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SETourniquet::Unload(CDM::TourniquetData& data) const
{
  SEPatientAction::Unload(data);
  if (HasCompartment())
    data.Compartment(m_Compartment);
  if (HasTourniquetLevel())
    data.TourniquetLevel(m_TourniquetLevel);
}
//-----------------------------------------------------------------------------
const char* SETourniquet::GetCompartment_cStr() const
{
  return m_Compartment.c_str();
}
//-----------------------------------------------------------------------------
std::string SETourniquet::GetCompartment() const
{
  return m_Compartment;
}
//-----------------------------------------------------------------------------
bool SETourniquet::HasCompartment() const
{  
  return !m_Compartment.empty();
}
//-----------------------------------------------------------------------------
void SETourniquet::SetCompartment(const char* name)
{
  return SetCompartment(std::string{ name });
}
//-----------------------------------------------------------------------------
void SETourniquet::SetCompartment(const std::string& name)
{
  m_Compartment = name;
}
//-----------------------------------------------------------------------------
void SETourniquet::InvalidateCompartment()
{
  m_Compartment = "";
}
//-----------------------------------------------------------------------------
bool SETourniquet::HasTourniquetLevel() const
{
  return m_TourniquetLevel == ((CDM::enumTourniquetApplicationLevel::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
CDM::enumTourniquetApplicationLevel SETourniquet::GetTourniquetLevel()
{
  return m_TourniquetLevel;
}
//-----------------------------------------------------------------------------
void SETourniquet::SetTourniquetLevel(CDM::enumTourniquetApplicationLevel::value level)
{
  m_TourniquetLevel = level;
}
//-----------------------------------------------------------------------------
void SETourniquet::ToString(std::ostream& str) const
{
  if (m_TourniquetLevel == CDM::enumTourniquetApplicationLevel::None) {
    str << "Patient Action : Remove tourniquet";
    if (HasComment())
      str << "\n\tComment: ";
    str << m_Comment;
    str << "\n\tCompartment: ";
    HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
  } else {
    str << "Patient Action : Tourniquet";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tApplication Level:  ";
    str << m_TourniquetLevel;
    str << "\n\tCompartment: ";
    HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
  }
  str << std::flush;
}
}
//-----------------------------------------------------------------------------