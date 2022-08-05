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

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/schema/cdm/Compartment.hxx>
#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
SESubstanceClearance::SESubstanceClearance(Logger* logger)
  : Loggable(logger)
{
  m_hasSystemic = false;
  m_hasCellular = false;
  m_CellDeathRate = nullptr;
  m_CellBirthRate = nullptr;
  m_ChargeInBlood = (CDM::enumCharge::value)-1;
  m_FractionExcretedInFeces = nullptr;
  m_FractionExcretedInUrine = nullptr;
  m_FractionMetabolizedInGut = nullptr;
  m_FractionUnboundInPlasma = nullptr;
  m_GlomerularFilterability = nullptr;
  m_IntrinsicClearance = nullptr;
  m_RenalDynamic = (RenalDynamic)-1;
  m_RenalClearance = nullptr;
  m_RenalReabsorptionRatio = nullptr;
  m_RenalTransportMaximum = nullptr;
  m_RenalFiltrationRate = nullptr;
  m_RenalReabsorptionRate = nullptr;
  m_RenalExcretionRate = nullptr;
  m_SystemicClearance = nullptr;
}
//-----------------------------------------------------------------------------
SESubstanceClearance::~SESubstanceClearance()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::Clear()
{
  m_hasSystemic = false;
  m_hasCellular = false;
  SAFE_DELETE(m_CellDeathRate);
  SAFE_DELETE(m_CellBirthRate);
  m_ChargeInBlood = (CDM::enumCharge::value)-1;
  SAFE_DELETE(m_FractionExcretedInFeces);
  SAFE_DELETE(m_FractionExcretedInUrine);
  SAFE_DELETE(m_FractionMetabolizedInGut);
  SAFE_DELETE(m_FractionUnboundInPlasma);
  SAFE_DELETE(m_GlomerularFilterability);
  SAFE_DELETE(m_IntrinsicClearance);
  m_RenalDynamic = (RenalDynamic)-1;
  SAFE_DELETE(m_RenalClearance);
  SAFE_DELETE(m_RenalReabsorptionRatio);
  SAFE_DELETE(m_RenalTransportMaximum);
  SAFE_DELETE(m_RenalFiltrationRate);
  SAFE_DELETE(m_RenalReabsorptionRate);
  SAFE_DELETE(m_RenalExcretionRate);
  SAFE_DELETE(m_SystemicClearance);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::IsValid() const
{
  if (HasSystemic()) {
    if (!HasFractionExcretedInFeces())
      return false;
    if (!HasFractionUnboundInPlasma())
      return false;
    if (!HasIntrinsicClearance())
      return false;
    if (!HasRenalClearance())
      return false;
    if (!HasSystemicClearance())
      return false;
  }

  if (HasRenalDynamic()) {
    if (GetRenalDynamic() == RenalDynamic::Regulation) {
      if (!HasChargeInBlood())
        return false;
      if (!HasFractionUnboundInPlasma())
        return false;
      if (!HasRenalReabsorptionRatio())
        return false;
      if (!HasRenalTransportMaximum())
        return false;
    } else if (GetRenalDynamic() == RenalDynamic::Clearance) {
      if (!HasRenalClearance())
        return false;
    } else
      return false;
  }

  if (HasCellular()) {
    if (!HasCellBirthRate())
      return false;
    if (!HasCellDeathRate())
      return false;
  }
  return true;
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
bool SESubstanceClearance::Load(const CDM::SubstanceClearanceData& in)
{
  Clear();
  // Make sure dups match
  if (in.Systemic().present() && in.RenalDynamics().present()) {
    if (in.RenalDynamics()->Clearance().present() && in.Systemic().get().RenalClearance().value() != in.RenalDynamics()->Clearance().get().value()) {
      Warning("Multiple Renal Clearances specified, but not the same. RenalDynamics will override Systemic.");
    }
  }
  if (in.Systemic().present() && in.RenalDynamics().present()) {
    if (in.RenalDynamics()->Regulation().present() && in.Systemic().get().FractionUnboundInPlasma().value() != in.RenalDynamics()->Regulation().get().FractionUnboundInPlasma().value()) {
      Warning("Multiple FractionUnboundInPlasma values specified, but not the same. RenalDynamics will override Systemic.");
    }
  }

  if (in.Systemic().present()) {
    SetSystemic(true);
    GetFractionExcretedInFeces().Load(in.Systemic()->FractionExcretedInFeces());
    if (in.Systemic()->FractionExcretedInUrine().present()) {
      GetFractionExcretedInUrine().Load(in.Systemic()->FractionExcretedInUrine()->value());
    }
    if (in.Systemic()->FractionMetabolizedInGut().present()) {
      GetFractionMetabolizedInGut().Load(in.Systemic()->FractionMetabolizedInGut()->value());
    }
    GetFractionUnboundInPlasma().Load(in.Systemic()->FractionUnboundInPlasma());
    GetIntrinsicClearance().Load(in.Systemic()->IntrinsicClearance());
    GetRenalClearance().Load(in.Systemic()->RenalClearance());
    GetSystemicClearance().Load(in.Systemic()->SystemicClearance());
  }

  if (in.RenalDynamics().present()) {
    if (in.RenalDynamics()->Regulation().present()) {
      m_RenalDynamic = RenalDynamic::Regulation;
      SetChargeInBlood(in.RenalDynamics()->Regulation()->ChargeInBlood());
      GetFractionUnboundInPlasma().Load(in.RenalDynamics()->Regulation()->FractionUnboundInPlasma());
      GetRenalReabsorptionRatio().Load(in.RenalDynamics()->Regulation()->ReabsorptionRatio());
      GetRenalTransportMaximum().Load(in.RenalDynamics()->Regulation()->TransportMaximum());
    } else if (in.RenalDynamics()->Clearance().present()) {
      m_RenalDynamic = RenalDynamic::Clearance;
      GetRenalClearance().Load(in.RenalDynamics()->Clearance().get());
    }

    if (in.RenalDynamics()->FiltrationRate().present())
      GetRenalFiltrationRate().Load(in.RenalDynamics()->FiltrationRate().get());
    if (in.RenalDynamics()->GlomerularFilterability().present())
      GetGlomerularFilterability().Load(in.RenalDynamics()->GlomerularFilterability().get());
    if (in.RenalDynamics()->ReabsorptionRate().present())
      GetRenalReabsorptionRate().Load(in.RenalDynamics()->ReabsorptionRate().get());
    if (in.RenalDynamics()->ExcretionRate().present())
      GetRenalExcretionRate().Load(in.RenalDynamics()->ExcretionRate().get());
  }

  if (in.CellRegulation().present()) {
    SetCellular(true);
    GetCellBirthRate().Load(in.CellRegulation()->CellBirthRate());
    GetCellDeathRate().Load(in.CellRegulation()->CellDeathRate());
  }

  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstanceClearanceData* SESubstanceClearance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::SubstanceClearanceData* data = new CDM::SubstanceClearanceData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::Unload(CDM::SubstanceClearanceData& data) const
{
  if (HasSystemic()) {
    CDM::Systemic* sys(new CDM::Systemic());
    data.Systemic(std::unique_ptr<CDM::Systemic>(sys));

    if (HasFractionExcretedInFeces())
      sys->FractionExcretedInFeces(std::unique_ptr<CDM::ScalarFractionData>(m_FractionExcretedInFeces->Unload()));
    if (HasFractionExcretedInUrine())
      sys->FractionExcretedInUrine(std::unique_ptr<CDM::ScalarFractionData>(m_FractionExcretedInUrine->Unload()));
    if (HasFractionMetabolizedInGut())
      sys->FractionMetabolizedInGut(std::unique_ptr<CDM::ScalarFractionData>(m_FractionMetabolizedInGut->Unload()));
    if (HasFractionUnboundInPlasma())
      sys->FractionUnboundInPlasma(std::unique_ptr<CDM::ScalarFractionData>(m_FractionUnboundInPlasma->Unload()));
    if (HasRenalClearance())
      sys->RenalClearance(std::unique_ptr<CDM::ScalarVolumePerTimeMassData>(m_RenalClearance->Unload()));
    if (HasIntrinsicClearance())
      sys->IntrinsicClearance(std::unique_ptr<CDM::ScalarVolumePerTimeMassData>(m_IntrinsicClearance->Unload()));
    if (HasSystemicClearance())
      sys->SystemicClearance(std::unique_ptr<CDM::ScalarVolumePerTimeMassData>(m_SystemicClearance->Unload()));
  }

  if (HasRenalDynamic()) {
    CDM::RenalDynamics* rd(new CDM::RenalDynamics());
    data.RenalDynamics(std::unique_ptr<CDM::RenalDynamics>(rd));

    if (m_RenalDynamic == RenalDynamic::Clearance && HasRenalClearance())
      rd->Clearance(std::unique_ptr<CDM::ScalarVolumePerTimeMassData>(m_RenalClearance->Unload()));
    else if (m_RenalDynamic == RenalDynamic::Regulation) {
      CDM::Regulation* rdr(new CDM::Regulation());
      rd->Regulation(std::unique_ptr<CDM::Regulation>(rdr));

      if (HasChargeInBlood())
        rdr->ChargeInBlood(m_ChargeInBlood);
      if (HasFractionUnboundInPlasma())
        rdr->FractionUnboundInPlasma(std::unique_ptr<CDM::ScalarFractionData>(m_FractionUnboundInPlasma->Unload()));
      if (HasRenalReabsorptionRatio())
        rdr->ReabsorptionRatio(std::unique_ptr<CDM::ScalarData>(m_RenalReabsorptionRatio->Unload()));
      if (HasRenalTransportMaximum())
        rdr->TransportMaximum(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_RenalTransportMaximum->Unload()));
    }
    if (HasGlomerularFilterability())
      rd->GlomerularFilterability(std::unique_ptr<CDM::ScalarData>(m_GlomerularFilterability->Unload()));
    if (HasRenalFiltrationRate())
      rd->FiltrationRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_RenalFiltrationRate->Unload()));
    if (HasRenalReabsorptionRate())
      rd->ReabsorptionRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_RenalReabsorptionRate->Unload()));
    if (HasRenalExcretionRate())
      rd->ExcretionRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_RenalExcretionRate->Unload()));
  }

  if (HasCellular()) {
    CDM::CellRegulation* cell(new CDM::CellRegulation());
    data.CellRegulation(std::unique_ptr<CDM::CellRegulation>(cell));

    if (HasCellBirthRate())
      cell->CellBirthRate(std::unique_ptr<CDM::ScalarFrequencyData>(m_CellBirthRate->Unload()));
    if (HasCellDeathRate())
      cell->CellDeathRate(std::unique_ptr<CDM::ScalarFrequencyData>(m_CellDeathRate->Unload()));
  }
};

