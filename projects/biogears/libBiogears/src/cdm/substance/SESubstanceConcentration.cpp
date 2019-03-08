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
#include <biogears/cdm/substance/SESubstanceConcentration.h>

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SESubstanceConcentration::SESubstanceConcentration(SESubstance& substance)
  : Loggable(substance.GetLogger())
  , m_Substance(substance)
{
  m_Concentration = nullptr;
}

SESubstanceConcentration::~SESubstanceConcentration()
{
  Clear();
}

void SESubstanceConcentration::Clear()
{
  SAFE_DELETE(m_Concentration);
}

bool SESubstanceConcentration::Load(const CDM::SubstanceConcentrationData& in)
{
  Clear();
  GetConcentration().Load(in.Concentration());
  return true;
}

CDM::SubstanceConcentrationData* SESubstanceConcentration::Unload() const
{
  CDM::SubstanceConcentrationData* data = new CDM::SubstanceConcentrationData();
  Unload(*data);
  return data;
}

void SESubstanceConcentration::Unload(CDM::SubstanceConcentrationData& data) const
{
  data.Name(m_Substance.GetName());
  if (m_Concentration != nullptr)
    data.Concentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_Concentration->Unload()));
}

bool SESubstanceConcentration::HasConcentration() const
{
  return (m_Concentration == nullptr) ? false : m_Concentration->IsValid();
}
SEScalarMassPerVolume& SESubstanceConcentration::GetConcentration()
{
  if (m_Concentration == nullptr)
    m_Concentration = new SEScalarMassPerVolume();
  return *m_Concentration;
}
double SESubstanceConcentration::GetConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_Concentration == nullptr)
    return SEScalar::dNaN();
  return m_Concentration->GetValue(unit);
}

SESubstance& SESubstanceConcentration::GetSubstance() const
{
  return m_Substance;
}
}