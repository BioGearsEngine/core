

%{
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEBrainInjury;

%ignore biogears::SEBrainInjury::Unload;
%ignore biogears::SEBrainInjury::Load;

%rename (SetBrainInjuryType) biogears::SEBrainInjury::SetType(CDM::enumBrainInjuryType::value t);
%rename (GetBrainInjuryType) biogears::SEBrainInjury::GetType() const;
%rename (HasBrainInjuryType) biogears::SEBrainInjury::HasType() const;
%rename (InvalidateBrainInjuryType) biogears::SEBrainInjury::InvalidateType();
%include <biogears/cdm/patient/actions/SEBrainInjury.h>
