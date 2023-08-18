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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>

#include <biogears/schema/cdm/Properties.hxx>

CDM_BIND_DECL(PhysiologyEngineStabilizationData)

namespace biogears {
class PhysiologyEngine;
class PhysiologyEngineTrack;
class PhysiologyEngineConfiguration;
class SECondition;

class  PhysiologyEngineStabilization : public Loggable {
public:
  BIOGEARS_API PhysiologyEngineStabilization(Logger* logger);
  BIOGEARS_API virtual ~PhysiologyEngineStabilization();

  BIOGEARS_API virtual void Clear();

  BIOGEARS_API virtual bool Load(const CDM::PhysiologyEngineStabilizationData& in);
  BIOGEARS_API virtual CDM::PhysiologyEngineStabilizationData* Unload() const;

  BIOGEARS_API bool operator==(PhysiologyEngineStabilization const&) const;
  BIOGEARS_API bool operator!=(PhysiologyEngineStabilization const&) const;

protected:
  BIOGEARS_API virtual void Unload(CDM::PhysiologyEngineStabilizationData& data) const;

public:
  BIOGEARS_API virtual bool Load(const char* file) = 0;
  BIOGEARS_API virtual bool Load(const std::string& file) = 0;

  BIOGEARS_API virtual bool StabilizeRestingState(PhysiologyEngine& engine) = 0;
  BIOGEARS_API virtual bool StabilizeFeedbackState(PhysiologyEngine& engine) = 0;
  BIOGEARS_API virtual bool StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions) = 0;

  BIOGEARS_API virtual void LogProgress(bool b);

  BIOGEARS_API virtual void CancelStabilization();

  BIOGEARS_API virtual bool HasCurrentTime() const;
  BIOGEARS_API virtual SEScalarTime& GetCurrentTime();

  BIOGEARS_API virtual bool HasStabilizationDuration() const;
  BIOGEARS_API virtual SEScalarTime& GetStabilizationDuration();

protected:
  bool m_Canceled;
  bool m_LogProgress;
  SEScalarTime* m_currentTime;
  std::stringstream m_ss;

  SEScalarTime* m_StabilizationDuration;
};
}