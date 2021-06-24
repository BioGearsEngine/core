
%{
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%include <std_map.i>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/scenario/requests/SEDataRequest.swg>

%nspace biogears::SESubstanceDataRequest;

%ignore Set(const SESubstance& substance, const char* name, const CCompoundUnit& unit);
%ignore Set(const SESubstance& substance, const std::string& name, const CCompoundUnit& unit);
%ignore Set(const SESubstance& substance, const char* name, const char* unit = "");
%ignore Set(const SESubstance& substance, const std::string& name, const std::string& unit = "");
%ignore Set(const SESubstance& substance, const char* cmpt, const char* name, const CCompoundUnit& unit);
%ignore Set(const SESubstance& substance, const std::string& cmpt, const std::string& name, const CCompoundUnit& unit);
%ignore Set(const SESubstance& substance, const std::string& cmpt, const std::string& name, const std::string& unit = "");

//We need to update SeteventCallback from C Function Ptr to a C++ Class which other 
//Languages can inherit from. using SWIGS director syntax
%ignore SetEventCallback;

%include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>
