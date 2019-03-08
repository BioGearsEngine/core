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

#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>
#include <biogears/schema/cdm/InhalerActions.hxx>

namespace biogears {
class SEInhaler;
class SESubstanceManager;

class BIOGEARS_API SEInhalerConfiguration : public SEInhalerAction {
public:
  SEInhalerConfiguration(SESubstanceManager& substances);
  virtual ~SEInhalerConfiguration() override;
  
  static constexpr const char* TypeTag() { return "SEInhalerConfiguration"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override;

  virtual bool IsValid() const override;

  virtual bool Load(const CDM::InhalerConfigurationData& in);
  virtual CDM::InhalerConfigurationData* Unload() const override;

protected:
  virtual void Unload(CDM::InhalerConfigurationData& data) const;

public:
  bool HasConfiguration() const;
  SEInhaler& GetConfiguration();
  const SEInhaler* GetConfiguration() const;

  virtual const char* GetConfigurationFile_cStr() const;
  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const char* fileName);
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;
  virtual void InvalidateConfigurationFile();

  virtual void ToString(std::ostream& str) const override;

protected:
  SESubstanceManager& m_Substances;

  std::string m_ConfigurationFile;
  SEInhaler* m_Configuration;
};
}