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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/enums/SEPhysiologyEnums.h>
#include <biogears/schema/cdm/ElectroCardioGram.hxx>

CDM_BIND_DECL(ElectroCardioGramInterpolatorData)

namespace biogears {
class SEScalarElectricPotential;
class SEElectroCardioGramInterpolationWaveform;
namespace io {
  class ElectroCardioGram;
}
} // namespace biogears

namespace std {
BG_EXT template class BIOGEARS_API map<biogears::SEHeartRhythm, biogears::SEElectroCardioGramInterpolationWaveform*>;
BG_EXT template class BIOGEARS_API map<CDM::ElectroCardioGramWaveformLeadNumberData, biogears::SEScalarElectricPotential*>;
BG_EXT template class BIOGEARS_API map<CDM::ElectroCardioGramWaveformLeadNumberData, map<biogears::SEHeartRhythm, biogears::SEElectroCardioGramInterpolationWaveform*>>;
}

namespace biogears {
class SEElectroCardioGramInterpolator : public Loggable {
  friend io::ElectroCardioGram;

public:
  BIOGEARS_API SEElectroCardioGramInterpolator(Logger* logger);
  BIOGEARS_API virtual ~SEElectroCardioGramInterpolator();

  BIOGEARS_API virtual void Clear(); // Deletes all members

  BIOGEARS_API virtual bool LoadWaveforms(const char* file, const SEScalarTime* timeStep = nullptr);
  BIOGEARS_API virtual bool LoadWaveforms(const std::string& file, const SEScalarTime* timeStep = nullptr);
  
  BIOGEARS_API bool operator==(SEElectroCardioGramInterpolator const&) const;
  BIOGEARS_API bool operator!=(SEElectroCardioGramInterpolator const&) const;

public:
  BIOGEARS_API virtual void Interpolate(const SEScalarTime& timeStep);
  BIOGEARS_API virtual bool StartNewCycle(SEHeartRhythm rhythm);
  BIOGEARS_API virtual void CalculateWaveformsElectricPotential();

  // These are where the interpolator will put the interpolated electric potential data for each lead when you call CalculateWaveformsElectricPotential
  // You must have a waveform for the lead of the current rhythm for these scalars to be populated
  BIOGEARS_API virtual bool CanInterpolateLeadPotential(CDM::ElectroCardioGramWaveformLeadNumberData lead, SEHeartRhythm rhythm) const;
  BIOGEARS_API virtual void SetLeadElectricPotential(CDM::ElectroCardioGramWaveformLeadNumberData lead, SEScalarElectricPotential& ep);

  BIOGEARS_API virtual bool HasWaveform(CDM::ElectroCardioGramWaveformLeadNumberData lead, SEHeartRhythm rhythm) const;
  BIOGEARS_API virtual SEElectroCardioGramInterpolationWaveform& GetWaveform(CDM::ElectroCardioGramWaveformLeadNumberData lead, SEHeartRhythm rhythm);
  BIOGEARS_API virtual const SEElectroCardioGramInterpolationWaveform* GetWaveform(CDM::ElectroCardioGramWaveformLeadNumberData lead, SEHeartRhythm rhythm) const;
  BIOGEARS_API virtual void RemoveWaveform(CDM::ElectroCardioGramWaveformLeadNumberData lead, SEHeartRhythm rhythm);

protected:
  BIOGEARS_API virtual void Interpolate(SEElectroCardioGramInterpolationWaveform& waveform, const SEScalarTime& timeStep);

  std::map<CDM::ElectroCardioGramWaveformLeadNumberData, SEScalarElectricPotential*> m_Leads;
  std::map<CDM::ElectroCardioGramWaveformLeadNumberData, std::map<SEHeartRhythm, SEElectroCardioGramInterpolationWaveform*>> m_Waveforms;
};
}