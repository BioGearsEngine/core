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
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
class TimeUnit;
namespace io {
  class Scenario;
}
class BIOGEARS_API SEAdvanceTime : public SEAction {
  friend io::Scenario;

public:
  SEAdvanceTime();
  virtual ~SEAdvanceTime() override;

  static constexpr const char* TypeTag() { return "SEAdvanceTime"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;

  virtual bool Load(const CDM::AdvanceTimeData& in);
  virtual CDM::AdvanceTimeData* Unload() const;

protected:
  virtual void Unload(CDM::AdvanceTimeData& data) const;

public:
  virtual void ToString(std::ostream& str) const override;

  virtual bool HasTime() const;
  virtual SEScalarTime& GetTime();
  virtual double GetTime(const TimeUnit& unit) const;

protected:
  SEScalarTime* m_Time;
};
}