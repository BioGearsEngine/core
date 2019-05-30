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
  m_LumenConcentration = nullptr;
  m_EnterocyteConcentration = nullptr;
  m_VilliConcentration = nullptr;
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
  SAFE_DELETE(m_LumenConcentration);
  SAFE_DELETE(m_EnterocyteConcentration);
  SAFE_DELETE(m_VilliConcentration);
}

bool SESubstanceOralState::Load(const CDM::SubstanceOralStateData& in)
{
  if (in.RouteSpecificData().TransmucosalRoute().present())
    m_TransmucosalState->Load(in.RouteSpecificData().TransmucosalRoute().get());
  if (in.RouteSpecificData().GastrointestinalRoute().present())
    m_GastrointestinalState->Load(in.RouteSpecificData().GastrointestinalRoute().get());
  GetStomachDissolvedMass().Load(in.StomachDissolvedMass());
  GetLumenConcentration().Load(in.LumenConcentration());
  GetEnterocyteConcentration().Load(in.EnterocyteConcentration());
  GetVilliConcentration().Load(in.VilliConcentration());

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
  data.LumenConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_LumenConcentration->Unload()));
  data.EnterocyteConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_EnterocyteConcentration->Unload()));
  data.VilliConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_VilliConcentration->Unload()));
}

void SESubstanceOralState::Initialize(SEScalarMass& dose, CDM::enumOralAdministration::value route)
{
  if (route == CDM::enumOralAdministration::Transmucosal)
    GetTransmucosalSpecificData().Initialize(dose.GetValue(MassUnit::ug));
  if (route == CDM::enumOralAdministration::Gastrointestinal)
    GetGastrointestinalSpecificData().Initialize(dose.GetValue(MassUnit::ug));

  GetStomachDissolvedMass().SetValue(0.0, MassUnit::ug);
  GetLumenConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  GetEnterocyteConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  GetVilliConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
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
SEScalarMassPerVolume& SESubstanceOralState::GetLumenConcentration()
{
  if (m_LumenConcentration == nullptr)
    m_LumenConcentration = new SEScalarMassPerVolume();
  return *m_LumenConcentration;
}
SEScalarMassPerVolume& SESubstanceOralState::GetEnterocyteConcentration()
{
  if (m_EnterocyteConcentration == nullptr)
    m_EnterocyteConcentration = new SEScalarMassPerVolume();
  return *m_EnterocyteConcentration;
}
SEScalarMassPerVolume& SESubstanceOralState::GetVilliConcentration()
{
  if (m_VilliConcentration == nullptr)
    m_VilliConcentration = new SEScalarMassPerVolume();
  return *m_VilliConcentration;
}

//--------------------------------------------------------------------------------------------------
// Oral Transmucosal State methods
SEOralTransmucosalState::SEOralTransmucosalState()
{
  m_MouthSolidDrugMass = nullptr;
  m_SalivaConcentration = nullptr;
  m_EpitheliumConcentration = nullptr;
  m_LaminaConcentration = nullptr;
}

SEOralTransmucosalState::~SEOralTransmucosalState()
{
  Clear();
}

void SEOralTransmucosalState::Clear()
{
  SAFE_DELETE(m_MouthSolidDrugMass);
  SAFE_DELETE(m_SalivaConcentration);
  SAFE_DELETE(m_EpitheliumConcentration);
  SAFE_DELETE(m_LaminaConcentration);
}

void SEOralTransmucosalState::Initialize(double dose_ug)
{
  GetMouthSolidDrugMass().SetValue(dose_ug, MassUnit::ug);
  GetSalivaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  GetEpitheliumConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  GetLaminaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
}

bool SEOralTransmucosalState::Load(const CDM::OralTransmucosalStateData& in)
{
  GetMouthSolidDrugMass().Load(in.MouthSolidDrugMass());
  GetSalivaConcentration().Load(in.SalivaConcentration());
  GetEpitheliumConcentration().Load(in.EpitheliumConcentration());
  GetLaminaConcentration().Load(in.LaminaConcentration());

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
  data.MouthSolidDrugMass(std::unique_ptr<CDM::ScalarMassData>(m_MouthSolidDrugMass->Unload()));
  data.SalivaConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_SalivaConcentration->Unload()));
  data.EpitheliumConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_EpitheliumConcentration->Unload()));
  data.LaminaConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_LaminaConcentration->Unload()));
}

SEScalarMass& SEOralTransmucosalState::GetMouthSolidDrugMass()
{
  if (m_MouthSolidDrugMass == nullptr)
    m_MouthSolidDrugMass = new SEScalarMass();
  return *m_MouthSolidDrugMass;
}
SEScalarMassPerVolume& SEOralTransmucosalState::GetSalivaConcentration()
{
  if (m_SalivaConcentration == nullptr)
    m_SalivaConcentration = new SEScalarMassPerVolume();
  return *m_SalivaConcentration;
}
SEScalarMassPerVolume& SEOralTransmucosalState::GetEpitheliumConcentration()
{
  if (m_EpitheliumConcentration == nullptr)
    m_EpitheliumConcentration = new SEScalarMassPerVolume();
  return *m_EpitheliumConcentration;
}
SEScalarMassPerVolume& SEOralTransmucosalState::GetLaminaConcentration()
{
  if (m_LaminaConcentration == nullptr)
    m_LaminaConcentration = new SEScalarMassPerVolume();
  return *m_LaminaConcentration;
}

//--------------------------------------------------------------------------------------
//Oral Gastrointestinal State methods
SEOralGastrointestinalState::SEOralGastrointestinalState()
{
  m_StomachSolidDrugMass = nullptr;
}

SEOralGastrointestinalState::~SEOralGastrointestinalState()
{
  Clear();
}

void SEOralGastrointestinalState::Clear()
{
  SAFE_DELETE(m_StomachSolidDrugMass);
}

void SEOralGastrointestinalState::Initialize(double dose_ug)
{
  GetStomachSolidDrugMass().SetValue(dose_ug, MassUnit::ug);
}

bool SEOralGastrointestinalState::Load(const CDM::OralGastrointestinalStateData& in)
{
  GetStomachSolidDrugMass().Load(in.StomachSolidDrugMass());

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
  data.StomachSolidDrugMass(std::unique_ptr<CDM::ScalarMassData>(m_StomachSolidDrugMass->Unload()));
}

SEScalarMass& SEOralGastrointestinalState::GetStomachSolidDrugMass()
{
  if (m_StomachSolidDrugMass == nullptr)
    m_StomachSolidDrugMass = new SEScalarMass();
  return *m_StomachSolidDrugMass;
}
}