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
#include <biogears/cdm/scenario/requests/SEDataRequest.h>
#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
class SESubstance;
class SESubstanceManager;
class SEDataRequestManager;
namespace io {
  class Scenario;
}

class BIOGEARS_API SESubstanceDataRequest : public SEDataRequest {
  friend class SEDataRequestManager;
  friend io::Scenario;


public:
  SESubstanceDataRequest(const SEDecimalFormat* dfault = nullptr);
  virtual ~SESubstanceDataRequest();

  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::SubstanceDataRequestData& in, const SESubstanceManager& substances);
  virtual CDM::SubstanceDataRequestData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceDataRequestData& data) const;

public:
  size_t HashCode() const override;
  virtual const char* GetCompartment() const;
  virtual void SetCompartment(const char* name);
  virtual void SetCompartment(const std::string& name);
  virtual bool HasCompartment() const;
  virtual void InvalidateCompartment();

  virtual const SESubstance* GetSubstance() const;
  virtual void SetSubstance(SESubstance* name);
  virtual bool HasSubstance() const;
  virtual void InvalidateSubstance();

  virtual void Set(const SESubstance& substance, const char* name, const CCompoundUnit& unit);
  virtual void Set(const SESubstance& substance, const std::string& name, const CCompoundUnit& unit);
  virtual void Set(const SESubstance& substance, const char* name, const char* unit = "");
  virtual void Set(const SESubstance& substance, const std::string& name, const std::string& unit = "");
  virtual void Set(const SESubstance& substance, const char* cmpt, const char* name, const CCompoundUnit& unit);
  virtual void Set(const SESubstance& substance, const std::string& cmpt, const std::string& name, const CCompoundUnit& unit);
  virtual void Set(const SESubstance& substance, const char* cmpt, const char* name, const char* unit = "");
  virtual void Set(const SESubstance& substance, const std::string& cmpt, const std::string& name, const std::string& unit = "");

protected:
  std::string m_Compartment;
  const SESubstance* m_Substance;
};
}