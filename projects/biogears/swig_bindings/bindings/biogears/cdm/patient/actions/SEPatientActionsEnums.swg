%{
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/patient/actions/PatientActionsEnums.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEPatientAssessmentType;
%nspace BIOGEARS_NAMESPACE SEBrainInjuryType;
%nspace BIOGEARS_NAMESPACE SEIntubationType;
%nspace BIOGEARS_NAMESPACE SEPneumothoraxType;
%nspace BIOGEARS_NAMESPACE SEBolusAdministration;
%nspace BIOGEARS_NAMESPACE SEInfectionSeverity;

%rename(SEPatientAssessmentType_toString) BIOGEARS_NAMESPACE ToString(const SEPatientAssessmentType& e);
%rename(SEBrainInjuryType_toString) BIOGEARS_NAMESPACE ToString(const SEBrainInjuryType& e);
%rename(SEIntubationType_toString) BIOGEARS_NAMESPACE ToString(const SEIntubationType& e);
%rename(SEPneumothoraxType_toString) BIOGEARS_NAMESPACE ToString(const SEPneumothoraxType& e);
%rename(SEBolusAdministration_toString) BIOGEARS_NAMESPACE ToString(const SEBolusAdministration& e);
%rename(SEInfectionSeverity_toString) BIOGEARS_NAMESPACE ToString(const SEInfectionSeverity& e);

%ignore operator<<(std::ostream& os, const SEPatientAssessmentType& e);
%ignore operator<<(std::ostream& os, const SEBrainInjuryType& e);
%ignore operator<<(std::ostream& os, const SEIntubationType& e);
%ignore operator<<(std::ostream& os, const SEPneumothoraxType& e);
%ignore operator<<(std::ostream& os, const SEBolusAdministration& e);
%ignore operator<<(std::ostream& os, const SEInfectionSeverity& e);

%include <biogears/cdm/patient/actions/PatientActionsEnums.h>