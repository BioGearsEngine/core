%{
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
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
) SEPulmonaryFunctionTest
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEFunctionVolumeVsTime.swg>

%nspace BIOGEARS_NAMESPACE SEPulmonaryFunctionTest;

%ignore BIOGEARS_NAMESPACE SEPulmonaryFunctionTest::Unload;
%ignore BIOGEARS_NAMESPACE SEPulmonaryFunctionTest::Load;

%include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
