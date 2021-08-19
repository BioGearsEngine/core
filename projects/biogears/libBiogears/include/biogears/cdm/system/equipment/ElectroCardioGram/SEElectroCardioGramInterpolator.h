/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/ElectroCardioGram.hxx>

CDM_BIND_DECL(ElectroCardioGramWaveformInterpolatorData)

namespace biogears {
class SEElectroCardioGramInterpolatorWaveform;
class SEScalarElectricPotential;

BG_EXT template class BIOGEARS_API std::map<CDM::enumHeartRhythm, SEElectroCardioGramInterpolatorWaveform*>;
BG_EXT template class BIOGEARS_API std::map<CDM::ElectroCardioGramWaveformLeadNumber, SEScalarElectricPotential*>;
BG_EXT template class BIOGEARS_API std::map<CDM::ElectroCardioGramWaveformLeadNumber, std::map<CDM::enumHeartRhythm, SEElectroCardioGramInterpolatorWaveform*>>;

namespace io {
  class ElectroCardioGram;
}

class  SEElectroCardioGramInterpolator : public Loggable {
  friend io::ElectroCardioGram;

public:
  BIOGEARS_API SEElectroCardioGramInterpolator(Logger* logger);
  BIOGEARS_API virtual ~SEElectroCardioGramInterpolator();

  BIOGEARS_API virtual void Clear(); // Deletes all members

  BIOGEARS_API virtual bool LoadWaveforms(const char* file, const SEScalarTime* timeStep = nullptr);
  BIOGEARS_API virtual bool LoadWaveforms(const std::string& file, const SEScalarTime* timeStep = nullptr);
  BIOGEARS_API virtual bool Load(const CDM::ElectroCardioGramWaveformInterpolatorData& in);
  BIOGEARS_API virtual CDM::ElectroCardioGramWaveformInterpolatorData* Unload() const;

protected:
  BIOGEARS_API virtual void Unload(CDM::ElectroCardioGramWaveformInterpolatorData& data) const;

public:
  BIOGEARS_API virtual void Interpolate(const SEScalarTime& timeStep);
  BIOGEARS_API virtual bool StartNewCycle(CDM::enumHeartRhythm::value rhythm);
  BIOGEARS_API virtual void CalculateWaveformsElectricPotential();

  // These are where the interpolator will put the interpolated electric potential data for each lead when you call CalculateWaveformsElectricPotential
  // You must have a waveform for the lead of the current rhythm for these scalars to be populated
  BIOGEARS_API virtual bool CanInterpolateLeadPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  BIOGEARS_API virtual void SetLeadElectricPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, SEScalarElectricPotential& ep);

  BIOGEARS_API virtual bool HasWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  BIOGEARS_API virtual SEElectroCardioGramInterpolatorWaveform& GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm);
  BIOGEARS_API virtual const SEElectroCardioGramInterpolatorWaveform* GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  BIOGEARS_API virtual void RemoveWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm);

protected:
  BIOGEARS_API virtual void Interpolate(SEElectroCardioGramInterpolatorWaveform& waveform, const SEScalarTime& timeStep);

  std::map<CDM::ElectroCardioGramWaveformLeadNumber, SEScalarElectricPotential*> m_Leads;
  std::map<CDM::ElectroCardioGramWaveformLeadNumber, std::map<CDM::enumHeartRhythm, SEElectroCardioGramInterpolatorWaveform*>> m_Waveforms;
};
}