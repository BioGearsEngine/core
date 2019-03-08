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
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>

#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceCompoundInfusion::SESubstanceCompoundInfusion(const SESubstanceCompound& compound)
  : SESubstanceAdministration()
  , m_Compound(compound)
{
  m_Rate = nullptr;
  m_BagVolume = nullptr;
}

SESubstanceCompoundInfusion::~SESubstanceCompoundInfusion()
{
  Clear();
}

void SESubstanceCompoundInfusion::Clear()
{
  SESubstanceAdministration::Clear();
  m_Rate = nullptr;
  m_BagVolume = nullptr;
  // m_Compound=nullptr; Keeping mapping!!
}

bool SESubstanceCompoundInfusion::Load(const CDM::SubstanceCompoundInfusionData& in)
{
  SESubstanceAdministration::Load(in);
  GetRate().Load(in.Rate());
  GetBagVolume().Load(in.BagVolume());
  return true;
}

bool SESubstanceCompoundInfusion::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasRate() && HasBagVolume();
}

bool SESubstanceCompoundInfusion::IsActive() const
{
  return IsValid() ? !m_Rate->IsZero() : false;
}

CDM::SubstanceCompoundInfusionData* SESubstanceCompoundInfusion::Unload() const
{
  CDM::SubstanceCompoundInfusionData* data(new CDM::SubstanceCompoundInfusionData());
  Unload(*data);
  return data;
}

void SESubstanceCompoundInfusion::Unload(CDM::SubstanceCompoundInfusionData& data) const
{
  SESubstanceAdministration::Unload(data);
  if (m_Rate != nullptr)
    data.Rate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_Rate->Unload()));
  if (m_BagVolume != nullptr)
    data.BagVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_BagVolume->Unload()));
  data.SubstanceCompound(m_Compound.GetName());
}

bool SESubstanceCompoundInfusion::HasRate() const
{
  return m_Rate == nullptr ? false : m_Rate->IsValid();
}
SEScalarVolumePerTime& SESubstanceCompoundInfusion::GetRate()
{
  if (m_Rate == nullptr)
    m_Rate = new SEScalarVolumePerTime();
  return *m_Rate;
}

bool SESubstanceCompoundInfusion::HasBagVolume() const
{
  return m_BagVolume == nullptr ? false : m_BagVolume->IsValid();
}
SEScalarVolume& SESubstanceCompoundInfusion::GetBagVolume()
{
  if (m_BagVolume == nullptr)
    m_BagVolume = new SEScalarVolume();
  return *m_BagVolume;
}

SESubstanceCompound& SESubstanceCompoundInfusion::GetSubstanceCompound() const
{
  return (SESubstanceCompound&)m_Compound;
}

void SESubstanceCompoundInfusion::ToString(std::ostream& str) const
{
  str << "Patient Action : Compound Infusion";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tRate: ";
  HasRate() ? str << *m_Rate : str << "NaN";
  str << "\n\tBag Volume: ";
  HasBagVolume() ? str << *m_BagVolume : str << "NaN";
  str << "\n\tSubstance Compound: " << m_Compound.GetName();
  str << std::flush;
}

}