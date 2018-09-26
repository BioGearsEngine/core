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

#include <biogears/cdm/properties/SEScalarLength.h>

namespace biogears {
const LengthUnit LengthUnit::m("m");
const LengthUnit LengthUnit::cm("cm");
const LengthUnit LengthUnit::mm("mm");
const LengthUnit LengthUnit::um("um");
const LengthUnit LengthUnit::in("in");
const LengthUnit LengthUnit::ft("ft");

CDM::ScalarLengthData* SEScalarLength::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarLengthData* data(new CDM::ScalarLengthData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool LengthUnit::IsValidUnit(const std::string& unit)
{
  if (m.GetString().compare(unit) == 0)
    return true;
  if (cm.GetString().compare(unit) == 0)
    return true;
  if (mm.GetString().compare(unit) == 0)
    return true;
  if (um.GetString().compare(unit) == 0)
    return true;
  if (in.GetString().compare(unit) == 0)
    return true;
  if (ft.GetString().compare(unit) == 0)
    return true;
  return false;
}

const LengthUnit& LengthUnit::GetCompoundUnit(const std::string& unit)
{
  if (m.GetString().compare(unit) == 0)
    return m;
  if (cm.GetString().compare(unit) == 0)
    return cm;
  if (mm.GetString().compare(unit) == 0)
    return mm;
  if (um.GetString().compare(unit) == 0)
    return um;
  if (in.GetString().compare(unit) == 0)
    return in;
  if (ft.GetString().compare(unit) == 0)
    return ft;
  std::stringstream err;
  err << unit << " is not a valid Length unit";
  throw CommonDataModelException(err.str());
}
}