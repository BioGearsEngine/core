%{
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
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
) SEUrinalysisMicroscopic
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarAmount.swg>

%nspace BIOGEARS_NAMESPACE SEUrinalysisMicroscopic;

%ignore BIOGEARS_NAMESPACE SEUrinalysisMicroscopic::Unload;
%ignore BIOGEARS_NAMESPACE SEUrinalysisMicroscopic::Load;

%include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
