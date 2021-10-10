
%{
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.swg>

%nspace biogears::SEGasCompartmentDataRequest;

%rename (SetProperties) biogears::SEGasCompartmentDataRequest::Set; 

%include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
