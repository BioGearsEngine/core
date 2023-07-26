
%{
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SECompartmentSubstanceDataRequest
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears//cdm/substance/SESubstance.swg>
%import <biogears/cdm/scenario/requests/SECompartmentDataRequest.swg>

%nspace BIOGEARS_NAMESPACE SECompartmentSubstanceDataRequest;

%ignore Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const std::string& unit = "");
%ignore Set(const std::string& cmpt, SESubstance& substance, const std::string& name, const CCompoundUnit& unit);

%include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>
