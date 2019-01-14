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
#include <string>
#include <vector>

#include <biogears/cdm/CommonDataModel.h>

#pragma once

CDM_BIND_DECL(CompartmentData)

namespace biogears {

class SEScalar;
class SEGasCompartment;
class SELiquidCompartment;
class SEThermalCompartment;
class SETissueCompartment;
class SESubstance;
class SECircuitManager;

class BIOGEARS_API SECompartment : public Loggable {
protected:
  SECompartment(const char* name, Logger* logger);
  SECompartment(const std::string& name, Logger* logger);

public:
  virtual ~SECompartment() override;

  virtual void Clear();

  virtual bool Load(const CDM::CompartmentData& in, SECircuitManager* circuits = nullptr);
  virtual CDM::CompartmentData* Unload() = 0;

protected:
  virtual void Unload(CDM::CompartmentData& data);

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual const SEScalar* GetScalar(const char* name) = 0;
  virtual const SEScalar* GetScalar(const std::string& name) = 0;

  virtual bool HasChildren() const = 0; // Compartments with children contain 'read only' scalars

  virtual void StateChange() = 0;

protected:
  std::string m_Name;

public:
  template <typename CompartmentType>
  static void FindLeaves(CompartmentType& cmpt, std::vector<CompartmentType*>& leaves)
  {
    for (CompartmentType* child : cmpt.GetChildren()) {
      if (!child->HasChildren())
        leaves.push_back(child);
      else
        FindLeaves(*child, leaves);
    }
  }
};
}