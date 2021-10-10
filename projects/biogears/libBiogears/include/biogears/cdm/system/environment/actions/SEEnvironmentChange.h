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
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/schema/cdm/EnvironmentActions.hxx>

namespace biogears {
namespace io {
 class EnvironmentActions;
}
class BIOGEARS_API SEEnvironmentChange : public SEEnvironmentAction {
  friend io::EnvironmentActions;
public:
  SEEnvironmentChange(SESubstanceManager& substances);
  virtual ~SEEnvironmentChange() override;

  static constexpr const char* TypeTag() { return "SEEnvironmentChange"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override;

  virtual bool IsValid() const override;

  virtual bool Load(const CDM::EnvironmentChangeData& in);
  virtual CDM::EnvironmentChangeData* Unload() const override;

protected:
  virtual void Unload(CDM::EnvironmentChangeData& data) const;

public:
  virtual bool HasConditions() const;
  virtual SEEnvironmentalConditions& GetConditions();
  virtual const SEEnvironmentalConditions* GetConditions() const;

  virtual const char* GetConditionsFile() const;
  virtual void SetConditionsFile(const char* fileName);
  virtual void SetConditionsFile(const std::string& fileName);
  virtual bool HasConditionsFile() const;
  virtual void InvalidateConditionsFile();

  virtual void ToString(std::ostream& str) const override;

protected:
  SESubstanceManager& m_Substances;

  std::string m_ConditionsFile;
  SEEnvironmentalConditions* m_Conditions;
};
}