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
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarArea.h>

namespace biogears {
SEImpairedAlveolarExchange::SEImpairedAlveolarExchange()
  : SEPatientCondition()
{
  m_ImpairedSurfaceArea = nullptr;
  m_ImpairedFraction = nullptr;
}
//-----------------------------------------------------------------------------
SEImpairedAlveolarExchange::~SEImpairedAlveolarExchange()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEImpairedAlveolarExchange::Clear()
{
  SAFE_DELETE(m_ImpairedSurfaceArea);
  SAFE_DELETE(m_ImpairedFraction);
}
//-----------------------------------------------------------------------------
bool SEImpairedAlveolarExchange::IsValid() const
{
  return HasImpairedFraction() || HasImpairedSurfaceArea();
}
//-----------------------------------------------------------------------------
bool SEImpairedAlveolarExchange::Load(const CDM::ImpairedAlveolarExchangeData& in)
{
  if (in.ImpairedSurfaceArea().present())
    GetImpairedSurfaceArea().Load(in.ImpairedSurfaceArea().get());
  if (in.ImpairedFraction().present())
    GetImpairedFraction().Load(in.ImpairedFraction().get());
  return true;
}
//-----------------------------------------------------------------------------
CDM::ImpairedAlveolarExchangeData* SEImpairedAlveolarExchange::Unload() const
//-----------------------------------------------------------------------------
{
  CDM::ImpairedAlveolarExchangeData* data = new CDM::ImpairedAlveolarExchangeData();
  Unload(*data);
  return data;
}
void SEImpairedAlveolarExchange::Unload(CDM::ImpairedAlveolarExchangeData& data) const
{
  if (HasImpairedSurfaceArea())
    data.ImpairedSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_ImpairedSurfaceArea->Unload()));
  if (HasImpairedFraction())
    data.ImpairedFraction(std::unique_ptr<CDM::Scalar0To1Data>(m_ImpairedFraction->Unload()));
}
//-----------------------------------------------------------------------------
bool SEImpairedAlveolarExchange::HasImpairedSurfaceArea() const
{
  return m_ImpairedSurfaceArea == nullptr ? false : m_ImpairedSurfaceArea->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarArea& SEImpairedAlveolarExchange::GetImpairedSurfaceArea()
{
  if (m_ImpairedSurfaceArea == nullptr)
    m_ImpairedSurfaceArea = new SEScalarArea();
  return *m_ImpairedSurfaceArea;
}
//-----------------------------------------------------------------------------
double SEImpairedAlveolarExchange::GetImpairedSurfaceArea(const AreaUnit& unit) const
{
  if (m_ImpairedSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_ImpairedSurfaceArea->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEImpairedAlveolarExchange::HasImpairedFraction() const
{
  return m_ImpairedFraction == nullptr ? false : m_ImpairedFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEImpairedAlveolarExchange::GetImpairedFraction()
{
  if (m_ImpairedFraction == nullptr)
    m_ImpairedFraction = new SEScalar0To1();
  return *m_ImpairedFraction;
}
//-----------------------------------------------------------------------------
double SEImpairedAlveolarExchange::GetImpairedFraction() const
{
  if (m_ImpairedFraction == nullptr)
    return SEScalar::dNaN();
  return m_ImpairedFraction->GetValue();
}
//-----------------------------------------------------------------------------
void SEImpairedAlveolarExchange::ToString(std::ostream& str) const
{
  str << "Impaired Alveoli Exchange :";
  str << "\n\tImpairedSurfaceArea :";
  HasImpairedSurfaceArea() ? str << *m_ImpairedSurfaceArea : str << "Not Provided";
  str << "\n\tImpairedFraction :";
  HasImpairedFraction() ? str << *m_ImpairedFraction : str << "Not Provided";

  str << std::flush;
}
//-----------------------------------------------------------------------------
}