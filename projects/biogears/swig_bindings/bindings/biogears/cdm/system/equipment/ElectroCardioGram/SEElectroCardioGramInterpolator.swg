// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarElectricPotential.swg>

%import <biogears/cdm/scenario/SEAction.swg>

%import <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.swg>

  %ignore  LoadWaveforms(const std::string& file, const SEScalarTime* timeStep = nullptr);
  %rename(CanInterpolateLeadPotential_const) CanInterpolateLeadPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  %rename(GetWaveform_const) GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;

%include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
