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
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalarVolumePerTime;
class VolumePerTimeUnit;

class BIOGEARS_API SEGastrointestinalSystem : public SESystem {
public:
  SEGastrointestinalSystem(Logger* logger);
  virtual ~SEGastrointestinalSystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasChymeAbsorptionRate() const;
  virtual SEScalarVolumePerTime& GetChymeAbsorptionRate();
  virtual double GetChymeAbsorptionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasStomachContents() const;
  virtual SENutrition& GetStomachContents();
  virtual const SENutrition* GetStomachContents() const;
  virtual void RemoveStomachContents();

  virtual bool Load(const CDM::GastrointestinalSystemData& in);
  virtual CDM::GastrointestinalSystemData* Unload() const;

protected:
  virtual void Unload(CDM::GastrointestinalSystemData& data) const;

protected:
  SEScalarVolumePerTime* m_ChymeAbsorptionRate;
  SENutrition* m_StomachContents;
};
}