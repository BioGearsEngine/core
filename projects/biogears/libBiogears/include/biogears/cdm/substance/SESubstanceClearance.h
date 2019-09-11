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

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/Substance.hxx>
#include "biogears/cdm/properties/SEScalarFrequency.h"

CDM_BIND_DECL(SubstanceClearanceData)

namespace biogears {
class SEScalar;
class SEScalarVolumePerTimeMass;
class VolumePerTimeMassUnit;
class MassPerTimeUnit;
class SEScalarMassPerTime;
class SESubstanceClearanceAnatomyEffect;
class SEScalarVolumePerTimeMass;
class SEScalarFraction;


enum class RenalDynamic { Clearance,
  Regulation };
class BIOGEARS_API SESubstanceClearance : public Loggable {
public:
  SESubstanceClearance(Logger* logger);
  virtual ~SESubstanceClearance();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::SubstanceClearanceData& in);
  virtual CDM::SubstanceClearanceData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceClearanceData& data) const;

public:
  virtual bool HasSystemic() const { return m_hasSystemic; }
  virtual void SetSystemic(bool b) { m_hasSystemic = b; }

  virtual bool HasCellular() const { return m_hasCellular; }
  virtual void SetCellular(bool b) { m_hasCellular = b; }
  
  virtual bool HasCellBirthRate() const;
  virtual SEScalarFrequency& GetCellBirthRate();
  virtual double GetCellBirthRate(const FrequencyUnit& unit) const;

  virtual bool HasCellDeathRate() const;
  virtual SEScalarFrequency& GetCellDeathRate();
  virtual double GetCellDeathRate(const FrequencyUnit& unit) const;

  virtual CDM::enumCharge::value GetChargeInBlood() const;
  virtual void SetChargeInBlood(CDM::enumCharge::value type);
  virtual bool HasChargeInBlood() const;
  virtual void InvalidateChargeInBlood();

  virtual bool HasFractionExcretedInFeces() const;
  virtual SEScalarFraction& GetFractionExcretedInFeces();
  virtual double GetFractionExcretedInFeces() const;

  virtual bool HasFractionExcretedInUrine() const;
  virtual SEScalarFraction& GetFractionExcretedInUrine();
  virtual double GetFractionExcretedInUrine() const;

  virtual bool HasFractionMetabolizedInGut() const;
  virtual SEScalarFraction& GetFractionMetabolizedInGut();
  virtual double GetFractionMetabolizedInGut() const;

  virtual bool HasFractionUnboundInPlasma() const;
  virtual SEScalarFraction& GetFractionUnboundInPlasma();
  virtual double GetFractionUnboundInPlasma() const;

  virtual bool HasGlomerularFilterability() const;
  virtual SEScalar& GetGlomerularFilterability();
  virtual double GetGlomerularFilterability() const;

  virtual bool HasIntrinsicClearance() const;
  virtual SEScalarVolumePerTimeMass& GetIntrinsicClearance();
  virtual double GetIntrinsicClearance(const VolumePerTimeMassUnit& unit) const;

  virtual RenalDynamic GetRenalDynamic() const;
  virtual void SetRenalDynamic(RenalDynamic d);
  virtual bool HasRenalDynamic() const;
  virtual void InvalidateRenalDynamic();

  virtual bool HasRenalClearance() const;
  virtual SEScalarVolumePerTimeMass& GetRenalClearance();
  virtual double GetRenalClearance(const VolumePerTimeMassUnit& unit) const;

  virtual bool HasRenalReabsorptionRatio() const;
  virtual SEScalar& GetRenalReabsorptionRatio();
  virtual double GetRenalReabsorptionRatio() const;

  virtual bool HasRenalTransportMaximum() const;
  virtual SEScalarMassPerTime& GetRenalTransportMaximum();
  virtual double GetRenalTransportMaximum(const MassPerTimeUnit& unit) const;

  virtual bool HasRenalFiltrationRate() const;
  virtual SEScalarMassPerTime& GetRenalFiltrationRate();
  virtual double GetRenalFiltrationRate(const MassPerTimeUnit& unit) const;

  virtual bool HasRenalReabsorptionRate() const;
  virtual SEScalarMassPerTime& GetRenalReabsorptionRate();
  virtual double GetRenalReabsorptionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasRenalExcretionRate() const;
  virtual SEScalarMassPerTime& GetRenalExcretionRate();
  virtual double GetRenalExcretionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasSystemicClearance() const;
  virtual SEScalarVolumePerTimeMass& GetSystemicClearance();
  virtual double GetSystemicClearance(const VolumePerTimeMassUnit& unit) const;

protected:
  bool m_hasSystemic;
  bool m_hasCellular;
  SEScalarFrequency* m_CellBirthRate;
  SEScalarFrequency* m_CellDeathRate;

  CDM::enumCharge::value m_ChargeInBlood;
  SEScalarFraction* m_FractionExcretedInFeces;
  SEScalarFraction* m_FractionExcretedInUrine;
  SEScalarFraction* m_FractionMetabolizedInGut;
  SEScalarFraction* m_FractionUnboundInPlasma;
  SEScalarVolumePerTimeMass* m_IntrinsicClearance;
  RenalDynamic m_RenalDynamic;
  SEScalarVolumePerTimeMass* m_RenalClearance;
  SEScalar* m_RenalReabsorptionRatio;
  SEScalarMassPerTime* m_RenalTransportMaximum;
  SEScalarMassPerTime* m_RenalFiltrationRate;
  SEScalarMassPerTime* m_RenalReabsorptionRate;
  SEScalarMassPerTime* m_RenalExcretionRate;
  SEScalar* m_GlomerularFilterability;
  SEScalarVolumePerTimeMass* m_SystemicClearance;
};
}