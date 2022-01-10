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
#include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>

#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SERadiationAbsorbedDose::SERadiationAbsorbedDose()
  : SEPatientAction()
{
  m_RadiationDose = nullptr;
}
//-------------------------------------------------------------------------------
SERadiationAbsorbedDose::~SERadiationAbsorbedDose()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SERadiationAbsorbedDose::Clear()
{

  SEPatientAction::Clear();
  SAFE_DELETE(m_RadiationDose);
}
//-------------------------------------------------------------------------------
bool SERadiationAbsorbedDose::IsValid() const
{
  return SEPatientAction::IsValid() && HasDose();
}
//-------------------------------------------------------------------------------
bool SERadiationAbsorbedDose::IsActive() const
{
  return IsValid() ? !m_RadiationDose->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SERadiationAbsorbedDose::Load(const CDM::RadiationAbsorbedDoseData& in)
{
  SEPatientAction::Load(in);
  GetDose().Load(in.RadiationDose());
  return true;
}
//-------------------------------------------------------------------------------
CDM::RadiationAbsorbedDoseData* SERadiationAbsorbedDose::Unload() const
{
  CDM::RadiationAbsorbedDoseData* data(new CDM::RadiationAbsorbedDoseData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SERadiationAbsorbedDose::Unload(CDM::RadiationAbsorbedDoseData& data) const
{
  SEPatientAction::Unload(data);
  if (m_RadiationDose != nullptr)
    data.RadiationDose(std::unique_ptr<CDM::ScalarEnergyPerMassData>(m_RadiationDose->Unload()));
}
//-------------------------------------------------------------------------------
bool SERadiationAbsorbedDose::HasDose() const
{
  return m_RadiationDose == nullptr ? false : m_RadiationDose->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarEnergyPerMass& SERadiationAbsorbedDose::GetDose()
{
  if (m_RadiationDose == nullptr)
    m_RadiationDose = new SEScalarEnergyPerMass();
  return *m_RadiationDose;
}
//-------------------------------------------------------------------------------
void SERadiationAbsorbedDose::ToString(std::ostream& str) const
{
  str << "Patient Action : Radiation Absorbed Dose";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tDose: ";
  HasDose() ? str << *m_RadiationDose : str << "Not Set";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SERadiationAbsorbedDose::operator==(const SERadiationAbsorbedDose& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_RadiationDose && rhs.m_RadiationDose) ? m_RadiationDose->operator==(*rhs.m_RadiationDose) : m_RadiationDose == rhs.m_RadiationDose;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SERadiationAbsorbedDose::operator!=(const SERadiationAbsorbedDose& rhs) const
{
  return !(*this == rhs);
}
}