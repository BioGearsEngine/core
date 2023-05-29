// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarElectricPotential.swg>
%import <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.swg>

%import <biogears/cdm/scenario/SEAction.swg>

%rename(GetData_const) GetData() const;
%include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.h>

%nspace std::map<CDM::enumHeartRhythm, BIOGEARS_NAMESPACE SEElectroCardioGramInterpolatorWaveform*>;
%template (map_enumHeartRhythm__ECGWaveformLead_ptr) std::map<CDM::enumHeartRhythm, BIOGEARS_NAMESPACE SEElectroCardioGramInterpolatorWaveform*>;

//%nspace std::map<CDM::ElectroCardioGramWaveformLeadNumber, std::map<CDM::enumHeartRhythm, BIOGEARS_NAMESPACE SEElectroCardioGramInterpolatorWaveform*>>;
//%template (map_ECGWaveformLead__map_enumHeartRhythm__ECGWaveformLead_ptr) std::map<CDM::ElectroCardioGramWaveformLeadNumber, std::map<CDM::enumHeartRhythm, BIOGEARS_NAMESPACE SEElectroCardioGramInterpolatorWaveform*>>;
