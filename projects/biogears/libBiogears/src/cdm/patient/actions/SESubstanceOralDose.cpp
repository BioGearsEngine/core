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
  str << "Patient Action : Oral Substance Adminstration";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << "\n\tAdministration Route: ";
  HasAdminRoute() ? str << GetAdminRoute() : str << "Not Set";
  str << "\n\tDose: ";
  HasDose() ? str << *m_Dose : str << "No Dose Set";
  str << std::flush;
}

//-----------------------------------------------------------------------------
//Oral Substance State data methods--makes calls to OralTransmucosalState and OralGastrointestinalState data defined below
SESubstanceOralState::SESubstanceOralState(const SESubstance& sub)
  : m_Substance(sub)
{
  m_TransmucosalState = nullptr;
  m_GastrointestinalState = nullptr;
  m_StomachDissolvedMass = nullptr;
  //m_EnterocyteMass = nullptr;
  //m_VilliCapillaryConcentration = nullptr;
  m_NumTransitMasses = 8; //Hard-coded for current model implementation!
}

SESubstanceOralState::~SESubstanceOralState()
{
  Clear();
}

void SESubstanceOralState::Clear()
{
  if (m_TransmucosalState != nullptr)
    m_TransmucosalState->Clear();
  if (m_GastrointestinalState != nullptr)
    m_GastrointestinalState->Clear();
  SAFE_DELETE(m_StomachDissolvedMass);
  m_TransitDissolvedMasses.clear();
  //SAFE_DELETE(m_EnterocyteMass);
  //SAFE_DELETE(m_VilliCapillaryConcentration);
  m_EnterocyteDissolvedMasses.clear();
}

bool SESubstanceOralState::Load(const CDM::SubstanceOralStateData& in)
{
  if (in.RouteSpecificData().TransmucosalRoute().present())
    m_TransmucosalState->Load(in.RouteSpecificData().TransmucosalRoute().get());
  if (in.RouteSpecificData().GastrointestinalRoute().present())
    m_GastrointestinalState->Load(in.RouteSpecificData().GastrointestinalRoute().get());
  GetStomachDissolvedMass().Load(in.StomachDissolvedMass());
  m_TransitDissolvedMasses.clear();
  for (auto transitMass : in.TransitDissolvedMasses()) {
    SEScalarMass* tMass = new SEScalarMass;
    tMass->Load(transitMass);
    m_TransitDissolvedMasses.push_back(tMass);
  }
  m_EnterocyteDissolvedMasses.clear();
  for (auto entMass : in.TransitDissolvedMasses()) {
    SEScalarMass* eMass = new SEScalarMass;
    eMass->Load(entMass);
    m_EnterocyteDissolvedMasses.push_back(eMass);
  }
  //GetEnterocyteMass().Load(in.EnterocyteMass());
  //GetVilliCapillaryConcentration().Load(in.VilliCapillaryConcentration());

  return true;
}

CDM::SubstanceOralStateData* SESubstanceOralState::Unload() const
{
  CDM::SubstanceOralStateData* data = new CDM::SubstanceOralStateData();
  Unload(*data);
  return data;
}
void SESubstanceOralState::Unload(CDM::SubstanceOralStateData& data) const
{
  if (m_TransmucosalState != nullptr)
    data.RouteSpecificData().TransmucosalRoute(std::unique_ptr<CDM::OralTransmucosalStateData>(m_TransmucosalState->Unload()));
  if (m_GastrointestinalState != nullptr)
    data.RouteSpecificData().GastrointestinalRoute(std::unique_ptr<CDM::OralGastrointestinalStateData>(m_GastrointestinalState->Unload()));
  data.StomachDissolvedMass(std::unique_ptr<CDM::ScalarMassData>(m_StomachDissolvedMass->Unload()));
  for (auto tMass : m_TransitDissolvedMasses) {
    data.TransitDissolvedMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(tMass->Unload()));
  }
  //data.EnterocyteMass(std::unique_ptr<CDM::ScalarMassData>(m_EnterocyteMass->Unload()));
  //data.VilliCapillaryConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_VilliCapillaryConcentration->Unload()));
  for (auto eMass : m_EnterocyteDissolvedMasses) {
    data.EnterocyteDissolvedMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(eMass->Unload()));
  }
}

