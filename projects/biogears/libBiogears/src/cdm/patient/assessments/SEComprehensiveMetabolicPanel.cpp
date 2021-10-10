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
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>

namespace biogears {
SEComprehensiveMetabolicPanel::SEComprehensiveMetabolicPanel()
{
  m_Albumin = nullptr;
  m_ALP = nullptr;
  m_ALT = nullptr;
  m_AST = nullptr;
  m_BUN = nullptr;
  m_Calcium = nullptr;
  m_Chloride = nullptr;
  m_CO2 = nullptr;
  m_Creatinine = nullptr;
  m_Glucose = nullptr;
  m_Potassium = nullptr;
  m_Sodium = nullptr;
  m_TotalBilirubin = nullptr;
  m_TotalProtein = nullptr;
}

SEComprehensiveMetabolicPanel::~SEComprehensiveMetabolicPanel()
{
  Clear();
}

void SEComprehensiveMetabolicPanel::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_Albumin);
  SAFE_DELETE(m_ALP);
  SAFE_DELETE(m_ALT);
  SAFE_DELETE(m_AST);
  SAFE_DELETE(m_BUN);
  SAFE_DELETE(m_Calcium);
  SAFE_DELETE(m_Chloride);
  SAFE_DELETE(m_CO2);
  SAFE_DELETE(m_Creatinine);
  SAFE_DELETE(m_Glucose);
  SAFE_DELETE(m_Potassium);
  SAFE_DELETE(m_Sodium);
  SAFE_DELETE(m_TotalBilirubin);
  SAFE_DELETE(m_TotalProtein);
}

void SEComprehensiveMetabolicPanel::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_Albumin);
  INVALIDATE_PROPERTY(m_ALP);
  INVALIDATE_PROPERTY(m_ALT);
  INVALIDATE_PROPERTY(m_AST);
  INVALIDATE_PROPERTY(m_BUN);
  INVALIDATE_PROPERTY(m_Calcium);
  INVALIDATE_PROPERTY(m_Chloride);
  INVALIDATE_PROPERTY(m_CO2);
  INVALIDATE_PROPERTY(m_Creatinine);
  INVALIDATE_PROPERTY(m_Glucose);
  INVALIDATE_PROPERTY(m_Potassium);
  INVALIDATE_PROPERTY(m_Sodium);
  INVALIDATE_PROPERTY(m_TotalBilirubin);
  INVALIDATE_PROPERTY(m_TotalProtein);
}

bool SEComprehensiveMetabolicPanel::Load(const CDM::ComprehensiveMetabolicPanelData& in)
{
  SEPatientAssessment::Load(in);
  // TODO
  return true;
}

CDM::ComprehensiveMetabolicPanelData* SEComprehensiveMetabolicPanel::Unload()
{
  CDM::ComprehensiveMetabolicPanelData* data = new CDM::ComprehensiveMetabolicPanelData();
  Unload(*data);
  return data;
}

void SEComprehensiveMetabolicPanel::Unload(CDM::ComprehensiveMetabolicPanelData& data)
{
  SEPatientAssessment::Unload(data);
  if (HasAlbumin())
    data.Albumin(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Albumin->Unload()));
  if (HasALP())
    data.ALP(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_ALP->Unload()));
  if (HasALT())
    data.ALT(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_ALT->Unload()));
  if (HasAST())
    data.AST(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_AST->Unload()));
  if (HasBUN())
    data.BUN(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_BUN->Unload()));
  if (HasCalcium())
    data.Calcium(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Calcium->Unload()));
  if (HasChloride())
    data.Chloride(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_Chloride->Unload()));
  if (HasCO2())
    data.CO2(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_CO2->Unload()));
  if (HasCreatinine())
    data.Creatinine(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Creatinine->Unload()));
  if (HasGlucose())
    data.Glucose(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Glucose->Unload()));
  if (HasPotassium())
    data.Potassium(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_Potassium->Unload()));
  if (HasSodium())
    data.Sodium(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_Sodium->Unload()));
  if (HasTotalBilirubin())
    data.TotalBilirubin(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_TotalBilirubin->Unload()));
  if (HasTotalProtein())
    data.TotalProtein(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_TotalProtein->Unload()));
}

