%{
#include <biogears/cdm/compartment/SECompartment.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SECompartment;

%ignore biogears::SECompartment::Unload;
%ignore biogears::SECompartment::Load;

%ignore biogears::SECompartment::FindLeaves;
%ignore biogears::SECompartment::GetScalar(const std::string& name);

%include <biogears/cdm/compartment/SECompartment.h>

%nspace std::vector<biogears::SECompartment*>;
%template (vector_SECompartment_ptr) std::vector<biogears::SECompartment*>;
