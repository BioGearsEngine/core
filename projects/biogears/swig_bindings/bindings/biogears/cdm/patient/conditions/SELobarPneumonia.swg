%{
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
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
) SELobarPneumonia
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SELobarPneumonia;

%ignore BIOGEARS_NAMESPACE SELobarPneumonia::Unload;
%ignore BIOGEARS_NAMESPACE SELobarPneumonia::Load;


%include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
