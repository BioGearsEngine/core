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

#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>
#include <biogears/schema/cdm/ElectroCardioGram.hxx>

CDM_BIND_DECL(ElectroCardioGramInterpolationWaveformData)

namespace biogears {

namespace io {
  class ElectroCardioGram;
}
class BIOGEARS_API SEElectroCardioGramInterpolationWaveform : public Loggable {
  friend io::ElectroCardioGram;

public:
  SEElectroCardioGramInterpolationWaveform(Logger* logger);
  virtual ~SEElectroCardioGramInterpolationWaveform();

  virtual void Clear(); // Deletes all members

  virtual bool Load(const CDM::ElectroCardioGramInterpolationWaveformData& in);
  virtual CDM::ElectroCardioGramInterpolationWaveformData* Unload() const;

  bool operator==(SEElectroCardioGramInterpolationWaveform const&) const;
  bool operator!=(SEElectroCardioGramInterpolationWaveform const&) const;

protected:
  virtual void Unload(CDM::ElectroCardioGramInterpolationWaveformData& data) const;

public:
  virtual bool HasLeadNumber() const;
  virtual CDM::ElectroCardioGramWaveformLeadNumberData GetLeadNumber() const;
  virtual void SetLeadNumber(CDM::ElectroCardioGramWaveformLeadNumberData n);
  virtual void InvalidateLeadNumber();

  virtual CDM::enumHeartRhythm::value GetRhythm() const;
  virtual void SetRhythm(CDM::enumHeartRhythm::value name);
  virtual bool HasRhythm() const;
  virtual void InvalidateRhythm();

  virtual bool HasData() const;
  virtual SEFunctionElectricPotentialVsTime& GetData();
  virtual const SEFunctionElectricPotentialVsTime* GetData() const;

  virtual bool HasTimeStep() const;
  virtual SEScalarTime& GetTimeStep();
  virtual double GetTimeStep(const TimeUnit& unit) const;

  virtual std::vector<unsigned int>& GetActiveIndicies() { return m_ActiveIndicies; }

protected:
  CDM::ElectroCardioGramWaveformLeadNumberData m_LeadNumber;
  CDM::enumHeartRhythm::value m_Rhythm;
  SEScalarTime* m_TimeStep;
  SEFunctionElectricPotentialVsTime* m_Data;
  std::vector<unsigned int> m_ActiveIndicies;
};
}