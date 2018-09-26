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
#include <biogears/schema/cdm/ElectroCardioGram.hxx>

namespace biogears {
class SEScalarElectricPotential;
class ElectricPotentialUnit;

class BIOGEARS_API SEElectroCardioGram : public SESystem {
protected:
public:
  SEElectroCardioGram(Logger* logger);
  virtual ~SEElectroCardioGram();

  virtual void Clear();

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::ElectroCardioGramData& in);
  virtual CDM::ElectroCardioGramData* Unload() const;

protected:
  virtual void Unload(CDM::ElectroCardioGramData& data) const;

public:
  virtual bool HasLead1ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead1ElectricPotential();
  virtual double GetLead1ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead2ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead2ElectricPotential();
  virtual double GetLead2ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead3ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead3ElectricPotential();
  virtual double GetLead3ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead4ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead4ElectricPotential();
  virtual double GetLead4ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead5ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead5ElectricPotential();
  virtual double GetLead5ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead6ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead6ElectricPotential();
  virtual double GetLead6ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead7ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead7ElectricPotential();
  virtual double GetLead7ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead8ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead8ElectricPotential();
  virtual double GetLead8ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead9ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead9ElectricPotential();
  virtual double GetLead9ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead10ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead10ElectricPotential();
  virtual double GetLead10ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead11ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead11ElectricPotential();
  virtual double GetLead11ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead12ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead12ElectricPotential();
  virtual double GetLead12ElectricPotential(const ElectricPotentialUnit& unit) const;

protected:
  SEScalarElectricPotential* m_Lead1ElectricPotential;
  SEScalarElectricPotential* m_Lead2ElectricPotential;
  SEScalarElectricPotential* m_Lead3ElectricPotential;
  SEScalarElectricPotential* m_Lead4ElectricPotential;
  SEScalarElectricPotential* m_Lead5ElectricPotential;
  SEScalarElectricPotential* m_Lead6ElectricPotential;
  SEScalarElectricPotential* m_Lead7ElectricPotential;
  SEScalarElectricPotential* m_Lead8ElectricPotential;
  SEScalarElectricPotential* m_Lead9ElectricPotential;
  SEScalarElectricPotential* m_Lead10ElectricPotential;
  SEScalarElectricPotential* m_Lead11ElectricPotential;
  SEScalarElectricPotential* m_Lead12ElectricPotential;

  std::stringstream m_ss;
};
}