bool SESubstanceOralState::Initialize(SEScalarMass& dose, CDM::enumOralAdministration::value route)
{
  if (route == CDM::enumOralAdministration::Transmucosal)
    GetTransmucosalSpecificData().Initialize(dose.GetValue(MassUnit::ug));
  if (route == CDM::enumOralAdministration::Gastrointestinal)
    GetGastrointestinalSpecificData().Initialize(dose.GetValue(MassUnit::ug));

  GetStomachDissolvedMass().SetValue(0.0, MassUnit::ug);
  std::vector<double> initDisTransit(m_NumTransitMasses); //All zeros, correct number of elements
  bool trSet = SetTransitDissolvedMasses(initDisTransit, MassUnit::ug);
  //GetEnterocyteMass().SetValue(0.0, MassUnit::ug);
  //GetVilliCapillaryConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  std::vector<double> initEnt(m_NumTransitMasses);
  bool entSet = SetEnterocyteDissolvedMasses(initEnt, MassUnit::ug);

  return (trSet&&entSet);
}

SEOralTransmucosalState& SESubstanceOralState::GetTransmucosalSpecificData()
{
  //Check to see if both states are null ptr because you should never be able to define both.
  //Thus, if GI State is set and we make a call to GetTransmucosalStateData, we will get a null response
  if (m_TransmucosalState == nullptr && m_GastrointestinalState == nullptr)
    m_TransmucosalState = new SEOralTransmucosalState();
  return *m_TransmucosalState;
}

bool SESubstanceOralState::IsTransmucosalRoute()
{
  return m_TransmucosalState == nullptr ? false : true;
}

SEOralGastrointestinalState& SESubstanceOralState::GetGastrointestinalSpecificData()
{
  //Check to see if both states are null ptr because you should never be able to define both.
  //Thus, if GI State is set and we make a call to GetTransmucosalStateData, we will get a null response
  if (m_TransmucosalState == nullptr && m_GastrointestinalState == nullptr)
    m_GastrointestinalState = new SEOralGastrointestinalState();
  return *m_GastrointestinalState;
}
bool SESubstanceOralState::IsGastrointestinalRoute()
{
  return m_GastrointestinalState == nullptr ? false : true;
}

SEScalarMass& SESubstanceOralState::GetStomachDissolvedMass()
{
  if (m_StomachDissolvedMass == nullptr)
    m_StomachDissolvedMass = new SEScalarMass();
  return *m_StomachDissolvedMass;
}
std::vector<SEScalarMass*>& SESubstanceOralState::GetTransitDissolvedMasses()
{
  //Going to initialize the masses inside this vector the first time it is called in Drugs.  It feels weird to set the
  //length here where it might be hard to find later.  If we change GI drug implementation, it should be easier to see
  //how to adjust the size of this vector
  return m_TransitDissolvedMasses;
}
std::vector<double> SESubstanceOralState::GetTransitDissolvedMasses(const MassUnit& unit)
{
  std::vector<double> transitMasses;
  for (auto tData : m_TransitDissolvedMasses) {
    transitMasses.push_back(tData->GetValue(unit));
  }
  return transitMasses;
}
bool SESubstanceOralState::SetTransitDissolvedMasses(std::vector<double>& tMasses, const MassUnit& unit)
{
  if (tMasses.size() != m_NumTransitMasses) {
    //Don't execute if the input vector is not the correct size
    return false;
  }
  if (m_TransitDissolvedMasses.empty()) {
    //The very first time we call this function, the member vector will be empty.  All values should initialize to 0
    for (size_t pos = 0; pos < m_NumTransitMasses; pos++) {
      SEScalarMass* gtMass = new SEScalarMass();
      gtMass->SetValue(tMasses[pos], unit);
      m_TransitDissolvedMasses.push_back(gtMass);
    }
  } else {
    //Vectors should be the same size, so we can iterate over them
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarMass*>::iterator scIt;
    for (dblIt = tMasses.begin(), scIt = m_TransitDissolvedMasses.begin(); dblIt != tMasses.end() && scIt != m_TransitDissolvedMasses.end(); dblIt++, scIt++) {
      (*scIt)->SetValue(*dblIt, unit);
    }
  }
  return true;
}

