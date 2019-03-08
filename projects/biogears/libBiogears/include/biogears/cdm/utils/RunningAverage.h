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
CDM_BIND_DECL(RunningAverageData);

namespace biogears {
class BIOGEARS_API RunningAverage {
public:
  RunningAverage();
  virtual ~RunningAverage();

  bool Load(const CDM::RunningAverageData& in);
  CDM::RunningAverageData* Unload() const;

protected:
  void Unload(CDM::RunningAverageData& data) const;

public:
  /** Reset Counter info */
  void Reset();
  /** Add value to Sample list and return the new average */
  double Sample(double d);
  /** Return the average value */
  double Value();

  unsigned int NumSamples() { return m_NumSamples; }

protected:
  double m_Sum;
  unsigned int m_NumSamples;
};
}