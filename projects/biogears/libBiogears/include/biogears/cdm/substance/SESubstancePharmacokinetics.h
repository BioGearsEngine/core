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

#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/schema/cdm/Substance.hxx>

CDM_BIND_DECL(SubstancePharmacokineticsData)

OPEN_BIOGEARS_NAMESPACE
namespace io {
  class Substance;
}
CLOSE_BIOGEARS_NAMESPACE // namespace biogears

namespace std {
BG_EXT template class BIOGEARS_API map<string, biogears::SESubstanceTissuePharmacokinetics*>;
}
OPEN_BIOGEARS_NAMESPACE
class BIOGEARS_API SESubstancePharmacokinetics : public Loggable {
  friend io::Substance;

public:
  SESubstancePharmacokinetics(Logger* logger);
  virtual ~SESubstancePharmacokinetics();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);
  virtual const SEScalar* GetScalar(const char* name);

  virtual bool Load(const CDM::SubstancePharmacokineticsData& in);
  virtual CDM::SubstancePharmacokineticsData* Unload() const;

  virtual bool HasPhysicochemicals() const;
  virtual SESubstancePhysicochemicals& GetPhysicochemicals();
  virtual const SESubstancePhysicochemicals* GetPhysicochemicals() const;

  virtual bool HasTissueKinetics() const;
  virtual bool HasTissueKinetics(const char* name) const;
  virtual bool HasTissueKinetics(const std::string& name) const;
  virtual SESubstanceTissuePharmacokinetics& GetTissueKinetics(const char* name);
  virtual SESubstanceTissuePharmacokinetics& GetTissueKinetics(const std::string& name);
  virtual const SESubstanceTissuePharmacokinetics* GetTissueKinetics(const char* name) const;
  virtual const SESubstanceTissuePharmacokinetics* GetTissueKinetics(const std::string& name) const;
  virtual void RemoveTissueKinetics(const char* name);
  virtual void RemoveTissueKinetics(const std::string& name);

  bool operator==(const SESubstancePharmacokinetics& rhs) const;
  bool operator!=(const SESubstancePharmacokinetics& rhs) const;

protected:
  virtual void Unload(CDM::SubstancePharmacokineticsData& data) const;

protected:
  SESubstancePhysicochemicals* m_Physicochemicals;
  std::map<std::string, SESubstanceTissuePharmacokinetics*> m_TissueKinetics;
};
CLOSE_BIOGEARS_NAMESPACE