%{
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/scenario/requests/SEDataRequest.swg>

%nspace BIOGEARS_NAMESPACE SEEquipmentDataRequest;

%ignore BIOGEARS_NAMESPACE SEEquipmentDataRequest::SetType(std::string const &);

//#if defined(SWIGCSHARP)
  %rename(SetDataRequestType) BIOGEARS_NAMESPACE SEEquipmentDataRequest::SetType(const char* t);
  %ignore BIOGEARS_NAMESPACE SEEquipmentDataRequest::SetType(const std::string& t);
  %rename(GetDataRequestType) BIOGEARS_NAMESPACE SEEquipmentDataRequest::GetType() const;
//#endif


%include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>

