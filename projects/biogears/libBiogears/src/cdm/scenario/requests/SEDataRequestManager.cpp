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
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>

#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SEDataRequestManager::SEDataRequestManager(Logger* logger)
  : Loggable(logger)
  , m_SamplesPerSecond(1.0)
  , m_ResultsFile("")

{
  m_DefaultDecimalFormatting = nullptr;
  m_OverrideDecimalFormatting = nullptr;
  Clear();
}
//-----------------------------------------------------------------------------
SEDataRequestManager::~SEDataRequestManager()
{
  Clear();
}
//-----------------------------------------------------------------------------
const char* SEDataRequestManager::GetResultsFilename_cStr() const
{
  return m_ResultsFile.c_str();
}
//-----------------------------------------------------------------------------
std::string SEDataRequestManager::GetResultsFilename() const
{
  return m_ResultsFile;
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::SetResultsFilename(const char* name)
{
  m_ResultsFile = name;
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::SetResultsFilename(const std::string& name)
{
  m_ResultsFile = name;
}
//-----------------------------------------------------------------------------
std::string SEDataRequestManager::GetWorkingDir() const
{
  return m_WorkingDir;
}
//-----------------------------------------------------------------------------
const char* SEDataRequestManager::GetWorkingDir_cStr() const
{
  return m_WorkingDir.c_str();
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::SetWorkingDir(const char* name)
{
  m_WorkingDir = name;
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::SetWorkingDir(const std::string& name)
{
  m_WorkingDir = name;
}
//-----------------------------------------------------------------------------
std::string SEDataRequestManager::GetResovedFilePath() const { return m_WorkingDir + m_ResultsFile; }
//-----------------------------------------------------------------------------
void SEDataRequestManager::Clear()
{
  m_SamplesPerSecond = 0; // Sample every time step
  DELETE_VECTOR(m_Requests);
  SAFE_DELETE(m_DefaultDecimalFormatting);
  SAFE_DELETE(m_OverrideDecimalFormatting);
}
//-----------------------------------------------------------------------------
bool SEDataRequestManager::Load(const CDM::DataRequestsData& in, SESubstanceManager& subMgr)
{
  Clear();
  if (in.Filename().present())
    m_ResultsFile = in.Filename().get();
  if (in.SamplesPerSecond().present())
    m_SamplesPerSecond = in.SamplesPerSecond().get();
  if (in.DefaultDecimalFormatting().present())
    GetDefaultDecimalFormatting().Load(in.DefaultDecimalFormatting().get());
  if (in.OverrideDecimalFormatting().present())
    GetOverrideDecimalFormatting().Load(in.OverrideDecimalFormatting().get());

  for (unsigned int i = 0; i < in.DataRequest().size(); i++) {
    SEDataRequest* dr = newFromBind(in.DataRequest()[i], subMgr, m_DefaultDecimalFormatting);
    if (dr != nullptr) {
      if (HasOverrideDecimalFormatting())
        ((SEDecimalFormat*)dr)->Set(*m_OverrideDecimalFormatting);
      m_Requests.push_back(dr);
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
CDM::DataRequestsData* SEDataRequestManager::Unload() const
{
  CDM::DataRequestsData* data = new CDM::DataRequestsData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::Unload(CDM::DataRequestsData& data) const
{
  data.SamplesPerSecond(m_SamplesPerSecond);
  if (HasResultsFilename()) {
    data.Filename(m_ResultsFile);
  }
  if (HasDefaultDecimalFormatting()) {
    data.DefaultDecimalFormatting(std::unique_ptr<CDM::DecimalFormatData>(m_DefaultDecimalFormatting->Unload()));
  }
  if (HasOverrideDecimalFormatting()) {
    data.OverrideDecimalFormatting(std::unique_ptr<CDM::DecimalFormatData>(m_OverrideDecimalFormatting->Unload()));
  }
  for (auto& dr : m_Requests) {
    auto ptr = dr->Unload();
    auto uptr = std::unique_ptr<CDM::DataRequestData>(ptr);
    data.DataRequest().push_back(std::move(uptr));
  }
}
//-----------------------------------------------------------------------------
bool SEDataRequestManager::HasDefaultDecimalFormatting() const
{
  return m_DefaultDecimalFormatting != nullptr;
}
//-----------------------------------------------------------------------------
SEDecimalFormat& SEDataRequestManager::GetDefaultDecimalFormatting()
{
  if (m_DefaultDecimalFormatting == nullptr)
    m_DefaultDecimalFormatting = new SEDecimalFormat();
  return *m_DefaultDecimalFormatting;
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::RemoveDefaultDecimalFormatting()
{
  SAFE_DELETE(m_DefaultDecimalFormatting);
}
//-----------------------------------------------------------------------------
bool SEDataRequestManager::HasOverrideDecimalFormatting() const
{
  return m_OverrideDecimalFormatting != nullptr;
}
//-----------------------------------------------------------------------------
SEDecimalFormat& SEDataRequestManager::GetOverrideDecimalFormatting()
{
  if (m_OverrideDecimalFormatting == nullptr)
    m_OverrideDecimalFormatting = new SEDecimalFormat();
  return *m_OverrideDecimalFormatting;
}
//-----------------------------------------------------------------------------
void SEDataRequestManager::RemoveOverrideDecimalFormatting()
{
  SAFE_DELETE(m_OverrideDecimalFormatting);
}
//-----------------------------------------------------------------------------
SEEnvironmentDataRequest& SEDataRequestManager::CreateEnvironmentDataRequest(const SEDecimalFormat* dfault)
{
  SEEnvironmentDataRequest* dr = new SEEnvironmentDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SEEquipmentDataRequest& SEDataRequestManager::CreateEquipmentDataRequest(const SEDecimalFormat* dfault)
{
  SEEquipmentDataRequest* dr = new SEEquipmentDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SEPatientDataRequest& SEDataRequestManager::CreatePatientDataRequest(const SEDecimalFormat* dfault)
{
  SEPatientDataRequest* dr = new SEPatientDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SEPhysiologyDataRequest& SEDataRequestManager::CreatePhysiologyDataRequest(const SEDecimalFormat* dfault)
{
  SEPhysiologyDataRequest* dr = new SEPhysiologyDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SESubstanceDataRequest& SEDataRequestManager::CreateSubstanceDataRequest(const SEDecimalFormat* dfault)
{
  SESubstanceDataRequest* dr = new SESubstanceDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SEGasCompartmentDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const SEDecimalFormat* dfault)
{
  SEGasCompartmentDataRequest* dr = new SEGasCompartmentDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SELiquidCompartmentDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const SEDecimalFormat* dfault)
{
  SELiquidCompartmentDataRequest* dr = new SELiquidCompartmentDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SEThermalCompartmentDataRequest& SEDataRequestManager::CreateThermalCompartmentDataRequest(const SEDecimalFormat* dfault)
{
  SEThermalCompartmentDataRequest* dr = new SEThermalCompartmentDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SETissueCompartmentDataRequest& SEDataRequestManager::CreateTissueCompartmentDataRequest(const SEDecimalFormat* dfault)
{
  SETissueCompartmentDataRequest* dr = new SETissueCompartmentDataRequest(dfault);
  m_Requests.push_back(dr);
  return *dr;
}
//-----------------------------------------------------------------------------
SEDataRequest* SEDataRequestManager::newFromBind(const CDM::DataRequestData& data, SESubstanceManager& substances, const SEDecimalFormat* dfault)
{
  const CDM::DataRequestData* drData = &data;
  const CDM::PhysiologyDataRequestData* physSysData = dynamic_cast<const CDM::PhysiologyDataRequestData*>(drData);
  if (physSysData != nullptr) {
    SEPhysiologyDataRequest* sys = new SEPhysiologyDataRequest(dfault);
    sys->Load(*physSysData);
    return sys;
  }
  const CDM::GasCompartmentDataRequestData* gasData = dynamic_cast<const CDM::GasCompartmentDataRequestData*>(drData);
  if (gasData != nullptr) {
    SEGasCompartmentDataRequest* Comp = new SEGasCompartmentDataRequest(dfault);
    Comp->Load(*gasData, substances);
    return Comp;
  }
  const CDM::LiquidCompartmentDataRequestData* liquidData = dynamic_cast<const CDM::LiquidCompartmentDataRequestData*>(drData);
  if (liquidData != nullptr) {
    SELiquidCompartmentDataRequest* Comp = new SELiquidCompartmentDataRequest(dfault);
    Comp->Load(*liquidData, substances);
    return Comp;
  }
  const CDM::ThermalCompartmentDataRequestData* thermData = dynamic_cast<const CDM::ThermalCompartmentDataRequestData*>(drData);
  if (thermData != nullptr) {
    SEThermalCompartmentDataRequest* Comp = new SEThermalCompartmentDataRequest(dfault);
    Comp->Load(*thermData);
    return Comp;
  }
  const CDM::TissueCompartmentDataRequestData* tissueData = dynamic_cast<const CDM::TissueCompartmentDataRequestData*>(drData);
  if (tissueData != nullptr) {
    SETissueCompartmentDataRequest* Comp = new SETissueCompartmentDataRequest(dfault);
    Comp->Load(*tissueData);
    return Comp;
  }
  const CDM::PatientDataRequestData* patData = dynamic_cast<const CDM::PatientDataRequestData*>(drData);
  if (patData != nullptr) {
    SEPatientDataRequest* sys = new SEPatientDataRequest(dfault);
    sys->Load(*patData);
    return sys;
  }
  const CDM::SubstanceDataRequestData* subData = dynamic_cast<const CDM::SubstanceDataRequestData*>(drData);
  if (subData != nullptr) {
    SESubstanceDataRequest* sub = new SESubstanceDataRequest(dfault);
    sub->Load(*subData, substances);
    return sub;
  }
  const CDM::EnvironmentDataRequestData* envData = dynamic_cast<const CDM::EnvironmentDataRequestData*>(drData);
  if (envData != nullptr) {
    SEEnvironmentDataRequest* env = new SEEnvironmentDataRequest(dfault);
    env->Load(*envData);
    return env;
  }
  const CDM::EquipmentDataRequestData* equipSysData = dynamic_cast<const CDM::EquipmentDataRequestData*>(drData);
  if (equipSysData != nullptr) {
    SEEquipmentDataRequest* sys = new SEEquipmentDataRequest(dfault);
    sys->Load(*equipSysData);
    return sys;
  }

  if (substances.GetLogger() != nullptr)
    substances.GetLogger()->Error("Unsupported DataRequest Received", "SEDataRequest::newFromBind");
  return nullptr;
}
//-----------------------------------------------------------------------------
}