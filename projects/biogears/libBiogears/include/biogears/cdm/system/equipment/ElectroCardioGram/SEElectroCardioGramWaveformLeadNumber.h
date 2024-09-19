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

class BIOGEARS_API SEElectroCardioGramWaveformLeadNumber : public Loggable {
  friend class io::ElectroCardioGram;
  friend bool operator==(int const&, SEElectroCardioGramWaveformLeadNumber const&);
  friend bool operator==(SEElectroCardioGramWaveformLeadNumber const&, int const&);

public:
  SEElectroCardioGramWaveformLeadNumber(Logger* logger);
  SEElectroCardioGramWaveformLeadNumber(Logger* logger, int value);
  ~SEElectroCardioGramWaveformLeadNumber() override;

  operator int&();
  operator int() const;
 
  SEElectroCardioGramWaveformLeadNumber& operator=(SEElectroCardioGramWaveformLeadNumber const&);
  bool operator==(SEElectroCardioGramWaveformLeadNumber const&) const;
  bool operator!=(SEElectroCardioGramWaveformLeadNumber const&) const;

public:
  int GetValue() const;
  void SetValue(int);

private:
  int m_value;
};
bool operator==(SEElectroCardioGramWaveformLeadNumber const& lhs, int const& rhs);
bool operator!=(SEElectroCardioGramWaveformLeadNumber const& lhs, int const& rhs);
bool operator==(int const& lhs, SEElectroCardioGramWaveformLeadNumber const& rhs);
bool operator!=(int const& lhs, SEElectroCardioGramWaveformLeadNumber const& rhs);
}