std::vector<SEScalarMass*>& SESubstanceOralState::GetEnterocyteDissolvedMasses()
{
  //Going to initialize the masses inside this vector the first time it is called in Drugs.  It feels weird to set the
  //length here where it might be hard to find later.  If we change GI drug implementation, it should be easier to see
  //how to adjust the size of this vector
  return m_EnterocyteDissolvedMasses;
}
std::vector<double> SESubstanceOralState::GetEnterocyteDissolvedMasses(const MassUnit& unit)
{
  std::vector<double> EnterocyteMasses;
  for (auto eData : m_EnterocyteDissolvedMasses) {
    EnterocyteMasses.push_back(eData->GetValue(unit));
  }
  return EnterocyteMasses;
}
bool SESubstanceOralState::SetEnterocyteDissolvedMasses(std::vector<double>& entMasses, const MassUnit& unit)
{
  if (entMasses.size() != m_NumTransitMasses) {
    //Don't execute if the input vector is not the correct size
    return false;
  }
  if (m_EnterocyteDissolvedMasses.empty()) {
    //The very first time we call this function, the member vector will be empty.  All values should initialize to 0
    for (size_t pos = 0; pos < m_NumTransitMasses; pos++) {
      SEScalarMass* entMass = new SEScalarMass();
      entMass->SetValue(entMasses[pos], unit);
      m_EnterocyteDissolvedMasses.push_back(entMass);
    }
  } else {
    //Vectors should be the same size, so we can iterate over them
    std::vector<double>::iterator dblIt;
    std::vector<SEScalarMass*>::iterator scIt;
    for (dblIt = entMasses.begin(), scIt = m_EnterocyteDissolvedMasses.begin(); dblIt != entMasses.end() && scIt != m_EnterocyteDissolvedMasses.end(); dblIt++, scIt++) {
      (*scIt)->SetValue(*dblIt, unit);
    }
  }
  return true;
}
//SEScalarMass& SESubstanceOralState::GetEnterocyteMass()
//{
//  if (m_EnterocyteMass == nullptr)
//    m_EnterocyteMass = new SEScalarMass();
//  return *m_EnterocyteMass;
//}
//SEScalarMassPerVolume& SESubstanceOralState::GetVilliCapillaryConcentration()
//{
//  if (m_VilliCapillaryConcentration == nullptr)
//    m_VilliCapillaryConcentration = new SEScalarMassPerVolume();
//  return *m_VilliCapillaryConcentration;
//}

//--------------------------------------------------------------------------------------------------
// Oral Transmucosal State methods
SEOralTransmucosalState::SEOralTransmucosalState()
{
  m_MouthSolidMass = nullptr;
  m_SalivaConcentration = nullptr;
  m_NumBuccalRegions = 7; //Hard-coded for current model implementation!
  m_NumSublingualRegions = 7; //Hard-coded for current model implementation!
}

SEOralTransmucosalState::~SEOralTransmucosalState()
{
  Clear();
}

void SEOralTransmucosalState::Clear()
{
  SAFE_DELETE(m_MouthSolidMass);
  SAFE_DELETE(m_SalivaConcentration);
  m_BuccalConcentrations.clear();
  m_SublingualConcentrations.clear();
}

bool SEOralTransmucosalState::Initialize(double dose_ug)
{
  GetMouthSolidMass().SetValue(dose_ug, MassUnit::ug);
  GetSalivaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  std::vector<double> initBuccal(m_NumBuccalRegions);
  std::vector<double> initSublingual(m_NumSublingualRegions);
  bool bucSet = SetBuccalConcentrations(initBuccal, MassPerVolumeUnit::ug_Per_mL);
  bool subSet = SetSublingualConcentrations(initSublingual, MassPerVolumeUnit::ug_Per_mL);
  return (bucSet && subSet);
}

bool SEOralTransmucosalState::Load(const CDM::OralTransmucosalStateData& in)
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

