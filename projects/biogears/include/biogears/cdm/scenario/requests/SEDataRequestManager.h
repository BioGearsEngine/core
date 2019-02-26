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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/scenario/requests/SEEnvironmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SELiquidCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEPatientDataRequest.h>
#include <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.h>
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>
#include <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>

CDM_BIND_DECL(DataRequestsData)

namespace biogears {
class BIOGEARS_API SEDataRequestManager : public Loggable {
public:
  SEDataRequestManager(Logger* logger);
  ~SEDataRequestManager();

  void Clear();

  bool Load(const CDM::DataRequestsData& in, SESubstanceManager& subMgr);
  CDM::DataRequestsData* Unload() const;

protected:
  void Unload(CDM::DataRequestsData& data) const;

public:
  bool HasResultsFilename() const { return !m_ResultsFile.empty(); }
  std::string GetResultsFilename() const;
  const char * GetResultsFilename_cStr() const;
  void SetResultsFilename(const char* name);
  void SetResultsFilename(const std::string& name);

  bool HasWorkingDir() const { return !m_ResultsFile.empty(); }
  std::string GetWorkingDir() const;
  const char * GetWorkingDir_cStr() const;
  void SetWorkingDir(const char* name);
  void SetWorkingDir(const std::string& name);

  std::string GetResovedFilePath() const; 
  //const char * GetResovedFilePath_cStr() const;

  double GetSamplesPerSecond() const { return m_SamplesPerSecond; }
  void SetSamplesPerSecond(double num) { m_SamplesPerSecond = num; }

  bool HasDataRequests() { return !m_Requests.empty(); }
  const std::vector<SEDataRequest*>& GetDataRequests() { return m_Requests; }

  virtual bool HasDefaultDecimalFormatting() const;
  virtual SEDecimalFormat& GetDefaultDecimalFormatting();
  virtual void RemoveDefaultDecimalFormatting();

  virtual bool HasOverrideDecimalFormatting() const;
  virtual SEDecimalFormat& GetOverrideDecimalFormatting();
  virtual void RemoveOverrideDecimalFormatting();

  SEEnvironmentDataRequest& CreateEnvironmentDataRequest(const SEDecimalFormat* dfault = nullptr);
  SEEquipmentDataRequest& CreateEquipmentDataRequest(const SEDecimalFormat* dfault = nullptr);
  SEPatientDataRequest& CreatePatientDataRequest(const SEDecimalFormat* dfault = nullptr);
  SEPhysiologyDataRequest& CreatePhysiologyDataRequest(const SEDecimalFormat* dfault = nullptr);
  SESubstanceDataRequest& CreateSubstanceDataRequest(const SEDecimalFormat* dfault = nullptr);

  SEGasCompartmentDataRequest& CreateGasCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);
  SELiquidCompartmentDataRequest& CreateLiquidCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);
  SEThermalCompartmentDataRequest& CreateThermalCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);
  SETissueCompartmentDataRequest& CreateTissueCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);

protected:
  double m_SamplesPerSecond;
  std::vector<SEDataRequest*> m_Requests;
  std::string m_ResultsFile;
  std::string m_WorkingDir;

  SEDecimalFormat* m_DefaultDecimalFormatting;
  SEDecimalFormat* m_OverrideDecimalFormatting;

  static SEDataRequest* newFromBind(const CDM::DataRequestData& dataRequest, SESubstanceManager& substances, const SEDecimalFormat* dfault = nullptr);
};
}