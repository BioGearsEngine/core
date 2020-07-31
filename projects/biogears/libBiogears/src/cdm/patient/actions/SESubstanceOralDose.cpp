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
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceOralDose::SESubstanceOralDose(const SESubstance& substance)
  : SESubstanceAdministration()
  , m_Substance(substance)
{
  m_AdminRoute = (CDM::enumOralAdministration::value)-1;
  m_Dose = nullptr;
}

SESubstanceOralDose::~SESubstanceOralDose()
{
  Clear();
}

void SESubstanceOralDose::Clear()
{
  SESubstanceAdministration::Clear();
  m_AdminRoute = (CDM::enumOralAdministration::value)-1;
  SAFE_DELETE(m_Dose);
}

bool SESubstanceOralDose::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasDose() && HasAdminRoute();
}

bool SESubstanceOralDose::IsActive() const
{
  return IsValid();
}

bool SESubstanceOralDose::Load(const CDM::SubstanceOralDoseData& in)
{
  SESubstanceAdministration::Load(in);
  GetDose().Load(in.Dose());
  m_AdminRoute = in.AdminRoute();
  return true;
}

CDM::SubstanceOralDoseData* SESubstanceOralDose::Unload() const
{
  CDM::SubstanceOralDoseData* data(new CDM::SubstanceOralDoseData());
  Unload(*data);
  return data;
}

void SESubstanceOralDose::Unload(CDM::SubstanceOralDoseData& data) const
{
  SESubstanceAdministration::Unload(data);
  if (m_Dose != nullptr)
    data.Dose(std::unique_ptr<CDM::ScalarMassData>(m_Dose->Unload()));
  if (HasAdminRoute())
    data.AdminRoute(m_AdminRoute);
  data.Substance(m_Substance.GetName());
}

CDM::enumOralAdministration::value SESubstanceOralDose::GetAdminRoute() const
{
  return m_AdminRoute;
}
void SESubstanceOralDose::SetAdminRoute(CDM::enumOralAdministration::value route)
{
  m_AdminRoute = route;
}
bool SESubstanceOralDose::HasAdminRoute() const
{
  return m_AdminRoute == ((CDM::enumOralAdministration::value)-1) ? false : true;
}

bool SESubstanceOralDose::HasDose() const
{
  return m_Dose == nullptr ? false : m_Dose->IsValid();
}
SEScalarMass& SESubstanceOralDose::GetDose()
{
  if (m_Dose == nullptr)
    m_Dose = new SEScalarMass();
  return *m_Dose;
}

SESubstance& SESubstanceOralDose::GetSubstance() const
{
  return (SESubstance&)m_Substance;
}

void SESubstanceOralDose::ToString(std::ostream& str) const
{
  str << "Patient Action : Oral Substance Administration";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << "\n\tAdministration Route: ";
  HasAdminRoute() ? str << GetAdminRoute() : str << "Not Set";
  str << "\n\tDose: ";
  HasDose() ? str << *m_Dose : str << "No Dose Set";
  str << std::flush;
}

//-----------------------------------------------------------------------------
// Oral Transmucosal State methods
SETransmucosalState::SETransmucosalState(const SESubstance& sub)
  : m_Substance(&sub)
{
  m_MouthSolidMass = nullptr;
  m_SalivaConcentration = nullptr;
  m_NumBuccalRegions = 7; //Hard-coded for current model implementation!
  m_NumSublingualRegions = 7; //Hard-coded for current model implementation!
}

SETransmucosalState::~SETransmucosalState()
{
  Clear();
}

void SETransmucosalState::Clear()
{
  SAFE_DELETE(m_MouthSolidMass);
  SAFE_DELETE(m_SalivaConcentration);
  m_BuccalConcentrations.clear();
  m_SublingualConcentrations.clear();
}

bool SETransmucosalState::Initialize(SEScalarMass& dose)
{
  GetMouthSolidMass().Set(dose);
  GetSalivaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  std::vector<double> initBuccal(m_NumBuccalRegions);
  std::vector<double> initSublingual(m_NumSublingualRegions);
  bool bucSet = SetBuccalConcentrations(initBuccal, MassPerVolumeUnit::ug_Per_mL);
  bool subSet = SetSublingualConcentrations(initSublingual, MassPerVolumeUnit::ug_Per_mL);
  return (bucSet && subSet);
}

bool SETransmucosalState::Load(const CDM::TransmucosalStateData& in)
{

  GetMouthSolidMass().Load(in.MouthSolidMass());
  GetSalivaConcentration().Load(in.SalivaConcentration());
  m_BuccalConcentrations.clear();
  for (auto brData : in.BuccalConcentrations()) {
    SEScalarMassPerVolume* buc = new SEScalarMassPerVolume;
    buc->Load(brData);
    m_BuccalConcentrations.push_back(buc);
  }
  m_SublingualConcentrations.clear();
  for (auto slData : in.SublingualConcentrations()) {
    SEScalarMassPerVolume* sl = new SEScalarMassPerVolume;
    sl->Load(slData);
    m_SublingualConcentrations.push_back(sl);
  }
  return true;
}

