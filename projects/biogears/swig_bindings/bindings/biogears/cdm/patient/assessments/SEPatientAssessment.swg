%{
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
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
) SEPatientAssessment
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEProperty;

%ignore BIOGEARS_NAMESPACE SEPatientAssessment::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientAssessment::Load;

%include <biogears/cdm/patient/assessments/SEPatientAssessment.h>


