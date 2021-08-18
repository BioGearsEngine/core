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

namespace biogears {
class BIOGEARS_API SEScalar0To1 : public SEScalar {

public:
  SEScalar0To1();
  virtual ~SEScalar0To1();

  CDM::Scalar0To1Data* Unload() const;

  double GetValue() const { return SEScalar::GetValue(); }
  double GetValue(const NoUnit& unitless) { return SEScalar::GetValue(); }

  void SetValue(double d);
  void SetValue(double d, const NoUnit& unitless);

  bool operator==(const SEScalar0To1&) const;
  bool operator!=(const SEScalar0To1&) const;

};
}
