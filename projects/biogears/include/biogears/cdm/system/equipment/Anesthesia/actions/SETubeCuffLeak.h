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
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
#include <biogears/schema/cdm/AnesthesiaActions.hxx>

class SEScalar0To1;

class BIOGEARS_API SETubeCuffLeak : public SEAnesthesiaMachineAction {
public:
  SETubeCuffLeak();
  virtual ~SETubeCuffLeak();

  virtual void Clear();

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::TubeCuffLeakData& in);
  virtual CDM::TubeCuffLeakData* Unload() const;

protected:
  virtual void Unload(CDM::TubeCuffLeakData& data) const;

public:
  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalar0To1* m_Severity;
};