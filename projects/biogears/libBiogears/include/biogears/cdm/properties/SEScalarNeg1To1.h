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
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class BIOGEARS_API SEScalarNeg1To1 : public SEScalar {

public:
  SEScalarNeg1To1();
  virtual ~SEScalarNeg1To1();    

  bool operator==(const SEScalarNeg1To1&) const;
  bool operator!=(const SEScalarNeg1To1&) const;

  CDM::ScalarNeg1To1Data* Unload() const override;

  using SEScalar::GetValue;
  double GetValue(const NoUnit& unitless) const;
  void SetValue(double d) override;
  void SetValue(double d, const NoUnit& unitless);
};
}