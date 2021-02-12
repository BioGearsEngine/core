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

CDM_BIND_DECL(CompartmentLinkData)

namespace biogears {
class SECircuitManager;
class SEScalar;
namespace io {
  class Compartment;
}
class BIOGEARS_API SECompartmentLink : public Loggable {
  friend io::Compartment;

protected:
  SECompartmentLink(const char* name, Logger* logger);
  SECompartmentLink(const std::string& name, Logger* logger);

public:
  virtual ~SECompartmentLink();

  virtual void Clear();

  virtual bool Load(const CDM::CompartmentLinkData& in, SECircuitManager* circuits = nullptr);
  virtual CDM::CompartmentLinkData* Unload() = 0;

protected:
  virtual void Unload(CDM::CompartmentLinkData& data);

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual const SEScalar* GetScalar(const char* name) = 0;
  virtual const SEScalar* GetScalar(const std::string& name) = 0;

protected:
  std::string m_Name;
};
}