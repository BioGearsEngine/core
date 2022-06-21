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
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/schema/cdm/PatientAssessments.hxx>

namespace biogears {
class SEBloodChemistry;
class SEScalar;

namespace io {
  class PatientAssessments;
}

class BIOGEARS_API SEProthrombinTime : public SEPatientAssessment {
friend io::PatientAssessments;
public:
  SEProthrombinTime();
  virtual ~SEProthrombinTime();

  static constexpr const char* TypeTag() { return "SEProthrombinTime"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::ProthrombinTimeData& in);
  virtual CDM::ProthrombinTimeData* Unload();

protected:
  virtual void Unload(CDM::ProthrombinTimeData& data);

public:
  virtual bool HasInternationalNormalizedRatio();
  virtual SEScalar& GetInternationalNormalizedRatio();

protected:
  SEScalar* m_InternationalNormalizedRatio;
};
}