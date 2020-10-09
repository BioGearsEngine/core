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
#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
SESubstanceNasalDose::SESubstanceNasalDose(const SESubstance& substance)
  : SESubstanceAdministration()
  , m_Substance(substance)
{
  m_Dose = nullptr;
}

SESubstanceNasalDose::~SESubstanceNasalDose()
{
  Clear();
}

void SESubstanceNasalDose::Clear()
{
  SESubstanceAdministration::Clear();
  SAFE_DELETE(m_Dose);

}

bool SESubstanceNasalDose::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasDose();
}

bool SESubstanceNasalDose::IsActive() const
{
  return IsValid();
}

bool SESubstanceNasalDose::Load(const CDM::SubstanceNasalDoseData& in)
{
  SESubstanceAdministration::Load(in);
  GetDose().Load(in.Dose());
  return true;
}

CDM::SubstanceNasalDoseData* SESubstanceNasalDose::Unload() const
{
  CDM::SubstanceNasalDoseData* data(new CDM::SubstanceNasalDoseData());
  Unload(*data);
  return data;
}

void SESubstanceNasalDose::Unload(CDM::SubstanceNasalDoseData& data) const
{
  SESubstanceAdministration::Unload(data);
  if (m_Dose != nullptr)
    data.Dose(std::unique_ptr<CDM::ScalarMassData>(m_Dose->Unload()));
  data.Substance(m_Substance.GetName());
}


bool SESubstanceNasalDose::HasDose() const
{
  return m_Dose == nullptr ? false : m_Dose->IsValid();
}
SEScalarMass& SESubstanceNasalDose::GetDose()
{
  if (m_Dose == nullptr)
    m_Dose = new SEScalarMass();
  return *m_Dose;
}

SESubstance& SESubstanceNasalDose::GetSubstance() const
{
  return (SESubstance&)m_Substance;
}

void SESubstanceNasalDose::ToString(std::ostream& str) const
{
  str << "Patient Action : Nasal Substance Administration";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << "\n\tDose: ";
  HasDose() ? str << *m_Dose : str << "No Dose Set";
  str << std::flush;
}

//-----------------------------------------------------------------------------
//Nasal State methods
SENasalState::SENasalState(const SESubstance& sub)
  : m_Substance(&sub)
{
  m_TotalNasalDose = nullptr;
  m_VenaCavaConcentration = nullptr;
  m_NumUnreleasedMasses = 3; //Hard-coded for current model implementation!
  m_NumReleasedMasses = 3; //Hard-coded for current model implementation!
}

SENasalState::~SENasalState()
{
  Clear();
}

void SENasalState::Clear()
{
  SAFE_DELETE(m_TotalNasalDose);
  SAFE_DELETE(m_VenaCavaConcentration);
  m_UnreleasedDrugMasses.clear();
  m_ReleasedDrugMasses.clear();
}

bool SENasalState::Initialize(SEScalarMass& dose)
{
  GetTotalNasalDose().Set(dose);
  GetVenaCavaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  std::vector<double> initUnreleasedMasses(m_NumUnreleasedMasses);
  std::vector<double> initReleasedMasses(m_NumReleasedMasses);
  bool unrelSet = SetUnreleasedNasalMasses(initUnreleasedMasses, MassUnit::mg);
  bool relSet = SetReleasedNasalMasses(initReleasedMasses, MassUnit::mg);
  return (unrelSet && relSet);
}

bool SENasalState::Load(const CDM::NasalStateData& in)
{

  GetTotalNasalDose().Load(in.TotalNasalDose());
  m_UnreleasedDrugMasses.clear();
  for (auto umData : in.UnreleasedDrugMasses()) {
    SEScalarMass* unrelMass = new SEScalarMass;
    unrelMass->Load(umData);
    m_UnreleasedDrugMasses.push_back(unrelMass);
  }
  m_ReleasedDrugMasses.clear();
  for (auto rmData : in.ReleasedDrugMasses()) {
    SEScalarMass* relMass = new SEScalarMass;
    relMass->Load(rmData);
    m_ReleasedDrugMasses.push_back(relMass);
  }
  return true;
}

