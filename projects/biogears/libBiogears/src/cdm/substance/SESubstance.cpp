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

#include "io/cdm/Substance.h"

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

SESubstanceDefinition::SESubstanceDefinition(SESubstanceDefinition const& obj)
  : ClearanceDefinition(obj.ClearanceDefinition)
  , Aerosolization(obj.Aerosolization)
  , Pharmacokinetics(obj.Pharmacokinetics)
  , Pharmacodynamics(obj.Pharmacodynamics)
  , Name(obj.Name)
  , Classification(obj.Classification)
  , State(obj.State)
  , Density(obj.Density)
  , MolarMass(obj.MolarMass)
  , MaximumDiffusionFlux(obj.MaximumDiffusionFlux)
  , MichaelisCoefficient(obj.MichaelisCoefficient)
  , MembraneResistance(obj.MembraneResistance)
  , AreaUnderCurve(obj.AreaUnderCurve)
  , AlveolarTransfer(obj.AlveolarTransfer)
  , DiffusingCapacity(obj.DiffusingCapacity)
  , RelativeDiffusionCoefficient(obj.RelativeDiffusionCoefficient)
  , SolubilityCoefficient(obj.SolubilityCoefficient) {};
//-----------------------------------------------------------------------------
SESubstanceDefinition::SESubstanceDefinition(SESubstanceDefinition&& obj)
  : Name(std::move(obj.Name))
  , Classification(std::move(obj.Classification))
  , State(std::move(obj.State))
  , Density(std::move(obj.Density))
  , MolarMass(std::move(obj.MolarMass))
  , MaximumDiffusionFlux(std::move(obj.MaximumDiffusionFlux))
  , MichaelisCoefficient(std::move(obj.MichaelisCoefficient))
  , MembraneResistance(std::move(obj.MembraneResistance))
  , AreaUnderCurve(std::move(obj.AreaUnderCurve))
  , AlveolarTransfer(std::move(obj.AlveolarTransfer))
  , DiffusingCapacity(std::move(obj.DiffusingCapacity))
  , RelativeDiffusionCoefficient(std::move(obj.RelativeDiffusionCoefficient))
  , SolubilityCoefficient(std::move(obj.SolubilityCoefficient))
  , ClearanceDefinition(std::move(obj.ClearanceDefinition))
  , Aerosolization(std::move(obj.Aerosolization))
  , Pharmacokinetics(std::move(obj.Pharmacokinetics))
  , Pharmacodynamics(std::move(obj.Pharmacodynamics))
{
}
//-----------------------------------------------------------------------------
SESubstanceDefinition ::~SESubstanceDefinition()
{
}
//-----------------------------------------------------------------------------
SESubstanceDefinition::SESubstanceDefinition(Logger* logger)
  : ClearanceDefinition()
  , Aerosolization(logger)
  , Pharmacokinetics(logger)
  , Pharmacodynamics(logger)
  , Name()
  , Classification()
  , State()
  , Density()
  , MolarMass()
  , MaximumDiffusionFlux()
  , MichaelisCoefficient()
  , MembraneResistance()
  , AreaUnderCurve()
  , AlveolarTransfer()
  , DiffusingCapacity()
  , RelativeDiffusionCoefficient()
  , SolubilityCoefficient() {};
//-----------------------------------------------------------------------------
SESubstanceDefinition::SESubstanceDefinition(SESubstanceClearanceDefinition definition, Logger* logger)
  : ClearanceDefinition(definition)
  , Aerosolization(logger)
  , Pharmacokinetics(logger)
  , Pharmacodynamics(logger)
  , Name()
  , Classification()
  , State()
  , Density()
  , MolarMass()
  , MaximumDiffusionFlux()
  , MichaelisCoefficient()
  , MembraneResistance()
  , AreaUnderCurve()
  , AlveolarTransfer()
  , DiffusingCapacity()
  , RelativeDiffusionCoefficient()
  , SolubilityCoefficient() {};
