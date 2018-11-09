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
  m_ArterialPHOR = nullptr;
  m_VenousPHOR = nullptr;
  m_OverrideConformance = CDM::enumOnOff::Off;
  m_CardiacOutputOR = nullptr;
  m_PressureOR = nullptr;
  m_HeartRateOR = nullptr;
  m_CoreTemperatureOR = nullptr;
  m_SkinTemperatureOR = nullptr;
  m_TotalMetabolicOR = nullptr;
  m_UrineProductionRateOR = nullptr;
  m_RespirationRateOR = nullptr;
  m_TidalVolumeOR = nullptr;
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
  SAFE_DELETE(m_ArterialPHOR);
  SAFE_DELETE(m_VenousPHOR);
  SAFE_DELETE(m_CardiacOutputOR);
  SAFE_DELETE(m_PressureOR);
  SAFE_DELETE(m_HeartRateOR);
  SAFE_DELETE(m_CoreTemperatureOR);
  SAFE_DELETE(m_SkinTemperatureOR);
  SAFE_DELETE(m_TotalMetabolicOR);
  SAFE_DELETE(m_UrineProductionRateOR);
  SAFE_DELETE(m_RespirationRateOR);
  SAFE_DELETE(m_TidalVolumeOR);
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
  if (!HasOverrideSwitch() || !HasOverrideConformance()) {
    return false;
  }
  return (GetOverrideSwitch() == CDM::enumOnOff::On && (GetOverrideConformance() == CDM::enumOnOff::On || GetOverrideConformance() == CDM::enumOnOff::Off));
}

bool SEOverride::Load(const CDM::OverrideData& in)
{
  SEPatientAction::Clear();
  SetOverrideSwitch(in.State());
  SetOverrideConformance(in.Conformant());
  if (in.ArterialBloodPHOverride().present()) {
    GetArterialPHOverride().Load(in.ArterialBloodPHOverride().get());
  } else {
    GetArterialPHOverride().Invalidate();
  }
  if (in.VenousBloodPHOverride().present()) {
    GetVenousPHOverride().Load(in.VenousBloodPHOverride().get());
  } else {
    GetVenousPHOverride().Invalidate();
  }
  if (in.CardiacOutputOverride().present()) {
    GetCardiacOutputOverride().Load(in.CardiacOutputOverride().get());
  } else {
    GetCardiacOutputOverride().Invalidate();
  }
  if (in.MeanArterialPressureOverride().present()) {
    GetMAPOverride().Load(in.MeanArterialPressureOverride().get());
  } else {
    GetMAPOverride().Invalidate();
  }
  if (in.HeartRateOverride().present()) {
    GetHeartRateOverride().Load(in.HeartRateOverride().get());
  } else {
    GetHeartRateOverride().Invalidate();
  }
  if (in.CoreTemperatureOverride().present()) {
    GetCoreTemperatureOverride().Load(in.CoreTemperatureOverride().get());
  } else {
    GetCoreTemperatureOverride().Invalidate();
  }
  if (in.SkinTemperatureOverride().present()) {
    GetSkinTemperatureOverride().Load(in.SkinTemperatureOverride().get());
  } else {
    GetSkinTemperatureOverride().Invalidate();
  }
  if (in.TotalMetabolicRateOverride().present()) {
    GetTotalMetabolicOverride().Load(in.TotalMetabolicRateOverride().get());
  } else {
    GetTotalMetabolicOverride().Invalidate();
  }
  if (in.UrineProductionRateOverride().present()) {
    GetUrineProductionRateOverride().Load(in.UrineProductionRateOverride().get());
  } else {
    GetUrineProductionRateOverride().Invalidate();
  }
  if (in.RespirationRateOverride().present()) {
    GetRespirationRateOverride().Load(in.RespirationRateOverride().get());
  } else {
    GetRespirationRateOverride().Invalidate();
  }
  if (in.TidalVolumeOverride().present()) {
    GetTidalVolumeOverride().Load(in.TidalVolumeOverride().get());
  } else {
    GetTidalVolumeOverride().Invalidate();
  }
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
  if (HasOverrideSwitch()) {
    data.State(m_OverrideSwitch);
  }
  if (HasOverrideConformance()) {
    data.Conformant(m_OverrideConformance);
  }
  if (HasArterialPHOverride()) {
    data.ArterialBloodPHOverride(std::unique_ptr<CDM::ScalarData>(m_ArterialPHOR->Unload()));
  }
  if (HasVenousPHOverride()) {
    data.VenousBloodPHOverride(std::unique_ptr<CDM::ScalarData>(m_VenousPHOR->Unload()));
  }
  if (HasCardiacOutputOverride()) {
    data.CardiacOutputOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_CardiacOutputOR->Unload()));
  }
  if (HasMAPOverride()) {
    data.MeanArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_PressureOR->Unload()));
  }
  if (HasHeartRateOverride()) {
    data.HeartRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateOR->Unload()));
  }
  if (HasCoreTemperatureOverride()) {
    data.CoreTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_CoreTemperatureOR->Unload()));
  }
  if (HasSkinTemperatureOverride()) {
    data.SkinTemperatureOverride(std::unique_ptr<CDM::ScalarTemperatureData>(m_SkinTemperatureOR->Unload()));
  }
  if (HasTotalMetabolicOverride()) {
    data.TotalMetabolicRateOverride(std::unique_ptr<CDM::ScalarPowerData>(m_TotalMetabolicOR->Unload()));
  }
  if (HasUrineProductionRateOverride()) {
    data.UrineProductionRateOverride(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_UrineProductionRateOR->Unload()));
  }
  if (HasRespirationRateOverride()) {
    data.RespirationRateOverride(std::unique_ptr<CDM::ScalarFrequencyData>(m_RespirationRateOR->Unload()));
  }
  if (HasTidalVolumeOverride()) {
    data.TidalVolumeOverride(std::unique_ptr<CDM::ScalarVolumeData>(m_TidalVolumeOR->Unload()));
  }
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

