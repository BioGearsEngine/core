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
class BIOGEARS_API SEScalarFraction : public SEScalar {

public:
  SEScalarFraction();

  virtual ~SEScalarFraction(); 

  bool operator==(const SEScalarFraction&) const;
  bool operator!=(const SEScalarFraction&) const;

  CDM::ScalarFractionData* Unload() const;

  using SEScalar::GetValue;
  double GetValue(const NoUnit& unitless) const;
  using SEScalar::SetValue;
  void SetValue(double d, const NoUnit& unitless);
};
}