%{
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SEComprehensiveMetabolicPanel;

%ignore BIOGEARS_NAMESPACE SEComprehensiveMetabolicPanel::Unload;
%ignore BIOGEARS_NAMESPACE SEComprehensiveMetabolicPanel::Load;

%include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