bool SEComprehensiveMetabolicPanel::HasAlbumin() const
{
  return m_Albumin == nullptr ? false : m_Albumin->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetAlbumin()
{
  if (m_Albumin == nullptr)
    m_Albumin = new SEScalarMassPerVolume();
  return *m_Albumin;
}

bool SEComprehensiveMetabolicPanel::HasALP() const
{
  return m_ALP == nullptr ? false : m_ALP->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetALP()
{
  if (m_ALP == nullptr)
    m_ALP = new SEScalarMassPerVolume();
  return *m_ALP;
}

bool SEComprehensiveMetabolicPanel::HasALT() const
{
  return m_ALT == nullptr ? false : m_ALT->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetALT()
{
  if (m_ALT == nullptr)
    m_ALT = new SEScalarMassPerVolume();
  return *m_ALT;
}

bool SEComprehensiveMetabolicPanel::HasAST() const
{
  return m_AST == nullptr ? false : m_AST->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetAST()
{
  if (m_AST == nullptr)
    m_AST = new SEScalarMassPerVolume();
  return *m_AST;
}

bool SEComprehensiveMetabolicPanel::HasBUN() const
{
  return m_BUN == nullptr ? false : m_BUN->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetBUN()
{
  if (m_BUN == nullptr)
    m_BUN = new SEScalarMassPerVolume();
  return *m_BUN;
}

bool SEComprehensiveMetabolicPanel::HasCalcium() const
{
  return m_Calcium == nullptr ? false : m_Calcium->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetCalcium()
{
  if (m_Calcium == nullptr)
    m_Calcium = new SEScalarMassPerVolume();
  return *m_Calcium;
}

bool SEComprehensiveMetabolicPanel::HasChloride() const
{
  return m_Chloride == nullptr ? false : m_Chloride->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetChloride()
{
  if (m_Chloride == nullptr)
    m_Chloride = new SEScalarAmountPerVolume();
  return *m_Chloride;
}

bool SEComprehensiveMetabolicPanel::HasCO2() const
{
  return m_CO2 == nullptr ? false : m_CO2->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetCO2()
{
  if (m_CO2 == nullptr)
    m_CO2 = new SEScalarAmountPerVolume();
  return *m_CO2;
}

bool SEComprehensiveMetabolicPanel::HasCreatinine() const
{
  return m_Creatinine == nullptr ? false : m_Creatinine->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetCreatinine()
{
  if (m_Creatinine == nullptr)
    m_Creatinine = new SEScalarMassPerVolume();
  return *m_Creatinine;
}

bool SEComprehensiveMetabolicPanel::HasGlucose() const
{
  return m_Glucose == nullptr ? false : m_Glucose->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetGlucose()
{
  if (m_Glucose == nullptr)
    m_Glucose = new SEScalarMassPerVolume();
  return *m_Glucose;
}

bool SEComprehensiveMetabolicPanel::HasPotassium() const
{
  return m_Potassium == nullptr ? false : m_Potassium->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetPotassium()
{
  if (m_Potassium == nullptr)
    m_Potassium = new SEScalarAmountPerVolume();
  return *m_Potassium;
}

bool SEComprehensiveMetabolicPanel::HasSodium() const
{
  return m_Sodium == nullptr ? false : m_Sodium->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetSodium()
{
  if (m_Sodium == nullptr)
    m_Sodium = new SEScalarAmountPerVolume();
  return *m_Sodium;
}

bool SEComprehensiveMetabolicPanel::HasTotalBilirubin() const
{
  return m_TotalBilirubin == nullptr ? false : m_TotalBilirubin->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetTotalBilirubin()
{
  if (m_TotalBilirubin == nullptr)
    m_TotalBilirubin = new SEScalarMassPerVolume();
  return *m_TotalBilirubin;
}

bool SEComprehensiveMetabolicPanel::HasTotalProtein() const
{
  return m_TotalProtein == nullptr ? false : m_TotalProtein->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetTotalProtein()
{
  if (m_TotalProtein == nullptr)
    m_TotalProtein = new SEScalarMassPerVolume();
  return *m_TotalProtein;
}
}