CDM::NasalStateData* SENasalState::Unload() const
{
  CDM::NasalStateData* data = new CDM::NasalStateData();
  Unload(*data);
  return data;
}
void SENasalState::Unload(CDM::NasalStateData& data) const
{
  data.TotalNasalDose(std::unique_ptr<CDM::ScalarMassData>(m_TotalNasalDose->Unload()));
  for (auto umData : m_UnreleasedDrugMasses) {
    data.UnreleasedDrugMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(umData->Unload()));
  }
  for (auto rmData : m_ReleasedDrugMasses) {
    data.ReleasedDrugMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(rmData->Unload()));
  }
  data.Substance(m_Substance->GetName());
}

SEScalarMass& SENasalState::GetTotalNasalDose()
{
  if (m_TotalNasalDose == nullptr)
    m_TotalNasalDose = new SEScalarMass();
  return *m_TotalNasalDose;
}
SEScalarMassPerVolume& SENasalState::GetVenaCavaConcentration()
{
  if (m_VenaCavaConcentration == nullptr)
    m_VenaCavaConcentration = new SEScalarMassPerVolume();
  return *m_VenaCavaConcentration;
}

std::vector<SEScalarMass*>& SENasalState::GetUnreleasedNasalMasses()
{
  return m_UnreleasedDrugMasses;
}
std::vector<double> SENasalState::GetUnreleasedNasalMasses(const MassUnit& unit)
{
  std::vector<double> unrelMasses;
  for (auto uMass : m_UnreleasedDrugMasses) {
    unrelMasses.push_back(uMass->GetValue(unit));
  }
  return unrelMasses;
}
bool SENasalState::SetUnreleasedNasalMasses(std::vector<double>& uMasses, const MassUnit& unit)
{
  if (uMasses.size() != m_NumUnreleasedMasses) {
    //Don't execute if the input vector is not the correct size
    return false;
  }
  if (m_UnreleasedDrugMasses.empty()) {
    //The very first time we call this function, the member vector will be empty.  All values should initialize to 0
    for (size_t pos = 0; pos < m_NumUnreleasedMasses; pos++) {
      SEScalarMass* unrelMass = new SEScalarMass();
      unrelMass->SetValue(uMasses[0], unit);
      m_UnreleasedDrugMasses.push_back(unrelMass);
    }
  } else {
    //Vectors should be the same size, so we can iterate over them
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarMass*>::iterator scIt;
    for (dblIt = uMasses.begin(), scIt = m_UnreleasedDrugMasses.begin(); dblIt != uMasses.end() && scIt != m_UnreleasedDrugMasses.end(); dblIt++, scIt++) {
      (*scIt)->SetValue(*dblIt, unit);
    }
  }
  return true;
}

std::vector<SEScalarMass*>& SENasalState::GetReleasedNasalMasses()
{
  return m_ReleasedDrugMasses;
}
std::vector<double> SENasalState::GetReleasedNasalMasses(const MassUnit& unit)
{
  std::vector<double> releasedMasses;
  for (auto rMass : m_ReleasedDrugMasses) {
    releasedMasses.push_back(rMass->GetValue(unit));
  }
  return releasedMasses;
}
bool SENasalState::SetReleasedNasalMasses(std::vector<double>& rMasses, const MassUnit& unit)
{
  if (rMasses.size() != m_NumReleasedMasses) {
    //Don't execute if the input vector is not the correct size
    return false;
  }
  if (m_ReleasedDrugMasses.empty()) {
    //The very first time we call this function, the member vector will be empty.  All values should initialize to 0
    for (size_t pos = 0; pos < m_NumReleasedMasses; pos++) {
      SEScalarMass* relMass = new SEScalarMass();
      relMass->SetValue(rMasses[0], unit);
      m_ReleasedDrugMasses.push_back(relMass);
    }
  } else {
    //Vectors should be the same size, so we can iterate over them
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarMass*>::iterator scIt;
    for (dblIt = rMasses.begin(), scIt = m_ReleasedDrugMasses.begin(); dblIt != rMasses.end() && scIt != m_ReleasedDrugMasses.end(); dblIt++, scIt++) {
      (*scIt)->SetValue(*dblIt, unit);
    }
  }

  return true;
}



};