//-------------------------------------------------------------------------------
#pragma optimize("", off)
SESubstanceDefinition& SESubstanceDefinition::operator=(SESubstanceDefinition const& rhs)
{
  if (this != &rhs) {


    Name = rhs.Name;
    Classification = rhs.Classification;
    State = rhs.State;
    Density = rhs.Density;
    MolarMass = rhs.MolarMass;
    MaximumDiffusionFlux = rhs.MaximumDiffusionFlux;
    MichaelisCoefficient = rhs.MichaelisCoefficient;
    MembraneResistance = rhs.MembraneResistance;
    AreaUnderCurve = rhs.AreaUnderCurve;
    AlveolarTransfer = rhs.AlveolarTransfer;
    DiffusingCapacity = rhs.DiffusingCapacity;
    RelativeDiffusionCoefficient = rhs.RelativeDiffusionCoefficient;
    SolubilityCoefficient = rhs.SolubilityCoefficient;
    ClearanceDefinition = rhs.ClearanceDefinition;
    Aerosolization = rhs.Aerosolization;
    Pharmacokinetics = rhs.Pharmacokinetics;
    Pharmacodynamics = rhs.Pharmacodynamics;
  }
  return *this;
}
SESubstanceDefinition& SESubstanceDefinition::operator=(SESubstanceDefinition&& rhs)
{
  if (this != &rhs) {

    SESubstanceDefinition temp(std::move(rhs));

    std::swap(Name, temp.Name);
    std::swap(Classification, temp.Classification);
    std::swap(State, temp.State);
    std::swap(Density, temp.Density);
    std::swap(MolarMass, temp.MolarMass);
    std::swap(MaximumDiffusionFlux, temp.MaximumDiffusionFlux);
    std::swap(MichaelisCoefficient, temp.MichaelisCoefficient);
    std::swap(MembraneResistance, temp.MembraneResistance);
    std::swap(AreaUnderCurve, temp.AreaUnderCurve);
    std::swap(AlveolarTransfer, temp.AlveolarTransfer);
    std::swap(DiffusingCapacity, temp.DiffusingCapacity);
    std::swap(RelativeDiffusionCoefficient, temp.RelativeDiffusionCoefficient);
    std::swap(SolubilityCoefficient, temp.SolubilityCoefficient);
    std::swap(ClearanceDefinition, temp.ClearanceDefinition);
    std::swap(Aerosolization, temp.Aerosolization);
    std::swap(Pharmacokinetics, temp.Pharmacokinetics);
    std::swap(Pharmacodynamics, temp.Pharmacodynamics);
  }
  return *this;
}
//-------------------------------------------------------------------------------
#pragma optimize("", on)
bool SESubstanceDefinition::operator==(SESubstanceDefinition const& rhs) const
{
  if (this == &rhs)
    return true;

  return Name == rhs.Name
          && Classification == rhs.Classification
          && State == rhs.State
#if defined(BIOGEARS_RIGOROUS_EQUIVILANCE_CHECKS) && !defined(BIOGEARS_LINIENT_SESUBSTANCE_DEFINITION_CHECKS)
          && Density == rhs.Density
          && MolarMass == rhs.MolarMass
          && MaximumDiffusionFlux == rhs.MaximumDiffusionFlux
          && MichaelisCoefficient == rhs.MichaelisCoefficient
          && MembraneResistance == rhs.MembraneResistance
          && AreaUnderCurve == rhs.AreaUnderCurve
          && AlveolarTransfer == rhs.AlveolarTransfer
          && DiffusingCapacity == rhs.DiffusingCapacity
          && RelativeDiffusionCoefficient == rhs.RelativeDiffusionCoefficient
          && SolubilityCoefficient == rhs.SolubilityCoefficient
          && ClearanceDefinition == rhs.ClearanceDefinition
          && Aerosolization == rhs.Aerosolization
          && Pharmacokinetics == rhs.Pharmacokinetics
          && Pharmacodynamics == rhs.Pharmacodynamics)
