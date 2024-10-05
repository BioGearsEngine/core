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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/enums/SESubstanceEnums.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>

#include <map>
#include<set>

#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
namespace io {
  class Scenario;
}
} // namespace biogears
#pragma warning(disable : 4661)
namespace std {
extern template class map<biogears::SESubstance*, const biogears::SESubstanceDefinition>;
extern template class map<biogears::SESubstanceCompound*, const biogears::SESubstanceCompound>;
}
#pragma warning(default : 4661)

namespace biogears {
class BIOGEARS_API SESubstanceManager : public Loggable {
  friend io::Scenario;

public:
  SESubstanceManager(Logger* logger);
  virtual ~SESubstanceManager();

  virtual void Clear();
  virtual void Reset();
  virtual bool LoadSubstanceDirectory();

  virtual const std::set<SESubstance*>& GetSubstances() const;
  virtual SESubstance* GetSubstance(const char* name) const;
  virtual SESubstance* GetSubstance(const std::string& name) const;
  virtual SESubstance* GetSubstance(const SESubstanceDefinition& definition) const;
  virtual void AddSubstance(SESubstance& substance);
  virtual SESubstance* AddSubstance(SESubstanceDefinition const& substance);

  virtual bool IsActive(const SESubstance& substance) const;
  virtual const std::set<SESubstance*>& GetActiveSubstances() const;
  virtual void AddActiveSubstance(SESubstance& substance);
  virtual SESubstance* AddActiveSubstance(SESubstanceDefinition const& substance);
  virtual void RemoveActiveSubstance(const SESubstance& substance);
  virtual void RemoveActiveSubstances(const std::set<SESubstance*>& substances);
  virtual void RemoveActiveSubstances();

  virtual const std::set<SESubstance*>& GetActiveGases() const;
  virtual const std::set<SESubstance*>& GetActiveLiquids() const;
  virtual const std::set<SESubstance*>& GetActiveDrugs() const;

  virtual const std::set<SESubstanceCompound*>& GetCompounds() const;
  virtual SESubstanceCompound* GetCompound(const char* name) const;
  virtual SESubstanceCompound* GetCompound(const std::string& name) const;
  virtual void AddCompound(SESubstanceCompound& compound);

  virtual bool IsActive(const SESubstanceCompound& compound) const;
  virtual const std::set<SESubstanceCompound*>& GetActiveCompounds() const;
  virtual void AddActiveCompound(SESubstanceCompound& compound);
  virtual void RemoveActiveCompound(SESubstanceCompound& compound);
  virtual void RemoveActiveCompounds(const std::set<SESubstanceCompound*>& compounds);

  virtual SESubstance* ReadSubstanceFile(const char* xmlFile);
  virtual SESubstance* ReadSubstanceFile(const std::string& xmlFile);

  bool operator==(SESubstanceManager const& rhs) const;
  bool operator!=(SESubstanceManager const& rhs) const;

protected:
  std::set<SESubstance*> m_Substances;
  std::set<SESubstance*> m_ActiveSubstances;
  std::set<SESubstance*> m_ActiveGases;
  std::set<SESubstance*> m_ActiveLiquids;
  std::set<SESubstance*> m_ActiveDrugs;

  std::set<SESubstanceCompound*> m_Compounds;
  std::set<SESubstanceCompound*> m_ActiveCompounds;

private:
  std::map<SESubstance*, SESubstanceDefinition> m_OriginalSubstanceData;
  std::map<SESubstanceCompound*, SESubstanceCompound> m_OriginalCompoundData;
};
}
