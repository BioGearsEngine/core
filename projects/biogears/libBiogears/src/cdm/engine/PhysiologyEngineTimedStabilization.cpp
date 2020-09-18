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
#include <biogears/cdm/engine/PhysiologyEngineTimedStabilization.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/utils/TimingProfile.h>

namespace biogears {
bool PhysiologyEngineTimedStabilization::StabilizeRestingState(PhysiologyEngine& engine)
{
  if (!GetRestingStabilizationTime().IsValid())
    return true; //No stabilization time requested
  return Stabilize(engine, GetRestingStabilizationTime());
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::StabilizeFeedbackState(PhysiologyEngine& engine)
{
  if (!HasFeedbackStabilizationTime())
    return true; //No stabilization time requested
  return Stabilize(engine, GetFeedbackStabilizationTime());
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions)
{
  if (conditions.empty())
    return true;
  double cTime_s;
  double maxTime_s = 0;
  for (const SECondition* c : conditions) {
    if (!HasConditionCriteria(c->GetName())) {
      Error("Engine does not support Condition");
      return false;
    }
    const PhysiologyEngineTimedStabilizationCriteria& sc = GetConditionCriteria(c->GetName());
    cTime_s = sc.GetTime().GetValue(TimeUnit::s);
    if (cTime_s > maxTime_s)
      maxTime_s = cTime_s;
  }
  SEScalarTime time;
  time.SetValue(maxTime_s, TimeUnit::s);
  return Stabilize(engine, time);
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::Stabilize(PhysiologyEngine& engine, const SEScalarTime& time)
{
  double sTime_s = time.GetValue(TimeUnit::s);
  if (sTime_s == 0)
    return true; //No stabilization time requested

  m_Cancelled = false;
  std::stringstream ss;
  TimingProfile profiler;
  Info("Initializing BioGears : 0%");
  if (m_LogProgress) {
    profiler.Start("Total");
    profiler.Start("Status");
  }
 
  ss.precision(3);
  double statusTime_s = 0; // Current time of this status cycle
  double statusStep_s = 50; //How long did it take to simulate this much time
  double dT_s = engine.GetTimeStep(TimeUnit::s);
  int count = (int)(sTime_s / dT_s);
  int ProgressStep = (int)(count * .1);
  int Progress = ProgressStep;

  for (int i = 0; i <= count; i++) {
    if (m_Cancelled)
      break;
    // Instead of calling AdvanceModelTime
    // We should have a method called AdvanceToRestingState
    // and it will advance time, AND check to see if it is at a Resting state or not
    // if it is we can break our loop. This will allow us to record our stabilization data
    engine.AdvanceModelTime();

    m_currentTime_s += dT_s;
    if (m_LogProgress) {
      statusTime_s += dT_s;
      if (statusTime_s > statusStep_s) {
        statusTime_s = 0;
        ss << "Current Time is " << m_currentTime_s << "s, it took "
           << profiler.GetElapsedTime_s("Status") << "s to simulate the past "
           << statusStep_s << "s" << std::flush;
        profiler.Reset("Status");
        Info(ss);
      }
      if (i > Progress) {
        Progress += ProgressStep;
        ss << "Initializing BioGears : " << (double)i / count * 100 << "% Complete";
        Info(ss);
      }
    }
  }
  if (m_LogProgress) {
    ss << "It took " << profiler.GetElapsedTime_s("Total") << "s to get engine to stable state";
    Info(ss);
  }
  // Save off how long it took us to stabilize
  GetStabilizationDuration().SetValue(m_currentTime_s, TimeUnit::s);
  return true;
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilization::PhysiologyEngineTimedStabilization(Logger* logger)
  : PhysiologyEngineStabilization(logger)
{
  m_FeedbackStabilizationTime = nullptr;
  GetStabilizationDuration().SetValue(0, TimeUnit::s);
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilization::~PhysiologyEngineTimedStabilization()
{
  Clear();
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilization::Clear()
{
  PhysiologyEngineStabilization::Clear();
  m_RestingStabilizationTime.Invalidate();
  SAFE_DELETE(m_FeedbackStabilizationTime);
  DELETE_VECTOR(m_ConditionCriteria);
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::Load(const CDM::PhysiologyEngineTimedStabilizationData& in)
{
  PhysiologyEngineStabilization::Load(in);
  GetRestingStabilizationTime().Load(in.RestingStabilizationTime());
  if (in.FeedbackStabilizationTime().present())
    GetFeedbackStabilizationTime().Load(in.FeedbackStabilizationTime().get());
  for (auto cc : in.ConditionStabilization()) {
    PhysiologyEngineTimedStabilizationCriteria* sc = new PhysiologyEngineTimedStabilizationCriteria(nullptr);
    sc->Load(cc);
    m_ConditionCriteria.push_back(sc);
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::PhysiologyEngineTimedStabilizationData* PhysiologyEngineTimedStabilization::Unload() const
{
  CDM::PhysiologyEngineTimedStabilizationData* data(new CDM::PhysiologyEngineTimedStabilizationData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilization::Unload(CDM::PhysiologyEngineTimedStabilizationData& data) const
{
  PhysiologyEngineStabilization::Unload(data);
  data.RestingStabilizationTime(std::unique_ptr<CDM::ScalarTimeData>(m_RestingStabilizationTime.Unload()));
  if (HasFeedbackStabilizationTime())
    data.FeedbackStabilizationTime(std::unique_ptr<CDM::ScalarTimeData>(m_FeedbackStabilizationTime->Unload()));
  for (auto cc : m_ConditionCriteria) {
    data.ConditionStabilization().push_back(std::unique_ptr<CDM::PhysiologyEngineTimedConditionStabilizationData>(cc->Unload()));
  }
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::Load(const char* file)
{
  return Load(std::string{ file });
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::Load(const std::string& file)
{
  CDM::PhysiologyEngineTimedStabilizationData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  data = Serializer::ReadFile(file, GetLogger());
  pData = dynamic_cast<CDM::PhysiologyEngineTimedStabilizationData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Unable to read stabilization file : " << file << std::endl;
    Info(ss);
    return false;
  }
  return Load(*pData);
}
//-------------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineTimedStabilization::GetRestingStabilizationTime()
{
  return m_RestingStabilizationTime;
}
//-------------------------------------------------------------------------------
double PhysiologyEngineTimedStabilization::GetRestingStabilizationTime(const TimeUnit& unit) const
{
  return m_RestingStabilizationTime.GetValue(unit);
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::HasFeedbackStabilizationTime() const
{
  return m_FeedbackStabilizationTime == nullptr ? false : m_FeedbackStabilizationTime->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineTimedStabilization::GetFeedbackStabilizationTime()
{
  if (m_FeedbackStabilizationTime == nullptr)
    m_FeedbackStabilizationTime = new SEScalarTime();
  return *m_FeedbackStabilizationTime;
}
//-------------------------------------------------------------------------------
double PhysiologyEngineTimedStabilization::GetFeedbackStabilizationTime(const TimeUnit& unit) const
{
  if (!HasFeedbackStabilizationTime())
    return SEScalar::dNaN();
  return m_FeedbackStabilizationTime->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::HasConditionCriteria(const char* name) const
{
  return HasConditionCriteria(std::string{ name });
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilization::HasConditionCriteria(const std::string& name) const
{
  for (PhysiologyEngineTimedStabilizationCriteria* c : m_ConditionCriteria) {
    if (c->GetName() == name)
      return true;
  }
  return false;
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilization::RemoveConditionCriteria(const char* name)
{
  return RemoveConditionCriteria(std::string{ name });
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilization::RemoveConditionCriteria(const std::string& name)
{
  for (auto itr = m_ConditionCriteria.begin(); itr != m_ConditionCriteria.end(); itr++) {
    const PhysiologyEngineTimedStabilizationCriteria* c = *itr;
    if (c->GetName() == name) {
      m_ConditionCriteria.erase(itr);
      SAFE_DELETE(c);
      return;
    }
  }
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilizationCriteria& PhysiologyEngineTimedStabilization::GetConditionCriteria(const char* name)
{
  return GetConditionCriteria(std::string{ name });
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilizationCriteria& PhysiologyEngineTimedStabilization::GetConditionCriteria(const std::string& name)
{
  for (auto itr = m_ConditionCriteria.begin(); itr != m_ConditionCriteria.end(); itr++) {
    if ((*itr)->GetName() == name)
      return *(*itr);
  }
  PhysiologyEngineTimedStabilizationCriteria* criteria = new PhysiologyEngineTimedStabilizationCriteria(GetLogger());
  criteria->SetName(name);
  m_ConditionCriteria.push_back(criteria);
  return *criteria;
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilizationCriteria* PhysiologyEngineTimedStabilization::GetConditionCriteria(const char* name) const
{
  return GetConditionCriteria(std::string{ name });
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilizationCriteria* PhysiologyEngineTimedStabilization::GetConditionCriteria(const std::string& name) const
{
  for (auto itr = m_ConditionCriteria.begin(); itr != m_ConditionCriteria.end(); itr++) {
    if ((*itr)->GetName() == name)
      return *itr;
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
const std::vector<PhysiologyEngineTimedStabilizationCriteria*>& PhysiologyEngineTimedStabilization::GetConditionCriteria() const
{
  return m_ConditionCriteria;
}
//-------------------------------------------------------------------------------
//////////////////////////////////////////////////
// PhysiologyEngineTimedStabilizationCriteria //
//////////////////////////////////////////////////

PhysiologyEngineTimedStabilizationCriteria::PhysiologyEngineTimedStabilizationCriteria(Logger* logger)
  : Loggable(logger)
{
  Clear();
}
//-------------------------------------------------------------------------------
PhysiologyEngineTimedStabilizationCriteria::~PhysiologyEngineTimedStabilizationCriteria()
{
  Clear();
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilizationCriteria::Clear()
{
  InvalidateName();
  m_Time.Invalidate();
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilizationCriteria::Load(const CDM::PhysiologyEngineTimedConditionStabilizationData& in)
{
  Clear();
  SetName(in.Name());
  GetTime().Load(in.Time());
  return true;
}
//-------------------------------------------------------------------------------
CDM::PhysiologyEngineTimedConditionStabilizationData* PhysiologyEngineTimedStabilizationCriteria::Unload() const
{
  CDM::PhysiologyEngineTimedConditionStabilizationData* data(new CDM::PhysiologyEngineTimedConditionStabilizationData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilizationCriteria::Unload(CDM::PhysiologyEngineTimedConditionStabilizationData& data) const
{
  data.Name(m_Name);
  data.Time(std::unique_ptr<CDM::ScalarTimeData>(GetTime().Unload()));
}
//-------------------------------------------------------------------------------
std::string PhysiologyEngineTimedStabilizationCriteria::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------
const char* PhysiologyEngineTimedStabilizationCriteria::GetName_cStr() const
{
  return m_Name.c_str();
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilizationCriteria::SetName(const char* name)
{
  m_Name = name;
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilizationCriteria::SetName(const std::string& name)
{
  m_Name = name;
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilizationCriteria::HasName() const
{
  return  m_Name.empty() ? false : true;
}
//-------------------------------------------------------------------------------
void PhysiologyEngineTimedStabilizationCriteria::InvalidateName()
{
  m_Name = "";
}
//-------------------------------------------------------------------------------
bool PhysiologyEngineTimedStabilizationCriteria::HasTime()
{
  return m_Time.IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineTimedStabilizationCriteria::GetTime()
{
  return m_Time;
}
//-------------------------------------------------------------------------------
const SEScalarTime& PhysiologyEngineTimedStabilizationCriteria::GetTime() const
{
  return m_Time;
}
//-------------------------------------------------------------------------------

}