#endif
    ;
}
bool SESubstanceDefinition::operator!=(SESubstanceDefinition const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
SESubstance::SESubstance(Logger* logger)
  : Loggable(logger)
  , m_def(logger)
  , m_Clearance(logger)
  , m_BloodConcentration()
  , m_EffectSiteConcentration()
  , m_MassInBody()
  , m_MassInBlood()
  , m_MassInTissue()
  , m_PlasmaConcentration()
  , m_SystemicMassCleared()
  , m_TissueConcentration()
  , m_EndTidalFraction()
  , m_EndTidalPressure()
{
}
//-----------------------------------------------------------------------------
SESubstance::SESubstance(SESubstanceDefinition definition)
  : Loggable(definition.Aerosolization.GetLogger())
  , m_def(definition)
  , m_Clearance(definition.ClearanceDefinition, definition.Aerosolization.GetLogger())
  , m_BloodConcentration()
  , m_EffectSiteConcentration()
  , m_MassInBody()
  , m_MassInBlood()
  , m_MassInTissue()
  , m_PlasmaConcentration()
  , m_SystemicMassCleared()
  , m_TissueConcentration()
  , m_EndTidalFraction()
  , m_EndTidalPressure()
{
  m_def.Name = "";
  m_def.State = SESubstanceState::Invalid;
  m_def.Classification = SESubstanceClass::Invalid;
}
//-----------------------------------------------------------------------------
SESubstance::~SESubstance()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstance::Clear()
{
  m_def.Name = "";
  m_def.State = SESubstanceState::Invalid;
  m_def.Classification = SESubstanceClass::Invalid;
  m_def.Density.Invalidate();
  m_def.MolarMass.Invalidate();

  m_def.MaximumDiffusionFlux.Invalidate();
  m_def.MichaelisCoefficient.Invalidate();
  m_def.MembraneResistance.Invalidate();
  m_def.AreaUnderCurve.Invalidate();
  m_BloodConcentration.Invalidate();
  m_EffectSiteConcentration.Invalidate();
  m_MassInBody.Invalidate();
  m_MassInBlood.Invalidate();
  m_MassInTissue.Invalidate();
  m_PlasmaConcentration.Invalidate();
  m_SystemicMassCleared.Invalidate();
  m_TissueConcentration.Invalidate();

  m_def.AlveolarTransfer.Invalidate();
  m_def.DiffusingCapacity.Invalidate();
  m_EndTidalFraction.Invalidate();
  m_EndTidalPressure.Invalidate();
  m_def.SolubilityCoefficient.Invalidate();
  m_def.RelativeDiffusionCoefficient.Invalidate();

  m_def.Aerosolization.Clear();
  m_Clearance.Clear();
  m_def.Pharmacokinetics.Clear();
  m_def.Pharmacodynamics.Clear();
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
std::string SESubstance::GetName() const
{
  return m_def.Name;
}
//-----------------------------------------------------------------------------
const char* SESubstance::GetName_cStr() const
{
  return m_def.Name.c_str();
}
//-----------------------------------------------------------------------------
void SESubstance::SetName(const char* name)
{
  m_def.Name = name;
}
//-----------------------------------------------------------------------------
void SESubstance::SetName(const std::string& name)
{
  m_def.Name = name;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasName() const
{
  return m_def.Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstance::InvalidateName()
{
  m_def.Name = "";
}
//-----------------------------------------------------------------------------
SESubstanceState SESubstance::GetState() const
{
  return m_def.State;
}
//-----------------------------------------------------------------------------
void SESubstance::SetState(SESubstanceState state)
{
  m_def.State = state;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasState() const
{
  return m_def.State == SESubstanceState::Invalid;
}
//-----------------------------------------------------------------------------
void SESubstance::InvalidateState()
{
  m_def.State = SESubstanceState::Invalid;
}
//-----------------------------------------------------------------------------
SESubstanceClass SESubstance::GetClassification() const
{
  return m_def.Classification;
}
//-----------------------------------------------------------------------------
void SESubstance::SetClassification(SESubstanceClass subClass)
{
  m_def.Classification = subClass;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasClassification() const
{
  return m_def.Classification != SESubstanceClass::Invalid;
}
//-----------------------------------------------------------------------------
void SESubstance::InvalidateClassification()
{
  m_def.Classification = SESubstanceClass::Invalid;
}
//-----------------------------------------------------------------------------
bool SESubstance::HasDensity() const
{
  return m_def.Density.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetDensity()
{
  return m_def.Density;
}
//-----------------------------------------------------------------------------
double SESubstance::GetDensity(const MassPerVolumeUnit& unit) const
{
  return m_def.Density.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMolarMass() const
{
  return m_def.MolarMass.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerAmount& SESubstance::GetMolarMass()
{
  return m_def.MolarMass;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMolarMass(const MassPerAmountUnit& unit) const
{

  return m_def.MolarMass.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMaximumDiffusionFlux() const
{
  return m_def.MaximumDiffusionFlux.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerAreaTime& SESubstance::GetMaximumDiffusionFlux()
{

  return m_def.MaximumDiffusionFlux;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMaximumDiffusionFlux(const MassPerAreaTimeUnit& unit) const
{

  return m_def.MaximumDiffusionFlux.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMichaelisCoefficient() const
{
  return m_def.MichaelisCoefficient.IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstance::GetMichaelisCoefficient()
{

  return m_def.MichaelisCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMichaelisCoefficient() const
{

  return m_def.MichaelisCoefficient.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMembraneResistance() const
{
  return m_def.MembraneResistance.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarElectricResistance& SESubstance::GetMembraneResistance()
{

  return m_def.MembraneResistance;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMembraneResistance(const ElectricResistanceUnit& unit) const
{

  return m_def.MembraneResistance.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasAerosolization() const
{
  return (m_def.Aerosolization.IsValid());
}
//-----------------------------------------------------------------------------
SESubstanceAerosolization& SESubstance::GetAerosolization()
{
  return m_def.Aerosolization;
}
//-----------------------------------------------------------------------------
const SESubstanceAerosolization* SESubstance::GetAerosolization() const
{
  return &m_def.Aerosolization;
}
//-----------------------------------------------------------------------------
void SESubstance::RemoveAerosolization()
{
  m_def.Aerosolization.Clear();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasAreaUnderCurve() const
{
  return m_def.AreaUnderCurve.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTimeMassPerVolume& SESubstance::GetAreaUnderCurve()
{
  return m_def.AreaUnderCurve;
}
//-----------------------------------------------------------------------------
double SESubstance::GetAreaUnderCurve(const TimeMassPerVolumeUnit& unit) const
{
  return m_def.AreaUnderCurve.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasBloodConcentration() const
{
  return m_BloodConcentration.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetBloodConcentration()
{
  return m_BloodConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetBloodConcentration(const MassPerVolumeUnit& unit) const
{
  return m_BloodConcentration.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasEffectSiteConcentration() const
{
  return m_EffectSiteConcentration.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetEffectSiteConcentration()
{

  return m_EffectSiteConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetEffectSiteConcentration(const MassPerVolumeUnit& unit) const
{

  return m_EffectSiteConcentration.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMassInBody() const
{
  return m_MassInBody.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetMassInBody()
{
  return m_MassInBody;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMassInBody(const MassUnit& unit) const
{
  return m_MassInBody.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMassInBlood() const
{
  return m_MassInBlood.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetMassInBlood()
{
  return m_MassInBlood;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMassInBlood(const MassUnit& unit) const
{
  return m_MassInBlood.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasMassInTissue() const
{
  return m_MassInTissue.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetMassInTissue()
{
  return m_MassInTissue;
}
//-----------------------------------------------------------------------------
double SESubstance::GetMassInTissue(const MassUnit& unit) const
{
  return m_MassInTissue.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasPlasmaConcentration() const
{
  return m_PlasmaConcentration.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetPlasmaConcentration()
{
  return m_PlasmaConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetPlasmaConcentration(const MassPerVolumeUnit& unit) const
{
  return m_PlasmaConcentration.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasSystemicMassCleared() const
{
  return m_SystemicMassCleared.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SESubstance::GetSystemicMassCleared()
{
  return m_SystemicMassCleared;
}
//-----------------------------------------------------------------------------
double SESubstance::GetSystemicMassCleared(const MassUnit& unit) const
{
  return m_SystemicMassCleared.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasTissueConcentration() const
{
  return m_TissueConcentration.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstance::GetTissueConcentration()
{
  return m_TissueConcentration;
}
//-----------------------------------------------------------------------------
double SESubstance::GetTissueConcentration(const MassPerVolumeUnit& unit) const
{
  return m_TissueConcentration.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasAlveolarTransfer() const
{
  return m_def.AlveolarTransfer.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTime& SESubstance::GetAlveolarTransfer()
{
  return m_def.AlveolarTransfer;
}
//-----------------------------------------------------------------------------
double SESubstance::GetAlveolarTransfer(const VolumePerTimeUnit& unit) const
{
  return m_def.AlveolarTransfer.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasDiffusingCapacity() const
{
  return m_def.DiffusingCapacity.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimePressure& SESubstance::GetDiffusingCapacity()
{
  return m_def.DiffusingCapacity;
}
//-----------------------------------------------------------------------------
double SESubstance::GetDiffusingCapacity(const VolumePerTimePressureUnit& unit) const
{
  return m_def.DiffusingCapacity.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasEndTidalFraction() const
{
  return m_EndTidalFraction.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstance::GetEndTidalFraction()
{
  return m_EndTidalFraction;
}
//-----------------------------------------------------------------------------
double SESubstance::GetEndTidalFraction() const
{
  return m_EndTidalFraction.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasEndTidalPressure() const
{
  return m_EndTidalPressure.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SESubstance::GetEndTidalPressure()
{
  return m_EndTidalPressure;
}
//-----------------------------------------------------------------------------
double SESubstance::GetEndTidalPressure(const PressureUnit& unit) const
{
  return m_EndTidalPressure.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasRelativeDiffusionCoefficient() const
{
  return m_def.RelativeDiffusionCoefficient.IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstance::GetRelativeDiffusionCoefficient()
{
  return m_def.RelativeDiffusionCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstance::GetRelativeDiffusionCoefficient() const
{
  return m_def.RelativeDiffusionCoefficient.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasSolubilityCoefficient() const
{
  return m_def.SolubilityCoefficient.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarInversePressure& SESubstance::GetSolubilityCoefficient()
{
  return m_def.SolubilityCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstance::GetSolubilityCoefficient(const InversePressureUnit& unit) const
{
  return m_def.SolubilityCoefficient.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstance::HasClearance() const
{
  return m_Clearance.IsValid();
}
//-----------------------------------------------------------------------------
SESubstanceClearance& SESubstance::GetClearance()
{
  return m_Clearance;
}
//-----------------------------------------------------------------------------
const SESubstanceClearance* SESubstance::GetClearance() const
{
  return &m_Clearance;
}
//-----------------------------------------------------------------------------
void SESubstance::RemoveClearance()
{
  m_Clearance.Clear();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasPK() const
{
  return (m_def.Pharmacokinetics.IsValid());
}
//-----------------------------------------------------------------------------
SESubstancePharmacokinetics& SESubstance::GetPK()
{
  return m_def.Pharmacokinetics;
}
//-----------------------------------------------------------------------------
const SESubstancePharmacokinetics* SESubstance::GetPK() const
{
  return &m_def.Pharmacokinetics;
}
//-----------------------------------------------------------------------------
void SESubstance::RemovePK()
{
  m_def.Pharmacokinetics.Clear();
}
//-----------------------------------------------------------------------------
bool SESubstance::HasPD() const
{
  return (m_def.Pharmacodynamics.IsValid());
}
//-----------------------------------------------------------------------------
SESubstancePharmacodynamics& SESubstance::GetPD()
{
  return m_def.Pharmacodynamics;
}
//-----------------------------------------------------------------------------
const SESubstancePharmacodynamics* SESubstance::GetPD() const
{
  return &m_def.Pharmacodynamics;
}
//-----------------------------------------------------------------------------
void SESubstance::RemovePD()
{
  m_def.Pharmacodynamics.Clear();
}
//-------------------------------------------------------------------------------
bool SESubstance::operator==(const SESubstance& rhs) const
{

  if (this == &rhs)
    return true;

  bool equivilant = m_def == rhs.m_def;
#if defined(BIOGEARS_RIGOROUS_EQUIVILANCE_CHECKS) && !defined(BIOGEARS_LINIENT_SESUBSTANCE_CHECKS)
  equivilant &= m_MassInBody == rhs.m_MassInBody;
  equivilant &= m_MassInBlood == rhs.m_MassInBlood;
  equivilant &= m_MassInTissue == rhs.m_MassInTissue;
  equivilant &= m_EndTidalFraction == rhs.m_EndTidalFraction;
  equivilant &= m_EndTidalPressure == rhs.m_EndTidalPressure;
  equivilant &= m_Clearance == rhs.m_Clearance;
#endif
  return equivilant;
}
bool SESubstance::operator!=(const SESubstance& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SESubstance::operator==(SESubstanceDefinition const& rhs) const
{
  return this->m_def == rhs;
}
bool SESubstance::operator!=(SESubstanceDefinition const& rhs) const
{
  return !(*this == rhs);
}
//------------------------------------------------------------------------------
SESubstanceDefinition SESubstance::GetDefinition() const
{
  // TODO: Refactor PTRs in all member components to simplify true Copies
  return m_def;
}
}
