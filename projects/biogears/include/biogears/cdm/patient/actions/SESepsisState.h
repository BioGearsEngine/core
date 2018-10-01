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

class BIOGEARS_API SESepsisState {
public:
  SESepsisState();
  virtual ~SESepsisState();

  virtual void Clear();

  virtual bool Load(const CDM::SepsisStateData& in);
  virtual CDM::SepsisStateData* Unload() const;
  virtual bool IsValid();

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  virtual void Unload(CDM::SepsisStateData& data) const;

public:
  virtual bool HasPathogen() const;
  virtual SEScalar& GetPathogen();
  virtual double GetPathogen() const;

  virtual bool HasMacrophage() const;
  virtual SEScalar& GetMacrophage();
  virtual double GetMacrophage() const;

  virtual bool HasNeutrophilResting() const;
  virtual SEScalar& GetNeutrophilResting();
  virtual double GetNeutrophilResting() const;

  virtual bool HasNeutrophilActive() const;
  virtual SEScalar& GetNeutrophilActive();
  virtual double GetNeutrophilActive() const;

  virtual bool HasTissueIntegrity() const;
  virtual SEScalar0To1& GetTissueIntegrity();
  virtual double GetTissueIntegrity() const;

  virtual bool HasAntiinflammation() const;
  virtual SEScalar& GetAntiinflammation();
  virtual double GetAntiinflammation() const;

protected:
  SEScalar* m_Pathogen;
  SEScalar* m_Macrophage;
  SEScalar* m_NeutrophilResting;
  SEScalar* m_NeutrophilActive;
  SEScalar0To1* m_TissueIntegrity;
  SEScalar* m_Antiinflammation;
};

}