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

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/substance/SEAntibioticPharmacodynamics.h>

namespace biogears {
SEAntibioticPharmacodynamics::SEAntibioticPharmacodynamics(Logger* logger)
  : Loggable(logger)
{
  m_AntibacterialIndex = (CDM::enumAntibioticPDIndex::value)-1;
  m_I50 = nullptr;
  m_AntibacterialEffect = nullptr;
}
//-----------------------------------------------------------------------------
SEAntibioticPharmacodynamics::~SEAntibioticPharmacodynamics()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEAntibioticPharmacodynamics::Clear()
{
  m_AntibacterialIndex = (CDM::enumAntibioticPDIndex::value)-1;
  m_I50 = nullptr;
  m_AntibacterialEffect = nullptr;
}
//-----------------------------------------------------------------------------
bool SEAntibioticPharmacodynamics::IsValid() const
{
  return (HasAntibacterialIndex() && HasI50() && HasAntibacterialEffect());
}
//-----------------------------------------------------------------------------
const SEScalar* SEAntibioticPharmacodynamics::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEAntibioticPharmacodynamics::GetScalar(const std::string& name)
{
  if (name.compare("I50") == 0)
    return &GetI50();
  if (name.compare("AntibacterialEffect") == 0)
    return &GetAntibacterialEffect();
}
//-----------------------------------------------------------------------------
bool SEAntibioticPharmacodynamics::Load(const CDM::AntibioticPharmacodynamicsData& in)
{
  m_AntibacterialIndex = in.AntibacterialIndex();
  GetI50().Load(in.I50());
  GetAntibacterialEffect().Load(in.AntibacterialEffect());
  return true;
}
//-----------------------------------------------------------------------------
CDM::AntibioticPharmacodynamicsData* SEAntibioticPharmacodynamics::Unload() const
{
  CDM::AntibioticPharmacodynamicsData* data(new CDM::AntibioticPharmacodynamicsData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEAntibioticPharmacodynamics::Unload(CDM::AntibioticPharmacodynamicsData& data) const
{
  if (HasAntibacterialIndex())
    data.AntibacterialIndex(m_AntibacterialIndex);
  if (HasI50())
    data.I50(std::unique_ptr<CDM::ScalarData>(m_I50->Unload()));
  if (HasAntibacterialEffect())
    data.AntibacterialEffect(std::unique_ptr<CDM::ScalarData>(m_AntibacterialEffect->Unload()));
}
//-----------------------------------------------------------------------------
bool SEAntibioticPharmacodynamics::HasAntibacterialIndex() const
{
  return m_AntibacterialIndex == ((CDM::enumAntibioticPDIndex::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
CDM::enumAntibioticPDIndex::value SEAntibioticPharmacodynamics::GetAntibacterialIndex() const
{
  return m_AntibacterialIndex;
}
//-----------------------------------------------------------------------------
bool SEAntibioticPharmacodynamics::HasI50() const
{
  return m_I50 == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEScalar& SEAntibioticPharmacodynamics::GetI50()
{
  if (m_I50 == nullptr)
    m_I50 = new SEScalar();
  return *m_I50;
}
//-----------------------------------------------------------------------------
bool SEAntibioticPharmacodynamics::HasAntibacterialEffect() const
{
  return m_AntibacterialEffect == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEScalar& SEAntibioticPharmacodynamics::GetAntibacterialEffect()
{
  if (m_AntibacterialEffect == nullptr)
    m_AntibacterialEffect = new SEScalar();
  return *m_AntibacterialEffect;
}
}