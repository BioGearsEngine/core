%{
#include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
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
) SEPsychomotorVigilanceTask
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEPsychomotorVigilanceTask;

%ignore BIOGEARS_NAMESPACE SEPsychomotorVigilanceTask::Unload;
%ignore BIOGEARS_NAMESPACE SEPsychomotorVigilanceTask::Load;

%include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
