%{
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
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
) SEConsciousRespiration
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>
%import <biogears/cdm/patient/actions/SEForcedExhale.swg>
%import <biogears/cdm/patient/actions/SEForcedInhale.swg>
%import <biogears/cdm/patient/actions/SEBreathHold.swg>
%import <biogears/cdm/patient/actions/SEUseInhaler.swg>

%nspace BIOGEARS_NAMESPACE SEConsciousRespiration;

%ignore BIOGEARS_NAMESPACE SEConsciousRespiration::Unload;
%ignore BIOGEARS_NAMESPACE SEConsciousRespiration::Load;

%include <biogears/cdm/patient/actions/SEConsciousRespiration.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEConsciousRespiration*>;
%template (vector_SEConsciousRespiration_ptr)  std::vector<BIOGEARS_NAMESPACE SEConsciousRespiration*>;