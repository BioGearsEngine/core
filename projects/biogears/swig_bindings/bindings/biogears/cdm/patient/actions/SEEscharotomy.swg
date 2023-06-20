%{
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEEscharotomy;

%ignore BIOGEARS_NAMESPACE SEEscharotomy::Unload;
%ignore BIOGEARS_NAMESPACE SEEscharotomy::Load;

%ignore BIOGEARS_NAMESPACE SEEscharotomy::SetLocation(std::string const&);

%include <biogears/cdm/patient/actions/SEEscharotomy.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEEscharotomy*>;
%template (vector_SEEscharotomy_ptr)  std::vector<BIOGEARS_NAMESPACE SEEscharotomy*>;