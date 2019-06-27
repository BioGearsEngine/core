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

#include <biogears/schema/cdm/Physiology.hxx>

namespace biogears {

class SEScalar;
class SEScalar0To1;
class SEImmuneMediators;

class BIOGEARS_API SEInflammationState {
public:
  SEInflammationState();
  virtual ~SEInflammationState();

  virtual void Clear();

  virtual bool Load(const CDM::InflammationStateData& in);
  virtual CDM::InflammationStateData* Unload() const;
  virtual bool IsValid();

  void InitializeState();
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  virtual void Unload(CDM::InflammationStateData& data) const;

public:
  virtual bool HasPathogen() const;
  virtual SEScalar& GetPathogen();
  virtual double GetPathogen() const;

  virtual bool HasTrauma() const;
  virtual SEScalar& GetTrauma();
  virtual double GetTrauma() const;

  virtual bool HasMacrophageResting() const;
  virtual SEScalar& GetMacrophageResting();
  virtual double GetMacrophageResting() const;

  virtual bool HasMacrophageActive() const;
  virtual SEScalar& GetMacrophageActive();
  virtual double GetMacrophageActive() const;

  virtual bool HasNeutrophilResting() const;
  virtual SEScalar& GetNeutrophilResting();
  virtual double GetNeutrophilResting() const;

  virtual bool HasNeutrophilActive() const;
  virtual SEScalar& GetNeutrophilActive();
  virtual double GetNeutrophilActive() const;

  virtual bool HasInducibleNOSynthasePre() const;
  virtual SEScalar& GetInducibleNOSynthasePre();
  virtual double GetInducibleNOSynthasePre() const;

  virtual bool HasInducibleNOSynthase() const;
  virtual SEScalar& GetInducibleNOSynthase();
  virtual double GetInducibleNOSynthase() const;

  virtual bool HasConstitutiveNOSynthase() const;
  virtual SEScalar& GetConstitutiveNOSynthase();
  virtual double GetConstitutiveNOSynthase() const;

  virtual bool HasNitrate() const;
  virtual SEScalar& GetNitrate();
  virtual double GetNitrate() const;

  virtual bool HasNitricOxide() const;
  virtual SEScalar& GetNitricOxide();
  virtual double GetNitricOxide() const;

  virtual bool HasTumorNecrosisFactor() const;
  virtual SEScalar& GetTumorNecrosisFactor();
  virtual double GetTumorNecrosisFactor() const;

  virtual bool HasInterleukin6() const;
  virtual SEScalar& GetInterleukin6();
  virtual double GetInterleukin6() const;

  virtual bool HasInterleukin10() const;
  virtual SEScalar& GetInterleukin10();
  virtual double GetInterleukin10() const;

  virtual bool HasInterleukin12() const;
  virtual SEScalar& GetInterleukin12();
  virtual double GetInterleukin12() const;

  virtual bool HasCatecholamines() const;
  virtual SEScalar& GetCatecholamines();
  virtual double GetCatecholamines() const;

  virtual bool HasTissueIntegrity() const;
  virtual SEScalar0To1& GetTissueIntegrity();
  virtual double GetTissueIntegrity() const;

  virtual bool HasInflammationSources() const;
  virtual std::vector<CDM::enumInflammationSource>& GetInflammationSources();

protected:
  SEScalar* m_Pathogen;
  SEScalar* m_Trauma;
  SEScalar* m_MacrophageResting;
  SEScalar* m_MacrophageActive;
  SEScalar* m_NeutrophilResting;
  SEScalar* m_NeutrophilActive;
  SEScalar* m_InducibleNOSynthasePre;
  SEScalar* m_InducibleNOSynthase;
  SEScalar* m_ConstitutiveNOSynthase;
  SEScalar* m_Nitrate;
  SEScalar* m_NitricOxide;
  SEScalar* m_TumorNecrosisFactor;
  SEScalar* m_Interleukin6;
  SEScalar* m_Interleukin10;
  SEScalar* m_Interleukin12;
  SEScalar* m_Catecholamines;
  SEScalar0To1* m_TissueIntegrity;
  std::vector<CDM::enumInflammationSource> m_InflammationSources;
};

class BIOGEARS_API SEInflammatoryResponse {
public:
  SEInflammatoryResponse();
  virtual ~SEInflammatoryResponse();

