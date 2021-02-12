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
#include <biogears/cdm/properties/SEProperty.h>
#include <biogears/cdm/utils/unitconversion/UCCommon.h>

CDM_BIND_DECL(HistogramData)

namespace biogears {
namespace io {
  class Property;
}
class BIOGEARS_API SEHistogram : public SEProperty {
public:
  friend io::Property;
  SEHistogram();
  virtual ~SEHistogram();

  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::HistogramData& in);
  virtual CDM::HistogramData* Unload() const;

protected:
  virtual void Unload(CDM::HistogramData& data) const;

public:
  virtual bool IsValid() const;
  virtual void Invalidate();

  virtual unsigned int NumberOfBins() const;
  virtual unsigned int NumberOfBoundaries() const;

  double GetDependentValue(unsigned int index) const;
  std::vector<double>& GetDependent();
  const std::vector<double>& GetDependent() const;

  double GetIndependentValue(unsigned int index) const;
  std::vector<double>& GetIndependent();
  const std::vector<double>& GetIndependent() const;

protected:
  std::vector<double> m_Dependent;
  std::vector<double> m_Independent;
};
}