// Blood Chemistry Overrides //
bool SEOverride::HasArterialPHOverride() const
{
  return m_ArterialPHOR == nullptr ? false : m_ArterialPHOR->IsValid();
}
SEScalar& SEOverride::GetArterialPHOverride()
{
  if (m_ArterialPHOR == nullptr) {
    m_ArterialPHOR = new SEScalar();
  }
  return *m_ArterialPHOR;
}
double SEOverride::GetArterialPHOverride() const
{
  if (m_ArterialPHOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialPHOR->GetValue();
}
bool SEOverride::HasVenousPHOverride() const
{
  return m_VenousPHOR == nullptr ? false : m_VenousPHOR->IsValid();
}
SEScalar& SEOverride::GetVenousPHOverride()
{
  if (m_VenousPHOR == nullptr) {
    m_VenousPHOR = new SEScalar();
  }
  return *m_VenousPHOR;
}
double SEOverride::GetVenousPHOverride() const
{
  if (m_VenousPHOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VenousPHOR->GetValue();
}

bool SEOverride::HasBloodChemistryOverride() const
{
  return HasArterialPHOverride() ? true : HasVenousPHOverride() ? true : false;
}

// Cardiovascular Overrides //
bool SEOverride::HasCardiacOutputOverride() const
{
  return m_CardiacOutputOR == nullptr ? false : m_CardiacOutputOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetCardiacOutputOverride()
{
  if (m_CardiacOutputOR == nullptr) {
    m_CardiacOutputOR = new SEScalarVolumePerTime();
  }
  return *m_CardiacOutputOR;
}
double SEOverride::GetCardiacOutputOverride(const VolumePerTimeUnit& unit) const
{
  if (m_CardiacOutputOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CardiacOutputOR->GetValue(unit);
}
bool SEOverride::HasMAPOverride() const
{
  return m_PressureOR == nullptr ? false : m_PressureOR->IsValid();
}
SEScalarPressure& SEOverride::GetMAPOverride()
{
  if (m_PressureOR == nullptr) {
    m_PressureOR = new SEScalarPressure();
  }
  return *m_PressureOR;
}
double SEOverride::GetMAPOverride(const PressureUnit& unit) const
{
  if (m_PressureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PressureOR->GetValue(unit);
}
bool SEOverride::HasHeartRateOverride() const
{
  return m_HeartRateOR == nullptr ? false : m_HeartRateOR->IsValid();
}
SEScalarFrequency& SEOverride::GetHeartRateOverride()
{
  if (m_HeartRateOR == nullptr) {
    m_HeartRateOR = new SEScalarFrequency();
  }
  return *m_HeartRateOR;
}
double SEOverride::GetHeartRateOverride(const FrequencyUnit& unit) const
{
  if (m_HeartRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartRateOR->GetValue(unit);
}

bool SEOverride::HasCardiovascularOverride() const
{
  return HasCardiacOutputOverride() ? true :
  HasMAPOverride() ? true : 
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
  if (m_CoreTemperatureOR == nullptr) {
    m_CoreTemperatureOR = new SEScalarTemperature();
  }
  return *m_CoreTemperatureOR;
}
double SEOverride::GetCoreTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_CoreTemperatureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CoreTemperatureOR->GetValue(unit);
}
bool SEOverride::HasSkinTemperatureOverride() const
{
  return m_SkinTemperatureOR == nullptr ? false : m_SkinTemperatureOR->IsValid();
}
SEScalarTemperature& SEOverride::GetSkinTemperatureOverride()
{
  if (m_SkinTemperatureOR == nullptr) {
    m_SkinTemperatureOR = new SEScalarTemperature();
  }
  return *m_SkinTemperatureOR;
}
double SEOverride::GetSkinTemperatureOverride(const TemperatureUnit& unit) const
{
  if (m_SkinTemperatureOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SkinTemperatureOR->GetValue(unit);
}
bool SEOverride::HasTotalMetabolicOverride() const
{
  return m_TotalMetabolicOR == nullptr ? false : m_TotalMetabolicOR->IsValid();
}
SEScalarPower& SEOverride::GetTotalMetabolicOverride()
{
  if (m_TotalMetabolicOR == nullptr) {
    m_TotalMetabolicOR = new SEScalarPower();
  }
  return *m_TotalMetabolicOR;
}
double SEOverride::GetTotalMetabolicOverride(const PowerUnit& unit) const
{
  if (m_TotalMetabolicOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalMetabolicOR->GetValue(unit);
}

bool SEOverride::HasEnergyOverride() const
{
  return HasCoreTemperatureOverride() ? true :
  HasSkinTemperatureOverride() ? true :
  HasTotalMetabolicOverride() ? true :
  false;
}

// Renal Overrides //
bool SEOverride::HasUrineProductionRateOverride() const
{
  return m_UrineProductionRateOR == nullptr ? false : m_UrineProductionRateOR->IsValid();
}
SEScalarVolumePerTime& SEOverride::GetUrineProductionRateOverride()
{
  if (m_UrineProductionRateOR == nullptr) {
    m_UrineProductionRateOR = new SEScalarVolumePerTime();
  }
  return *m_UrineProductionRateOR;
}
double SEOverride::GetUrineProductionRateOverride(const VolumePerTimeUnit& unit) const
{
  if (m_UrineProductionRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_UrineProductionRateOR->GetValue(unit);
}

bool SEOverride::HasRenalOverride() const
{
  return HasUrineProductionRateOverride() ? true : false;
}


// Respiratory Overrides //
bool SEOverride::HasRespirationRateOverride() const
{
  return m_RespirationRateOR == nullptr ? false : m_RespirationRateOR->IsValid();
}
SEScalarFrequency& SEOverride::GetRespirationRateOverride()
{
  if (m_RespirationRateOR == nullptr) {
    m_RespirationRateOR = new SEScalarFrequency();
  }
  return *m_RespirationRateOR;
}
double SEOverride::GetRespirationRateOverride(const FrequencyUnit& unit) const
{
  if (m_RespirationRateOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RespirationRateOR->GetValue(unit);
}
bool SEOverride::HasTidalVolumeOverride() const
{
  return m_TidalVolumeOR == nullptr ? false : m_TidalVolumeOR->IsValid();
}
SEScalarVolume& SEOverride::GetTidalVolumeOverride()
{
  if (m_TidalVolumeOR == nullptr) {
    m_TidalVolumeOR = new SEScalarVolume();
  }
  return *m_TidalVolumeOR;
}
double SEOverride::GetTidalVolumeOverride(const VolumeUnit& unit) const
{
  if (m_TidalVolumeOR == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TidalVolumeOR->GetValue(unit);
}

bool SEOverride::HasRespiratoryOverride() const
{
  return HasRespirationRateOverride() ? true : HasTidalVolumeOverride() ? true : false;
}


void SEOverride::ToString(std::ostream& str) const
{
  str << "Patient Action : Override Parameter";
  if (HasComment()) {
    str << "\n\tComment: " << m_Comment;
  }

  str << "\n\tState: ";
  HasOverrideSwitch() ? str << GetOverrideSwitch() : str << "Not Set";
  str << "\n\tConformant: ";
  HasOverrideConformance() ? str << GetOverrideConformance() : str << "Not Set";
  if (GetOverrideConformance() == CDM::enumOnOff::Off && GetOverrideSwitch() == CDM::enumOnOff::On) {
    str << ("\n\tOverride has turned conformance off. Outputs no longer resemble validated parameters.");
  }
  if (HasArterialPHOverride()) {
    str << "\n\tArterial PH: ";
    HasArterialPHOverride() ? str << *m_ArterialPHOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tArterial PH has a lower bound of 0 and an upper bound of 14.";
    }
    str << std::flush;
  }
  if (HasVenousPHOverride()) {
    str << "\n\tVenous PH: ";
    HasVenousPHOverride() ? str << *m_VenousPHOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tVenous PH has a lower bound of 0 and an upper bound of 14.";
    }
    str << std::flush;
  }
  if (HasCardiacOutputOverride()) {
    str << "\n\tCardiac Output: ";
    HasCardiacOutputOverride() ? str << *m_CardiacOutputOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCardiac Output has a lower bound of 0 L/min and an upper bound of 100 L/min.";
    }
    str << std::flush;
  }
  if (HasMAPOverride())
  {
    str << "\n\tMean Arterial Pressure: ";
    HasMAPOverride() ? str << *m_PressureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tMean Arterial Pressure has a lower bound of 60 mmHg and an upper bound of 105 mmHg.";
    } else {
      str << "\n\tPharmacodynamics have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasHeartRateOverride()) {
    str << "\n\tHeart Rate: ";
    HasHeartRateOverride() ? str << *m_HeartRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tHeart Rate has a lower bound of 30 bpm and an upper bound of 240 bpm.";
    } else {
      str << "\n\tPharmacodynamics have been turned off due to conformance being turned off.";
    }
    str << std::flush;
  }
  if (HasCoreTemperatureOverride())
  {
    str << "\n\tCore Temperature: ";
    HasCoreTemperatureOverride() ? str << *m_CoreTemperatureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tCore Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    }
    str << std::flush;
  }
  if (HasSkinTemperatureOverride()) {
    str << "\n\tSkin Temperature: ";
    HasSkinTemperatureOverride() ? str << *m_SkinTemperatureOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tSkin Temperature has a lower bound of 0 degrees Celsius and an upper bound of 200 degrees Celsius.";
    }
    str << std::flush;
  }
  if (HasTotalMetabolicOverride()) {
    str << "\n\tTotal Metabolic Rate: ";
    HasTotalMetabolicOverride() ? str << *m_TotalMetabolicOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTotal Metabolic Rate has a lower bound of 1 kcal/day and an upper bound of 5000 kcal/day.";
    }
    str << std::flush;
  }
  if (HasUrineProductionRateOverride()) {
    str << "\n\tUrine Production Rate: ";
    HasUrineProductionRateOverride() ? str << *m_UrineProductionRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tUrine Production Rate has a lower bound of 0 mL/min and an upper bound of 1000 mL/min.";
    }
    str << std::flush;
  }
  if (HasRespirationRateOverride()) {
    str << "\n\tRespiration Rate: ";
    HasRespirationRateOverride() ? str << *m_RespirationRateOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tRespiration Rate has a lower bound of 0 breaths/min and an upper bound of 60 breaths/min.";
    }
    str << std::flush;
  }
  if (HasTidalVolumeOverride()) {
    str << "\n\tTidal Volume: ";
    HasTidalVolumeOverride() ? str << *m_TidalVolumeOR : str << "Not Set";
    if (m_OverrideConformance == CDM::enumOnOff::On) {
      str << "\n\tTidal Volume has a lower bound of 0 mL and an upper bound of 10000 mL.";
    }
    str << std::flush;
  }
}
}
