%{
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
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
) SEPatientAssessmentRequest
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEPatientAssessmentRequest;

%ignore BIOGEARS_NAMESPACE SEPatientAssessmentRequest::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientAssessmentRequest::Load;

%rename (SetPatientAssessmentType) BIOGEARS_NAMESPACE SEPatientAssessmentRequest::SetType;
%rename (GetPatientAssessmentType) BIOGEARS_NAMESPACE SEPatientAssessmentRequest::GetType;

%include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEPatientAssessmentRequest*>;
%template (vector_SEPatientAssessmentRequest_ptr)  std::vector<BIOGEARS_NAMESPACE SEPatientAssessmentRequest*>;