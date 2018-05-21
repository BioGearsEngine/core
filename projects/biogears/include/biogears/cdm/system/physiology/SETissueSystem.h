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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/TissueSystemData.hxx>

class BIOGEARS_API SETissueSystem : public SESystem
{
public:

  SETissueSystem(Logger* logger);
  virtual ~SETissueSystem();

  virtual void Clear();// Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::TissueSystemData& in);
  virtual CDM::TissueSystemData* Unload() const;
protected:
  virtual void Unload(CDM::TissueSystemData& data) const;

public:

  virtual bool HasCarbonDioxideProductionRate() const;
  virtual SEScalarVolumePerTime& GetCarbonDioxideProductionRate();
  virtual double GetCarbonDioxideProductionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasDehydrationFraction() const;
  virtual SEScalar0To1& GetDehydrationFraction();
  virtual double GetDehydrationFraction() const;

  virtual bool HasExtracellularFluidVolume() const;
  virtual SEScalarVolume& GetExtracellularFluidVolume();
  virtual double GetExtracellularFluidVolume(const VolumeUnit& unit) const;

  virtual bool HasExtravascularFluidVolume() const;
  virtual SEScalarVolume& GetExtravascularFluidVolume();
  virtual double GetExtravascularFluidVolume(const VolumeUnit& unit) const;

  virtual bool HasIntracellularFluidVolume() const;
  virtual SEScalarVolume& GetIntracellularFluidVolume();
  virtual double GetIntracellularFluidVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalBodyFluidVolume() const;
  virtual SEScalarVolume& GetTotalBodyFluidVolume();
  virtual double GetTotalBodyFluidVolume(const VolumeUnit& unit) const;

  virtual bool HasIntracellularFluidPH() const;
  virtual SEScalar& GetIntracellularFluidPH();
  virtual double GetIntracellularFluidPH() const;

  virtual bool HasOxygenConsumptionRate() const;
  virtual SEScalarVolumePerTime& GetOxygenConsumptionRate();
  virtual double GetOxygenConsumptionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasRespiratoryExchangeRatio() const;
  virtual SEScalar& GetRespiratoryExchangeRatio();
  virtual double GetRespiratoryExchangeRatio() const;

  virtual bool HasLiverInsulinSetPoint() const;
  virtual SEScalarAmountPerVolume& GetLiverInsulinSetPoint();
  virtual double GetLiverInsulinSetPoint(const AmountPerVolumeUnit& unit) const;

  virtual bool HasLiverGlucagonSetPoint() const;
  virtual SEScalarMassPerVolume& GetLiverGlucagonSetPoint();
  virtual double GetLiverGlucagonSetPoint(const MassPerVolumeUnit& unit) const;

  virtual bool HasMuscleInsulinSetPoint() const;
  virtual SEScalarAmountPerVolume& GetMuscleInsulinSetPoint();
  virtual double GetMuscleInsulinSetPoint(const AmountPerVolumeUnit& unit) const;

  virtual bool HasMuscleGlucagonSetPoint() const;
  virtual SEScalarMassPerVolume& GetMuscleGlucagonSetPoint();
  virtual double GetMuscleGlucagonSetPoint(const MassPerVolumeUnit& unit) const;

  virtual bool HasFatInsulinSetPoint() const;
  virtual SEScalarAmountPerVolume& GetFatInsulinSetPoint();
  virtual double GetFatInsulinSetPoint(const AmountPerVolumeUnit& unit) const;

  virtual bool HasFatGlucagonSetPoint() const;
  virtual SEScalarMassPerVolume& GetFatGlucagonSetPoint();
  virtual double GetFatGlucagonSetPoint(const MassPerVolumeUnit& unit) const;

  virtual bool HasLiverGlycogen() const;
  virtual SEScalarMass& GetLiverGlycogen();
  virtual double GetLiverGlycogen(const MassUnit& unit) const;

  virtual bool HasMuscleGlycogen() const;
  virtual SEScalarMass& GetMuscleGlycogen();
  virtual double GetMuscleGlycogen(const MassUnit& unit) const;

  virtual bool HasStoredProtein() const;
  virtual SEScalarMass& GetStoredProtein();
  virtual double GetStoredProtein(const MassUnit& unit) const;

  virtual bool HasStoredFat() const;
  virtual SEScalarMass& GetStoredFat();
  virtual double GetStoredFat(const MassUnit& unit) const;


protected:
  SEScalarVolumePerTime * m_CarbonDioxideProductionRate;
  SEScalar0To1*				m_DehydrationFraction;
  SEScalarVolume*           m_ExtracellularFluidVolume;
  SEScalarVolume*           m_ExtravascularFluidVolume;
  SEScalarVolume*           m_IntracellularFluidVolume;
  SEScalarVolume*           m_TotalBodyFluidVolume;
  SEScalar*                 m_IntracellularFluidPH;
  SEScalarVolumePerTime*	  m_OxygenConsumptionRate;
  SEScalar*				          m_RespiratoryExchangeRatio;
  SEScalarAmountPerVolume*  m_LiverInsulinSetPoint;
  SEScalarMassPerVolume*    m_LiverGlucagonSetPoint;
  SEScalarAmountPerVolume*  m_MuscleInsulinSetPoint;
  SEScalarMassPerVolume*    m_MuscleGlucagonSetPoint;
  SEScalarAmountPerVolume*  m_FatInsulinSetPoint;
  SEScalarMassPerVolume*    m_FatGlucagonSetPoint;
  SEScalarMass*             m_LiverGlycogen;
  SEScalarMass*             m_MuscleGlycogen;
  SEScalarMass*             m_StoredProtein;
  SEScalarMass*             m_StoredFat;

};
