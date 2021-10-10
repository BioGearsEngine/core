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

#include <biogears/cdm/patient/actions/SEPainStimulus.h>

namespace biogears {
SEPainStimulus::SEPainStimulus()
  : SEPatientAction()
{
  m_Severity = nullptr;
  m_HalfLife = nullptr;
  m_Location = "";
}
//-----------------------------------------------------------------------------
SEPainStimulus::~SEPainStimulus()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEPainStimulus::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_HalfLife);
  m_Location.clear();
}
//-----------------------------------------------------------------------------
bool SEPainStimulus::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}
//-----------------------------------------------------------------------------
bool SEPainStimulus::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}
//-----------------------------------------------------------------------------
bool SEPainStimulus::Load(const CDM::PainStimulusData& in)
{
  SEPatientAction::Load(in);
  GetSeverity().Load(in.Severity());
  if (in.HalfLife().present()) {
    GetHalfLife().Load(in.HalfLife().get());
  } else {
    GetHalfLife().Invalidate();
  }
  m_Location = in.Location();
  return true;
}
//-----------------------------------------------------------------------------
CDM::PainStimulusData* SEPainStimulus::Unload() const
{
  CDM::PainStimulusData* data(new CDM::PainStimulusData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEPainStimulus::Unload(CDM::PainStimulusData& data) const
{
  SEPatientAction::Unload(data);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
  if (HasHalfLife())
    data.HalfLife(std::unique_ptr<CDM::ScalarTimeData>(m_HalfLife->Unload()));
  if (HasLocation())
    data.Location(m_Location);
}
//-----------------------------------------------------------------------------
bool SEPainStimulus::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEPainStimulus::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-----------------------------------------------------------------------------
bool SEPainStimulus::HasHalfLife() const
{
  return m_HalfLife == nullptr ? false : m_HalfLife->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& SEPainStimulus::GetHalfLife()
{
  if (m_HalfLife == nullptr)
    m_HalfLife = new SEScalarTime();
  return *m_HalfLife;
}
//-----------------------------------------------------------------------------
std::string SEPainStimulus::GetLocation() const
{
  return m_Location;
}
  //-----------------------------------------------------------------------------
const char* SEPainStimulus::GetLocation_cStr() const
{
  return m_Location.c_str();
}
//-----------------------------------------------------------------------------
bool SEPainStimulus::HasLocation() const
{
  return !m_Location.empty();
}
//-----------------------------------------------------------------------------
void SEPainStimulus::SetLocation(const char* name)
{
  SetLocation(std::string{ name });
}
//-----------------------------------------------------------------------------
void SEPainStimulus::SetLocation(const std::string& name)
{
  m_Location = name;
}
//-----------------------------------------------------------------------------
void SEPainStimulus::ToString(std::ostream& str) const
{
  if (m_Severity->GetValue() == 0.0) {
    str << "Patient Action : Stop Pain Stimulus";
    if (HasComment())
      str << "\n\tComment: ";
    str << m_Comment;
    str << "\n\tLocation: ";
    HasLocation() ? str << GetLocation() : str << "No Location Set";
  } else {
    str << "Patient Action : Pain Stimulus";
    if (HasComment()) {
      str << "\n\tComment: " << m_Comment;
    }
    str << "\n\tSeverity:  ";
    str << *m_Severity;
    str << "\n\tLocation: ";
    HasLocation() ? str << GetLocation() : str << "No Location Set";
    if (HasHalfLife()) {
      str << "\n\tHalfLife: " << m_HalfLife;
    }
    str << std::flush;
  }
}
//-------------------------------------------------------------------------------
bool SEPainStimulus::operator==( const SEPainStimulus& rhs) const
{
bool equivilant = m_Comment == rhs.m_Comment;
  equivilant = m_Location == rhs.m_Location;
  equivilant &= (m_Severity && rhs.m_Severity) ? m_Severity->operator==(*rhs.m_Severity) : m_Severity == rhs.m_Severity;
  equivilant &= (m_HalfLife && rhs.m_HalfLife) ? m_HalfLife->operator==(*rhs.m_HalfLife) : m_HalfLife == rhs.m_HalfLife;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEPainStimulus::operator!=( const SEPainStimulus& rhs) const
{
  return !(*this == rhs);
}
}
