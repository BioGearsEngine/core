%{
#include <biogears/cdm/patient/conditions/SEStarvation.h>
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
) SEStarvation
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEStarvation;

%ignore BIOGEARS_NAMESPACE SEStarvation::Unload;
%ignore BIOGEARS_NAMESPACE SEStarvation::Load;


%include <biogears/cdm/patient/conditions/SEStarvation.h>
