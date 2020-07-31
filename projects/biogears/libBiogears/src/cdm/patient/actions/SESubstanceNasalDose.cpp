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

};