  virtual void Clear();

  virtual bool Load(const CDM::InflammatoryResponseData& in);
  virtual CDM::InflammatoryResponseData* Unload() const;
  virtual bool IsValid();

  void Initialize();
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  virtual void Unload(CDM::InflammatoryResponseData& data) const;

public:
  virtual bool HasPathogenTissue() const;
  virtual SEScalar& GetPathogenTissue();
  virtual double GetPathogenTissue() const;

  virtual bool HasPathogenBlood() const;
  virtual SEScalar& GetPathogenBlood();
  virtual double GetPathogenBlood() const;

  virtual bool HasTrauma() const;
  virtual SEScalar& GetTrauma();
  virtual double GetTrauma() const;

  virtual bool HasTissueMediators() const;
  virtual SEImmuneMediators& GetTissueMediators();

  virtual bool HasBloodMediators() const;
  virtual SEImmuneMediators& GetBloodMediators();

  virtual bool HasTissueIntegrity() const;
  virtual SEScalar0To1& GetTissueIntegrity();
  virtual double GetTissueIntegrity() const;

  virtual bool HasInflammationLevel() const;
  virtual SEScalar0To1& GetInflammationLevel();
  virtual double GetInflammationLevel() const;

  virtual bool HasInflammationSources() const;
  virtual std::vector<CDM::enumInflammationSource>& GetInflammationSources();

protected:
  SEScalar* m_PathogenTissue;
  SEScalar* m_PathogenBlood;
  SEScalar* m_Trauma;
  SEImmuneMediators* m_TissueMediators;
  SEImmuneMediators* m_BloodMediators;
  SEScalar0To1* m_InflammationLevel;
  SEScalar0To1* m_TissueIntegrity;
  std::vector<CDM::enumInflammationSource> m_InflammationSources;
};

class BIOGEARS_API SEImmuneMediators {
public:
  SEImmuneMediators();
  virtual ~SEImmuneMediators();

  virtual void Clear();

  virtual bool Load(const CDM::ImmuneMediatorData& in);
  virtual CDM::ImmuneMediatorData* Unload() const;
  virtual bool IsValid();

  void Initialize(bool TissueSpace);		//True means we're initializing Tissue Mediators, false means we're initializing Blood Mediators
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  virtual void Unload(CDM::ImmuneMediatorData& data) const;

public:
  virtual bool HasAntibodies() const;
  virtual SEScalar& GetAntibodies();
  virtual double GetAntibodies() const;

  virtual bool HasMacrophageResting() const;
  virtual SEScalar& GetMacrophageResting();
  virtual double GetMacrophageResting() const;

  virtual bool HasMacrophageActive() const;
  virtual SEScalar& GetMacrophageActive();
  virtual double GetMacrophageActive() const;

  virtual bool HasNeutrophilResting() const;
  virtual SEScalar& GetNeutrophilResting();
  virtual double GetNeutrophilResting() const;

  virtual bool HasNeutrophilActive() const;
  virtual SEScalar& GetNeutrophilActive();
  virtual double GetNeutrophilActive() const;

  virtual bool HasNitricOxide() const;
  virtual SEScalar& GetNitricOxide();
  virtual double GetNitricOxide() const;

  virtual bool HasTumorNecrosisFactor() const;
  virtual SEScalar& GetTumorNecrosisFactor();
  virtual double GetTumorNecrosisFactor() const;

  virtual bool HasInterleukin10() const;
  virtual SEScalar& GetInterleukin10();
  virtual double GetInterleukin10() const;

protected:
  SEScalar* m_Antibodies;
  SEScalar* m_Interleukin10;
  SEScalar* m_MacrophageResting;
  SEScalar* m_MacrophageActive;
  SEScalar* m_NeutrophilResting;
  SEScalar* m_NeutrophilActive;
  SEScalar* m_NitricOxide;
  SEScalar* m_TumorNecrosisFactor;
};
}
