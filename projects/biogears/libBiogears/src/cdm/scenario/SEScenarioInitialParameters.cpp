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

#include "io/cdm/Scenario.h"
#include "io/cdm/EngineConfiguration.h"
#include "io/cdm/Patient.h"

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
  , m_DoTrackStabilization(false)

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
  m_DoTrackStabilization = false;
  SAFE_DELETE(m_Patient);
  SAFE_DELETE(m_Configuration);
  DELETE_VECTOR(m_Conditions);
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::Load(const CDM::ScenarioInitialParametersData& in)
{
  io::Scenario::UnMarshall(in, *this);
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
  io::Scenario::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEScenarioInitialParameters::IsValid() const
{
  if (HasPatientFile() || HasPatient())
    return true;
  return false;
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
  CDM_BIOGEARS_CONFIGURATION_COPY(PhysiologyEngineConfiguration, config, GetConfiguration());
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
  CDM_PATIENT_COPY(Patient, patient, GetPatient());
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

  auto conditionData = io::Conditions::factory(&c);
  auto conditionCopy = io::Conditions::factory(conditionData.get(), m_SubMgr).release();
  m_Conditions.push_back(conditionCopy);

}
//-----------------------------------------------------------------------------
const std::vector<SECondition*>& SEScenarioInitialParameters::GetConditions() const
{
  return m_Conditions;
}
//-----------------------------------------------------------------------------

bool SEScenarioInitialParameters::TrackingStabilization() const
{
  return m_DoTrackStabilization;
}
//-----------------------------------------------------------------------------

void SEScenarioInitialParameters::SetTrackStabilization(bool flag)
{
  m_DoTrackStabilization = flag;
}
//-----------------------------------------------------------------------------

bool SEScenarioInitialParameters::operator==(SEScenarioInitialParameters const& rhs) const
{
  if (this == &rhs)
    return true;

  bool equivilant = &m_SubMgr == &rhs.m_SubMgr;
  equivilant &= ((m_Configuration && rhs.m_Configuration) ? m_Configuration->operator==(*rhs.m_Configuration) : m_Configuration == rhs.m_Configuration);
  equivilant &= ((m_Patient && rhs.m_Patient) ? m_Patient->operator==(*rhs.m_Patient) : m_Patient == rhs.m_Patient);
  equivilant &= m_PatientFile == rhs.m_PatientFile;
  equivilant &= m_DoTrackStabilization == rhs.m_DoTrackStabilization;
  if (m_Conditions.size() == rhs.m_Conditions.size()) {
    for (auto idx = 0; idx < m_Conditions.size(); ++idx) {
      equivilant &= m_Conditions[idx]->operator==(*rhs.m_Conditions[idx]);
    }
  } else {
    equivilant = false;
  }

  return equivilant;
}

bool SEScenarioInitialParameters::operator!=(SEScenarioInitialParameters const& rhs) const
{
    return !(*this == rhs);
}
//-----------------------------------------------------------------------------

}