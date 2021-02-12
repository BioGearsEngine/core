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

#include <memory>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
class SESubstance;
class SESubstanceManager;
namespace io {
  class Substance;
}
class BIOGEARS_API SESubstanceCompound : public Loggable {
  friend io::Substance;

public:
  explicit SESubstanceCompound(const std::string& name, Logger* logger);
  explicit SESubstanceCompound(const char* name, Logger* logger);
  SESubstanceCompound(Logger* logger);
  virtual ~SESubstanceCompound();

  static std::unique_ptr<SESubstanceCompound> make_unique(const std::string& name, Logger* logger);
  virtual void Clear();

  virtual bool Load(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr);
  virtual CDM::SubstanceCompoundData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceCompoundData& data) const;

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual CDM::enumSubstanceClass::value GetClassification() const;
  virtual void SetClassification(CDM::enumSubstanceClass::value subClass);
  virtual bool HasClassification() const;
  virtual void InvalidateClassification();
  virtual bool GetRhFactor() const;
  virtual void SetRhFactor(bool subFactor);
  virtual bool HasRhFactor() const;

  bool HasComponent() const;
  bool HasComponent(const SESubstance& substance) const;
  std::vector<SESubstanceConcentration>& GetComponents();
  const std::vector<SESubstanceConcentration> GetComponents() const;
  SESubstanceConcentration& GetComponent(SESubstance& substance);
  const SESubstanceConcentration GetComponent(SESubstance& substance) const;
  void RemoveComponent(const SESubstance& substance);

protected:
  std::string m_Name;
  CDM::enumSubstanceClass::value m_Classification;
  bool m_RhFactor;

  std::vector<SESubstanceConcentration> m_Components;
};
}