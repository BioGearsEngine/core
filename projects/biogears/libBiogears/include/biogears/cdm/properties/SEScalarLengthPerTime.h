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

#pragma once
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class BIOGEARS_API LengthPerTimeUnit : public CCompoundUnit {
public:
  LengthPerTimeUnit(const char* u);
  LengthPerTimeUnit(const std::string& u);

  virtual ~LengthPerTimeUnit();

  bool operator==(const LengthPerTimeUnit&) const;
  bool operator!=(const LengthPerTimeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const LengthPerTimeUnit& GetCompoundUnit(const char* unit);
  static const LengthPerTimeUnit& GetCompoundUnit(const std::string& unit);

  static const LengthPerTimeUnit m_Per_s;
  static const LengthPerTimeUnit cm_Per_s;
  static const LengthPerTimeUnit m_Per_min;
  static const LengthPerTimeUnit cm_Per_min;
  static const LengthPerTimeUnit ft_Per_s;
  static const LengthPerTimeUnit ft_Per_min;
};
BG_EXT template class BIOGEARS_API SEScalarQuantity<LengthPerTimeUnit>;

class BIOGEARS_API SEScalarLengthPerTime : public SEScalarQuantity<LengthPerTimeUnit> {
public:
  SEScalarLengthPerTime();
  virtual ~SEScalarLengthPerTime();

  CDM::ScalarLengthPerTimeData* Unload() const override;

  using SEScalarQuantity<LengthPerTimeUnit>::SetValue;
  using SEScalarQuantity<LengthPerTimeUnit>::GetValue;

  bool operator==(const SEScalarLengthPerTime&) const;
  bool operator!=(const SEScalarLengthPerTime&) const;
};
}