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
#include <biogears/schema/NervousSystemData.hxx>
#include <biogears/cdm/system/physiology/SEPupillaryResponse.h>

class BIOGEARS_API SENervousSystem : public SESystem {
public:
  SENervousSystem(Logger* logger);
  virtual ~SENervousSystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::NervousSystemData& in);
  virtual CDM::NervousSystemData* Unload() const;

protected:
  virtual void Unload(CDM::NervousSystemData& data) const;

public:
  virtual bool HasBaroreceptorHeartRateScale() const;
  virtual SEScalar& GetBaroreceptorHeartRateScale();
  virtual double GetBaroreceptorHeartRateScale() const;

  virtual bool HasBaroreceptorHeartElastanceScale() const;
  virtual SEScalar& GetBaroreceptorHeartElastanceScale();
  virtual double GetBaroreceptorHeartElastanceScale() const;

  virtual bool HasBaroreceptorResistanceScale() const;
  virtual SEScalar& GetBaroreceptorResistanceScale();
  virtual double GetBaroreceptorResistanceScale() const;

  virtual bool HasBaroreceptorComplianceScale() const;
  virtual SEScalar& GetBaroreceptorComplianceScale();
  virtual double GetBaroreceptorComplianceScale() const;

  virtual bool HasChemoreceptorHeartRateScale() const;
  virtual SEScalar& GetChemoreceptorHeartRateScale();
  virtual double GetChemoreceptorHeartRateScale() const;

  virtual bool HasChemoreceptorHeartElastanceScale() const;
  virtual SEScalar& GetChemoreceptorHeartElastanceScale();
  virtual double GetChemoreceptorHeartElastanceScale() const;

  virtual bool HasLeftEyePupillaryResponse() const;
  virtual SEPupillaryResponse& GetLeftEyePupillaryResponse();
  virtual const SEPupillaryResponse* GetLeftEyePupillaryResponse() const;
  virtual void RemoveLeftEyePupillaryResponse();

  virtual bool HasRightEyePupillaryResponse() const;
  virtual SEPupillaryResponse& GetRightEyePupillaryResponse();
  virtual const SEPupillaryResponse* GetRightEyePupillaryResponse() const;
  virtual void RemoveRightEyePupillaryResponse();

protected:
  SEScalar* m_BaroreceptorHeartRateScale;
  SEScalar* m_BaroreceptorHeartElastanceScale;
  SEScalar* m_BaroreceptorResistanceScale;
  SEScalar* m_BaroreceptorComplianceScale;
  SEPupillaryResponse* m_LeftEyePupillaryResponse;
  SEPupillaryResponse* m_RightEyePupillaryResponse;
  SEScalar* m_ChemoreceptorHeartRateScale;
  SEScalar* m_ChemoreceptorHeartElastanceScale;
};
