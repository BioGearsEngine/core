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
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>

namespace biogears {
class SESubstance;
class SESubstanceManager;
class SEDataRequestManager;
namespace io {
  class DataRequests;
}

class BIOGEARS_API SECompartmentSubstanceDataRequest : public SECompartmentDataRequest {
  friend class SEDataRequestManager;
  friend io::DataRequests;

public:
  SECompartmentSubstanceDataRequest(const SEDecimalFormat* dfault = nullptr);
  virtual ~SECompartmentSubstanceDataRequest() override;

  virtual void Clear() override; // clear memory

 
  bool operator==(SECompartmentSubstanceDataRequest const&) const;
  bool operator!=(SECompartmentSubstanceDataRequest const&) const;

public:
  virtual size_t HashCode() const override;

  virtual SESubstance* GetSubstance() const;
  virtual void SetSubstance(SESubstance* sub);
  virtual bool HasSubstance() const;
  virtual void InvalidateSubstance();

  using SECompartmentDataRequest::Set;
  virtual void Set(const char* cmpt, SESubstance& substance, const char* name, const char* unit = "");
  virtual void Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const std::string& unit = "");
  virtual void Set(const char* cmpt, SESubstance& substance, const char* name, const CCompoundUnit& unit);
  virtual void Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const CCompoundUnit& unit);

protected:
  SESubstance* m_Substance;
};
}