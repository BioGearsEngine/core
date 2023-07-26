%{
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
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
) SEComprehensiveMetabolicPanel
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SEComprehensiveMetabolicPanel;

%ignore BIOGEARS_NAMESPACE SEComprehensiveMetabolicPanel::Unload;
%ignore BIOGEARS_NAMESPACE SEComprehensiveMetabolicPanel::Load;

%include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
