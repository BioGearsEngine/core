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
#include <biogears/cdm/properties/SEHistogram.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class LengthUnit;

class BIOGEARS_API SEHistogramFractionVsLength : public SEHistogram {
public:
  SEHistogramFractionVsLength();
  virtual ~SEHistogramFractionVsLength();

  virtual void Clear() override;

  virtual bool IsVaild() const;

  virtual bool Load(const CDM::HistogramFractionVsLengthData& in);
  virtual CDM::HistogramFractionVsLengthData* Unload() const override;

protected:
  virtual void Unload(CDM::HistogramFractionVsLengthData& data) const;

public:
  virtual double GetFractionValue(unsigned int index) const;
  virtual std::vector<double>& GetFraction();
  virtual const std::vector<double>& GetFraction() const;

  virtual double GetLengthValue(unsigned int index, const LengthUnit& unit) const;
  virtual std::vector<double>& GetLength();
  virtual const std::vector<double>& GetLength() const;
  virtual const LengthUnit* GetLengthUnit() const;
  virtual void SetLengthUnit(const LengthUnit& unit);

protected:
  const LengthUnit* m_LengthUnit;
};
}