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
#include <string>

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/schema/cdm/DataRequests.hxx>
#include <biogears/schema/cdm/Actions.hxx>
#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/BiogearsEnums.h>

namespace biogears {
namespace io {
  class Actions;
}
class BIOGEARS_API SESerializeState : public SEAction {
  friend io::Actions;

public:
  SESerializeState();
  virtual ~SESerializeState() override;

  static constexpr const char* TypeTag() { return "SESerializeState"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;

  virtual bool Load(const CDM::SerializeStateData& in, std::default_random_engine *rd = nullptr);
  virtual CDM::SerializeStateData* Unload() const override;

  bool operator==(SESerializeState const&) const;
  bool operator!=(SESerializeState const&) const;

protected:
  virtual void Unload(CDM::SerializeStateData& data) const;

public:
  virtual void ToString(std::ostream& str) const override;

  virtual SESerializationType GetType() const;
  virtual void SetType(SESerializationType t);
  virtual bool HasType() const;
  virtual void InvalidateType();

  virtual bool HasFilename() const;
  virtual std::string GetFilename() const;
  virtual const char* GetFilename_cStr() const;
  virtual void SetFilename(const char* filename);
  virtual void SetFilename(const std::string& filename);
  virtual void InvalidateFilename();

protected:
  std::string m_Filename;
  SESerializationType m_Type;
};
}