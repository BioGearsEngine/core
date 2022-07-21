%{
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/properties/SEDecimalFormat.swg>

%import <biogears/cdm/scenario/requests/SECompartmentDataRequest.swg>
%import <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.swg>
%import <biogears/cdm/scenario/requests/SEEnvironmentDataRequest.swg>
%import <biogears/cdm/scenario/requests/SEEquipmentDataRequest.swg>
%import <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.swg>
%import <biogears/cdm/scenario/requests/SELiquidCompartmentDataRequest.swg>
%import <biogears/cdm/scenario/requests/SEPatientDataRequest.swg>
%import <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.swg>
%import <biogears/cdm/scenario/requests/SESubstanceDataRequest.swg>
%import <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.swg>
%import <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.swg>

%nspace biogears::SEDataRequestManager;

namespace biogears {
  class SEDataRequestManager {
  public: 
    SEDataRequestManager(Logger* logger);
    
    SEEnvironmentDataRequest& CreateEnvironmentDataRequest(const SEDecimalFormat* dfault = nullptr);
    SEEquipmentDataRequest& CreateEquipmentDataRequest(const SEDecimalFormat* dfault = nullptr);
    SEPatientDataRequest& CreatePatientDataRequest(const SEDecimalFormat* dfault = nullptr);
    SEPhysiologyDataRequest& CreatePhysiologyDataRequest(const SEDecimalFormat* dfault = nullptr);
    SESubstanceDataRequest& CreateSubstanceDataRequest(const SEDecimalFormat* dfault = nullptr);

    SEGasCompartmentDataRequest& CreateGasCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);
    SELiquidCompartmentDataRequest& CreateLiquidCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);
    SEThermalCompartmentDataRequest& CreateThermalCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);
    SETissueCompartmentDataRequest& CreateTissueCompartmentDataRequest(const SEDecimalFormat* dfault = nullptr);

    bool HasResultsFilename() const;
    std::string GetResultsFilename() const;
    void SetResultsFilename(const std::string& name);

    bool HasWorkingDir() const;
    std::string GetWorkingDir() const;
    void SetWorkingDir(const char* name);

    std::string GetResovedFilePath() const;
 
    double GetSamplesPerSecond() const;
    void SetSamplesPerSecond(double num);

    bool HasDataRequests();
  };
}
