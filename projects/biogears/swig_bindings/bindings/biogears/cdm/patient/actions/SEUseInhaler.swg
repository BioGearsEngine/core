%{
#include <biogears/cdm/patient/actions/SEUseInhaler.h>
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
) SEUseInhaler
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEUseInhaler;

%ignore BIOGEARS_NAMESPACE SEUseInhaler::Unload;
%ignore BIOGEARS_NAMESPACE SEUseInhaler::Load;

%include <biogears/cdm/patient/actions/SEUseInhaler.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEUseInhaler*>;
%template (vector_SEUseInhaler_ptr)  std::vector<BIOGEARS_NAMESPACE SEUseInhaler*>;
