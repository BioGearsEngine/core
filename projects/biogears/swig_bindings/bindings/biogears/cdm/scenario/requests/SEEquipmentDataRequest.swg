%{
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%include <std_map.i>

%import <biogears/cdm/scenario/requests/SEDataRequest.swg>

%nspace biogears::SEEquipmentDataRequest;

%ignore biogears::SEEquipmentDataRequest::SetType(std::string const &);

//#if defined(SWIGCSHARP)
  %rename(SetDataRequestType) biogears::SEEquipmentDataRequest::SetType(const char* t);
  %ignore biogears::SEEquipmentDataRequest::SetType(const std::string& t);
  %rename(GetDataRequestType) biogears::SEEquipmentDataRequest::GetType() const;
//#endif


%include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>

