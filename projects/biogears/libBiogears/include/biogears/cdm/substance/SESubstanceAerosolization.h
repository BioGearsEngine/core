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
#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
class SEScalar;
class SEScalarNeg1To1;
class SEScalar0To1;
class SEHistogramFractionVsLength;
namespace io {
  class Substance;
}
class BIOGEARS_API SESubstanceAerosolization : public Loggable {
  friend io::Substance;
public:
  SESubstanceAerosolization(Logger* logger);
  virtual ~SESubstanceAerosolization();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::SubstanceAerosolizationData& in);
  virtual CDM::SubstanceAerosolizationData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceAerosolizationData& data) const;

public:
  virtual bool HasBronchioleModifier() const;
  virtual SEScalarNeg1To1& GetBronchioleModifier();
  virtual double GetBronchioleModifier() const;

  virtual bool HasInflammationCoefficient() const;
  virtual SEScalar0To1& GetInflammationCoefficient();
  virtual double GetInflammationCoefficient() const;

  virtual bool HasParticulateSizeDistribution() const;
  virtual SEHistogramFractionVsLength& GetParticulateSizeDistribution();
  virtual const SEHistogramFractionVsLength* GetParticulateSizeDistribution() const;

protected:
  SEScalarNeg1To1* m_BronchioleModifier;
  SEScalar0To1* m_InflammationCoefficient;
  SEHistogramFractionVsLength* m_ParticulateSizeDistribution;
};
}