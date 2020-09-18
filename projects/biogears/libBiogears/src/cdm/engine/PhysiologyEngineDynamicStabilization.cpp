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
#include <biogears/cdm/engine/PhysiologyEngineDynamicStabilization.h>

//Standad Includes
#include <cmath>
//Project Includes
#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


namespace biogears {
bool PhysiologyEngineDynamicStabilization::StabilizeRestingState(PhysiologyEngine& engine)
{
  Info("Converging to a steady state");
  return Stabilize(engine, GetRestingCriteria());
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::StabilizeFeedbackState(PhysiologyEngine& engine)
{
  if (!HasFeedbackCriteria())
    return true;
  Info("Converging feedback to a steady state");
  return Stabilize(engine, GetFeedbackCriteria());
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions)
{
  if (conditions.empty())
    return true;
  // Grab the criteria based on the conditions we have
  PhysiologyEngineDynamicStabilizationCriteria* sc;
  m_ActiveConditions.clear();
  for (auto c : conditions) {
    sc = GetConditionCriteria(c->GetName());
    if (sc == nullptr) {
      Error("Engine does not support Condition");
      return false;
    } else
      m_ActiveConditions.push_back(sc);
  }
  if (m_ActiveConditions.size() == 1) {
    Info("Converging provided condition to a steady state");
    return Stabilize(engine, *m_ActiveConditions[0]);
  } else {
    if (!Merge()) {
      Error("Unable to merge conditions");
      return false;
    }
    Info("Converging provided conditions to a steady state");
    return Stabilize(engine, m_MergedConditions);
  }
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::Stabilize(PhysiologyEngine& engine, const PhysiologyEngineDynamicStabilizationCriteria& criteria)
{
  const std::vector<PropertyConvergence*>& properties = criteria.GetPropertyConvergence();
  if (properties.empty())
    return true; //nothing to do here...

  m_Cancelled = false;
  std::stringstream ss;
  TimingProfile profiler;
  if (m_LogProgress) {
    profiler.Start("Total");
    profiler.Start("Status");
  }
  // Execute System initialization time
  PhysiologyEngineTrack* tracker = engine.GetEngineTrack();
  bool hasOptionalProperties = false;

  // Grab all the convergence properties
  for (PropertyConvergence* pc : properties) {
    if (pc->IsOptional()) {
      hasOptionalProperties = true;
    }
    if (tracker) {
      tracker->ConnectRequest(pc->GetDataRequest(), pc->GetDataRequestScalar());
    }
    if (!pc->GetDataRequestScalar().HasScalar()) {
      ss << "Cannot find convergence property " << pc->GetDataRequest().GetName();
      throw CommonDataModelException(ss.str());
    }
  }

  ss.precision(3);
  double statusTime_s = 0; // Current time of this status cycle
  double statusStep_s = 10; //How long did it take to simulate this much time
  double stablizationTime_s = 0;
  double dT_s = engine.GetTimeStep(TimeUnit::s);

  PhysiologyEngineDynamicStabilizer stabilizer(dT_s, criteria);
  while (!(stabilizer.HasConverged() && stabilizer.HasConvergedOptional())) {
    if (m_Cancelled)
      break;
    if (stabilizer.HasExceededTime())
      break;

    engine.AdvanceModelTime();
    stablizationTime_s += dT_s;
    m_currentTime_s += dT_s;
    if (m_LogProgress) {
      statusTime_s += dT_s;
      if (statusTime_s > statusStep_s) {
        statusTime_s = 0;
        ss << "Converging System... it took "
           << profiler.GetElapsedTime_s("Status") << "s to simulate the past "
           << statusStep_s << "s" << std::flush;
        profiler.Reset("Status");
        Info(ss);
      }
    }
    stabilizer.Converge();
  }
  double covTime_s = criteria.GetConvergenceTime(TimeUnit::s);
  if (stabilizer.HasExceededTime()) {
    Error("Could not converge to provided criteria");
    for (PropertyConvergence* pc : properties) {
      if (stablizationTime_s - pc->GetLastErrorTime_s() < covTime_s) {
        m_ss << stablizationTime_s << "s - " << pc->GetDataRequest().GetName() << " is not converging, last error time was " << pc->GetLastErrorTime_s() << "s setting a target value of " << pc->GetCurrentTarget() << " and the current value is " << pc->GetDataRequestScalar().GetValue();
        Error(m_ss);
      }
      /* else
      {
        m_ss << stablizationTime_s << "s - " << pc->GetDataRequest().GetName() << " is converging, last error time was " << pc->GetLastErrorTime_s() << "s";
        Info(m_ss);
      }*/
    }
    return false;
  } else if (m_LogProgress) {
    ss << "Convergence took " << profiler.GetElapsedTime_s("Total") << "s to simulate " << stablizationTime_s << "s to get engine to a steady state";
    Info(ss);
    if (hasOptionalProperties && !stabilizer.HasConvergedOptional()) {
      ss << "Could not converge optional properties";
      Warning(ss);
      for (PropertyConvergence* pc : properties) {
        if (stablizationTime_s - pc->GetLastErrorTime_s() < covTime_s) {
          m_ss << stablizationTime_s << "s - " << pc->GetDataRequest().GetName() << " is not converging, last error time was " << pc->GetLastErrorTime_s() << "s setting a target value of " << pc->GetCurrentTarget() << " and the current value is " << pc->GetDataRequestScalar().GetValue();
          Error(m_ss);
        }
      }
    }
  }
  // Save off how long it took us to stabilize
  GetStabilizationDuration().SetValue(stablizationTime_s, TimeUnit::s);
  return true;
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilizer::PhysiologyEngineDynamicStabilizer(double timeStep_s, const PhysiologyEngineDynamicStabilizationCriteria& criteria)
  : Loggable(criteria.GetLogger())
  , m_properties(criteria.GetPropertyConvergence())
{
  m_dT_s = timeStep_s;
  m_totTime_s = 0;
  m_passTime_s = 0;
  m_optsPassTime_s = 0;
  m_covTime_s = criteria.GetConvergenceTime(TimeUnit::s);
  m_minTime_s = criteria.GetMinimumReactionTime(TimeUnit::s);
  m_maxTime_s = criteria.GetMaximumAllowedStabilizationTime(TimeUnit::s) + m_minTime_s;
  // We will run for at least minTime, THEN to the max time, so maxTime_s is the sum of both

  m_converged = false;
  m_convergedOptional = false;
  m_exceededTime = false;
  m_hasOptionalProperties = false;
  for (PropertyConvergence* pc : m_properties) {
    if (pc->IsOptional()) {
      m_hasOptionalProperties = true;
      break;
    }
  }
  if (!m_hasOptionalProperties)
    m_convergedOptional = true;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilizer::Converge()
{
  bool passedTimeStep = false;
  bool passedTimeStepOptions = false;

  m_totTime_s += m_dT_s;
  if (m_totTime_s < m_minTime_s)
    return; // Wait for it

  std::stringstream ss;
  passedTimeStep = true;
  passedTimeStepOptions = true;
  for (PropertyConvergence* pc : m_properties) {
    bool b = pc->Test(m_totTime_s);
    if (pc->IsOptional()) {
      passedTimeStepOptions &= b;
    } else {
      passedTimeStep &= b;
    }
  }

  if (passedTimeStep)
    m_passTime_s += m_dT_s;
  else
    m_passTime_s = 0;
  if (passedTimeStepOptions)
    m_optsPassTime_s += m_dT_s;
  else
    m_optsPassTime_s = 0;

  if (!m_converged && m_passTime_s >= m_covTime_s) { // We have converged
    m_converged = true;
    if (m_hasOptionalProperties)
      Warning("We have passed required convergence criteria, attempting to converge optional properties from multiple conditions.");
  }
  if (m_converged && m_optsPassTime_s >= m_covTime_s) { // We have converged optional props
    m_convergedOptional = true;
    Warning("We have passed required and optional convergence criteria.");
  }

  if (m_totTime_s > m_maxTime_s) {
    m_converged = false;
    m_convergedOptional = false;
    m_exceededTime = true;
  }
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::Merge()
{
  // Get a std::hash<std::string>()("foo"); for each Property convergence Data Request
  // Don't forget to add hashes of compartments and substance names! if applicable
  // Put it in a map<hash#,vector<PropertyConvergence*>>
  // After this loop, go through the map and remove any
  // entries where the vector length is < activeConditions.size()
  // We only want convergence objects if ALL active conditions have them
  // From there find the PropertyConvergence with the largest %diff
  // Add that pointer to the m_MergedConditions (will need new friend method as that method should not be public)
  Info("Merging Conditions");
  m_MergedConditions.Clear();
  m_MergedConditions.SetName("MergedCondition"); // May want to include what conditions we are combining in the name?
  double time_s;
  double maxConv_s = 0;
  double maxMinStabilize_s = 0;
  double maxMaxStabilize_s = 0;
  const std::vector<PropertyConvergence*>* vCondPConv;
  std::map<std::string, std::vector<PropertyConvergence*>*> cMap;
  for (const PhysiologyEngineDynamicStabilizationCriteria* c : m_ActiveConditions) {
    m_ss << "Analyzing " << c->GetName();
    Info(m_ss);
    vCondPConv = &c->GetPropertyConvergence();
    for (PropertyConvergence* pConv : *vCondPConv) {
      auto cMapItr = cMap.find(pConv->GetDataRequest().GetName());
      if (cMapItr != cMap.end())
        cMapItr->second->push_back(pConv);
      else {
        std::vector<PropertyConvergence*>* vec = new std::vector<PropertyConvergence*>();
        vec->push_back(pConv);
        cMap[pConv->GetDataRequest().GetName()] = vec;
      }
    }

    time_s = c->GetConvergenceTime(TimeUnit::s);
    if (time_s > maxConv_s)
      maxConv_s = time_s;
    time_s = c->GetMinimumReactionTime(TimeUnit::s);
    if (time_s > maxMinStabilize_s)
      maxMinStabilize_s = time_s;
    time_s = c->GetMaximumAllowedStabilizationTime(TimeUnit::s);
    if (time_s > maxMaxStabilize_s)
      maxMaxStabilize_s = time_s;
  }
  double tmpError;
  PropertyConvergence* pConv;
  for (auto i = cMap.begin(); i != cMap.end(); i++) {
    // Let's find the Convergence that has the largest % difference
    double pErr = 0;
    for (auto j = i->second->begin(); j != i->second->end(); j++) {
      tmpError = (*j)->GetPercentError();
      if (tmpError > pErr) {
        pErr = tmpError;
        pConv = (*j);
      }
    }
    if (i->second->size() == m_ActiveConditions.size()) {
      m_ss << "Merged Convergance for property " << pConv->GetDataRequest().GetName() << " using " << pConv->GetPercentError() << "% error";
      Info(m_ss);
    } else {
      pConv->SetOptional(true);
      m_ss << "Not all conditions contain " << i->first << " in convergence criteria. Making convergence on this property optional.";
      Warning(m_ss);
    }
    m_MergedConditions.m_PropertyConvergence.push_back(pConv);
  }

  DELETE_MAP_SECOND(cMap); // Clean up our Map
  m_MergedConditions.GetConvergenceTime().SetValue(maxConv_s, TimeUnit::s);
  m_ss << "Merged Convergence Time : " << m_MergedConditions.GetConvergenceTime();
  Info(m_ss);
  m_MergedConditions.GetMinimumReactionTime().SetValue(maxMinStabilize_s, TimeUnit::s);
  m_ss << "Merged Minimum Reaction Time : " << m_MergedConditions.GetMinimumReactionTime();
  Info(m_ss);
  m_MergedConditions.GetMaximumAllowedStabilizationTime().SetValue(maxMaxStabilize_s, TimeUnit::s);
  m_ss << "Merged Maximum Allowed Stabilization Time : " << m_MergedConditions.GetMaximumAllowedStabilizationTime();
  Info(m_ss);
  return true;
}
//-----------------------------------------------------------------------------
// This basically tests the current property with the target property and if they are in a window of acceptance
bool PropertyConvergence::Test(double time_s)
{
  double v = !m_DataRequestScalar.HasUnit() ? m_DataRequestScalar.GetValue() : m_DataRequestScalar.GetValue(m_DataRequestScalar.GetUnit()->GetString());
  if (std::isnan(m_Target)) {
    m_Target = v;
    return false; // Initially target will be NaN until it is pulled for the first time
  }
  m_LastError = GeneralMath::PercentTolerance(m_Target, v);
  if (m_LastError < m_Error) {
    return true;
  }
  m_ss << time_s << "s - Resetting the target for " << GetDataRequest().GetName() << " to " << v << ", the last target value was " << m_Target;
  Info(m_ss);
  // Not converging, so this is now our target property
  m_Target = v;
  m_LastErrorTime_s = time_s;
  return false;
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilization::PhysiologyEngineDynamicStabilization(Logger* logger)
  : PhysiologyEngineStabilization(logger)
  , m_RestingCriteria(logger)
  , m_MergedConditions(logger)
{
  m_FeedbackCriteria = nullptr;
  m_RestingCriteria.SetName("Resting");
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilization::~PhysiologyEngineDynamicStabilization()
{
  Clear();
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilization::Clear()
{
  PhysiologyEngineStabilization::Clear();
  m_MergedConditions.m_PropertyConvergence.clear(); // \todo Make copies of stabilization criteria
  m_MergedConditions.Clear();
  m_ActiveConditions.clear();
  m_RestingCriteria.Clear();
  SAFE_DELETE(m_FeedbackCriteria);
  DELETE_VECTOR(m_ConditionCriteria);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::Load(const CDM::PhysiologyEngineDynamicStabilizationData& in)
{
  PhysiologyEngineStabilization::Load(in);
  GetRestingCriteria().Load(in.RestingStabilizationCriteria());
  if (in.FeedbackStabilizationCriteria().present())
    GetFeedbackCriteria().Load(in.FeedbackStabilizationCriteria().get());
  for (auto cData : in.ConditionStabilization()) {
    PhysiologyEngineDynamicStabilizationCriteria* c = new PhysiologyEngineDynamicStabilizationCriteria(GetLogger());
    c->Load(cData.Criteria());
    c->SetName(cData.Name());
    AddConditionCriteria(*c);
  }
  return true;
}
//-----------------------------------------------------------------------------
CDM::PhysiologyEngineDynamicStabilizationData* PhysiologyEngineDynamicStabilization::Unload() const
{
  CDM::PhysiologyEngineDynamicStabilizationData* data(new CDM::PhysiologyEngineDynamicStabilizationData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilization::Unload(CDM::PhysiologyEngineDynamicStabilizationData& data) const
{
  PhysiologyEngineStabilization::Unload(data);
  data.RestingStabilizationCriteria(std::unique_ptr<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>(GetRestingCriteria().Unload()));
  if (HasFeedbackCriteria())
    data.FeedbackStabilizationCriteria(std::unique_ptr<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>(GetFeedbackCriteria()->Unload()));
  for (auto& c : m_ConditionCriteria) {
    std::unique_ptr<CDM::PhysiologyEngineDynamicConditionStabilizationData> csData(new CDM::PhysiologyEngineDynamicConditionStabilizationData());
    csData->Criteria(std::unique_ptr<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>(c->Unload()));
    csData->Name(c->GetName());
    data.ConditionStabilization().push_back(*csData);
  }
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::Load(const char* file)
{
  return Load(std::string { file });
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::Load(const std::string& file)
{
  CDM::PhysiologyEngineDynamicStabilizationData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  data = Serializer::ReadFile(file, GetLogger());
  pData = dynamic_cast<CDM::PhysiologyEngineDynamicStabilizationData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Unable to read stabilization file : " << file << std::endl;
    Info(ss);
    return false;
  }
  return Load(*pData);
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilizationCriteria& PhysiologyEngineDynamicStabilization::GetRestingCriteria()
{
  return m_RestingCriteria;
}
//-----------------------------------------------------------------------------
const PhysiologyEngineDynamicStabilizationCriteria& PhysiologyEngineDynamicStabilization::GetRestingCriteria() const
{
  return m_RestingCriteria;
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilization::HasFeedbackCriteria() const
{
  return m_FeedbackCriteria != nullptr;
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilizationCriteria& PhysiologyEngineDynamicStabilization::GetFeedbackCriteria()
{
  if (m_FeedbackCriteria == nullptr) {
    m_FeedbackCriteria = new PhysiologyEngineDynamicStabilizationCriteria(GetLogger());
    m_FeedbackCriteria->SetName("Feedback");
  }
  return *m_FeedbackCriteria;
}
//-----------------------------------------------------------------------------
const PhysiologyEngineDynamicStabilizationCriteria* PhysiologyEngineDynamicStabilization::GetFeedbackCriteria() const
{
  return m_FeedbackCriteria == nullptr ? nullptr : m_FeedbackCriteria;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilization::RemoveConditionCriteria(const char* name)
{
  RemoveConditionCriteria(std::string { name });
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilization::RemoveConditionCriteria(const std::string& name)
{
  for (auto itr = m_ConditionCriteria.begin(); itr != m_ConditionCriteria.end(); itr++) {
    if ((*itr)->GetName() == name) {
      m_ConditionCriteria.erase(itr);
      return;
    }
  }
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilizationCriteria* PhysiologyEngineDynamicStabilization::GetConditionCriteria(const char* name) const
{
  return GetConditionCriteria(std::string { name });
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilizationCriteria* PhysiologyEngineDynamicStabilization::GetConditionCriteria(const std::string& name) const
{
  for (auto c : m_ConditionCriteria) {
    if (c->GetName() == name)
      return c;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilization::AddConditionCriteria(PhysiologyEngineDynamicStabilizationCriteria& criteria)
{
  for (auto c : m_ConditionCriteria) {
    if (c->GetName() == criteria.GetName())
      return;
  }
  m_ConditionCriteria.push_back(&criteria);
}
//-----------------------------------------------------------------------------
const std::vector<PhysiologyEngineDynamicStabilizationCriteria*>& PhysiologyEngineDynamicStabilization::GetConditionCriteria() const
{
  return m_ConditionCriteria;
}
//-----------------------------------------------------------------------------
//////////////////////////////////////////////////
// PhysiologyEngineDynamicStabilizationCriteria //
//////////////////////////////////////////////////

PhysiologyEngineDynamicStabilizationCriteria::PhysiologyEngineDynamicStabilizationCriteria(Logger* logger)
  : Loggable(logger)
  , m_DataRequestMgr(logger)
{
  m_ConvergenceTime = nullptr;
  m_MinimumReactionTime = nullptr;
  m_MaximumAllowedStabilizationTime = nullptr;
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilizationCriteria::~PhysiologyEngineDynamicStabilizationCriteria()
{
  Clear();
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilizationCriteria::Clear()
{
  SAFE_DELETE(m_ConvergenceTime);
  SAFE_DELETE(m_MinimumReactionTime);
  SAFE_DELETE(m_MaximumAllowedStabilizationTime);
  DELETE_VECTOR(m_PropertyConvergence);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilizationCriteria::Load(const CDM::PhysiologyEngineDynamicStabilizationCriteriaData& in)
{
  Clear();
  GetConvergenceTime().Load(in.ConvergenceTime());
  GetMinimumReactionTime().Load(in.MinimumReactionTime());
  GetMaximumAllowedStabilizationTime().Load(in.MaximumAllowedStabilizationTime());
  for (auto pcData : in.PropertyConvergence())
    CreateSystemPropertyConvergence(pcData.PercentDifference(), pcData.Name());
  return true;
}
//-----------------------------------------------------------------------------
CDM::PhysiologyEngineDynamicStabilizationCriteriaData* PhysiologyEngineDynamicStabilizationCriteria::Unload() const
{
  CDM::PhysiologyEngineDynamicStabilizationCriteriaData* data(new CDM::PhysiologyEngineDynamicStabilizationCriteriaData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilizationCriteria::Unload(CDM::PhysiologyEngineDynamicStabilizationCriteriaData& data) const
{
  data.ConvergenceTime(std::unique_ptr<CDM::ScalarTimeData>(m_ConvergenceTime->Unload()));
  data.MinimumReactionTime(std::unique_ptr<CDM::ScalarTimeData>(m_MinimumReactionTime->Unload()));
  data.MaximumAllowedStabilizationTime(std::unique_ptr<CDM::ScalarTimeData>(m_MaximumAllowedStabilizationTime->Unload()));
  for (auto pc : m_PropertyConvergence) {
    std::unique_ptr<CDM::PhysiologyEngineDynamicStabilizationCriteriaPropertyData> pcData(new CDM::PhysiologyEngineDynamicStabilizationCriteriaPropertyData());
    pcData->Name(pc->GetDataRequest().GetName());
    pcData->PercentDifference(pc->m_Target);
    data.PropertyConvergence().push_back(*pcData.get());
  }
}
//-----------------------------------------------------------------------------
std::string PhysiologyEngineDynamicStabilizationCriteria::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* PhysiologyEngineDynamicStabilizationCriteria::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilizationCriteria::SetName(const char* name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilizationCriteria::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilizationCriteria::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineDynamicStabilizationCriteria::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilizationCriteria::HasConvergenceTime() const
{
  return m_ConvergenceTime == nullptr ? false : m_ConvergenceTime->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineDynamicStabilizationCriteria::GetConvergenceTime()
{
  if (m_ConvergenceTime == nullptr)
    m_ConvergenceTime = new SEScalarTime();
  return *m_ConvergenceTime;
}
//-----------------------------------------------------------------------------
double PhysiologyEngineDynamicStabilizationCriteria::GetConvergenceTime(const TimeUnit& unit) const
{
  if (m_ConvergenceTime == nullptr)
    return SEScalar::dNaN();
  return m_ConvergenceTime->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilizationCriteria::HasMinimumReactionTime() const
{
  return m_MinimumReactionTime == nullptr ? false : m_MinimumReactionTime->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineDynamicStabilizationCriteria::GetMinimumReactionTime()
{
  if (m_MinimumReactionTime == nullptr)
    m_MinimumReactionTime = new SEScalarTime();
  return *m_MinimumReactionTime;
}
//-----------------------------------------------------------------------------
double PhysiologyEngineDynamicStabilizationCriteria::GetMinimumReactionTime(const TimeUnit& unit) const
{
  if (m_MinimumReactionTime == nullptr)
    return SEScalar::dNaN();
  return m_MinimumReactionTime->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineDynamicStabilizationCriteria::HasMaximumAllowedStabilizationTime() const
{
  return m_MaximumAllowedStabilizationTime == nullptr ? false : m_MaximumAllowedStabilizationTime->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineDynamicStabilizationCriteria::GetMaximumAllowedStabilizationTime()
{
  if (m_MaximumAllowedStabilizationTime == nullptr)
    m_MaximumAllowedStabilizationTime = new SEScalarTime();
  return *m_MaximumAllowedStabilizationTime;
}
//-----------------------------------------------------------------------------
double PhysiologyEngineDynamicStabilizationCriteria::GetMaximumAllowedStabilizationTime(const TimeUnit& unit) const
{
  if (m_MaximumAllowedStabilizationTime == nullptr)
    return SEScalar::dNaN();
  return m_MaximumAllowedStabilizationTime->GetValue(unit);
}
//-----------------------------------------------------------------------------
/// \brief
/// Sets convergence critera for a specific properter.
///
/// \param  percentError - The acceptable percent difference window that the specified property
///                        must stay within for a specified amount of time for convergence to happen
///
/// \param  name - The name of the property to converge
///
/// \details
/// Mulitple convergence property criteria can be specified. All properties must converge for the
//  specified convergence time. Percent Difference is calculated as such : Math.abs(difference / average) * 100.0
//--------------------------------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateSystemPropertyConvergence(double percentError, const char* name)
{
  return CreateSystemPropertyConvergence(percentError, std::string { name });
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateSystemPropertyConvergence(double percentError, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name.c_str())
      return *pc;
  }
  PropertyConvergence* p = new PropertyConvergence(m_DataRequestMgr.CreatePhysiologyDataRequest(), GetLogger());
  p->m_DataRequest.SetName(name);
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
/// \brief
/// Sets convergence critera for a specific properter.
///
/// \param  percentError - The acceptable percent difference window that the specified property
///                        must stay within for a specified amount of time for convergence to happen
///
/// \param  cmpt - The name of the compartment
///
/// \details
/// Mulitple convergence property criteria can be specified. All properties must converge for the
//  specified convergence time. Percent Difference is calculated as such : Math.abs(difference / average) * 100.0
//--------------------------------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateGasCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name)
{
  return CreateGasCompartmentPropertyConvergence(percentError, std::string { cmpt }, std::string { name });
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateGasCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name)
      return *pc;
  }
  SEGasCompartmentDataRequest& dr = m_DataRequestMgr.CreateGasCompartmentDataRequest();
  dr.SetCompartment(cmpt);
  dr.SetName(name);
  PropertyConvergence* p = new PropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateGasCompartmentPropertyConvergence(double percentError, const char* cmpt, SESubstance& substance, const char* name)
{
  return CreateGasCompartmentPropertyConvergence(percentError, std::string { cmpt }, substance, std::string { name });
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateGasCompartmentPropertyConvergence(double percentError, const std::string& cmpt, SESubstance& substance, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name.c_str())
      return *pc;
  }
  SEGasCompartmentDataRequest& dr = m_DataRequestMgr.CreateGasCompartmentDataRequest();
  dr.SetCompartment(cmpt);
  dr.SetName(name);
  dr.SetSubstance(&substance);
  PropertyConvergence* p = new PropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateLiquidCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name)
{
  return CreateLiquidCompartmentPropertyConvergence(percentError, std::string { cmpt }, std::string { name });
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateLiquidCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name.c_str())
      return *pc;
  }
  SELiquidCompartmentDataRequest& dr = m_DataRequestMgr.CreateLiquidCompartmentDataRequest();
  dr.SetCompartment(cmpt);
  dr.SetName(name);
  PropertyConvergence* p = new PropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateLiquidCompartmentPropertyConvergence(double percentError, const char* cmpt, SESubstance& substance, const char* name)
{
  return CreateLiquidCompartmentPropertyConvergence(percentError, std::string { cmpt }, substance, std::string { name });
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateLiquidCompartmentPropertyConvergence(double percentError, const std::string& cmpt, SESubstance& substance, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name)
      return *pc;
  }
  SELiquidCompartmentDataRequest& dr = m_DataRequestMgr.CreateLiquidCompartmentDataRequest();
  dr.SetCompartment(cmpt);
  dr.SetName(name);
  dr.SetSubstance(&substance);
  PropertyConvergence* p = new PropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateThermalCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name)
{
  return CreateThermalCompartmentPropertyConvergence(percentError, std::string { cmpt }, std::string { name });
}

//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateThermalCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name)
      return *pc;
  }
  SEThermalCompartmentDataRequest& dr = m_DataRequestMgr.CreateThermalCompartmentDataRequest();
  dr.SetCompartment(cmpt);
  dr.SetName(name);
  PropertyConvergence* p = new PropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateTissueCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name)
{
  return CreateTissueCompartmentPropertyConvergence(percentError, std::string { cmpt }, std::string { name });
}
//-----------------------------------------------------------------------------
PropertyConvergence& PhysiologyEngineDynamicStabilizationCriteria::CreateTissueCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name)
{
  for (PropertyConvergence* pc : m_PropertyConvergence) {
    if (pc->m_DataRequest.GetName() == name)
      return *pc;
  }
  SETissueCompartmentDataRequest& dr = m_DataRequestMgr.CreateTissueCompartmentDataRequest();
  dr.SetCompartment(cmpt);
  dr.SetName(name);
  PropertyConvergence* p = new PropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}
//-----------------------------------------------------------------------------
const std::vector<PropertyConvergence*>& PhysiologyEngineDynamicStabilizationCriteria::GetPropertyConvergence() const
{
  return m_PropertyConvergence;
}
//-----------------------------------------------------------------------------
/////////////////////////
// PropertyConvergence //
/////////////////////////

PropertyConvergence::PropertyConvergence(SEDataRequest& dr, Logger* logger)
  : Loggable(logger)
  , m_DataRequest(dr)
  , m_DataRequestScalar(logger)
{
  m_Error = 0;
  m_Target = 0;
  m_LastError = 0;
  m_LastErrorTime_s = 0;
  m_Optional = false;
  m_LastError = SEScalar::dNaN();
  m_Target = SEScalar::dNaN();
}
//-----------------------------------------------------------------------------
PropertyConvergence::~PropertyConvergence()
{
}
//-----------------------------------------------------------------------------
void PropertyConvergence::TrackScalar(const SEScalar& s)
{
  m_DataRequestScalar.SetScalar(&s, m_DataRequest);
}
//-----------------------------------------------------------------------------
double PropertyConvergence::GetPercentError() const
{
  return m_Error;
}
//-----------------------------------------------------------------------------
double PropertyConvergence::GetLastPercentError() const
{
  return m_LastError;
}
//-----------------------------------------------------------------------------
double PropertyConvergence::GetLastErrorTime_s() const
{
  return m_LastErrorTime_s;
}
//-----------------------------------------------------------------------------
double PropertyConvergence::GetCurrentTarget() const
{
  return m_Target;
}
//-----------------------------------------------------------------------------
SEDataRequest& PropertyConvergence::GetDataRequest() const
{
  return m_DataRequest;
}
//-----------------------------------------------------------------------------
SEDataRequestScalar& PropertyConvergence::GetDataRequestScalar()
{
  return m_DataRequestScalar;
}
//-----------------------------------------------------------------------------
bool PropertyConvergence::IsOptional() const
{
  return m_Optional;
}
//-----------------------------------------------------------------------------
void PropertyConvergence::SetOptional(bool b)
{
  m_Optional = b;
}
//-----------------------------------------------------------------------------
}