//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasCellBirthRate() const
{
  return (m_CellBirthRate == nullptr) ? false : m_CellBirthRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SESubstanceClearance::GetCellBirthRate()
{
  if (m_CellBirthRate == nullptr)
    m_CellBirthRate = new SEScalarFrequency();
  return *m_CellBirthRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetCellBirthRate(const FrequencyUnit& unit) const
{
  if (m_CellBirthRate == nullptr)
    return SEScalar::dNaN();
  return m_CellBirthRate->GetValue(unit);
}

//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasCellDeathRate() const
{
  return (m_CellDeathRate == nullptr) ? false : m_CellDeathRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SESubstanceClearance::GetCellDeathRate()
{
  if (m_CellDeathRate == nullptr)
    m_CellDeathRate = new SEScalarFrequency();
  return *m_CellDeathRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetCellDeathRate(const FrequencyUnit& unit) const
{
  if (m_CellDeathRate == nullptr)
    return SEScalar::dNaN();
  return m_CellDeathRate->GetValue(unit);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CDM::enumCharge::value SESubstanceClearance::GetChargeInBlood() const
{
  return m_ChargeInBlood;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::SetChargeInBlood(CDM::enumCharge::value c)
{
  m_ChargeInBlood = c;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasChargeInBlood() const
{
  return m_ChargeInBlood == ((CDM::enumCharge::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::InvalidateChargeInBlood()
{
  m_ChargeInBlood = (CDM::enumCharge::value)-1;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionExcretedInFeces() const
{
  return (m_FractionExcretedInFeces == nullptr) ? false : m_FractionExcretedInFeces->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionExcretedInFeces()
{
  if (m_FractionExcretedInFeces == nullptr)
    m_FractionExcretedInFeces = new SEScalarFraction();
  return *m_FractionExcretedInFeces;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionExcretedInFeces() const
{
  if (m_FractionExcretedInFeces == nullptr)
    return SEScalar::dNaN();
  return m_FractionExcretedInFeces->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionExcretedInUrine() const
{
  return (m_FractionExcretedInUrine == nullptr) ? false : m_FractionExcretedInUrine->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionExcretedInUrine()
{
  if (m_FractionExcretedInUrine == nullptr)
    m_FractionExcretedInUrine = new SEScalarFraction();
  return *m_FractionExcretedInUrine;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionExcretedInUrine() const
{
  if (m_FractionExcretedInUrine == nullptr)
    return SEScalar::dNaN();
  return m_FractionExcretedInUrine->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionMetabolizedInGut() const
{
  return (m_FractionMetabolizedInGut == nullptr) ? false : m_FractionMetabolizedInGut->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionMetabolizedInGut()
{
  if (m_FractionMetabolizedInGut == nullptr)
    m_FractionMetabolizedInGut = new SEScalarFraction();
  return *m_FractionMetabolizedInGut;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionMetabolizedInGut() const
{
  if (m_FractionMetabolizedInGut == nullptr)
    return SEScalar::dNaN();
  return m_FractionMetabolizedInGut->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasFractionUnboundInPlasma() const
{
  return (m_FractionUnboundInPlasma == nullptr) ? false : m_FractionUnboundInPlasma->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SESubstanceClearance::GetFractionUnboundInPlasma()
{
  if (m_FractionUnboundInPlasma == nullptr)
    m_FractionUnboundInPlasma = new SEScalarFraction();
  return *m_FractionUnboundInPlasma;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetFractionUnboundInPlasma() const
{
  if (m_FractionUnboundInPlasma == nullptr)
    return SEScalar::dNaN();
  return m_FractionUnboundInPlasma->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasIntrinsicClearance() const
{
  return (m_IntrinsicClearance == nullptr) ? false : m_IntrinsicClearance->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimeMass& SESubstanceClearance::GetIntrinsicClearance()
{
  if (m_IntrinsicClearance == nullptr)
    m_IntrinsicClearance = new SEScalarVolumePerTimeMass();
  return *m_IntrinsicClearance;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetIntrinsicClearance(const VolumePerTimeMassUnit& unit) const
{
  if (m_IntrinsicClearance == nullptr)
    return SEScalar::dNaN();
  return m_IntrinsicClearance->GetValue(unit);
}
//-----------------------------------------------------------------------------
RenalDynamic SESubstanceClearance::GetRenalDynamic() const
{
  return m_RenalDynamic;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::SetRenalDynamic(RenalDynamic d)
{
  m_RenalDynamic = d;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalDynamic() const
{
  return m_RenalDynamic == ((RenalDynamic)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SESubstanceClearance::InvalidateRenalDynamic()
{
  m_RenalDynamic = (RenalDynamic)-1;
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalClearance() const
{
  return (m_RenalClearance == nullptr) ? false : m_RenalClearance->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimeMass& SESubstanceClearance::GetRenalClearance()
{
  if (m_RenalClearance == nullptr)
    m_RenalClearance = new SEScalarVolumePerTimeMass();
  return *m_RenalClearance;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalClearance(const VolumePerTimeMassUnit& unit) const
{
  if (m_RenalClearance == nullptr)
    return SEScalar::dNaN();
  return m_RenalClearance->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalReabsorptionRatio() const
{
  return (m_RenalReabsorptionRatio == nullptr) ? false : m_RenalReabsorptionRatio->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstanceClearance::GetRenalReabsorptionRatio()
{
  if (m_RenalReabsorptionRatio == nullptr)
    m_RenalReabsorptionRatio = new SEScalar();
  return *m_RenalReabsorptionRatio;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalReabsorptionRatio() const
{
  if (m_RenalReabsorptionRatio == nullptr)
    return SEScalar::dNaN();
  return m_RenalReabsorptionRatio->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalTransportMaximum() const
{
  return (m_RenalTransportMaximum == nullptr) ? false : m_RenalTransportMaximum->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalTransportMaximum()
{
  if (m_RenalTransportMaximum == nullptr)
    m_RenalTransportMaximum = new SEScalarMassPerTime();
  return *m_RenalTransportMaximum;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalTransportMaximum(const MassPerTimeUnit& unit) const
{
  if (m_RenalTransportMaximum == nullptr)
    return SEScalar::dNaN();
  return m_RenalTransportMaximum->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalFiltrationRate() const
{
  return (m_RenalFiltrationRate == nullptr) ? false : m_RenalFiltrationRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalFiltrationRate()
{
  if (m_RenalFiltrationRate == nullptr)
    m_RenalFiltrationRate = new SEScalarMassPerTime();
  return *m_RenalFiltrationRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalFiltrationRate(const MassPerTimeUnit& unit) const
{
  if (m_RenalFiltrationRate == nullptr)
    return SEScalar::dNaN();
  return m_RenalFiltrationRate->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalReabsorptionRate() const
{
  return (m_RenalReabsorptionRate == nullptr) ? false : m_RenalReabsorptionRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalReabsorptionRate()
{
  if (m_RenalReabsorptionRate == nullptr)
    m_RenalReabsorptionRate = new SEScalarMassPerTime();
  return *m_RenalReabsorptionRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalReabsorptionRate(const MassPerTimeUnit& unit) const
{
  if (m_RenalReabsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_RenalReabsorptionRate->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasRenalExcretionRate() const
{
  return (m_RenalExcretionRate == nullptr) ? false : m_RenalExcretionRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerTime& SESubstanceClearance::GetRenalExcretionRate()
{
  if (m_RenalExcretionRate == nullptr)
    m_RenalExcretionRate = new SEScalarMassPerTime();
  return *m_RenalExcretionRate;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetRenalExcretionRate(const MassPerTimeUnit& unit) const
{
  if (m_RenalExcretionRate == nullptr)
    return SEScalar::dNaN();
  return m_RenalExcretionRate->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasGlomerularFilterability() const
{
  return (m_GlomerularFilterability == nullptr) ? false : m_GlomerularFilterability->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstanceClearance::GetGlomerularFilterability()
{
  if (m_GlomerularFilterability == nullptr)
    m_GlomerularFilterability = new SEScalar();
  return *m_GlomerularFilterability;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetGlomerularFilterability() const
{
  if (m_GlomerularFilterability == nullptr)
    return SEScalar::dNaN();
  return m_GlomerularFilterability->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceClearance::HasSystemicClearance() const
{
  return (m_SystemicClearance == nullptr) ? false : m_SystemicClearance->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTimeMass& SESubstanceClearance::GetSystemicClearance()
{
  if (m_SystemicClearance == nullptr)
    m_SystemicClearance = new SEScalarVolumePerTimeMass();
  return *m_SystemicClearance;
}
//-----------------------------------------------------------------------------
double SESubstanceClearance::GetSystemicClearance(const VolumePerTimeMassUnit& unit) const
{
  if (m_SystemicClearance == nullptr)
    return SEScalar::dNaN();
  return m_SystemicClearance->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SESubstanceClearance::operator==( const SESubstanceClearance& rhs) const
{
  bool equivilant = m_hasSystemic == rhs.m_hasSystemic;
  equivilant &= m_hasCellular == rhs.m_hasCellular;
  equivilant &= (m_CellBirthRate && rhs.m_CellBirthRate) ? m_CellBirthRate->operator==(*rhs.m_CellBirthRate) : m_CellBirthRate == rhs.m_CellBirthRate;
  equivilant &= (m_CellDeathRate && rhs.m_CellDeathRate) ? m_CellDeathRate->operator==(*rhs.m_CellDeathRate) : m_CellDeathRate == rhs.m_CellDeathRate;

  equivilant &= m_ChargeInBlood == rhs.m_ChargeInBlood;
  
  equivilant &= (m_FractionExcretedInFeces && rhs.m_FractionExcretedInFeces) ? m_FractionExcretedInFeces->operator==(*rhs.m_FractionExcretedInFeces) : m_FractionExcretedInFeces == rhs.m_FractionExcretedInFeces;
  equivilant &= (m_FractionExcretedInUrine && rhs.m_FractionExcretedInUrine) ? m_FractionExcretedInUrine->operator==(*rhs.m_FractionExcretedInUrine) : m_FractionExcretedInUrine == rhs.m_FractionExcretedInUrine;
  equivilant &= (m_FractionMetabolizedInGut && rhs.m_FractionMetabolizedInGut) ? m_FractionMetabolizedInGut->operator==(*rhs.m_FractionMetabolizedInGut) : m_FractionMetabolizedInGut == rhs.m_FractionMetabolizedInGut;
  equivilant &= (m_FractionUnboundInPlasma && rhs.m_FractionUnboundInPlasma) ? m_FractionUnboundInPlasma->operator==(*rhs.m_FractionUnboundInPlasma) : m_FractionUnboundInPlasma == rhs.m_FractionUnboundInPlasma;
  equivilant &= (m_IntrinsicClearance && rhs.m_IntrinsicClearance) ? m_IntrinsicClearance->operator==(*rhs.m_IntrinsicClearance) : m_IntrinsicClearance == rhs.m_IntrinsicClearance;
  equivilant &= m_RenalDynamic == rhs.m_RenalDynamic;
  equivilant &= (m_RenalClearance && rhs.m_RenalClearance) ? m_RenalClearance->operator==(*rhs.m_RenalClearance) : m_RenalClearance == rhs.m_RenalClearance;
  equivilant &= (m_RenalReabsorptionRatio && rhs.m_RenalReabsorptionRatio) ? m_RenalReabsorptionRatio->operator==(*rhs.m_RenalReabsorptionRatio) : m_RenalReabsorptionRatio == rhs.m_RenalReabsorptionRatio;
  equivilant &= (m_RenalTransportMaximum && rhs.m_RenalTransportMaximum) ? m_RenalTransportMaximum->operator==(*rhs.m_RenalTransportMaximum) : m_RenalTransportMaximum == rhs.m_RenalTransportMaximum;
  equivilant &= (m_RenalFiltrationRate && rhs.m_RenalFiltrationRate) ? m_RenalFiltrationRate->operator==(*rhs.m_RenalFiltrationRate) : m_RenalFiltrationRate == rhs.m_RenalFiltrationRate;
  equivilant &= (m_RenalReabsorptionRate && rhs.m_RenalReabsorptionRate) ? m_RenalReabsorptionRate->operator==(*rhs.m_RenalReabsorptionRate) : m_RenalReabsorptionRate == rhs.m_RenalReabsorptionRate;
  equivilant &= (m_RenalExcretionRate && rhs.m_RenalExcretionRate) ? m_RenalExcretionRate->operator==(*rhs.m_RenalExcretionRate) : m_RenalExcretionRate == rhs.m_RenalExcretionRate;
  equivilant &= (m_GlomerularFilterability && rhs.m_GlomerularFilterability) ? m_GlomerularFilterability->operator==(*rhs.m_GlomerularFilterability) : m_GlomerularFilterability == rhs.m_GlomerularFilterability;
  equivilant &= (m_SystemicClearance && rhs.m_SystemicClearance) ? m_SystemicClearance->operator==(*rhs.m_SystemicClearance) : m_SystemicClearance == rhs.m_SystemicClearance;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstanceClearance::operator!=( const SESubstanceClearance& rhs) const
{
  return !(*this == rhs);
}
}
