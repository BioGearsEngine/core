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
#include <biogears/schema/CommonDataModel.hxx>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>
CDM_BIND_DECL(ElectroCardioGramInterpolationWaveformData)

namespace biogears {

namespace io {
  class ElectroCardioGram;
}
class SEElectroCardioGramInterpolatorWaveform : public Loggable {
  friend io::ElectroCardioGram;

public:
  BIOGEARS_API SEElectroCardioGramInterpolatorWaveform(Logger const* logger);
  BIOGEARS_API virtual ~SEElectroCardioGramInterpolatorWaveform();

  BIOGEARS_API virtual void Clear(); // Deletes all members

  BIOGEARS_API virtual bool Load(const CDM::ElectroCardioGramInterpolationWaveformData& in);
  BIOGEARS_API virtual CDM::ElectroCardioGramInterpolationWaveformData* Unload() const;

protected:
  BIOGEARS_API virtual void Unload(CDM::ElectroCardioGramInterpolationWaveformData& data) const;

public:
  BIOGEARS_API virtual bool HasLeadNumber() const;
  BIOGEARS_API virtual CDM::ElectroCardioGramWaveformLeadNumber GetLeadNumber() const;
  BIOGEARS_API virtual void SetLeadNumber(CDM::ElectroCardioGramWaveformLeadNumber n);
  BIOGEARS_API virtual void InvalidateLeadNumber();

  BIOGEARS_API virtual CDM::enumHeartRhythm::value GetRhythm() const;
  BIOGEARS_API virtual void SetRhythm(CDM::enumHeartRhythm::value name);
  BIOGEARS_API virtual bool HasRhythm() const;
  BIOGEARS_API virtual void InvalidateRhythm();
   
  BIOGEARS_API virtual bool HasData() const;
  BIOGEARS_API virtual SEFunctionElectricPotentialVsTime& GetData();
  BIOGEARS_API virtual const SEFunctionElectricPotentialVsTime* GetData() const;
   
  BIOGEARS_API virtual bool HasTimeStep() const;
  BIOGEARS_API virtual SEScalarTime& GetTimeStep();
  BIOGEARS_API virtual double GetTimeStep(const TimeUnit& unit) const;
   
  BIOGEARS_API virtual std::vector<unsigned int>& GetActiveIndicies() { return m_ActiveIndicies; }

private:
  CDM::ElectroCardioGramWaveformLeadNumber m_LeadNumber;
  CDM::enumHeartRhythm::value m_Rhythm;
  SEScalarTime* m_TimeStep;
  SEFunctionElectricPotentialVsTime* m_Data;
  std::vector<unsigned int> m_ActiveIndicies;
};
}