

%{
#include <biogears/cdm/patient/actions/SEBurnWound.h>
%}

%import <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEBurnWound;

%ignore BIOGEARS_NAMESPACE SEBurnWound::Unload;
%ignore BIOGEARS_NAMESPACE SEBurnWound::Load;

%ignore BIOGEARS_NAMESPACE SEBurnWound::SetCompartment(std::string const &);

%include <biogears/cdm/patient/actions/SEBurnWound.h>
