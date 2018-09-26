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
#include <biogears/cdm/patient/actions/SEForcedExhale.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarTime.h>

namespace biogears {
SEForcedExhale::SEForcedExhale()
  : SEConsciousRespirationCommand()
{
  m_ExpiratoryReserveVolumeFraction = nullptr;
  m_Period = nullptr;
}

SEForcedExhale::~SEForcedExhale()
{
  Clear();
}

void SEForcedExhale::Clear()
{
  SEConsciousRespirationCommand::Clear();
  SAFE_DELETE(m_ExpiratoryReserveVolumeFraction);
  SAFE_DELETE(m_Period);
}

bool SEForcedExhale::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasExpiratoryReserveVolumeFraction() && HasPeriod();
}

bool SEForcedExhale::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEForcedExhale::Load(const CDM::ForcedExhaleData& in)
{
  SEConsciousRespirationCommand::Load(in);
  GetExpiratoryReserveVolumeFraction().Load(in.ExpiratoryReserveVolumeFraction());
  GetPeriod().Load(in.Period());
  return true;
}

CDM::ForcedExhaleData* SEForcedExhale::Unload() const
{
  CDM::ForcedExhaleData* data(new CDM::ForcedExhaleData());
  Unload(*data);
  return data;
}

void SEForcedExhale::Unload(CDM::ForcedExhaleData& data) const
{
  SEConsciousRespirationCommand::Unload(data);
  if (m_ExpiratoryReserveVolumeFraction != nullptr)
    data.ExpiratoryReserveVolumeFraction(std::unique_ptr<CDM::Scalar0To1Data>(m_ExpiratoryReserveVolumeFraction->Unload()));
  if (m_Period != nullptr)
    data.Period(std::unique_ptr<CDM::ScalarTimeData>(m_Period->Unload()));
}

bool SEForcedExhale::HasExpiratoryReserveVolumeFraction() const
{
  return m_ExpiratoryReserveVolumeFraction == nullptr ? false : m_ExpiratoryReserveVolumeFraction->IsValid();
}
SEScalar0To1& SEForcedExhale::GetExpiratoryReserveVolumeFraction()
{
  if (m_ExpiratoryReserveVolumeFraction == nullptr)
    m_ExpiratoryReserveVolumeFraction = new SEScalar0To1();
  return *m_ExpiratoryReserveVolumeFraction;
}

bool SEForcedExhale::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEForcedExhale::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}

void SEForcedExhale::ToString(std::ostream& str) const
{
  str << "Forced Exhale";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tExpiratoryReserveVolumeFraction: ";
  HasExpiratoryReserveVolumeFraction() ? str << *m_ExpiratoryReserveVolumeFraction : str << "NaN";
  str << "\n\tPeriod: ";
  HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}
}