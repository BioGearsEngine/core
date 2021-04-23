%{
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/patient/actions/PatientActionsEnums.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SEPatientAssessmentType;
%nspace biogears::SEBrainInjuryType;
%nspace biogears::SEIntubationType;
%nspace biogears::SEPneumothoraxType;
%nspace biogears::SEBolusAdministration;
%nspace biogears::SEInfectionSeverity;

%rename(SEPatientAssessmentType_toString) biogears::ToString(const SEPatientAssessmentType& e);
%rename(SEBrainInjuryType_toString) biogears::ToString(const SEBrainInjuryType& e);
%rename(SEIntubationType_toString) biogears::ToString(const SEIntubationType& e);
%rename(SEPneumothoraxType_toString) biogears::ToString(const SEPneumothoraxType& e);
%rename(SEBolusAdministration_toString) biogears::ToString(const SEBolusAdministration& e);
%rename(SEInfectionSeverity_toString) biogears::ToString(const SEInfectionSeverity& e);

%ignore operator<<(std::ostream& os, const SEPatientAssessmentType& e);
%ignore operator<<(std::ostream& os, const SEBrainInjuryType& e);
%ignore operator<<(std::ostream& os, const SEIntubationType& e);
%ignore operator<<(std::ostream& os, const SEPneumothoraxType& e);
%ignore operator<<(std::ostream& os, const SEBolusAdministration& e);
%ignore operator<<(std::ostream& os, const SEInfectionSeverity& e);

%include <biogears/cdm/patient/actions/PatientActionsEnums.h>