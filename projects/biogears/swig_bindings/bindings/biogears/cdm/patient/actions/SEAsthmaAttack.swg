%{
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
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
) SEAsthmaAttack
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEAsthmaAttack;

%ignore BIOGEARS_NAMESPACE SEAsthmaAttack::Unload;
%ignore BIOGEARS_NAMESPACE SEAsthmaAttack::Load;

%include <biogears/cdm/patient/actions/SEAsthmaAttack.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEAsthmaAttack*>;
%template (vector_SEAsthmaAttack_ptr)  std::vector<BIOGEARS_NAMESPACE SEAsthmaAttack*>;