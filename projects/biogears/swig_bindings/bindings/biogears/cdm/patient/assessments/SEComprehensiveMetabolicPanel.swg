%{
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>

%nspace biogears::SEComprehensiveMetabolicPanel;

%ignore biogears::SEComprehensiveMetabolicPanel::Unload;
%ignore biogears::SEComprehensiveMetabolicPanel::Load;

%include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
