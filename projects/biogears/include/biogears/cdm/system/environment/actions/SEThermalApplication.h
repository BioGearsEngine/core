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
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/schema/cdm/EnvironmentActions.hxx>

namespace biogears {
class SEActiveHeating;
class SEActiveCooling;
class SEAppliedTemperature;

class BIOGEARS_API SEThermalApplication : public SEEnvironmentAction {
public:
  SEThermalApplication();
  virtual ~SEThermalApplication() override;

  static constexpr const char* TypeTag() { return "SEThermalApplication"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override;

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::ThermalApplicationData& in);
  virtual CDM::ThermalApplicationData* Unload() const override;

protected:
  virtual void Unload(CDM::ThermalApplicationData& data) const;

public:
  virtual bool HasActiveHeating() const;
  virtual SEActiveHeating& GetActiveHeating();
  virtual void RemoveActiveHeating();

  virtual bool HasActiveCooling() const;
  virtual SEActiveCooling& GetActiveCooling();
  virtual void RemoveActiveCooling();

  virtual bool HasAppliedTemperature() const;
  virtual SEAppliedTemperature& GetAppliedTemperature();
  virtual void RemoveAppliedTemperature();

  virtual void ToString(std::ostream& str) const override;

protected:
  bool m_ClearContents;
  SEActiveHeating* m_ActiveHeating;
  SEActiveCooling* m_ActiveCooling;
  SEAppliedTemperature* m_AppliedTemperature;
};
}