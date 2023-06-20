

%{
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEBrainInjury;

%ignore BIOGEARS_NAMESPACE SEBrainInjury::Unload;
%ignore BIOGEARS_NAMESPACE SEBrainInjury::Load;

%rename (SetBrainInjuryType) BIOGEARS_NAMESPACE SEBrainInjury::SetType(CDM::enumBrainInjuryType::value t);
%rename (GetBrainInjuryType) BIOGEARS_NAMESPACE SEBrainInjury::GetType() const;
%rename (HasBrainInjuryType) BIOGEARS_NAMESPACE SEBrainInjury::HasType() const;
%rename (InvalidateBrainInjuryType) BIOGEARS_NAMESPACE SEBrainInjury::InvalidateType();
%include <biogears/cdm/patient/actions/SEBrainInjury.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEBrainInjury*>;
%template (vector_SEBrainInjury_ptr)  std::vector<BIOGEARS_NAMESPACE SEBrainInjury*>;