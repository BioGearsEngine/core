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
#include "biogears/cdm/properties/SEScalarMassPerAmount.h"
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/schema/cdm/PatientAssessments.hxx>

namespace biogears {
class SEScalarFraction;
class SEScalarMassPerVolume;
class SEScalarMassPerAmount;
class SEScalarVolume;
class SEScalarAmountPerVolume;
namespace io {
  class PatientAssessments;
}
class BIOGEARS_API SECompleteBloodCount : public SEPatientAssessment {
  friend io::PatientAssessments;

public:
  SECompleteBloodCount();
  virtual ~SECompleteBloodCount();

  static constexpr const char* TypeTag() { return "SECompleteBloodCount"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset() override; //reset values
  virtual void Clear() override; //clear memory

  virtual bool Load(const CDM::CompleteBloodCountData& in);
  virtual CDM::CompleteBloodCountData* Unload() override;

protected:
  virtual void Unload(CDM::CompleteBloodCountData& data);

public:
  virtual bool HasHematocrit() const;
  virtual SEScalarFraction& GetHematocrit();

  virtual bool HasHemoglobin() const;
  virtual SEScalarMassPerVolume& GetHemoglobin();

  virtual bool HasLymphocyteCellCount() const;
  virtual SEScalarAmountPerVolume& GetLymphocyteCellCount();

  virtual bool HasPlateletCount() const;
  virtual SEScalarAmountPerVolume& GetPlateletCount();

  virtual bool HasMeanCorpuscularHemoglobin() const;
  virtual SEScalarMassPerAmount& GetMeanCorpuscularHemoglobin();

  virtual bool HasMeanCorpuscularHemoglobinConcentration() const;
  virtual SEScalarMassPerVolume& GetMeanCorpuscularHemoglobinConcentration();

  virtual bool HasMeanCorpuscularVolume() const;
  virtual SEScalarVolume& GetMeanCorpuscularVolume();

  virtual bool HasNeutrophilCount() const;
  virtual SEScalarAmountPerVolume& GetNeutrophilCount();

  virtual bool HasRedBloodCellCount() const;
  virtual SEScalarAmountPerVolume& GetRedBloodCellCount();

  virtual bool HasWhiteBloodCellCount() const;
  virtual SEScalarAmountPerVolume& GetWhiteBloodCellCount();

protected:
  SEScalarFraction* m_Hematocrit;
  SEScalarMassPerVolume* m_Hemoglobin;
  SEScalarAmountPerVolume* m_LymphocyteCellCount;
  SEScalarAmountPerVolume* m_PlateletCount;
  SEScalarMassPerAmount* m_MeanCorpuscularHemoglobin;
  SEScalarMassPerVolume* m_MeanCorpuscularHemoglobinConcentration;
  SEScalarVolume* m_MeanCorpuscularVolume;
  SEScalarAmountPerVolume* m_NeutrophilCellCount;
  SEScalarAmountPerVolume* m_RedBloodCellCount;
  SEScalarAmountPerVolume* m_WhiteBloodCellCount;
};
}