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
CDM_BIND_DECL(PharmacodynamicModifierData)

namespace biogears {

class SEScalarFrequency;
class FrequencyUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;
class SEPharmacodynamicModifier;

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
  virtual SEPharmacodynamicModifier& GetBronchodilation();
  virtual const SEPharmacodynamicModifier* GetBronchodilation() const;

  virtual bool HasDiastolicPressureModifier() const;
  virtual SEPharmacodynamicModifier& GetDiastolicPressureModifier();
  virtual const SEPharmacodynamicModifier* GetDiastolicPressureModifier() const;

  virtual bool HasEMaxShapeParameter() const;
  virtual SEScalar& GetEMaxShapeParameter();
  virtual double GetEMaxShapeParameter() const;

  virtual bool HasFeverModifier() const;
  virtual SEPharmacodynamicModifier& GetFeverModifier();
  virtual const SEPharmacodynamicModifier* GetFeverModifier() const;

  virtual bool HasHeartRateModifier() const;
  virtual SEPharmacodynamicModifier& GetHeartRateModifier();
  virtual const SEPharmacodynamicModifier* GetHeartRateModifier() const;

  virtual bool HasHemorrhageModifier() const;
  virtual SEPharmacodynamicModifier& GetHemorrhageModifier();
  virtual const SEPharmacodynamicModifier* GetHemorrhageModifier() const;

  virtual bool HasNeuromuscularBlock() const;
  virtual SEPharmacodynamicModifier& GetNeuromuscularBlock();
  virtual const SEPharmacodynamicModifier* GetNeuromuscularBlock() const;

  virtual bool HasPainModifier() const;
  virtual SEPharmacodynamicModifier& GetPainModifier();
  virtual const SEPharmacodynamicModifier* GetPainModifier() const;

  virtual bool HasPupilReactivityModifier() const;
  virtual SEPharmacodynamicModifier& GetPupilReactivityModifier();
  virtual const SEPharmacodynamicModifier* GetPupilReactivityModifier() const;

  virtual bool HasPupilSizeModifier() const;
  virtual SEPharmacodynamicModifier& GetPupilSizeModifier();
  virtual const SEPharmacodynamicModifier* GetPupilSizeModifier() const;

  virtual bool HasRespirationRateModifier() const;
  virtual SEPharmacodynamicModifier& GetRespirationRateModifier();
  virtual const SEPharmacodynamicModifier* GetRespirationRateModifier() const;

  virtual bool HasSedation() const;
  virtual SEPharmacodynamicModifier& GetSedation();
  virtual const SEPharmacodynamicModifier* GetSedation() const;

  virtual bool HasSystolicPressureModifier() const;
  virtual SEPharmacodynamicModifier& GetSystolicPressureModifier();
  virtual const SEPharmacodynamicModifier* GetSystolicPressureModifier() const;

  virtual bool HasTidalVolumeModifier() const;
  virtual SEPharmacodynamicModifier& GetTidalVolumeModifier();
  virtual const SEPharmacodynamicModifier* GetTidalVolumeModifier() const;

  virtual bool HasTubularPermeabilityModifier() const;
  virtual SEPharmacodynamicModifier& GetTubularPermeabilityModifier();
  virtual const SEPharmacodynamicModifier* GetTubularPermeabilityModifier() const;

  virtual bool HasCentralNervousModifier() const;
  virtual SEPharmacodynamicModifier& GetCentralNervousModifier();
  virtual const SEPharmacodynamicModifier* GetCentralNervousModifier() const;

  virtual bool HasEffectSiteRateConstant() const;
  virtual SEScalarFrequency& GetEffectSiteRateConstant();
  virtual double GetEffectSiteRateConstant(const FrequencyUnit& unit) const;

private:
  SEScalarFrequency* m_AntibacterialEffect;
  SEPharmacodynamicModifier* m_Bronchodilation;
  SEPharmacodynamicModifier* m_DiastolicPressureModifier;
  SEScalar* m_EMaxShapeParameter;
  SEPharmacodynamicModifier* m_FeverModifier;
  SEPharmacodynamicModifier* m_HeartRateModifier;
  SEPharmacodynamicModifier* m_HemorrhageModifier;
  SEPharmacodynamicModifier* m_NeuromuscularBlock;
  SEPharmacodynamicModifier* m_PainModifier;
  SEPharmacodynamicModifier* m_PupilReactivityModifier;
  SEPharmacodynamicModifier* m_PupilSizeModifier;
  SEPharmacodynamicModifier* m_RespirationRateModifier;
  SEPharmacodynamicModifier* m_Sedation;
  SEPharmacodynamicModifier* m_SystolicPressureModifier;
  SEPharmacodynamicModifier* m_TidalVolumeModifier;
  SEPharmacodynamicModifier* m_TubularPermeabilityModifier;
  SEPharmacodynamicModifier* m_CentralNervousModifier;
  SEScalarFrequency* m_EffectSiteRateConstant;
};

class BIOGEARS_API SEPharmacodynamicModifier {
public:
  SEPharmacodynamicModifier();
  virtual ~SEPharmacodynamicModifier();

  virtual void Clear();
  virtual bool IsValid() const;
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::PharmacodynamicModifierData& in);
  virtual CDM::PharmacodynamicModifierData* Unload() const;

  virtual bool HasEMax() const;
  virtual SEScalarFraction& GetEMax();
  virtual double GetEMax() const;

  virtual bool HasEC50() const;
  virtual SEScalarMassPerVolume& GetEC50();
  virtual double GetEC50(const MassPerVolumeUnit& unit) const;

protected:
  virtual void Unload(CDM::PharmacodynamicModifierData& data) const;

private:
  SEScalarFraction* m_EMax;
  SEScalarMassPerVolume* m_EC50;
};
}