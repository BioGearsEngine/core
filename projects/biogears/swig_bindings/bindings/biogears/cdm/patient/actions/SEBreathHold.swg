%{
#include <biogears/cdm/patient/actions/SEBreathHold.h>
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
) SEBreathHold
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%nspace BIOGEARS_NAMESPACE SEBreathHold;

%ignore BIOGEARS_NAMESPACE SEBreathHold::Unload;
%ignore BIOGEARS_NAMESPACE SEBreathHold::Load;

%include <biogears/cdm/patient/actions/SEBreathHold.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEBreathHold*>;
%template (vector_SEBreathHold_ptr)  std::vector<BIOGEARS_NAMESPACE SEBreathHold*>;