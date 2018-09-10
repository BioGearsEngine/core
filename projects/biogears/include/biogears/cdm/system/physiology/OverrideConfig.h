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
CDM_BIND_DECL(OverrideConfigData)
#include <biogears/schema/SEPatient.hxx>
class OverrideConfigData;

class BIOGEARS_API OverrideConfig : public Loggable {
public:
  OverrideConfig(Logger* logger);
  virtual ~OverrideConfig();

  virtual void Clear(); // Deletes all members

  virtual bool LoadOverrideConfig(const CDM::OverrideConfigData& in);
  virtual CDM::OverrideConfigDataData* Unload() const;

protected:
  virtual void Unload(CDM::OverrideConfigData& data) const;

public:
  
protected:

  std::map<CDM::ElectroCardioGramWaveformLeadNumber, SEScalarElectricPotential*> m_Leads;
  std::map<CDM::ElectroCardioGramWaveformLeadNumber, std::map<CDM::enumHeartRhythm, SEElectroCardioGramInterpolatorWaveform*>> m_Waveforms;
};