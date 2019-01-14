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

#include <biogears/exports.h>
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
#include <biogears/schema/cdm/PatientConditions.hxx>

namespace biogears {
class SEScalarVolume;

class BIOGEARS_API SEChronicPericardialEffusion : public SEPatientCondition {
public:
  SEChronicPericardialEffusion();
  virtual ~SEChronicPericardialEffusion();

  virtual void Clear();

  virtual bool IsValid() const;

  virtual bool Load(const CDM::ChronicPericardialEffusionData& in);
  virtual CDM::ChronicPericardialEffusionData* Unload() const;

protected:
  virtual void Unload(CDM::ChronicPericardialEffusionData& data) const;

public:
  virtual std::string GetName() const { return "ChronicPericardialEffusion"; }
  virtual const char* GetName_cStr() const { return "ChronicPericardialEffusion"; }

  virtual bool HasAccumulatedVolume() const;
  virtual SEScalarVolume& GetAccumulatedVolume();

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalarVolume* m_AccumulatedVolume;
};
}