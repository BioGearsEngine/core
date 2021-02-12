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

#include <biogears/cdm/engine/PhysiologyEngineStabilization.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/cdm/EngineConfiguration.hxx>

CDM_BIND_DECL(PhysiologyEngineTimedConditionStabilizationData)

namespace biogears {
namespace io {
  class EngineConfiguration;
}

class BIOGEARS_API PhysiologyEngineTimedStabilizationCriteria : public Loggable {
public:
  friend io::EngineConfiguration;
  PhysiologyEngineTimedStabilizationCriteria(Logger* logger);
  virtual ~PhysiologyEngineTimedStabilizationCriteria();

  virtual void Clear();

  virtual bool Load(const CDM::PhysiologyEngineTimedConditionStabilizationData& in);
  virtual CDM::PhysiologyEngineTimedConditionStabilizationData* Unload() const;

protected:
  virtual void Unload(CDM::PhysiologyEngineTimedConditionStabilizationData& data) const;

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual bool HasTime();
  virtual SEScalarTime& GetTime();
  virtual const SEScalarTime& GetTime() const;

protected:
  std::string m_Name;
  SEScalarTime m_Time;
};

CDM_BIND_DECL(PhysiologyEngineTimedStabilizationData)
class  PhysiologyEngineTimedStabilization : public PhysiologyEngineStabilization {
public:
  friend io::EngineConfiguration;
  BIOGEARS_API PhysiologyEngineTimedStabilization(Logger* logger);
  BIOGEARS_API virtual ~PhysiologyEngineTimedStabilization();

  BIOGEARS_API virtual void Clear() override;

  BIOGEARS_API virtual bool Load(const CDM::PhysiologyEngineTimedStabilizationData& in);
  BIOGEARS_API virtual CDM::PhysiologyEngineTimedStabilizationData* Unload() const override;

protected:
  BIOGEARS_API virtual void Unload(CDM::PhysiologyEngineTimedStabilizationData& data) const;

public:
  BIOGEARS_API virtual bool Load(const char* file) override;
  BIOGEARS_API virtual bool Load(const std::string& file) override;

  BIOGEARS_API virtual bool StabilizeRestingState(PhysiologyEngine& engine) override;
  BIOGEARS_API virtual bool StabilizeFeedbackState(PhysiologyEngine& engine) override;
  BIOGEARS_API virtual bool StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions) override;

  BIOGEARS_API virtual SEScalarTime& GetRestingStabilizationTime();
  BIOGEARS_API virtual double GetRestingStabilizationTime(const TimeUnit& unit) const;

  BIOGEARS_API virtual bool HasFeedbackStabilizationTime() const;
  BIOGEARS_API virtual SEScalarTime& GetFeedbackStabilizationTime();
  BIOGEARS_API virtual double GetFeedbackStabilizationTime(const TimeUnit& unit) const;

  BIOGEARS_API virtual bool HasConditionCriteria(const  char* name) const;
  BIOGEARS_API virtual bool HasConditionCriteria(const std::string& name) const;
  BIOGEARS_API virtual void RemoveConditionCriteria(const char* name);
  BIOGEARS_API virtual void RemoveConditionCriteria(const std::string& name);
  BIOGEARS_API virtual const std::vector<PhysiologyEngineTimedStabilizationCriteria*>& GetConditionCriteria() const;
  BIOGEARS_API virtual PhysiologyEngineTimedStabilizationCriteria& GetConditionCriteria(const char* name);
  BIOGEARS_API virtual PhysiologyEngineTimedStabilizationCriteria& GetConditionCriteria(const std::string& name);
  BIOGEARS_API virtual PhysiologyEngineTimedStabilizationCriteria* GetConditionCriteria(const char* name) const;
  BIOGEARS_API virtual PhysiologyEngineTimedStabilizationCriteria* GetConditionCriteria(const std::string& name) const;

protected:
  BIOGEARS_API virtual bool Stabilize(PhysiologyEngine& engine, const SEScalarTime& time);

  SEScalarTime m_RestingStabilizationTime;
  SEScalarTime* m_FeedbackStabilizationTime;
  std::vector<PhysiologyEngineTimedStabilizationCriteria*> m_ConditionCriteria;
};
}
