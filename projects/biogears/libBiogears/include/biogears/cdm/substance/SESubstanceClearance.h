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
#include <string>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/enums/SESubstanceEnums.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>

namespace biogears {

namespace io {
  class Substance;
}

struct BIOGEARS_API SESubstanceClearanceDefinition {
  bool HasSystemic = false;
  bool HasCellular = false;
  SEScalarFrequency CellBirthRate;
  SEScalarFrequency CellDeathRate;

  SECharge ChargeInBlood;

  SEScalarVolumePerTimeMass IntrinsicClearance;
  RenalDynamicsType RenalDynamic;
  SEScalarVolumePerTimeMass RenalClearance;
  SEScalar RenalReabsorptionRatio;
  SEScalarMassPerTime RenalTransportMaximum;
  SEScalarMassPerTime RenalFiltrationRate;
  SEScalarMassPerTime RenalReabsorptionRate;
  SEScalarMassPerTime RenalExcretionRate;
  SEScalar GlomerularFilterability;
  SEScalarVolumePerTimeMass SystemicClearance;

  bool operator==(SESubstanceClearanceDefinition const& rhs) const;
  bool operator!=(SESubstanceClearanceDefinition const& rhs) const;
};

class BIOGEARS_API SESubstanceClearance : public Loggable {
  friend io::Substance;

public:
  SESubstanceClearance(Logger* logger = nullptr);
  SESubstanceClearance(SESubstanceClearanceDefinition definition, Logger* logger);
  virtual ~SESubstanceClearance();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  void Invalidate();

public:
  virtual bool HasSystemic() const;
  virtual void SetSystemic(bool b);
  virtual bool HasCellular() const;
  virtual void SetCellular(bool b);

  virtual bool HasCellBirthRate() const;
  virtual SEScalarFrequency& GetCellBirthRate();
  virtual double GetCellBirthRate(const FrequencyUnit& unit) const;

  virtual bool HasCellDeathRate() const;
  virtual SEScalarFrequency& GetCellDeathRate();
  virtual double GetCellDeathRate(const FrequencyUnit& unit) const;

  virtual SECharge GetChargeInBlood() const;
  virtual void SetChargeInBlood(SECharge type);
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

  virtual RenalDynamicsType GetRenalDynamic() const;
  virtual void SetRenalDynamic(RenalDynamicsType d);
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

  bool operator==(const SESubstanceClearance& rhs) const;
  bool operator!=(const SESubstanceClearance& rhs) const;

protected:
  SESubstanceClearanceDefinition m_def;

  SEScalarFraction m_FractionExcretedInFeces;
  SEScalarFraction m_FractionExcretedInUrine;
  SEScalarFraction m_FractionMetabolizedInGut;
  SEScalarFraction m_FractionUnboundInPlasma;
};
}