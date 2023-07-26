%{
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
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
) SEForcedInhale
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE ForcedInhale;

%ignore BIOGEARS_NAMESPACE ForcedInhale::Unload;
%ignore BIOGEARS_NAMESPACE ForcedInhale::Load;

%include <biogears/cdm/patient/actions/SEForcedInhale.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEForcedInhale*>;
%template (vector_SEForcedInhale_ptr)  std::vector<BIOGEARS_NAMESPACE SEForcedInhale*>;