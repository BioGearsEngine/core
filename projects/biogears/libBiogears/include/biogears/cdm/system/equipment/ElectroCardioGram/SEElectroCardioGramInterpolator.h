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
namespace io {
  class ElectroCardioGram;
}
class BIOGEARS_API SEElectroCardioGramInterpolator : public Loggable {
  friend io::ElectroCardioGram;

public:
  SEElectroCardioGramInterpolator(Logger* logger);
  virtual ~SEElectroCardioGramInterpolator();

  virtual void Clear(); // Deletes all members

  virtual bool LoadWaveforms(const char* file, const SEScalarTime* timeStep = nullptr);
  virtual bool LoadWaveforms(const std::string& file, const SEScalarTime* timeStep = nullptr);
  virtual bool Load(const CDM::ElectroCardioGramWaveformInterpolatorData& in);
  virtual CDM::ElectroCardioGramWaveformInterpolatorData* Unload() const;

protected:
  virtual void Unload(CDM::ElectroCardioGramWaveformInterpolatorData& data) const;

public:
  virtual void Interpolate(const SEScalarTime& timeStep);
  virtual bool StartNewCycle(CDM::enumHeartRhythm::value rhythm);
  virtual void CalculateWaveformsElectricPotential();

  // These are where the interpolator will put the interpolated electric potential data for each lead when you call CalculateWaveformsElectricPotential
  // You must have a waveform for the lead of the current rhythm for these scalars to be populated
  virtual bool CanInterpolateLeadPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  virtual void SetLeadElectricPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, SEScalarElectricPotential& ep);

  virtual bool HasWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  virtual SEElectroCardioGramInterpolatorWaveform& GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm);
  virtual const SEElectroCardioGramInterpolatorWaveform* GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const;
  virtual void RemoveWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm);

protected:
  virtual void Interpolate(SEElectroCardioGramInterpolatorWaveform& waveform, const SEScalarTime& timeStep);

  std::map<CDM::ElectroCardioGramWaveformLeadNumber, SEScalarElectricPotential*> m_Leads;
  std::map<CDM::ElectroCardioGramWaveformLeadNumber, std::map<CDM::enumHeartRhythm, SEElectroCardioGramInterpolatorWaveform*>> m_Waveforms;
};
}