CDM::OralTransmucosalStateData* SEOralTransmucosalState::Unload() const
{
  CDM::OralTransmucosalStateData* data = new CDM::OralTransmucosalStateData();
  Unload(*data);
  return data;
}
void SEOralTransmucosalState::Unload(CDM::OralTransmucosalStateData& data) const
{
  data.MouthSolidMass(std::unique_ptr<CDM::ScalarMassData>(m_MouthSolidMass->Unload()));
  data.SalivaConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_SalivaConcentration->Unload()));
  for (auto bcData : m_BuccalConcentrations) {
    data.BuccalConcentrations().push_back(std::unique_ptr<CDM::ScalarMassPerVolumeData>(bcData->Unload()));
  }
  for (auto slData : m_SublingualConcentrations) {
    data.SublingualConcentrations().push_back(std::unique_ptr<CDM::ScalarMassPerVolumeData>(slData->Unload()));
  }
}

SEScalarMass& SEOralTransmucosalState::GetMouthSolidMass()
{
  if (m_MouthSolidMass == nullptr)
    m_MouthSolidMass = new SEScalarMass();
  return *m_MouthSolidMass;
}
SEScalarMassPerVolume& SEOralTransmucosalState::GetSalivaConcentration()
{
  if (m_SalivaConcentration == nullptr)
    m_SalivaConcentration = new SEScalarMassPerVolume();
  return *m_SalivaConcentration;
}
std::vector<SEScalarMassPerVolume*>& SEOralTransmucosalState::GetBuccalConcentrations()
{
  return m_BuccalConcentrations;
}
std::vector<double> SEOralTransmucosalState::GetBuccalConcentrations(const MassPerVolumeUnit& unit)
{
  std::vector<double> buccalMasses;
  for (auto bMass : m_BuccalConcentrations) {
    buccalMasses.push_back(bMass->GetValue(unit));
  }
  return buccalMasses;
}
bool SEOralTransmucosalState::SetBuccalConcentrations(std::vector<double>& bMasses, const MassPerVolumeUnit& unit)
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

std::vector<SEScalarMassPerVolume*>& SEOralTransmucosalState::GetSublingualConcentrations()
{
  return m_SublingualConcentrations;
}
std::vector<double> SEOralTransmucosalState::GetSublingualConcentrations(const MassPerVolumeUnit& unit)
{
  std::vector<double> sublingualMasses;
  for (auto slMass : m_SublingualConcentrations) {
    sublingualMasses.push_back(slMass->GetValue(unit));
  }
  return sublingualMasses;
}
bool SEOralTransmucosalState::SetSublingualConcentrations(std::vector<double>& slMasses, const MassPerVolumeUnit& unit)
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

//--------------------------------------------------------------------------------------
//Oral Gastrointestinal State methods
SEOralGastrointestinalState::SEOralGastrointestinalState()
{
  m_StomachSolidMass = nullptr;
}

SEOralGastrointestinalState::~SEOralGastrointestinalState()
{
  Clear();
}

void SEOralGastrointestinalState::Clear()
{
  SAFE_DELETE(m_StomachSolidMass);
}

bool SEOralGastrointestinalState::Initialize(double dose_ug)
{
  GetStomachSolidMass().SetValue(dose_ug, MassUnit::ug);
  return true;
}

bool SEOralGastrointestinalState::Load(const CDM::OralGastrointestinalStateData& in)
{
  GetStomachSolidMass().Load(in.StomachSolidMass());

  return true;
}

CDM::OralGastrointestinalStateData* SEOralGastrointestinalState::Unload() const
{
  CDM::OralGastrointestinalStateData* data = new CDM::OralGastrointestinalStateData();
  Unload(*data);
  return data;
}
void SEOralGastrointestinalState::Unload(CDM::OralGastrointestinalStateData& data) const
{
  data.StomachSolidMass(std::unique_ptr<CDM::ScalarMassData>(m_StomachSolidMass->Unload()));
}

SEScalarMass& SEOralGastrointestinalState::GetStomachSolidMass()
{
  if (m_StomachSolidMass == nullptr)
    m_StomachSolidMass = new SEScalarMass();
  return *m_StomachSolidMass;
}
}