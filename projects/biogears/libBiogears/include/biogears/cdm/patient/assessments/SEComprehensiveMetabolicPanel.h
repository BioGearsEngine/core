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
class SEBloodChemistrySystem;
class SEScalarMassPerVolume;
class SEScalarAmountPerVolume;

class BIOGEARS_API SEComprehensiveMetabolicPanel : public SEPatientAssessment {
public:
  SEComprehensiveMetabolicPanel(Logger* logger);
  virtual ~SEComprehensiveMetabolicPanel();

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::ComprehensiveMetabolicPanelData& in);
  virtual CDM::ComprehensiveMetabolicPanelData* Unload();

protected:
  virtual void Unload(CDM::ComprehensiveMetabolicPanelData& data);

public:
  bool HasAlbumin();
  SEScalarMassPerVolume& GetAlbumin();

  bool HasALP();
  SEScalarMassPerVolume& GetALP();

  bool HasALT();
  SEScalarMassPerVolume& GetALT();

  bool HasAST();
  SEScalarMassPerVolume& GetAST();

  bool HasBUN();
  SEScalarMassPerVolume& GetBUN();

  bool HasCalcium();
  SEScalarMassPerVolume& GetCalcium();

  bool HasChloride();
  SEScalarAmountPerVolume& GetChloride();

  bool HasCO2();
  SEScalarAmountPerVolume& GetCO2();

  bool HasCreatinine();
  SEScalarMassPerVolume& GetCreatinine();

  bool HasGlucose();
  SEScalarMassPerVolume& GetGlucose();

  bool HasPotassium();
  SEScalarAmountPerVolume& GetPotassium();

  bool HasSodium();
  SEScalarAmountPerVolume& GetSodium();

  bool HasTotalBilirubin();
  SEScalarMassPerVolume& GetTotalBilirubin();

  bool HasTotalProtein();
  SEScalarMassPerVolume& GetTotalProtein();

protected:
  SEScalarMassPerVolume* m_Albumin;
  SEScalarMassPerVolume* m_ALP;
  SEScalarMassPerVolume* m_ALT;
  SEScalarMassPerVolume* m_AST;
  SEScalarMassPerVolume* m_BUN;
  SEScalarMassPerVolume* m_Calcium;
  SEScalarAmountPerVolume* m_Chloride;
  SEScalarAmountPerVolume* m_CO2;
  SEScalarMassPerVolume* m_Creatinine;
  SEScalarMassPerVolume* m_Glucose;
  SEScalarAmountPerVolume* m_Potassium;
  SEScalarAmountPerVolume* m_Sodium;
  SEScalarMassPerVolume* m_TotalBilirubin;
  SEScalarMassPerVolume* m_TotalProtein;
};
}