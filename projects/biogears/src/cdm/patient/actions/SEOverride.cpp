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

#include <biogears/cdm/patient/actions/SEOverride.h>

#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>


namespace biogears {
SEOverride::SEOverride()
  : SEPatientAction()
{
  m_OverrideSwitch = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::Off;
  m_PressureOR = nullptr;
  m_HeartRateOR = nullptr;
  m_CoreTemperatureOR = nullptr;
  m_SkinTemperatureOR = nullptr;
}

SEOverride::~SEOverride()
{
  Clear();
}

void SEOverride::Clear()
{
  SEPatientAction::Clear();
  m_OverrideSwitch = CDM::enumOnOff::Off;
  m_OverrideConformance = CDM::enumOnOff::Off;
  SAFE_DELETE(m_PressureOR);
  SAFE_DELETE(m_HeartRateOR);
  SAFE_DELETE(m_CoreTemperatureOR);
  SAFE_DELETE(m_SkinTemperatureOR);
}

bool SEOverride::IsValid() const
{
  if (!HasOverrideSwitch()) {
    Error("Override must have state.");
    return false;
  }
  if (GetOverrideSwitch() == CDM::enumOnOff::On) {
    return true;
  } else {
    return false;
  }
}

bool SEOverride::IsActive() const
{
  if (!HasOverrideSwitch() || !HasOverrideConformance())
    return false;
  return (GetOverrideSwitch() == CDM::enumOnOff::On && (GetOverrideConformance() == CDM::enumOnOff::On || GetOverrideConformance() == CDM::enumOnOff::Off));
}

bool SEOverride::Load(const CDM::OverrideData& in)
{
  SEPatientAction::Clear();
  SetOverrideSwitch(in.State());
  SetOverrideConformance(in.Conformant());
  if (in.MeanArterialPressureOverride().present())
    GetMAPOverride().Load(in.MeanArterialPressureOverride().get());
  else
    GetMAPOverride().Invalidate();
  if (in.HeartRateOverride().present())
    GetHeartRateOverride().Load(in.HeartRateOverride().get());
  else
    GetHeartRateOverride().Invalidate();
  if (in.CoreTemperatureOverride().present())
    GetCoreTemperatureOverride().Load(in.CoreTemperatureOverride().get());
  else
    GetCoreTemperatureOverride().Invalidate();
  if (in.SkinTemperatureOverride().present())
    GetSkinTemperatureOverride().Load(in.SkinTemperatureOverride().get());
  else
    GetSkinTemperatureOverride().Invalidate();
  //SEPatientAction::Load(in);
  //return true;
  return IsValid();
}

CDM::OverrideData* SEOverride::Unload() const
{
  CDM::OverrideData* data(new CDM::OverrideData());
  Unload(*data);
  return data;
}

void SEOverride::Unload(CDM::OverrideData& data) const
{
  SEPatientAction::Unload(data);
  if (HasOverrideSwitch())
    data.State(m_OverrideSwitch);
  if (HasOverrideConformance())
    data.Conformant(m_OverrideConformance);
  if (HasMAPOverride())
    data.MeanArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_PressureOR->Unload()));
  if (HasHeartRateOverride())
    data.HeartRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateOR->Unload()));
  if (HasCoreTemperatureOverride())
    data.CoreTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_CoreTemperatureOR->Unload()));
  if (HasSkinTemperatureOverride())
    data.SkinTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_SkinTemperatureOR->Unload()));
}

CDM::enumOnOff::value SEOverride::GetOverrideSwitch() const
{
  return m_OverrideSwitch;
}
void SEOverride::SetOverrideSwitch(CDM::enumOnOff::value state)
{
  m_OverrideSwitch = state;
}
bool SEOverride::HasOverrideSwitch() const
{
  return (m_OverrideSwitch == CDM::enumOnOff::Off) ? true : 
  (m_OverrideSwitch == CDM::enumOnOff::On) ? true : false;
}
void SEOverride::InvalidateOverrideSwitch()
{
  m_OverrideSwitch = (CDM::enumOnOff::Off);
}
CDM::enumOnOff::value SEOverride::GetOverrideConformance() const
{
  return m_OverrideConformance;
}
void SEOverride::SetOverrideConformance(CDM::enumOnOff::value valid)
{
  m_OverrideConformance = valid;
}
bool SEOverride::HasOverrideConformance() const
{
  return (m_OverrideConformance == CDM::enumOnOff::Off) ? true : 
  (m_OverrideConformance == CDM::enumOnOff::On) ? true : false;
}
void SEOverride::InvalidateOverrideConformance()
{
  m_OverrideConformance = (CDM::enumOnOff::Off);
}
bool SEOverride::IsOverrideConformant()
{
  return (m_OverrideConformance == CDM::enumOnOff::On) ? true : false;
}

