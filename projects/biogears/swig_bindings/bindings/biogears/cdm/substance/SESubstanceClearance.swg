%{
#include <biogears/cdm/substance/SESubstanceClearance.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarNeg1To1.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEHistogramFractionVsLength.swg>

%import <biogears/cdm/properties/SEScalarFrequency.swg>  
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimeMass.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>

%nspace biogears::SESubstanceClearance;

%ignore biogears::SESubstanceClearance::Unload;
%ignore biogears::SESubstanceClearance::Load;

%ignore biogears::SESubstanceClearance::GetScalar(const std::string& name);

%rename (GetFractionExcretedInFeces_const) biogears::SESubstanceClearance::GetFractionExcretedInFeces() const;
%rename (GetFractionExcretedInUrine_const) biogears::SESubstanceClearance::GetFractionExcretedInUrine() const;
%rename (GetFractionMetabolizedInGut_const) biogears::SESubstanceClearance::GetFractionMetabolizedInGut() const;
%rename (GetFractionUnboundInPlasma_const) biogears::SESubstanceClearance::GetFractionUnboundInPlasma() const;
%rename (GetGlomerularFilterability_const) biogears::SESubstanceClearance::GetGlomerularFilterability() const;
%rename (GetRenalReabsorptionRatio_const) biogears::SESubstanceClearance::GetRenalReabsorptionRatio() const;

%include <biogears/cdm/substance/SESubstanceClearance.h>