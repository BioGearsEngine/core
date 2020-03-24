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
  m_DecayRate = 0; //default value unless user specified
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
  SAFE_DELETE(m_DecayRate);
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
  if (in.DecayRate().present()) {
    GetDecayRate().Load(in.DecayRate().get());
  } else {
    GetDecayRate().SetValue(0.0, FrequencyUnit::Per_s);
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
  if (HasDecayRate())
    data.DecayRate(std::unique_ptr<CDM::ScalarFrequencyData>(m_DecayRate->Unload()));
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
bool SEPainStimulus::HasDecayRate() const
{
  return m_DecayRate == nullptr ? false : m_DecayRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SEPainStimulus::GetDecayRate()
{
  if (m_DecayRate == nullptr)
    m_DecayRate = new SEScalarFrequency();
  return *m_DecayRate;
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
    if (m_DecayRate->GetValue() > 0.0) {
      str << "\n\tPain Decay Rate: " << m_DecayRate;
    }
    str << std::flush;
  }
}
//-----------------------------------------------------------------------------
}