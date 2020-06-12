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
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceBolus::SESubstanceBolus(const SESubstance& substance)
  : SESubstanceAdministration()
  , m_Substance(substance)
{
  m_AdminRoute = (CDM::enumBolusAdministration::value)-1;
  m_AdminTime = nullptr;
  m_Dose = nullptr;
  m_Concentration = nullptr;
}

SESubstanceBolus::~SESubstanceBolus()
{
  Clear();
}

void SESubstanceBolus::Clear()
{
  SESubstanceAdministration::Clear();
  m_AdminRoute = (CDM::enumBolusAdministration::value)-1;
  SAFE_DELETE(m_AdminTime);
  SAFE_DELETE(m_Dose);
  SAFE_DELETE(m_Concentration);
  // m_Substance=nullptr; Keeping mapping!!
}

bool SESubstanceBolus::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasDose() && HasConcentration() && HasAdminRoute();
}

bool SESubstanceBolus::IsActive() const
{
  return IsValid();
}

bool SESubstanceBolus::Load(const CDM::SubstanceBolusData& in)
{
  SESubstanceAdministration::Load(in);
  if (in.AdminTime().present()) {
    GetAdminTime().Load(in.AdminTime().get());
  }
  GetDose().Load(in.Dose());
  GetConcentration().Load(in.Concentration());
  m_AdminRoute = in.AdminRoute();
  return true;
}

CDM::SubstanceBolusData* SESubstanceBolus::Unload() const
{
  CDM::SubstanceBolusData* data(new CDM::SubstanceBolusData());
  Unload(*data);
  return data;
}

void SESubstanceBolus::Unload(CDM::SubstanceBolusData& data) const
{
  SESubstanceAdministration::Unload(data);
  if (m_Dose != nullptr)
    data.Dose(std::unique_ptr<CDM::ScalarVolumeData>(m_Dose->Unload()));
  if (m_Concentration != nullptr)
    data.Concentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Concentration->Unload()));
  if (HasAdminRoute())
    data.AdminRoute(m_AdminRoute);
  if (HasAdminTime())
    data.AdminTime(std::unique_ptr<CDM::ScalarTimeData>(m_AdminTime->Unload()));
  data.Substance(m_Substance.GetName());
}

bool SESubstanceBolus::HasAdminTime() const
{
  return m_AdminTime == nullptr ? false : m_AdminTime->IsValid();
}
SEScalarTime& SESubstanceBolus::GetAdminTime()
{
  if (m_AdminTime == nullptr)
    m_AdminTime = new SEScalarTime();
  return *m_AdminTime;
}

CDM::enumBolusAdministration::value SESubstanceBolus::GetAdminRoute() const
{
  return m_AdminRoute;
}
void SESubstanceBolus::SetAdminRoute(CDM::enumBolusAdministration::value route)
{
  m_AdminRoute = route;
}
bool SESubstanceBolus::HasAdminRoute() const
{
  return m_AdminRoute == ((CDM::enumBolusAdministration::value)-1) ? false : true;
}
void SESubstanceBolus::InvalidateAdminRoute()
{
  m_AdminRoute = (CDM::enumBolusAdministration::value)-1;
}

bool SESubstanceBolus::HasDose() const
{
  return m_Dose == nullptr ? false : m_Dose->IsValid();
}
SEScalarVolume& SESubstanceBolus::GetDose()
{
  if (m_Dose == nullptr)
    m_Dose = new SEScalarVolume();
  return *m_Dose;
}

bool SESubstanceBolus::HasConcentration() const
{
  return m_Concentration == nullptr ? false : m_Concentration->IsValid();
}
SEScalarMassPerVolume& SESubstanceBolus::GetConcentration()
{
  if (m_Concentration == nullptr)
    m_Concentration = new SEScalarMassPerVolume();
  return *m_Concentration;
}

SESubstance& SESubstanceBolus::GetSubstance() const
{
  return (SESubstance&)m_Substance;
}

void SESubstanceBolus::ToString(std::ostream& str) const
{
  str << "Patient Action : Substance Bolus";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tDose: ";
  HasDose() ? str << *m_Dose : str << "No Dose Set";
  str << "\n\tConcentration: ";
  HasConcentration() ? str << *m_Concentration : str << "NaN";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << "\n\tAdministration Route: ";
  HasAdminRoute() ? str << GetAdminRoute() : str << "Not Set";
  HasAdminTime() ? str << "\n\tAdminTime: " << *m_AdminTime : str<<"";
  str << std::flush;
}

SESubstanceBolusState::SESubstanceBolusState(const SESubstance& sub)
  : m_substance(sub)
{
  m_elapsedTime.SetValue(0, TimeUnit::s);
  m_administeredDose.SetValue(0, VolumeUnit::mL);
}
SESubstanceBolusState::~SESubstanceBolusState()
{
}
bool SESubstanceBolusState::Load(const CDM::SubstanceBolusStateData& in)
{
  m_elapsedTime.Load(in.ElapsedTime());
  m_administeredDose.Load(in.AdministeredDose());
  return true;
}
CDM::SubstanceBolusStateData* SESubstanceBolusState::Unload() const
{
  CDM::SubstanceBolusStateData* data = new CDM::SubstanceBolusStateData();
  Unload(*data);
  return data;
}
void SESubstanceBolusState::Unload(CDM::SubstanceBolusStateData& data) const
{
  data.Substance(m_substance.GetName());
  data.ElapsedTime(std::unique_ptr<CDM::ScalarTimeData>(m_elapsedTime.Unload()));
  data.AdministeredDose(std::unique_ptr<CDM::ScalarVolumeData>(m_administeredDose.Unload()));
}
}