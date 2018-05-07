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

#include <biogears/cdm/stdafx.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>

const LengthPerTimeUnit LengthPerTimeUnit::m_Per_s("m/s");
const LengthPerTimeUnit LengthPerTimeUnit::cm_Per_s("cm/s");
const LengthPerTimeUnit LengthPerTimeUnit::m_Per_min("m/min");
const LengthPerTimeUnit LengthPerTimeUnit::cm_Per_min("cm/min");
const LengthPerTimeUnit LengthPerTimeUnit::ft_Per_s("ft/s");
const LengthPerTimeUnit LengthPerTimeUnit::ft_Per_min("ft/min");

CDM::ScalarLengthPerTimeData* SEScalarLengthPerTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarLengthPerTimeData* data(new CDM::ScalarLengthPerTimeData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool LengthPerTimeUnit::IsValidUnit(const std::string& unit)
{
  if (m_Per_s.GetString().compare(unit) == 0)
    return true;
  if (cm_Per_s.GetString().compare(unit) == 0)
    return true;
  if (m_Per_min.GetString().compare(unit) == 0)
    return true;
  if (cm_Per_min.GetString().compare(unit) == 0)
    return true;
  if (ft_Per_s.GetString().compare(unit) == 0)
    return true;
  if (ft_Per_min.GetString().compare(unit) == 0)
    return true;
  return false;
}

const LengthPerTimeUnit& LengthPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (m_Per_s.GetString().compare(unit) == 0)
    return m_Per_s;
  if (cm_Per_s.GetString().compare(unit) == 0)
    return cm_Per_s;
  if (m_Per_min.GetString().compare(unit) == 0)
    return m_Per_min;
  if (cm_Per_min.GetString().compare(unit) == 0)
    return cm_Per_min;
  if (ft_Per_s.GetString().compare(unit) == 0)
    return ft_Per_s;
  if (ft_Per_min.GetString().compare(unit) == 0)
    return ft_Per_min;
  std::stringstream err;
  err << unit << " is not a valid LengthPerTime unit";
  throw CommonDataModelException(err.str());
}