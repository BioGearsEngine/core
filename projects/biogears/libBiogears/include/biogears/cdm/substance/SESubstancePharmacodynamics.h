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

#include <biogears/cdm/enums/SESubstanceEnums.h>
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>

namespace biogears {
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;
class SEScalarFraction;
class SEPharmacodynamicModifier;

namespace io {
  class Substance;
}
} // namespace biogears
#pragma warning(disable : 4661)

namespace std {
extern template class map<string, biogears::SEPharmacodynamicModifier*>;
}
#pragma warning(default : 4661)

namespace biogears {
class BIOGEARS_API SESubstancePharmacodynamics : public Loggable {
  friend io::Substance;

public:
  SESubstancePharmacodynamics(SESubstancePharmacodynamics const& obj);
  SESubstancePharmacodynamics(SESubstancePharmacodynamics&& obj);
  SESubstancePharmacodynamics(Logger* logger = nullptr);

  SESubstancePharmacodynamics& operator=(SESubstancePharmacodynamics const& rhs);
  SESubstancePharmacodynamics& operator=(SESubstancePharmacodynamics&& rhs);

  virtual ~SESubstancePharmacodynamics();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

public:
  virtual bool HasAntibacterialEffect() const;
  virtual SEScalarFrequency& GetAntibacterialEffect();
  virtual double GetAntibacterialEffect(const FrequencyUnit& unit) const;

  virtual bool HasBronchodilation() const;
  virtual SEPharmacodynamicModifier& GetBronchodilation();
  virtual SEPharmacodynamicModifier const& GetBronchodilation() const;

  virtual bool HasDiastolicPressureModifier() const;
  virtual SEPharmacodynamicModifier& GetDiastolicPressureModifier();
  virtual SEPharmacodynamicModifier const& GetDiastolicPressureModifier() const;

  virtual bool HasEMaxShapeParameter() const;
  virtual SEScalar& GetEMaxShapeParameter();
  virtual double GetEMaxShapeParameter() const;

  virtual bool HasFeverModifier() const;
  virtual SEPharmacodynamicModifier& GetFeverModifier();
  virtual SEPharmacodynamicModifier const& GetFeverModifier() const;

  virtual bool HasHeartRateModifier() const;
  virtual SEPharmacodynamicModifier& GetHeartRateModifier();
  virtual SEPharmacodynamicModifier const& GetHeartRateModifier() const;

  virtual bool HasHemorrhageModifier() const;
  virtual SEPharmacodynamicModifier& GetHemorrhageModifier();
  virtual SEPharmacodynamicModifier const& GetHemorrhageModifier() const;

  virtual bool HasNeuromuscularBlock() const;
  virtual SEPharmacodynamicModifier& GetNeuromuscularBlock();
  virtual SEPharmacodynamicModifier const& GetNeuromuscularBlock() const;

  virtual bool HasPainModifier() const;
  virtual SEPharmacodynamicModifier& GetPainModifier();
  virtual SEPharmacodynamicModifier const& GetPainModifier() const;

  virtual bool HasPupilReactivityModifier() const;
  virtual SEPharmacodynamicModifier& GetPupilReactivityModifier();
  virtual SEPharmacodynamicModifier const& GetPupilReactivityModifier() const;

  virtual bool HasPupilSizeModifier() const;
  virtual SEPharmacodynamicModifier& GetPupilSizeModifier();
  virtual SEPharmacodynamicModifier const& GetPupilSizeModifier() const;

  virtual bool HasRespirationRateModifier() const;
  virtual SEPharmacodynamicModifier& GetRespirationRateModifier();
  virtual SEPharmacodynamicModifier const& GetRespirationRateModifier() const;

  virtual bool HasSedation() const;
  virtual SEPharmacodynamicModifier& GetSedation();
  virtual SEPharmacodynamicModifier const& GetSedation() const;

  virtual bool HasSystolicPressureModifier() const;
  virtual SEPharmacodynamicModifier& GetSystolicPressureModifier();
  virtual SEPharmacodynamicModifier const& GetSystolicPressureModifier() const;

  virtual bool HasTidalVolumeModifier() const;
  virtual SEPharmacodynamicModifier& GetTidalVolumeModifier();
  virtual SEPharmacodynamicModifier const& GetTidalVolumeModifier() const;

  virtual bool HasTubularPermeabilityModifier() const;
  virtual SEPharmacodynamicModifier& GetTubularPermeabilityModifier();
  virtual SEPharmacodynamicModifier const& GetTubularPermeabilityModifier() const;

  virtual bool HasCentralNervousModifier() const;
  virtual SEPharmacodynamicModifier& GetCentralNervousModifier();
  virtual SEPharmacodynamicModifier const& GetCentralNervousModifier() const;

  virtual bool HasEffectSiteRateConstant() const;
  virtual SEScalarFrequency& GetEffectSiteRateConstant();
  virtual double GetEffectSiteRateConstant(const FrequencyUnit& unit) const;

  virtual std::map<std::string, SEPharmacodynamicModifier*> GetPharmacodynamicModifiers() const;

  bool operator==(const SESubstancePharmacodynamics& rhs) const;
  bool operator!=(const SESubstancePharmacodynamics& rhs) const;

private:
  std::map<std::string, SEPharmacodynamicModifier*> m_Modifiers;
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
  friend io::Substance;

public:
  SEPharmacodynamicModifier();
  SEPharmacodynamicModifier(SEScalarMassPerVolume const&, SEScalarFraction const&);
  SEPharmacodynamicModifier(SEPharmacodynamicModifier const& obj);
  SEPharmacodynamicModifier(SEPharmacodynamicModifier&& obj);
  ~SEPharmacodynamicModifier();

  SEPharmacodynamicModifier& operator=(SEPharmacodynamicModifier const& rhs);
  SEPharmacodynamicModifier& operator=(SEPharmacodynamicModifier&& rhs);

  void Clear();
  bool IsValid() const;
  const SEScalar* GetScalar(const char* name);
  const SEScalar* GetScalar(const std::string& name);

  bool HasEMax() const;
  SEScalarFraction& GetEMax();
  double GetEMax() const;

  bool HasEC50() const;
  SEScalarMassPerVolume& GetEC50();
  double GetEC50(const MassPerVolumeUnit& unit) const;

  void Set(SEScalarFraction const& eMax, SEScalarMassPerVolume const& ec50);

  bool operator==(const SEPharmacodynamicModifier& rhs) const;
  bool operator!=(const SEPharmacodynamicModifier& rhs) const;

private:
  SEScalarFraction* m_EMax;
  SEScalarMassPerVolume* m_EC50;
};
}