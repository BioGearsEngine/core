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
#include <biogears/cdm/substance/SESubstanceClearance.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/schema/cdm/Compartment.hxx>
#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
SESubstanceClearance::SESubstanceClearance(Logger* logger)
  : Loggable(logger)
  , m_def()
  , m_FractionExcretedInFeces()
  , m_FractionExcretedInUrine()
  , m_FractionMetabolizedInGut()
  , m_FractionUnboundInPlasma()
{
  m_def.HasSystemic = false;
  m_def.HasCellular = false;

  m_def.ChargeInBlood = SECharge::Invalid;
  m_def.RenalDynamic = RenalDynamicsType::Invalid;
}
SESubstanceClearance::SESubstanceClearance(SESubstanceClearanceDefinition definition, Logger* logger)
  : Loggable(logger)
  , m_def(definition)
  , m_FractionExcretedInFeces()
  , m_FractionExcretedInUrine()
  , m_FractionMetabolizedInGut()
  , m_FractionUnboundInPlasma()
{
  m_def.HasSystemic = false;
  m_def.HasCellular = false;

  m_def.ChargeInBlood = SECharge::Invalid;
  m_def.RenalDynamic = RenalDynamicsType::Invalid;
}
//-----------------------------------------------------------------------------
SESubstanceClearance::~SESubstanceClearance()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::Clear()
{
  m_def.HasSystemic = false;
  m_def.HasCellular = false;

  m_def.ChargeInBlood = SECharge::Invalid;
  m_FractionExcretedInFeces.Invalidate();
  m_FractionExcretedInUrine.Invalidate();
  m_FractionMetabolizedInGut.Invalidate();
  m_FractionUnboundInPlasma.Invalidate();
  m_def.GlomerularFilterability.Invalidate();
  m_def.IntrinsicClearance.Invalidate();
  m_def.RenalDynamic = RenalDynamicsType::Invalid;
  m_def.RenalClearance.Invalidate();
  m_def.RenalReabsorptionRatio.Invalidate();
  m_def.RenalTransportMaximum.Invalidate();
  m_def.RenalFiltrationRate.Invalidate();
  m_def.RenalReabsorptionRate.Invalidate();
  m_def.RenalExcretionRate.Invalidate();
  m_def.SystemicClearance.Invalidate();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::IsValid() const
{
  if (HasSystemic()) {
    return HasFractionExcretedInFeces()
      && HasFractionUnboundInPlasma()
      && HasIntrinsicClearance()
      && HasRenalClearance()
      && HasSystemicClearance();
  }

  if (HasRenalDynamic()) {
    if (GetRenalDynamic() == RenalDynamicsType::Regulation) {
      return HasChargeInBlood()
        && HasFractionUnboundInPlasma()
        && HasRenalReabsorptionRatio()
        && HasRenalTransportMaximum();
    } else if (GetRenalDynamic() == RenalDynamicsType::Clearance) {
      return HasRenalClearance();
    } else
      return false;
  }

  if (HasCellular()) {
    return HasCellBirthRate()
      && HasCellDeathRate();
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasSystemic() const
{
  return m_def.HasSystemic;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::SetSystemic(bool b)
{
  m_def.HasSystemic = b;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasCellular() const
{
  return m_def.HasCellular;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::SetCellular(bool b)
{
  m_def.HasCellular = b;
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstanceClearance::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstanceClearance::GetScalar(const std::string& name)
{
  if (name.compare("CellDeathRate") == 0)
    return &GetCellDeathRate();
  if (name.compare("CellBirthRate") == 0)
    return &GetCellBirthRate();
  if (name.compare("FractionExcretedInFeces") == 0)
    return &GetFractionExcretedInFeces();
  if (name.compare("FractionExcretedInUrine") == 0)
    return &GetFractionExcretedInUrine();
  if (name.compare("FractionMetabolizedInGut") == 0)
    return &GetFractionMetabolizedInGut();
  if (name.compare("FractionUnboundInPlasma") == 0)
    return &GetFractionUnboundInPlasma();
  if (name.compare("IntrinsicClearance") == 0)
    return &GetIntrinsicClearance();
  if (name.compare("GlomerularFilterability") == 0)
    return &GetGlomerularFilterability();
  if (name.compare("RenalClearance") == 0)
    return &GetRenalClearance();
  if (name.compare("RenalReabsorptionRatio") == 0)
    return &GetRenalReabsorptionRatio();
  if (name.compare("RenalTransportMaximum") == 0)
    return &GetRenalTransportMaximum();
  if (name.compare("RenalFiltrationRate") == 0)
    return &GetRenalFiltrationRate();
  if (name.compare("RenalReabsorptionRate") == 0)
    return &GetRenalReabsorptionRate();
  if (name.compare("RenalExcretionRate") == 0)
    return &GetRenalExcretionRate();
  if (name.compare("SystemicClearance") == 0)
    return &GetSystemicClearance();

  return nullptr;
}

//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasCellBirthRate() const
{
  return m_def.CellBirthRate.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SESubstanceClearance::GetCellBirthRate()
{

  return m_def.CellBirthRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetCellBirthRate(const FrequencyUnit& unit) const
{
  return m_def.CellBirthRate.GetValue(unit);
}

//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasCellDeathRate() const
{
  return m_def.CellDeathRate.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SESubstanceClearance::GetCellDeathRate()
{
  m_def.HasCellular = true;
  return m_def.CellDeathRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetCellDeathRate(const FrequencyUnit& unit) const
{
  return m_def.CellDeathRate.GetValue(unit);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
SECharge SESubstanceClearance::GetChargeInBlood() const
{
  return m_def.ChargeInBlood;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::SetChargeInBlood(SECharge c)
{
  m_def.ChargeInBlood = c;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasChargeInBlood() const
{
  return m_def.ChargeInBlood == SECharge::Invalid ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::InvalidateChargeInBlood()
{
  m_def.ChargeInBlood = (SECharge)-1;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionExcretedInFeces() const
{
  return m_FractionExcretedInFeces.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionExcretedInFeces()
{
  return m_FractionExcretedInFeces;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionExcretedInFeces() const
{
  return m_FractionExcretedInFeces.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionExcretedInUrine() const
{
  return m_FractionExcretedInUrine.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionExcretedInUrine()
{
  return m_FractionExcretedInUrine;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionExcretedInUrine() const
{

  return m_FractionExcretedInUrine.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionMetabolizedInGut() const
{
  return m_FractionMetabolizedInGut.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionMetabolizedInGut()
{
  return m_FractionMetabolizedInGut;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionMetabolizedInGut() const
{

  return m_FractionMetabolizedInGut.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionUnboundInPlasma() const
{
  return m_FractionUnboundInPlasma.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionUnboundInPlasma()
{

  return m_FractionUnboundInPlasma;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionUnboundInPlasma() const
{
  return m_FractionUnboundInPlasma.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasIntrinsicClearance() const
{
  return m_def.IntrinsicClearance.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimeMass& SESubstanceClearance::GetIntrinsicClearance()
{
  return m_def.IntrinsicClearance;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetIntrinsicClearance(const VolumePerTimeMassUnit& unit) const
{

  return m_def.IntrinsicClearance.GetValue(unit);
}
//-----------------------------------------------------------------------------
RenalDynamicsType SESubstanceClearance::GetRenalDynamic() const
{
  return m_def.RenalDynamic;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::SetRenalDynamic(RenalDynamicsType d)
{
  m_def.RenalDynamic = d;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalDynamic() const
{
  return m_def.RenalDynamic == ((RenalDynamicsType)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::InvalidateRenalDynamic()
{
  m_def.RenalDynamic = (RenalDynamicsType)-1;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalClearance() const
{
  return m_def.RenalClearance.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimeMass& SESubstanceClearance::GetRenalClearance()
{
  return m_def.RenalClearance;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalClearance(const VolumePerTimeMassUnit& unit) const
{
  return m_def.RenalClearance.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalReabsorptionRatio() const
{
  return m_def.RenalReabsorptionRatio.IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstanceClearance::GetRenalReabsorptionRatio()
{
  return m_def.RenalReabsorptionRatio;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalReabsorptionRatio() const
{

  return m_def.RenalReabsorptionRatio.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalTransportMaximum() const
{
  return m_def.RenalTransportMaximum.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalTransportMaximum()
{
  return m_def.RenalTransportMaximum;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalTransportMaximum(const MassPerTimeUnit& unit) const
{

  return m_def.RenalTransportMaximum.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalFiltrationRate() const
{
  return m_def.RenalFiltrationRate.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalFiltrationRate()
{
  return m_def.RenalFiltrationRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalFiltrationRate(const MassPerTimeUnit& unit) const
{

  return m_def.RenalFiltrationRate.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalReabsorptionRate() const
{
  return m_def.RenalReabsorptionRate.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalReabsorptionRate()
{
  return m_def.RenalReabsorptionRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalReabsorptionRate(const MassPerTimeUnit& unit) const
{

  return m_def.RenalReabsorptionRate.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalExcretionRate() const
{
  return m_def.RenalExcretionRate.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalExcretionRate()
{
  return m_def.RenalExcretionRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalExcretionRate(const MassPerTimeUnit& unit) const
{

  return m_def.RenalExcretionRate.GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasGlomerularFilterability() const
{
  return m_def.GlomerularFilterability.IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstanceClearance::GetGlomerularFilterability()
{
  return m_def.GlomerularFilterability;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetGlomerularFilterability() const
{

  return m_def.GlomerularFilterability.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasSystemicClearance() const
{
  return m_def.SystemicClearance.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimeMass& SESubstanceClearance::GetSystemicClearance()
{

  return m_def.SystemicClearance;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetSystemicClearance(const VolumePerTimeMassUnit& unit) const
{

  return m_def.SystemicClearance.GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SESubstanceClearance::operator==(const SESubstanceClearance& rhs) const
{
  bool equivilant = m_def.HasSystemic == rhs.m_def.HasSystemic;
  equivilant &= m_def.HasCellular == rhs.m_def.HasCellular;
  equivilant &= m_def.CellBirthRate == rhs.m_def.CellBirthRate;
  equivilant &= m_def.CellDeathRate == rhs.m_def.CellDeathRate;

  equivilant &= m_def.ChargeInBlood == rhs.m_def.ChargeInBlood;

  equivilant &= m_FractionExcretedInFeces == rhs.m_FractionExcretedInFeces;
  equivilant &= m_FractionExcretedInUrine == rhs.m_FractionExcretedInUrine;
  equivilant &= m_FractionMetabolizedInGut == rhs.m_FractionMetabolizedInGut;
  equivilant &= m_FractionUnboundInPlasma == rhs.m_FractionUnboundInPlasma;
  equivilant &= m_def.IntrinsicClearance == rhs.m_def.IntrinsicClearance;
  equivilant &= m_def.RenalDynamic == rhs.m_def.RenalDynamic;
  equivilant &= m_def.RenalClearance == rhs.m_def.RenalClearance;
  equivilant &= m_def.RenalReabsorptionRatio == rhs.m_def.RenalReabsorptionRatio;
  equivilant &= m_def.RenalTransportMaximum == rhs.m_def.RenalTransportMaximum;
  equivilant &= m_def.RenalFiltrationRate == rhs.m_def.RenalFiltrationRate;
  equivilant &= m_def.RenalReabsorptionRate == rhs.m_def.RenalReabsorptionRate;
  equivilant &= m_def.RenalExcretionRate == rhs.m_def.RenalExcretionRate;
  equivilant &= m_def.GlomerularFilterability == rhs.m_def.GlomerularFilterability;
  equivilant &= m_def.SystemicClearance == rhs.m_def.SystemicClearance;
  return equivilant;
}
bool SESubstanceClearance::operator!=(const SESubstanceClearance& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool SESubstanceClearanceDefinition::operator==(SESubstanceClearanceDefinition const& rhs) const
{
  if (this == &rhs)
    return true;

  return (HasCellular == rhs.HasCellular
          && HasCellular == rhs.HasCellular
          && CellBirthRate == rhs.CellBirthRate
          && CellDeathRate == rhs.CellDeathRate

          && ChargeInBlood == rhs.ChargeInBlood

          && IntrinsicClearance == rhs.IntrinsicClearance
          && RenalDynamic == rhs.RenalDynamic
          && RenalClearance == rhs.RenalClearance
          && RenalReabsorptionRatio == rhs.RenalReabsorptionRatio
          && RenalTransportMaximum == rhs.RenalTransportMaximum
          && RenalFiltrationRate == rhs.RenalFiltrationRate
          && RenalReabsorptionRate == rhs.RenalReabsorptionRate
          && RenalExcretionRate == rhs.RenalExcretionRate
          && GlomerularFilterability == rhs.GlomerularFilterability
          && SystemicClearance == rhs.SystemicClearance);
}
bool SESubstanceClearanceDefinition::operator!=(SESubstanceClearanceDefinition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}
