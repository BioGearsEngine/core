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

#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>

CDM_BIND_DECL(SubstancePharmacodynamicsData)

namespace biogears {

class SEScalarFrequency;
class FrequencyUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;

class BIOGEARS_API SESubstancePharmacodynamics : Loggable {
public:
  SESubstancePharmacodynamics(Logger* logger);
  virtual ~SESubstancePharmacodynamics();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::SubstancePharmacodynamicsData& in);
  virtual CDM::SubstancePharmacodynamicsData* Unload() const;

protected:
  virtual void Unload(CDM::SubstancePharmacodynamicsData& data) const;

public:
  virtual bool HasAntibacterialEffect() const;
  virtual SEScalarFrequency& GetAntibacterialEffect();
  virtual double GetAntibacterialEffect(const FrequencyUnit& unit) const;

  virtual bool HasBronchodilation() const;
  virtual SEScalarFraction& GetBronchodilation();
  virtual double GetBronchodilation() const;

  virtual bool HasDiastolicPressureModifier() const;
  virtual SEScalarFraction& GetDiastolicPressureModifier();
  virtual double GetDiastolicPressureModifier() const;

  virtual bool HasEC50() const;
  virtual SEScalarMassPerVolume& GetEC50();
  virtual double GetEC50(const MassPerVolumeUnit& unit) const;

  virtual bool HasEMaxShapeParameter() const;
  virtual SEScalar& GetEMaxShapeParameter();
  virtual double GetEMaxShapeParameter() const;

  virtual bool HasHeartRateModifier() const;
  virtual SEScalarFraction& GetHeartRateModifier();
  virtual double GetHeartRateModifier() const;

  virtual bool HasHemorrhageModifier() const;
  virtual SEScalarFraction& GetHemorrhageModifier();
  virtual double GetHemorrhageModifier() const;

  virtual bool HasNeuromuscularBlock() const;
  virtual SEScalarFraction& GetNeuromuscularBlock();
  virtual double GetNeuromuscularBlock() const;

  virtual bool HasPupillaryResponse() const;
  virtual SEPupillaryResponse& GetPupillaryResponse();
  virtual const SEPupillaryResponse* GetPupillaryResponse() const;
  virtual void RemovePupillaryResponse();

  virtual bool HasRespirationRateModifier() const;
  virtual SEScalarFraction& GetRespirationRateModifier();
  virtual double GetRespirationRateModifier() const;

  virtual bool HasSedation() const;
  virtual SEScalarFraction& GetSedation();
  virtual double GetSedation() const;

  virtual bool HasSystolicPressureModifier() const;
  virtual SEScalarFraction& GetSystolicPressureModifier();
  virtual double GetSystolicPressureModifier() const;

  virtual bool HasTidalVolumeModifier() const;
  virtual SEScalarFraction& GetTidalVolumeModifier();
  virtual double GetTidalVolumeModifier() const;

  virtual bool HasTubularPermeabilityModifier() const;
  virtual SEScalarFraction& GetTubularPermeabilityModifier();
  virtual double GetTubularPermeabilityModifier() const;

  virtual bool HasCentralNervousModifier() const;
  virtual SEScalarFraction& GetCentralNervousModifier();
  virtual double GetCentralNervousModifier() const;

  virtual bool HasEffectSiteRateConstant() const;
  virtual SEScalarFrequency& GetEffectSiteRateConstant();
  virtual double GetEffectSiteRateConstant(const FrequencyUnit& unit) const;

protected:
  virtual void CalculateDerived();

  SEScalarFrequency* m_AntibacterialEffect;
  SEScalarFraction* m_Bronchodilation;
  SEScalarFraction* m_DiastolicPressureModifier;
  SEScalarMassPerVolume* m_EC50;
  SEScalar* m_EMaxShapeParameter;
  SEScalarFraction* m_HeartRateModifier;
  SEScalarFraction* m_HemorrhageModifier;
  SEScalarFraction* m_NeuromuscularBlock;
  SEPupillaryResponse* m_PupillaryResponse;
  SEScalarFraction* m_RespirationRateModifier;
  SEScalarFraction* m_Sedation;
  SEScalarFraction* m_SystolicPressureModifier;
  SEScalarFraction* m_TidalVolumeModifier;
  SEScalarFraction* m_TubularPermeabilityModifier;
  SEScalarFraction* m_CentralNervousModifier;
  SEScalarFrequency* m_EffectSiteRateConstant;
};
}