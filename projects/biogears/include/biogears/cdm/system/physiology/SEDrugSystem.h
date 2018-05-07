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
#include <biogears/schema/DrugSystemData.hxx>
#include <biogears/schema/enumOnOff.hxx>
#include <biogears/cdm/system/physiology/SEPupillaryResponse.h>

class BIOGEARS_API SEDrugSystem : public SESystem
{
public:

	SEDrugSystem(Logger* logger);
	virtual ~SEDrugSystem();

  virtual void Clear();// Deletes all members
  
	virtual const SEScalar* GetScalar(const std::string& name);
	
  virtual bool Load(const CDM::DrugSystemData& in);
  virtual CDM::DrugSystemData* Unload() const;
protected:
  virtual void Unload(CDM::DrugSystemData& data) const;
public:

	virtual bool HasBronchodilationLevel() const;
	virtual SEScalarFraction& GetBronchodilationLevel();
	virtual double GetBronchodilationLevel() const;

	virtual bool HasHeartRateChange() const;
	virtual SEScalarFrequency& GetHeartRateChange();
  virtual double GetHeartRateChange(const FrequencyUnit& unit) const;

	virtual bool HasMeanBloodPressureChange() const;
	virtual SEScalarPressure& GetMeanBloodPressureChange();
  virtual double GetMeanBloodPressureChange(const PressureUnit& unit) const;

	virtual bool HasNeuromuscularBlockLevel() const;
	virtual SEScalarFraction& GetNeuromuscularBlockLevel();
	virtual double GetNeuromuscularBlockLevel() const;

	virtual bool HasPulsePressureChange() const;
	virtual SEScalarPressure& GetPulsePressureChange();
  virtual double GetPulsePressureChange(const PressureUnit& unit) const;

  virtual bool HasPupillaryResponse() const;
  virtual SEPupillaryResponse& GetPupillaryResponse();
  virtual const SEPupillaryResponse* GetPupillaryResponse() const;
  virtual void RemovePupillaryResponse();

	virtual bool HasRespirationRateChange() const;
	virtual SEScalarFrequency& GetRespirationRateChange();
  virtual double GetRespirationRateChange(const FrequencyUnit& unit) const;

	virtual bool HasSedationLevel() const;
	virtual SEScalarFraction& GetSedationLevel();
	virtual double GetSedationLevel() const;

	virtual bool HasTidalVolumeChange() const;
	virtual SEScalarVolume& GetTidalVolumeChange();
  virtual double GetTidalVolumeChange(const VolumeUnit& unit) const;

  virtual bool HasTubularPermeabilityChange() const;
  virtual SEScalarFraction& GetTubularPermeabilityChange();
  virtual double GetTubularPermeabilityChange() const;

  virtual bool HasCentralNervousResponse() const;
  virtual SEScalarFraction& GetCentralNervousResponse();
  virtual double GetCentralNervousResponse() const;



protected:

	SEScalarFraction*          m_BronchodilationLevel;
	SEScalarFrequency*         m_HeartRateChange;
	SEScalarPressure*          m_MeanBloodPressureChange;
	SEScalarFraction*          m_NeuromuscularBlockLevel;
	SEScalarPressure*          m_PulsePressureChange;
  SEPupillaryResponse*       m_PupillaryResponse;
	SEScalarFrequency*         m_RespirationRateChange;
	SEScalarFraction*          m_SedationLevel;
	SEScalarVolume*            m_TidalVolumeChange;
  SEScalarFraction*          m_TubularPermeabilityChange;
  SEScalarFraction*			   m_CentralNervousResponse;
};