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
#include <biogears/cdm/properties/SEFunction.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

namespace biogears {
class BIOGEARS_API SEFunctionVolumeVsTime : public SEFunction {
public:
  SEFunctionVolumeVsTime();
  virtual ~SEFunctionVolumeVsTime();

  virtual void Clear();

  virtual bool Load(const CDM::FunctionVolumeVsTimeData& in);
  virtual CDM::FunctionVolumeVsTimeData* Unload() const;

protected:
  virtual void Unload(CDM::FunctionVolumeVsTimeData& data) const;

public:
  double GetIndependentValue(unsigned int index) = delete;
  virtual double GetTimeValue(unsigned int index, const TimeUnit& unit);
  virtual std::vector<double>& GetTime();
  virtual const TimeUnit* GetTimeUnit();
  virtual void SetTimeUnit(const TimeUnit& unit);

  double GetDependentValue(unsigned int index) = delete;
  virtual double GetVolumeValue(unsigned int index, const VolumeUnit& unit);
  virtual std::vector<double>& GetVolume();
  virtual const VolumeUnit* GetVolumeUnit();
  virtual void SetVolumeUnit(const VolumeUnit& unit);

protected:
  const TimeUnit* m_TimeUnit;
  const VolumeUnit* m_VolumeUnit;
};
}