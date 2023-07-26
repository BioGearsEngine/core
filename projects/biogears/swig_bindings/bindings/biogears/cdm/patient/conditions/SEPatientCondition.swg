%{
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
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
) SEPatientCondition
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/scenario/SECondition.swg>

%nspace BIOGEARS_NAMESPACE SEPatientCondition;

%ignore BIOGEARS_NAMESPACE SEPatientCondition::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientCondition::Load;

%ignore BIOGEARS_NAMESPACE SECondition::ToString(std::ostream& str) const; 

%include <biogears/cdm/patient/conditions/SEPatientCondition.h>
