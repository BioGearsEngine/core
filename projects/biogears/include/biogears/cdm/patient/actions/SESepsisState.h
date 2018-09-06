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
#include <biogears/cdm/utils/Logger.h>
#include <biogears/exports.h>
#include <biogears/schema/SepsisStateData.hxx>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/schema/ScalarData.hxx>


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

  virtual bool HasNeutrophil() const;
  virtual SEScalar& GetNeutrophil();
  virtual double GetNeutrophil() const;

  virtual bool HasTissueDamage() const;
  virtual SEScalar& GetTissueDamage();
  virtual double GetTissueDamage() const;

  virtual bool HasAntiinflammation() const;
  virtual SEScalar& GetAntiinflammation();
  virtual double GetAntiinflammation() const;

protected:
  SEScalar* m_Pathogen;
  SEScalar* m_Neutrophil;
  SEScalar* m_TissueDamage;
  SEScalar* m_Antiinflammation;
};