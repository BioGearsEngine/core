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
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>

#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

// Private Include
#include <io/cdm/Environment.h>

namespace biogears {
SEAppliedTemperature::SEAppliedTemperature(Logger* logger)
  : Loggable(logger)
{
  m_Temperature = nullptr;
  m_SurfaceArea = nullptr;
  m_SurfaceAreaFraction = nullptr;
  m_State = CDM::enumOnOff::On;
}
//-----------------------------------------------------------------------------
SEAppliedTemperature::~SEAppliedTemperature()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEAppliedTemperature::Clear()
{
  SAFE_DELETE(m_Temperature);
  SAFE_DELETE(m_SurfaceArea);
  SAFE_DELETE(m_SurfaceAreaFraction);
  m_State = CDM::enumOnOff::On;
}
//-----------------------------------------------------------------------------
const SEScalar* SEAppliedTemperature::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEAppliedTemperature::GetScalar(const std::string& name)
{
  if (name.compare("Temperature") == 0)
    return &GetTemperature();
  if (name.compare("SurfaceArea") == 0)
    return &GetSurfaceArea();
  if (name.compare("SurfaceAreaFraction") == 0)
    return &GetSurfaceAreaFraction();
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SEAppliedTemperature::Load(const CDM::AppliedTemperatureData& in)
{
  io::Environment::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::AppliedTemperatureData* SEAppliedTemperature::Unload() const
{
  CDM::AppliedTemperatureData* data = new CDM::AppliedTemperatureData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEAppliedTemperature::Unload(CDM::AppliedTemperatureData& data) const
{
  io::Environment::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
bool SEAppliedTemperature::HasTemperature() const
{
  return m_Temperature == nullptr ? false : m_Temperature->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTemperature& SEAppliedTemperature::GetTemperature()
{
  if (m_Temperature == nullptr)
    m_Temperature = new SEScalarTemperature();
  return *m_Temperature;
}
//-----------------------------------------------------------------------------
double SEAppliedTemperature::GetTemperature(const TemperatureUnit& unit) const
{
  if (m_Temperature == nullptr)
    return SEScalar::dNaN();
  return m_Temperature->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAppliedTemperature::HasSurfaceArea() const
{
  return m_SurfaceArea == nullptr ? false : m_SurfaceArea->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarArea& SEAppliedTemperature::GetSurfaceArea()
{
  if (m_SurfaceArea == nullptr)
    m_SurfaceArea = new SEScalarArea();
  return *m_SurfaceArea;
}
//-----------------------------------------------------------------------------
double SEAppliedTemperature::GetSurfaceArea(const AreaUnit& unit) const
{
  if (m_SurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_SurfaceArea->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAppliedTemperature::HasSurfaceAreaFraction() const
{
  return m_SurfaceAreaFraction == nullptr ? false : m_SurfaceAreaFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEAppliedTemperature::GetSurfaceAreaFraction()
{
  if (m_SurfaceAreaFraction == nullptr)
    m_SurfaceAreaFraction = new SEScalarFraction();
  return *m_SurfaceAreaFraction;
}
//-----------------------------------------------------------------------------
double SEAppliedTemperature::GetSurfaceAreaFraction() const
{
  if (m_SurfaceAreaFraction == nullptr)
    return SEScalar::dNaN();
  return m_SurfaceAreaFraction->GetValue();
}
//-----------------------------------------------------------------------------
CDM::enumOnOff::value SEAppliedTemperature::GetState() const
{
  return m_State;
}
//-----------------------------------------------------------------------------
void SEAppliedTemperature::SetState(CDM::enumOnOff::value onOff)
{
  m_State = onOff;
}
//-----------------------------------------------------------------------------
void SEAppliedTemperature::ToString(std::ostream& str) const
{
  str << "Applied Temperature :";
  str << "\n\tTemperature :";
  HasTemperature() ? str << *m_Temperature : str << "NaN";
  str << "\n\tSurfaceArea :";
  HasSurfaceArea() ? str << *m_SurfaceArea : str << "NaN";
  str << "\n\tSurfaceAreaFraction :";
  HasSurfaceAreaFraction() ? str << *m_SurfaceAreaFraction : str << "NaN";
  str << "\n\tState :" << m_State;
  str << std::flush;
}
//-----------------------------------------------------------------------------
bool SEAppliedTemperature::operator==(SEAppliedTemperature const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_State == rhs.m_State
    && ((m_Temperature && rhs.m_Temperature) ? m_Temperature->operator==(*rhs.m_Temperature) : m_Temperature == rhs.m_Temperature)
    && ((m_SurfaceArea && rhs.m_SurfaceArea) ? m_SurfaceArea->operator==(*rhs.m_SurfaceArea) : m_SurfaceArea == rhs.m_SurfaceArea)
    && ((m_SurfaceAreaFraction && rhs.m_SurfaceAreaFraction) ? m_SurfaceAreaFraction->operator==(*rhs.m_SurfaceAreaFraction) : m_SurfaceAreaFraction == rhs.m_SurfaceAreaFraction);
  
}
bool SEAppliedTemperature::operator!=(SEAppliedTemperature const& rhs) const
{
  return !(this->operator==(rhs));
}
}