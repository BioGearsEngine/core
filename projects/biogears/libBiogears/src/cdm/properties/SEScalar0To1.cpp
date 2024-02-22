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

#include <biogears/cdm/properties/SEScalar0To1.h>

namespace biogears {
SEScalar0To1::SEScalar0To1()
  : SEScalar()
{
}
//-------------------------------------------------------------------------------
SEScalar0To1::~SEScalar0To1()
{
}
//-------------------------------------------------------------------------------
void SEScalar0To1::Load(const CDM::ScalarData& in, std::random_device* rd)
{
  Clear();
  SEProperty::Load(in);

  if (in.deviation().present() && rd) {
    auto nd = std::normal_distribution(in.value(), in.deviation().get());
    SetValue( std::clamp(nd(*rd), 0.0, 1.0));
  } else {
    SetValue(in.value());
  }
  if (in.unit().present()) {
    std::string u = in.unit().get();
    if (!("unitless" == u || "" == u || u.empty())) {
      throw CommonDataModelException("CDM::Scalar API is intended to be unitless, You are trying to load a ScalarData with a unit defined");
    }
  }
  m_readOnly = in.readOnly();
}

//-------------------------------------------------------------------------------
CDM::Scalar0To1Data* SEScalar0To1::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::Scalar0To1Data* data(new CDM::Scalar0To1Data());
  SEScalar::Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEScalar0To1::SetValue(double d)
{
  if (d > 1 || d < 0)
    throw CommonDataModelException("SEScalar0To1 must be between [0,1]");
  SEScalar::SetValue(d);
}
//-------------------------------------------------------------------------------
void SEScalar0To1::SetValue(double d, const NoUnit& unitless)
{
  SetValue(d);
}
//-------------------------------------------------------------------------------
bool SEScalar0To1::operator==(const SEScalar0To1& obj) const
{
  return m_value == obj.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar0To1::operator!=(const SEScalar0To1& obj) const
{
  return !(*this == obj);
}
}