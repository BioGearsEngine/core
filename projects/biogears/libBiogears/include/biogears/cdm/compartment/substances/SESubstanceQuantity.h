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
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/SubstanceQuantity.hxx>

namespace biogears {
namespace io {
  class SubstanceQuantity;
}
class BIOGEARS_API SESubstanceQuantity : public Loggable {
  friend io::SubstanceQuantity;

public:
  SESubstanceQuantity(SESubstance& sub);

  virtual ~SESubstanceQuantity() override;

  virtual void Clear() = 0; //clear memory
  virtual void Invalidate() = 0;

  virtual bool Load(const CDM::SubstanceQuantityData& in);
  virtual CDM::SubstanceQuantityData* Unload() = 0;

protected:
  virtual void Unload(CDM::SubstanceQuantityData& data);

public:
  virtual SESubstance& GetSubstance() const { return m_Substance; }

  virtual const SEScalar* GetScalar(const char* name) = 0;
  virtual const SEScalar* GetScalar(const std::string& name) = 0;

protected:
  SESubstance& m_Substance;
};
}