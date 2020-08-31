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
#include "biogears/cdm/properties/SEScalar.h"
#include <biogears/schema/cdm/PatientAssessments.hxx>

namespace biogears {
class SEScalar;

class BIOGEARS_API SESequentialOrganFailureAssessment : public SEPatientAssessment {
public:
  SESequentialOrganFailureAssessment();
  virtual ~SESequentialOrganFailureAssessment();
  
  static constexpr const char* TypeTag() {return "SESequentialOrganFailureAssessment"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::SequentialOrganFailureAssessmentData& in);
  virtual CDM::SequentialOrganFailureAssessmentData* Unload();

protected:
  virtual void Unload(CDM::SequentialOrganFailureAssessmentData& data);

public:
  virtual bool HasRespirationSOFA();
  virtual SEScalar& GetRespirationSOFA();

  virtual bool HasCoagulationSOFA();
  virtual SEScalar& GetCoagulationSOFA();

  virtual bool HasLiverSOFA();
  virtual SEScalar& GetLiverSOFA();

  virtual bool HasCardiovascularSOFA();
  virtual SEScalar& GetCardiovascularSOFA();

  virtual bool HasCentralNervousSOFA();
  virtual SEScalar& GetCentralNervousSOFA();

  virtual bool HasRenalSOFA();
  virtual SEScalar& GetRenalSOFA();

protected:
  SEScalar* m_RespirationSOFA;
  SEScalar* m_CoagulationSOFA;
  SEScalar* m_LiverSOFA;
  SEScalar* m_CardiovascularSOFA;
  SEScalar* m_CentralNervousSOFA;
  SEScalar* m_RenalSOFA;

};
}