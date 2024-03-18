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
#include <biogears/cdm/system/environment/SEActiveCooling.h>

#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SEActiveCooling::SEActiveCooling(Logger* logger)
  : Loggable(logger)
{
  m_Power = nullptr;
  m_SurfaceArea = nullptr;
  m_SurfaceAreaFraction = nullptr;
}
//-----------------------------------------------------------------------------
SEActiveCooling::~SEActiveCooling()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEActiveCooling::Clear()
{
  SAFE_DELETE(m_Power);
  SAFE_DELETE(m_SurfaceArea);
  SAFE_DELETE(m_SurfaceAreaFraction);
}
//-----------------------------------------------------------------------------
void SEActiveCooling::Reset()
{
  Clear();
}
//-----------------------------------------------------------------------------
const SEScalar* SEActiveCooling::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEActiveCooling::GetScalar(const std::string& name)
{
  if (name.compare("Power") == 0)
    return &GetPower();
  if (name.compare("SurfaceArea") == 0)
    return &GetSurfaceArea();
  if (name.compare("SurfaceAreaFraction") == 0)
    return &GetSurfaceAreaFraction();
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SEActiveCooling::Load(const CDM::ActiveCoolingData& in, std::random_device *rd)
{
  GetPower().Load(in.Power(), rd);
  if (in.SurfaceArea().present())
    GetSurfaceArea().Load(in.SurfaceArea().get(), rd);
  if (in.SurfaceAreaFraction().present())
    GetSurfaceAreaFraction().Load(in.SurfaceAreaFraction().get(), rd);
  return true;
}
//-----------------------------------------------------------------------------
CDM::ActiveCoolingData* SEActiveCooling::Unload() const
{
  CDM::ActiveCoolingData* data = new CDM::ActiveCoolingData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEActiveCooling::Unload(CDM::ActiveCoolingData& data) const
{
  if (HasPower())
    data.Power(std::unique_ptr<CDM::ScalarPowerData>(m_Power->Unload()));
  if (HasSurfaceArea())
    data.SurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_SurfaceArea->Unload()));
  if (HasSurfaceAreaFraction())
    data.SurfaceAreaFraction(std::unique_ptr<CDM::ScalarFractionData>(m_SurfaceAreaFraction->Unload()));
}
//-----------------------------------------------------------------------------
bool SEActiveCooling::HasPower() const
{
  return m_Power == nullptr ? false : m_Power->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPower& SEActiveCooling::GetPower()
{
  if (m_Power == nullptr)
    m_Power = new SEScalarPower();
  return *m_Power;
}
//-----------------------------------------------------------------------------
double SEActiveCooling::GetPower(const PowerUnit& unit) const
{
  if (m_Power == nullptr)
    return SEScalar::dNaN();
  return m_Power->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEActiveCooling::HasSurfaceArea() const
{
  return m_SurfaceArea == nullptr ? false : m_SurfaceArea->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarArea& SEActiveCooling::GetSurfaceArea()
{
  if (m_SurfaceArea == nullptr)
    m_SurfaceArea = new SEScalarArea();
  return *m_SurfaceArea;
}
//-----------------------------------------------------------------------------
double SEActiveCooling::GetSurfaceArea(const AreaUnit& unit) const
{
  if (m_SurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_SurfaceArea->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEActiveCooling::HasSurfaceAreaFraction() const
{
  return m_SurfaceAreaFraction == nullptr ? false : m_SurfaceAreaFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEActiveCooling::GetSurfaceAreaFraction()
{
  if (m_SurfaceAreaFraction == nullptr)
    m_SurfaceAreaFraction = new SEScalarFraction();
  return *m_SurfaceAreaFraction;
}
//-----------------------------------------------------------------------------
double SEActiveCooling::GetSurfaceAreaFraction() const
{
  if (m_SurfaceAreaFraction == nullptr)
    return SEScalar::dNaN();
  return m_SurfaceAreaFraction->GetValue();
}
//-----------------------------------------------------------------------------
void SEActiveCooling::ToString(std::ostream& str) const
{
  str << "Active Cooling :";
  str << "\n\tPower :";
  HasPower() ? str << *m_Power : str << "NaN";
  str << "\n\tSurfaceArea :";
  HasSurfaceArea() ? str << *m_SurfaceArea : str << "NaN";
  str << "\n\tSurfaceAreaFraction :";
  HasSurfaceAreaFraction() ? str << *m_SurfaceAreaFraction : str << "NaN";
  str << std::flush;
}
//-----------------------------------------------------------------------------
bool SEActiveCooling::operator==(SEActiveCooling const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_Power && rhs.m_Power) ? m_Power->operator==(*rhs.m_Power) : m_Power == rhs.m_Power)
    && ((m_SurfaceArea && rhs.m_SurfaceArea) ? m_SurfaceArea->operator==(*rhs.m_SurfaceArea) : m_SurfaceArea == rhs.m_SurfaceArea)
    && ((m_SurfaceAreaFraction && rhs.m_SurfaceAreaFraction) ? m_SurfaceAreaFraction->operator==(*rhs.m_SurfaceAreaFraction) : m_SurfaceAreaFraction == rhs.m_SurfaceAreaFraction);
}
bool SEActiveCooling::operator!=(SEActiveCooling const& rhs) const
{
  return !(this->operator==(rhs));
}

}