CDM::TransmucosalStateData* SETransmucosalState::Unload() const
{
  CDM::TransmucosalStateData* data = new CDM::TransmucosalStateData();
  Unload(*data);
  return data;
}
void SETransmucosalState::Unload(CDM::TransmucosalStateData& data) const
{
  data.MouthSolidMass(std::unique_ptr<CDM::ScalarMassData>(m_MouthSolidMass->Unload()));
  data.SalivaConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_SalivaConcentration->Unload()));
  for (auto bcData : m_BuccalConcentrations) {
    data.BuccalConcentrations().push_back(std::unique_ptr<CDM::ScalarMassPerVolumeData>(bcData->Unload()));
  }
  for (auto slData : m_SublingualConcentrations) {
    data.SublingualConcentrations().push_back(std::unique_ptr<CDM::ScalarMassPerVolumeData>(slData->Unload()));
  }
  data.Substance(m_Substance->GetName());
}

SEScalarMass& SETransmucosalState::GetMouthSolidMass()
{
  if (m_MouthSolidMass == nullptr)
    m_MouthSolidMass = new SEScalarMass();
  return *m_MouthSolidMass;
}
SEScalarMassPerVolume& SETransmucosalState::GetSalivaConcentration()
{
  if (m_SalivaConcentration == nullptr)
    m_SalivaConcentration = new SEScalarMassPerVolume();
  return *m_SalivaConcentration;
}
std::vector<SEScalarMassPerVolume*>& SETransmucosalState::GetBuccalConcentrations()
{
  return m_BuccalConcentrations;
}
std::vector<double> SETransmucosalState::GetBuccalConcentrations(const MassPerVolumeUnit& unit)
{
  std::vector<double> buccalMasses;
  for (auto bMass : m_BuccalConcentrations) {
    buccalMasses.push_back(bMass->GetValue(unit));
  }
  return buccalMasses;
}
bool SETransmucosalState::SetBuccalConcentrations(std::vector<double>& bMasses, const MassPerVolumeUnit& unit)
{
  if (bMasses.size() != m_NumBuccalRegions) {
    //Don't execute if the input vector is not the correct size
    return false;
  }
  if (m_BuccalConcentrations.empty()) {
    //The very first time we call this function, the member vector will be empty.  All values should initialize to 0
    for (size_t pos = 0; pos < m_NumBuccalRegions; pos++) {
      SEScalarMassPerVolume* brMass = new SEScalarMassPerVolume();
      brMass->SetValue(bMasses[0], unit);
      m_BuccalConcentrations.push_back(brMass);
    }
  } else {
    //Vectors should be the same size, so we can iterate over them
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarMassPerVolume*>::iterator scIt;
    for (dblIt = bMasses.begin(), scIt = m_BuccalConcentrations.begin(); dblIt != bMasses.end() && scIt != m_BuccalConcentrations.end(); dblIt++, scIt++) {
      (*scIt)->SetValue(*dblIt, unit);
    }
  }
  return true;
}

std::vector<SEScalarMassPerVolume*>& SETransmucosalState::GetSublingualConcentrations()
{
  return m_SublingualConcentrations;
}
std::vector<double> SETransmucosalState::GetSublingualConcentrations(const MassPerVolumeUnit& unit)
{
  std::vector<double> sublingualMasses;
  for (auto slMass : m_SublingualConcentrations) {
    sublingualMasses.push_back(slMass->GetValue(unit));
  }
  return sublingualMasses;
}
bool SETransmucosalState::SetSublingualConcentrations(std::vector<double>& slMasses, const MassPerVolumeUnit& unit)
{
  if (slMasses.size() != m_NumSublingualRegions) {
    //Don't execute if the input vector is not the correct size
    return false;
  }
  if (m_SublingualConcentrations.empty()) {
    //The very first time we call this function, the member vector will be empty.  All values should initialize to 0
    for (size_t pos = 0; pos < m_NumSublingualRegions; pos++) {
      SEScalarMassPerVolume* slMass = new SEScalarMassPerVolume();
      slMass->SetValue(slMasses[0], unit);
      m_SublingualConcentrations.push_back(slMass);
    }
  } else {
    //Vectors should be the same size, so we can iterate over them
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarMassPerVolume*>::iterator scIt;
    for (dblIt = slMasses.begin(), scIt = m_SublingualConcentrations.begin(); dblIt != slMasses.end() && scIt != m_SublingualConcentrations.end(); dblIt++, scIt++) {
      (*scIt)->SetValue(*dblIt, unit);
    }
  }

  return true;
}

};