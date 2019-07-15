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
#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {

class SEScalar;

class BIOGEARS_API SEAntibioticPharmacodynamics : Loggable {
public:
  SEAntibioticPharmacodynamics(Logger* logger);
  virtual ~SEAntibioticPharmacodynamics();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::AntibioticPharmacodynamicsData& in);
  virtual CDM::AntibioticPharmacodynamicsData* Unload() const;

protected:
  virtual void Unload(CDM::AntibioticPharmacodynamicsData& data) const;

public:
  virtual bool HasAntibacterialIndex() const;
  CDM::enumAntibioticPDIndex::value GetAntibacterialIndex() const;

  virtual bool HasI50() const;
  virtual SEScalar& GetI50();

  virtual bool HasAntibacterialEffect() const;
  virtual SEScalar& GetAntibacterialEffect();

protected:
  CDM::enumAntibioticPDIndex::value m_AntibacterialIndex;
  SEScalar* m_I50;
  SEScalar* m_AntibacterialEffect;


};
}