%{
#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
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
) SEConsciousRespirationCommand
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEConsciousRespirationCommand;

%ignore BIOGEARS_NAMESPACE SEConsciousRespirationCommand::Unload;
%ignore BIOGEARS_NAMESPACE SEConsciousRespirationCommand::Load;

%include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
	
%nspace  std::vector<BIOGEARS_NAMESPACE SEConsciousRespirationCommand*>;
%template (vector_SEConsciousRespirationCommand_ptr)  std::vector<BIOGEARS_NAMESPACE SEConsciousRespirationCommand*>;