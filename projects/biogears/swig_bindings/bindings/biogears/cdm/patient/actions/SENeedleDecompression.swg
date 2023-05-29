%{
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SENeedleDecompression;

%ignore BIOGEARS_NAMESPACE SENeedleDecompression::Unload;
%ignore BIOGEARS_NAMESPACE SENeedleDecompression::Load;

%include <biogears/cdm/patient/actions/SENeedleDecompression.h>
