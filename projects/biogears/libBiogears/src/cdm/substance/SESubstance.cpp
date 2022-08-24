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
#include <biogears/cdm/substance/SESubstance.h>

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>

namespace biogears {
SESubstance::SESubstance(Logger* logger)
  : Loggable(logger)
{
  m_Name = "";
  m_State = (CDM::enumSubstanceState::value)-1;
  m_Classification = (CDM::enumSubstanceClass::value)-1;
  m_Density = nullptr;
  m_MolarMass = nullptr;

  m_MaximumDiffusionFlux = nullptr;
  m_MichaelisCoefficient = nullptr;
  m_MembraneResistance = nullptr;

  m_Aerosolization = nullptr;
  m_AreaUnderCurve = nullptr;
  m_BloodConcentration = nullptr;
  m_EffectSiteConcentration = nullptr;
  m_MassInBody = nullptr;
  m_MassInBlood = nullptr;
  m_MassInTissue = nullptr;
  m_PlasmaConcentration = nullptr;
  m_SystemicMassCleared = nullptr;
  m_TissueConcentration = nullptr;

  m_AlveolarTransfer = nullptr;
  m_DiffusingCapacity = nullptr;
  m_EndTidalFraction = nullptr;
  m_EndTidalPressure = nullptr;
  m_SolubilityCoefficient = nullptr;
  m_RelativeDiffusionCoefficient = nullptr;

  m_Clearance = nullptr;
  m_PK = nullptr;
  m_PD = nullptr;
}
//-----------------------------------------------------------------------------
SESubstance::~SESubstance()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstance::Clear()
{
  m_Name = "";
  m_State = (CDM::enumSubstanceState::value)-1;
  m_Classification = (CDM::enumSubstanceClass::value)-1;
  SAFE_DELETE(m_Density);
  SAFE_DELETE(m_MolarMass);

  SAFE_DELETE(m_MaximumDiffusionFlux);
  SAFE_DELETE(m_MichaelisCoefficient);
  SAFE_DELETE(m_MembraneResistance);
  SAFE_DELETE(m_AreaUnderCurve);
  SAFE_DELETE(m_BloodConcentration);
  SAFE_DELETE(m_EffectSiteConcentration);
  SAFE_DELETE(m_MassInBody);
  SAFE_DELETE(m_MassInBlood);
  SAFE_DELETE(m_MassInTissue);
  SAFE_DELETE(m_PlasmaConcentration);
  SAFE_DELETE(m_SystemicMassCleared);
  SAFE_DELETE(m_TissueConcentration);

  SAFE_DELETE(m_AlveolarTransfer);
  SAFE_DELETE(m_DiffusingCapacity);
  SAFE_DELETE(m_EndTidalFraction);
  SAFE_DELETE(m_EndTidalPressure);
  SAFE_DELETE(m_SolubilityCoefficient);
  SAFE_DELETE(m_RelativeDiffusionCoefficient);

  SAFE_DELETE(m_Aerosolization);
  SAFE_DELETE(m_Clearance);
  SAFE_DELETE(m_PK);
  SAFE_DELETE(m_PD);
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstance::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstance::GetScalar(const std::string& name)
{
  if (name.compare("Density") == 0)
    return &GetDensity();
  if (name.compare("MolarMass") == 0)
    return &GetMolarMass();

  if (name.compare("MaximumDiffusionFlux") == 0)
    return &GetMaximumDiffusionFlux();
  if (name.compare("MichaelisCoefficient") == 0)
    return &GetMichaelisCoefficient();
  if (name.compare("MembraneConductivity") == 0)
    return &GetMembraneResistance();
  if (name.compare("AreaUnderCurve") == 0)
    return &GetAreaUnderCurve();
  if (name.compare("BloodConcentration") == 0)
    return &GetBloodConcentration();
  if (name.compare("EffectSiteConcentration") == 0)
    return &GetEffectSiteConcentration();
  if (name.compare("MassInBody") == 0)
    return &GetMassInBody();
  if (name.compare("MassInBlood") == 0)
    return &GetMassInBlood();
  if (name.compare("MassInTissue") == 0)
    return &GetMassInTissue();
  if (name.compare("PlasmaConcentration") == 0)
    return &GetPlasmaConcentration();
  if (name.compare("SystemicMassCleared") == 0)
    return &GetSystemicMassCleared();
  if (name.compare("TissueConcentration") == 0)
    return &GetTissueConcentration();

  if (name.compare("AlveolarTransfer") == 0)
    return &GetAlveolarTransfer();
  if (name.compare("DiffusingCapacity") == 0)
    return &GetDiffusingCapacity();
  if (name.compare("EndTidalFraction") == 0)
    return &GetEndTidalFraction();
  if (name.compare("EndTidalPressure") == 0)
    return &GetEndTidalPressure();
  if (name.compare("RelativeDiffusionCoefficient") == 0)
    return &GetRelativeDiffusionCoefficient();
  if (name.compare("SolubilityCoefficient") == 0)
    return &GetSolubilityCoefficient();

  size_t split = name.find('-');
  if (split != name.npos) {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "Aerosolization")
      return GetAerosolization().GetScalar(prop);
    if (child == "Clearance")
      return GetClearance().GetScalar(prop);
    if (child == "PK")
      return GetPK().GetScalar(prop);
    if (child == "PD")
      return GetPD().GetScalar(prop);
  }

  return nullptr;
}
//-----------------------------------------------------------------------------
bool SESubstance::Load(const CDM::SubstanceData& in)
{
  Clear();
  m_Name = in.Name();

  if (in.State().present())
    m_State = in.State().get();
  if (in.Classification().present())
    m_Classification = in.Classification().get();
  if (in.Density().present())
    GetDensity().Load(in.Density().get());
  if (in.MolarMass().present())
    GetMolarMass().Load(in.MolarMass().get());

  if (in.MaximumDiffusionFlux().present())
    GetMaximumDiffusionFlux().Load(in.MaximumDiffusionFlux().get());
  if (in.MichaelisCoefficient().present())
    GetMichaelisCoefficient().Load(in.MichaelisCoefficient().get());
  if (in.MembraneResistance().present())
    GetMembraneResistance().Load(in.MembraneResistance().get());
  if (in.AreaUnderCurve().present())
    GetAreaUnderCurve().Load(in.AreaUnderCurve().get());
  if (in.BloodConcentration().present())
    GetBloodConcentration().Load(in.BloodConcentration().get());
  if (in.EffectSiteConcentration().present())
    GetEffectSiteConcentration().Load(in.EffectSiteConcentration().get());
  if (in.MassInBody().present())
    GetMassInBody().Load(in.MassInBody().get());
  if (in.MassInBlood().present())
    GetMassInBlood().Load(in.MassInBlood().get());
  if (in.MassInTissue().present())
    GetMassInTissue().Load(in.MassInTissue().get());
  if (in.PlasmaConcentration().present())
    GetPlasmaConcentration().Load(in.PlasmaConcentration().get());
  if (in.SystemicMassCleared().present())
    GetSystemicMassCleared().Load(in.SystemicMassCleared().get());
  if (in.TissueConcentration().present())
    GetTissueConcentration().Load(in.TissueConcentration().get());

  if (in.AlveolarTransfer().present())
    GetAlveolarTransfer().Load(in.AlveolarTransfer().get());
  if (in.DiffusingCapacity().present())
    GetDiffusingCapacity().Load(in.DiffusingCapacity().get());
  if (in.EndTidalFraction().present())
    GetEndTidalFraction().Load(in.EndTidalFraction().get());
  if (in.EndTidalPressure().present())
    GetEndTidalPressure().Load(in.EndTidalPressure().get());
  if (in.RelativeDiffusionCoefficient().present())
    GetRelativeDiffusionCoefficient().Load(in.RelativeDiffusionCoefficient().get());
  if (in.SolubilityCoefficient().present())
    GetSolubilityCoefficient().Load(in.SolubilityCoefficient().get());

  if (in.Aerosolization().present())
    GetAerosolization().Load(in.Aerosolization().get());
  if (in.Clearance().present())
    GetClearance().Load(in.Clearance().get());
  if (in.Pharmacokinetics().present())
    GetPK().Load(in.Pharmacokinetics().get());
  if (in.Pharmacodynamics().present())
    GetPD().Load(in.Pharmacodynamics().get());

  if (HasClearance() && HasPK() &&
    GetPK().HasPhysicochemicals() && 
    GetClearance().HasFractionUnboundInPlasma() &&
    !GetClearance().GetFractionUnboundInPlasma().Equals(GetPK().GetPhysicochemicals().GetFractionUnboundInPlasma())) {
    Fatal("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
  }

  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstanceData* SESubstance::Unload() const
{
  CDM::SubstanceData* data = new CDM::SubstanceData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstance::Unload(CDM::SubstanceData& data) const
{
  if (HasName()) {
    data.Name(m_Name);
  } else {
    data.Name("Unknown Substance");
  }
  if (HasState())
    data.State(m_State);
  if (HasClassification())
    data.Classification(m_Classification);
  if (HasDensity())
    data.Density(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Density->Unload()));
  if (HasMolarMass())
    data.MolarMass(std::unique_ptr<CDM::ScalarMassPerAmountData>(m_MolarMass->Unload()));

  if (HasMaximumDiffusionFlux())
    data.MaximumDiffusionFlux(std::unique_ptr<CDM::ScalarMassPerAreaTimeData>(m_MaximumDiffusionFlux->Unload()));
  if (HasMichaelisCoefficient())
    data.MichaelisCoefficient(std::unique_ptr<CDM::ScalarData>(m_MichaelisCoefficient->Unload()));
  if (HasMembraneResistance())
    data.MembraneResistance(std::unique_ptr<CDM::ScalarElectricResistanceData>(m_MembraneResistance->Unload()));
  if (HasAreaUnderCurve())
    data.AreaUnderCurve(std::unique_ptr<CDM::ScalarTimeMassPerVolumeData>(m_AreaUnderCurve->Unload()));
  if (HasBloodConcentration())
    data.BloodConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_BloodConcentration->Unload()));
  if (HasEffectSiteConcentration())
    data.EffectSiteConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_EffectSiteConcentration->Unload()));
  if (HasMassInBody())
    data.MassInBody(std::unique_ptr<CDM::ScalarMassData>(m_MassInBody->Unload()));
  if (HasMassInBlood())
    data.MassInBlood(std::unique_ptr<CDM::ScalarMassData>(m_MassInBlood->Unload()));
  if (HasMassInTissue())
    data.MassInTissue(std::unique_ptr<CDM::ScalarMassData>(m_MassInTissue->Unload()));
  if (HasPlasmaConcentration())
    data.PlasmaConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_PlasmaConcentration->Unload()));
  if (HasSystemicMassCleared())
    data.SystemicMassCleared(std::unique_ptr<CDM::ScalarMassData>(m_SystemicMassCleared->Unload()));
  if (HasTissueConcentration())
    data.TissueConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_TissueConcentration->Unload()));

  if (HasAlveolarTransfer())
    data.AlveolarTransfer(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_AlveolarTransfer->Unload()));
  if (HasDiffusingCapacity())
    data.DiffusingCapacity(std::unique_ptr<CDM::ScalarVolumePerTimePressureData>(m_DiffusingCapacity->Unload()));
  if (HasEndTidalFraction())
    data.EndTidalFraction(std::unique_ptr<CDM::ScalarFractionData>(m_EndTidalFraction->Unload()));
  if (HasEndTidalPressure())
    data.EndTidalPressure(std::unique_ptr<CDM::ScalarPressureData>(m_EndTidalPressure->Unload()));
  if (HasSolubilityCoefficient())
    data.SolubilityCoefficient(std::unique_ptr<CDM::ScalarInversePressureData>(m_SolubilityCoefficient->Unload()));
  if (HasRelativeDiffusionCoefficient())
    data.RelativeDiffusionCoefficient(std::unique_ptr<CDM::ScalarData>(m_RelativeDiffusionCoefficient->Unload()));

  if (HasAerosolization())
    data.Aerosolization(std::unique_ptr<CDM::SubstanceAerosolizationData>(m_Aerosolization->Unload()));
  if (HasClearance())
    data.Clearance(std::unique_ptr<CDM::SubstanceClearanceData>(m_Clearance->Unload()));
  if (HasPK())
    data.Pharmacokinetics(std::unique_ptr<CDM::SubstancePharmacokineticsData>(m_PK->Unload()));
  if (HasPD())
    data.Pharmacodynamics(std::unique_ptr<CDM::SubstancePharmacodynamicsData>(m_PD->Unload()));
};
//-----------------------------------------------------------------------------
std::string SESubstance::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SESubstance::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SESubstance::SetName(const char* name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
void SESubstance::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstance::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
CDM::enumSubstanceState::value SESubstance::GetState() const
{
  return m_State;
}
//-----------------------------------------------------------------------------
void SESubstance::SetState(CDM::enumSubstanceState::value state)
{
  m_State = state;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasState() const
{
  return m_State == ((CDM::enumSubstanceState::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstance::InvalidateState()
{
  m_State = (CDM::enumSubstanceState::value)-1;
}
//-----------------------------------------------------------------------------
CDM::enumSubstanceClass::value SESubstance::GetClassification() const
{
  return m_Classification;
}
//-----------------------------------------------------------------------------
void SESubstance::SetClassification(CDM::enumSubstanceClass::value subClass)
{
  m_Classification = subClass;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasClassification() const
{
  return m_Classification == ((CDM::enumSubstanceClass::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstance::InvalidateClassification()
{
  m_Classification = (CDM::enumSubstanceClass::value)-1;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasDensity() const
{
  return (m_Density == nullptr) ? false : m_Density->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetDensity()
{
  if (m_Density == nullptr)
    m_Density = new SEScalarMassPerVolume();
  return *m_Density;
}
//-----------------------------------------------------------------------------
double SESubstance::GetDensity(const MassPerVolumeUnit& unit) const
{
  if (m_Density == nullptr)
    return SEScalar::dNaN();
  return m_Density->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMolarMass() const
{
  return (m_MolarMass == nullptr) ? false : m_MolarMass->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerAmount& SESubstance::GetMolarMass()
{
  if (m_MolarMass == nullptr)
    m_MolarMass = new SEScalarMassPerAmount();
  return *m_MolarMass;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMolarMass(const MassPerAmountUnit& unit) const
{
  if (m_MolarMass == nullptr)
    return SEScalar::dNaN();
  return m_MolarMass->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMaximumDiffusionFlux() const
{
  return (m_MaximumDiffusionFlux == nullptr) ? false : m_MaximumDiffusionFlux->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerAreaTime& SESubstance::GetMaximumDiffusionFlux()
{
  if (m_MaximumDiffusionFlux == nullptr)
    m_MaximumDiffusionFlux = new SEScalarMassPerAreaTime();
  return *m_MaximumDiffusionFlux;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMaximumDiffusionFlux(const MassPerAreaTimeUnit& unit) const
{
  if (m_MaximumDiffusionFlux == nullptr)
    return SEScalar::dNaN();
  return m_MaximumDiffusionFlux->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMichaelisCoefficient() const
{
  return (m_MichaelisCoefficient == nullptr) ? false : m_MichaelisCoefficient->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstance::GetMichaelisCoefficient()
{
  if (m_MichaelisCoefficient == nullptr)
    m_MichaelisCoefficient = new SEScalar();
  return *m_MichaelisCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMichaelisCoefficient() const
{
  if (m_MichaelisCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_MichaelisCoefficient->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMembraneResistance() const
{
  return (m_MembraneResistance == nullptr) ? false : m_MembraneResistance->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarElectricResistance& SESubstance::GetMembraneResistance()
{
  if (m_MembraneResistance == nullptr)
    m_MembraneResistance = new SEScalarElectricResistance();
  return *m_MembraneResistance;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMembraneResistance(const ElectricResistanceUnit& unit) const
{
  if (m_MembraneResistance == nullptr)
    return SEScalarElectricResistance::dNaN();
  return m_MembraneResistance->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasAerosolization() const
{
  return (m_Aerosolization != nullptr && m_Aerosolization->IsValid());
}
//-----------------------------------------------------------------------------
SESubstanceAerosolization& SESubstance::GetAerosolization()
{
  if (m_Aerosolization == nullptr)
    m_Aerosolization = new SESubstanceAerosolization(GetLogger());
  return *m_Aerosolization;
}
//-----------------------------------------------------------------------------
const SESubstanceAerosolization* SESubstance::GetAerosolization() const
{
  return m_Aerosolization;
}
//-----------------------------------------------------------------------------
void SESubstance::RemoveAerosolization()
{
  SAFE_DELETE(m_Aerosolization);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasAreaUnderCurve() const
{
  return (m_AreaUnderCurve == nullptr) ? false : m_AreaUnderCurve->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTimeMassPerVolume& SESubstance::GetAreaUnderCurve()
{
  if (m_AreaUnderCurve == nullptr)
    m_AreaUnderCurve = new SEScalarTimeMassPerVolume();
  return *m_AreaUnderCurve;
}
//-----------------------------------------------------------------------------
double SESubstance::GetAreaUnderCurve(const TimeMassPerVolumeUnit& unit) const
{
  if (m_AreaUnderCurve == nullptr)
    return SEScalar::dNaN();
  return m_AreaUnderCurve->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasBloodConcentration() const
{
  return (m_BloodConcentration == nullptr) ? false : m_BloodConcentration->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetBloodConcentration()
{
  if (m_BloodConcentration == nullptr)
    m_BloodConcentration = new SEScalarMassPerVolume();
  return *m_BloodConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetBloodConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_BloodConcentration == nullptr)
    return SEScalar::dNaN();
  return m_BloodConcentration->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasEffectSiteConcentration() const
{
  return (m_EffectSiteConcentration == nullptr) ? false : m_EffectSiteConcentration->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetEffectSiteConcentration()
{
  if (m_EffectSiteConcentration == nullptr)
    m_EffectSiteConcentration = new SEScalarMassPerVolume();
  return *m_EffectSiteConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetEffectSiteConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_EffectSiteConcentration == nullptr)
    return SEScalar::dNaN();
  return m_EffectSiteConcentration->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMassInBody() const
{
  return (m_MassInBody == nullptr) ? false : m_MassInBody->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetMassInBody()
{
  if (m_MassInBody == nullptr)
    m_MassInBody = new SEScalarMass();
  return *m_MassInBody;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMassInBody(const MassUnit& unit) const
{
  if (m_MassInBody == nullptr)
    return SEScalar::dNaN();
  return m_MassInBody->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMassInBlood() const
{
  return (m_MassInBlood == nullptr) ? false : m_MassInBlood->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetMassInBlood()
{
  if (m_MassInBlood == nullptr)
    m_MassInBlood = new SEScalarMass();
  return *m_MassInBlood;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMassInBlood(const MassUnit& unit) const
{
  if (m_MassInBlood == nullptr)
    return SEScalar::dNaN();
  return m_MassInBlood->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMassInTissue() const
{
  return (m_MassInTissue == nullptr) ? false : m_MassInTissue->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetMassInTissue()
{
  if (m_MassInTissue == nullptr)
    m_MassInTissue = new SEScalarMass();
  return *m_MassInTissue;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMassInTissue(const MassUnit& unit) const
{
  if (m_MassInTissue == nullptr)
    return SEScalar::dNaN();
  return m_MassInTissue->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasPlasmaConcentration() const
{
  return (m_PlasmaConcentration == nullptr) ? false : m_PlasmaConcentration->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetPlasmaConcentration()
{
  if (m_PlasmaConcentration == nullptr)
    m_PlasmaConcentration = new SEScalarMassPerVolume();
  return *m_PlasmaConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetPlasmaConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_PlasmaConcentration == nullptr)
    return SEScalar::dNaN();
  return m_PlasmaConcentration->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasSystemicMassCleared() const
{
  return (m_SystemicMassCleared == nullptr) ? false : m_SystemicMassCleared->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetSystemicMassCleared()
{
  if (m_SystemicMassCleared == nullptr)
    m_SystemicMassCleared = new SEScalarMass();
  return *m_SystemicMassCleared;
}
//-----------------------------------------------------------------------------
double SESubstance::GetSystemicMassCleared(const MassUnit& unit) const
{
  if (m_SystemicMassCleared == nullptr)
    return SEScalar::dNaN();
  return m_SystemicMassCleared->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasTissueConcentration() const
{
  return (m_TissueConcentration == nullptr) ? false : m_TissueConcentration->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetTissueConcentration()
{
  if (m_TissueConcentration == nullptr)
    m_TissueConcentration = new SEScalarMassPerVolume();
  return *m_TissueConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetTissueConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_TissueConcentration == nullptr)
    return SEScalar::dNaN();
  return m_TissueConcentration->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasAlveolarTransfer() const
{
  return m_AlveolarTransfer == nullptr ? false : m_AlveolarTransfer->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTime& SESubstance::GetAlveolarTransfer()
{
  if (m_AlveolarTransfer == nullptr)
    m_AlveolarTransfer = new SEScalarVolumePerTime();
  return *m_AlveolarTransfer;
}
//-----------------------------------------------------------------------------
double SESubstance::GetAlveolarTransfer(const VolumePerTimeUnit& unit) const
{
  if (m_AlveolarTransfer == nullptr)
    return SEScalar::dNaN();
  return m_AlveolarTransfer->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasDiffusingCapacity() const
{
  return m_DiffusingCapacity == nullptr ? false : m_DiffusingCapacity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimePressure& SESubstance::GetDiffusingCapacity()
{
  if (m_DiffusingCapacity == nullptr)
    m_DiffusingCapacity = new SEScalarVolumePerTimePressure();
  return *m_DiffusingCapacity;
}
//-----------------------------------------------------------------------------
double SESubstance::GetDiffusingCapacity(const VolumePerTimePressureUnit& unit) const
{
  if (m_DiffusingCapacity == nullptr)
    return SEScalar::dNaN();
  return m_DiffusingCapacity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasEndTidalFraction() const
{
  return (m_EndTidalFraction == nullptr) ? false : m_EndTidalFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstance::GetEndTidalFraction()
{
  if (m_EndTidalFraction == nullptr)
    m_EndTidalFraction = new SEScalarFraction();
  return *m_EndTidalFraction;
}
//-----------------------------------------------------------------------------
double SESubstance::GetEndTidalFraction() const
{
  if (m_EndTidalFraction == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalFraction->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasEndTidalPressure() const
{
  return (m_EndTidalPressure == nullptr) ? false : m_EndTidalPressure->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SESubstance::GetEndTidalPressure()
{
  if (m_EndTidalPressure == nullptr)
    m_EndTidalPressure = new SEScalarPressure();
  return *m_EndTidalPressure;
}
//-----------------------------------------------------------------------------
double SESubstance::GetEndTidalPressure(const PressureUnit& unit) const
{
  if (m_EndTidalPressure == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalPressure->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasRelativeDiffusionCoefficient() const
{
  return (m_RelativeDiffusionCoefficient == nullptr) ? false : m_RelativeDiffusionCoefficient->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstance::GetRelativeDiffusionCoefficient()
{
  if (m_RelativeDiffusionCoefficient == nullptr)
    m_RelativeDiffusionCoefficient = new SEScalar();
  return *m_RelativeDiffusionCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstance::GetRelativeDiffusionCoefficient() const
{
  if (m_RelativeDiffusionCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_RelativeDiffusionCoefficient->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasSolubilityCoefficient() const
{
  return (m_SolubilityCoefficient == nullptr) ? false : m_SolubilityCoefficient->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarInversePressure& SESubstance::GetSolubilityCoefficient()
{
  if (m_SolubilityCoefficient == nullptr)
    m_SolubilityCoefficient = new SEScalarInversePressure();
  return *m_SolubilityCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstance::GetSolubilityCoefficient(const InversePressureUnit& unit) const
{
  if (m_SolubilityCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_SolubilityCoefficient->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasClearance() const
{
  return (m_Clearance != nullptr && m_Clearance->IsValid());
}
//-----------------------------------------------------------------------------
SESubstanceClearance& SESubstance::GetClearance()
{
  if (m_Clearance == nullptr)
    m_Clearance = new SESubstanceClearance(GetLogger());
  return *m_Clearance;
}
//-----------------------------------------------------------------------------
const SESubstanceClearance* SESubstance::GetClearance() const
{
  return m_Clearance;
}
//-----------------------------------------------------------------------------
void SESubstance::RemoveClearance()
{
  SAFE_DELETE(m_Clearance);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasPK() const
{
  return (m_PK != nullptr && m_PK->IsValid());
}
//-----------------------------------------------------------------------------
SESubstancePharmacokinetics& SESubstance::GetPK()
{
  if (m_PK == nullptr)
    m_PK = new SESubstancePharmacokinetics(GetLogger());
  return *m_PK;
}
//-----------------------------------------------------------------------------
const SESubstancePharmacokinetics* SESubstance::GetPK() const
{
  return m_PK;
}
//-----------------------------------------------------------------------------
void SESubstance::RemovePK()
{
  SAFE_DELETE(m_PK);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasPD() const
{
  return (m_PD != nullptr && m_PD->IsValid());
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics& SESubstance::GetPD()
{
  if (m_PD == nullptr)
    m_PD = new SESubstancePharmacodynamics(GetLogger());
  return *m_PD;
}
//-----------------------------------------------------------------------------
const SESubstancePharmacodynamics* SESubstance::GetPD() const
{
  return m_PD;
}
//-----------------------------------------------------------------------------
void SESubstance::RemovePD()
{
  SAFE_DELETE(m_PD);
}
//-------------------------------------------------------------------------------
bool SESubstance::operator==(const SESubstance& rhs) const
{
  bool equivilant = m_Name == rhs.m_Name;
  equivilant &= m_Classification == rhs.m_Classification;
  equivilant &= m_State == rhs.m_State;

  equivilant &= (m_Density && rhs.m_Density) ? m_Density->operator==(*rhs.m_Density) : m_Density == rhs.m_Density;
  equivilant &= (m_MolarMass && rhs.m_MolarMass) ? m_MolarMass->operator==(*rhs.m_MolarMass) : m_MolarMass == rhs.m_MolarMass;

  equivilant &= (m_MaximumDiffusionFlux && rhs.m_MaximumDiffusionFlux) ? m_MaximumDiffusionFlux->operator==(*rhs.m_MaximumDiffusionFlux) : m_MaximumDiffusionFlux == rhs.m_MaximumDiffusionFlux;
  equivilant &= (m_MichaelisCoefficient && rhs.m_MichaelisCoefficient) ? m_MichaelisCoefficient->operator==(*rhs.m_MichaelisCoefficient) : m_MichaelisCoefficient == rhs.m_MichaelisCoefficient;
  equivilant &= (m_MembraneResistance && rhs.m_MembraneResistance) ? m_MembraneResistance->operator==(*rhs.m_MembraneResistance) : m_MembraneResistance == rhs.m_MembraneResistance;


  equivilant &= (m_Aerosolization && rhs.m_Aerosolization) ? m_Aerosolization->operator==(*rhs.m_Aerosolization) : m_Aerosolization == rhs.m_Aerosolization;
  equivilant &= (m_AreaUnderCurve && rhs.m_AreaUnderCurve) ? m_AreaUnderCurve->operator==(*rhs.m_AreaUnderCurve) : m_AreaUnderCurve == rhs.m_AreaUnderCurve;
  equivilant &= (m_BloodConcentration && rhs.m_BloodConcentration) ? m_BloodConcentration->operator==(*rhs.m_BloodConcentration) : m_BloodConcentration == rhs.m_BloodConcentration;
  equivilant &= (m_EffectSiteConcentration && rhs.m_EffectSiteConcentration) ? m_EffectSiteConcentration->operator==(*rhs.m_EffectSiteConcentration) : m_EffectSiteConcentration == rhs.m_EffectSiteConcentration;
  equivilant &= (m_MassInBody && rhs.m_MassInBody) ? m_MassInBody->operator==(*rhs.m_MassInBody) : m_MassInBody == rhs.m_MassInBody;
  equivilant &= (m_MassInBlood && rhs.m_MassInBlood) ? m_MassInBlood->operator==(*rhs.m_MassInBlood) : m_MassInBlood == rhs.m_MassInBlood;
  equivilant &= (m_MassInTissue && rhs.m_MassInTissue) ? m_MassInTissue->operator==(*rhs.m_MassInTissue) : m_MassInTissue == rhs.m_MassInTissue;
  equivilant &= (m_PlasmaConcentration && rhs.m_PlasmaConcentration) ? m_PlasmaConcentration->operator==(*rhs.m_PlasmaConcentration) : m_PlasmaConcentration == rhs.m_PlasmaConcentration;
  equivilant &= (m_SystemicMassCleared && rhs.m_SystemicMassCleared) ? m_SystemicMassCleared->operator==(*rhs.m_SystemicMassCleared) : m_SystemicMassCleared == rhs.m_SystemicMassCleared;
  equivilant &= (m_TissueConcentration && rhs.m_TissueConcentration) ? m_TissueConcentration->operator==(*rhs.m_TissueConcentration) : m_TissueConcentration == rhs.m_TissueConcentration;

  equivilant &= (m_AlveolarTransfer && rhs.m_AlveolarTransfer) ? m_AlveolarTransfer->operator==(*rhs.m_AlveolarTransfer) : m_AlveolarTransfer == rhs.m_AlveolarTransfer;
  equivilant &= (m_DiffusingCapacity && rhs.m_DiffusingCapacity) ? m_DiffusingCapacity->operator==(*rhs.m_DiffusingCapacity) : m_DiffusingCapacity == rhs.m_DiffusingCapacity;
  equivilant &= (m_EndTidalFraction && rhs.m_EndTidalFraction) ? m_EndTidalFraction->operator==(*rhs.m_EndTidalFraction) : m_EndTidalFraction == rhs.m_EndTidalFraction;
  equivilant &= (m_EndTidalPressure && rhs.m_EndTidalPressure) ? m_EndTidalPressure->operator==(*rhs.m_EndTidalPressure) : m_EndTidalPressure == rhs.m_EndTidalPressure;
  equivilant &= (m_RelativeDiffusionCoefficient && rhs.m_RelativeDiffusionCoefficient) ? m_RelativeDiffusionCoefficient->operator==(*rhs.m_RelativeDiffusionCoefficient) : m_RelativeDiffusionCoefficient == rhs.m_RelativeDiffusionCoefficient;
  equivilant &= (m_SolubilityCoefficient && rhs.m_SolubilityCoefficient) ? m_SolubilityCoefficient->operator==(*rhs.m_SolubilityCoefficient) : m_SolubilityCoefficient == rhs.m_SolubilityCoefficient;

  equivilant &= (m_Clearance && rhs.m_Clearance) ? m_Clearance->operator==(*rhs.m_Clearance) : m_Clearance == rhs.m_Clearance;
  equivilant &= (m_PK && rhs.m_PK) ? m_PK->operator==(*rhs.m_PK) : m_PK == rhs.m_PK;
  equivilant &= (m_PD && rhs.m_PD) ? m_PD->operator==(*rhs.m_PD) : m_PD == rhs.m_PD;

  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstance::operator!=(const SESubstance& rhs) const
{
  return !(*this == rhs);
}
}
