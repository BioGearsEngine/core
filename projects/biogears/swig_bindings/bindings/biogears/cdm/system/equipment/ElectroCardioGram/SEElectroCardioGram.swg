// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarElectricPotential.swg>

%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/scenario/SEAction.swg>

 %ignore LoadWaveforms(const std::string& file, const SEScalarTime* timeStep = nullptr);
 %ignore GetScalar(std::string const&);
 %rename(GetWaveform_const) GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;

%include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>