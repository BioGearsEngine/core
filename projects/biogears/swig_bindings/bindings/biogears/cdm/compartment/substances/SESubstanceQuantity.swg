%{
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
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
) SESubstanceQuantity
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceQuantity;

%ignore BIOGEARS_NAMESPACE SESubstanceQuantity::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceQuantity::Load;

%ignore GetScalar(const std::string& name);

%include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
