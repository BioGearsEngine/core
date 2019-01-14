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
class BIOGEARS_API PhysiologyEngineTimedStabilizationCriteria : public Loggable {
public:
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
class BIOGEARS_API PhysiologyEngineTimedStabilization : public PhysiologyEngineStabilization {
public:
  PhysiologyEngineTimedStabilization(Logger* logger);
  virtual ~PhysiologyEngineTimedStabilization();

  virtual void Clear() override;

  virtual bool Load(const CDM::PhysiologyEngineTimedStabilizationData& in);
  virtual CDM::PhysiologyEngineTimedStabilizationData* Unload() const override;

protected:
  virtual void Unload(CDM::PhysiologyEngineTimedStabilizationData& data) const;

public:
  virtual bool Load(const char* file) override;
  virtual bool Load(const std::string& file) override;

  virtual bool StabilizeRestingState(PhysiologyEngine& engine) override;
  virtual bool StabilizeFeedbackState(PhysiologyEngine& engine) override;
  virtual bool StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions) override;

  virtual SEScalarTime& GetRestingStabilizationTime();
  virtual double GetRestingStabilizationTime(const TimeUnit& unit) const;

  virtual bool HasFeedbackStabilizationTime() const;
  virtual SEScalarTime& GetFeedbackStabilizationTime();
  virtual double GetFeedbackStabilizationTime(const TimeUnit& unit) const;

  virtual bool HasConditionCriteria(const  char* name) const;
  virtual bool HasConditionCriteria(const std::string& name) const;
  virtual void RemoveConditionCriteria(const char* name);
  virtual void RemoveConditionCriteria(const std::string& name);
  virtual const std::vector<PhysiologyEngineTimedStabilizationCriteria*>& GetConditionCriteria() const;
  virtual PhysiologyEngineTimedStabilizationCriteria& GetConditionCriteria(const char* name);
  virtual PhysiologyEngineTimedStabilizationCriteria& GetConditionCriteria(const std::string& name);
  virtual PhysiologyEngineTimedStabilizationCriteria* GetConditionCriteria(const char* name) const;
  virtual PhysiologyEngineTimedStabilizationCriteria* GetConditionCriteria(const std::string& name) const;

protected:
  virtual bool Stabilize(PhysiologyEngine& engine, const SEScalarTime& time);

  SEScalarTime m_RestingStabilizationTime;
  SEScalarTime* m_FeedbackStabilizationTime;
  std::vector<PhysiologyEngineTimedStabilizationCriteria*> m_ConditionCriteria;
};
}