// Cardiovascular Overrides //
bool SEOverride::HasMAPOverride() const
{
  return m_PressureOR == nullptr ? false : m_PressureOR->IsValid();
}
SEScalarPressure& SEOverride::GetMAPOverride()
{
  if (m_PressureOR == nullptr)
    m_PressureOR = new SEScalarPressure();
  return *m_PressureOR;
}
double SEOverride::GetMAPOverride(const PressureUnit& unit) const
{
  if (m_PressureOR == nullptr)
    return SEScalar::dNaN();
  return m_PressureOR->GetValue(unit);
}
bool SEOverride::HasHeartRateOverride() const
{
  return m_HeartRateOR == nullptr ? false : m_HeartRateOR->IsValid();
}
SEScalarFrequency& SEOverride::GetHeartRateOverride()
{
  if (m_HeartRateOR == nullptr)
    m_HeartRateOR = new SEScalarFrequency();
  return *m_HeartRateOR;
}
double SEOverride::GetHeartRateOverride(const FrequencyUnit& unit) const
{
  if (m_HeartRateOR == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateOR->GetValue(unit);
}

bool SEOverride::HasCardiovascularOverride() const
{
  return HasMAPOverride() ? true : 
  HasHeartRateOverride() ? true :
  false;
 
}

// Energy Overrides //
bool SEOverride::HasCoreTemperatureOverride() const
{
  return m_CoreTemperatureOR == nullptr ? false : m_CoreTemperatureOR->IsValid();
}
SEScalarTemperature& SEOverride::GetCoreTemperatureOverride()
{
  if (m_CoreTemperatureOR == nullptr)
    m_CoreTemperatureOR = new SEScalarTemperature();
  return *m_CoreTemperatureOR;
}
double SEOverride::GetCoreTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_CoreTemperatureOR == nullptr)
    return SEScalar::dNaN();
  return m_CoreTemperatureOR->GetValue(unit);
}
bool SEOverride::HasSkinTemperatureOverride() const
{
  return m_SkinTemperatureOR == nullptr ? false : m_SkinTemperatureOR->IsValid();
}
SEScalarTemperature& SEOverride::GetSkinTemperatureOverride()
{
  if (m_SkinTemperatureOR == nullptr)
    m_SkinTemperatureOR = new SEScalarTemperature();
  return *m_SkinTemperatureOR;
}
double SEOverride::GetSkinTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_SkinTemperatureOR == nullptr)
    return SEScalar::dNaN();
  return m_SkinTemperatureOR->GetValue(unit);
}

bool SEOverride::HasEnergyOverride() const
{
  return HasCoreTemperatureOverride() ? true :
  HasSkinTemperatureOverride() ? true :
  false;
}

void SEOverride::ToString(std::ostream& str) const
{
  str << "Patient Action : Override Parameter";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;

  str << "\n\tState: ";
  HasOverrideSwitch() ? str << GetOverrideSwitch() : str << "Not Set";
  str << "\n\tValid: ";
  HasOverrideConformance() ? str << GetOverrideConformance() : str << "Not Set";
  if (GetOverrideConformance() == CDM::enumOnOff::Off && GetOverrideSwitch() == CDM::enumOnOff::On) {
    str << ("\n\tOverride has turned conformance off. Outputs no longer resemble validated parameters.");
  }
  if (HasMAPOverride())
  {
    str << "\n\tMean Arterial Pressure: ";
    HasMAPOverride() ? str << *m_PressureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On)
      str << "\n\tMean Arterial Pressure has a lower bound of 60 mmHg and an upper bound of 105 mmHg.";
    str << std::flush;
  }
  if (HasHeartRateOverride()) {
    str << "\n\tHeart Rate: ";
    HasHeartRateOverride() ? str << *m_HeartRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On)
      str << "\n\tHeart Rate has a lower bound of 30 bpm and an upper bound of 240 bpm.";
    str << std::flush;
  }
  if (HasCoreTemperatureOverride())
  {
    str << "\n\tCore Temperature: ";
    HasCoreTemperatureOverride() ? str << *m_CoreTemperatureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On)
      str << "\n\tCore Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    str << std::flush;
  }
  if (HasSkinTemperatureOverride()) {
    str << "\n\tSkin Temperature: ";
    HasSkinTemperatureOverride() ? str << *m_SkinTemperatureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On)
      str << "\n\tSkin Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    str << std::flush;
  }
}
}
