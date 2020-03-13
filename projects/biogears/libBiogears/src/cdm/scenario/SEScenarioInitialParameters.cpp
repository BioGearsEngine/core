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
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>

#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
SEScenarioInitialParameters::SEScenarioInitialParameters(SESubstanceManager& subMgr)
  : Loggable(subMgr.GetLogger())
  , m_SubMgr(subMgr)
 
{
  m_Configuration = nullptr;
  m_Patient = nullptr;
  Clear();
}

//-----------------------------------------------------------------------------
SEScenarioInitialParameters::~SEScenarioInitialParameters()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::Clear()
{
  m_PatientFile = "";
  SAFE_DELETE(m_Patient);
  SAFE_DELETE(m_Configuration);
  DELETE_VECTOR(m_Conditions);
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::Load(const CDM::ScenarioInitialParametersData& in)
{
  Clear();

  if (in.Configuration().present())
    GetConfiguration().Load(in.Configuration().get());

  if (in.PatientFile().present())
    m_PatientFile = in.PatientFile().get();
  else if (in.Patient().present())
    GetPatient().Load(in.Patient().get());
  else {
    Error("No patient provided");
    return false;
  }

  for (unsigned int i = 0; i < in.Condition().size(); i++) {
    SECondition* c = SECondition::newFromBind(in.Condition()[i], m_SubMgr);
    if (c != nullptr)
      m_Conditions.push_back(c);
  }

  return IsValid();
}
//-----------------------------------------------------------------------------
CDM::ScenarioInitialParametersData* SEScenarioInitialParameters::Unload() const
{
  CDM::ScenarioInitialParametersData* data = new CDM::ScenarioInitialParametersData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::Unload(CDM::ScenarioInitialParametersData& data) const
{
  if (HasPatientFile())
    data.PatientFile(m_PatientFile);
  else if (HasPatient())
    data.Patient(std::unique_ptr<CDM::PatientData>(m_Patient->Unload()));
  for (SECondition* c : m_Conditions)
    data.Condition().push_back(std::unique_ptr<CDM::ConditionData>(c->Unload()));
  if (HasConfiguration())
    data.Configuration(std::unique_ptr<CDM::PhysiologyEngineConfigurationData>(m_Configuration->Unload()));
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::IsValid() const
{
  if (HasPatientFile() && HasPatient())
    return false;
  return true;
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
//-----------------------------------------------------------------------------
PhysiologyEngineConfiguration& SEScenarioInitialParameters::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new PhysiologyEngineConfiguration(GetLogger());
  return *m_Configuration;
}
//-----------------------------------------------------------------------------
const PhysiologyEngineConfiguration* SEScenarioInitialParameters::GetConfiguration() const
{
  return m_Configuration;
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::SetConfiguration(const PhysiologyEngineConfiguration& config)
{
  CDM_COPY((&config), (&GetConfiguration()));
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::InvalidateConfiguration()
{
  SAFE_DELETE(m_Configuration);
}
//-----------------------------------------------------------------------------
const char* SEScenarioInitialParameters::GetPatientFile() const
{
  return m_PatientFile.c_str();
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::SetPatientFile(const char* patientFile)
{
  InvalidatePatient();
  m_PatientFile = patientFile;
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::SetPatientFile(const std::string& patientFile)
{
  InvalidatePatient();
  m_PatientFile = patientFile;
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::HasPatientFile() const
{
  return m_PatientFile.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::InvalidatePatientFile()
{
  m_PatientFile = "";
}
//-----------------------------------------------------------------------------
SEPatient& SEScenarioInitialParameters::GetPatient()
{
  InvalidatePatientFile();
  if (m_Patient == nullptr)
    m_Patient = new SEPatient(GetLogger());
  return *m_Patient;
}
//-----------------------------------------------------------------------------
const SEPatient* SEScenarioInitialParameters::GetPatient() const
{
  return m_Patient;
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::SetPatient(const SEPatient& patient)
{
  CDM_COPY((&patient), (&GetPatient()));
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::HasPatient() const
{
  return m_Patient != nullptr;
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::InvalidatePatient()
{
  SAFE_DELETE(m_Patient);
}
//-----------------------------------------------------------------------------
void SEScenarioInitialParameters::AddCondition(const SECondition& c)
{
  CDM::ConditionData* bind = c.Unload();
  m_Conditions.push_back(SECondition::newFromBind(*bind, m_SubMgr));
  delete bind;
}
//-----------------------------------------------------------------------------
const std::vector<SECondition*>& SEScenarioInitialParameters::GetConditions() const
{
  return m_Conditions;
}
//-----------------------------------------------------------------------------
}