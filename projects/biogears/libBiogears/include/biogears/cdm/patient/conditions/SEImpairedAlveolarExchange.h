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
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>

namespace biogears {
class SEScalarArea;
class AreaUnit;
class SEScalar0To1;
namespace io {
  class PatientConditions;
}
class BIOGEARS_API SEImpairedAlveolarExchange : public SEPatientCondition {
  friend io::PatientConditions;

public:
  SEImpairedAlveolarExchange();
  virtual ~SEImpairedAlveolarExchange();

  virtual void Clear() override ;

  virtual bool IsValid() const override ;

  bool operator==(SEImpairedAlveolarExchange const&) const;
  bool operator!=(SEImpairedAlveolarExchange const&) const;
  bool operator==(SECondition const& rhs) const override;
  bool operator!=(SECondition const& rhs) const override;

  virtual std::string GetName() const  override { return "ImpairedAlveolarExchange"; }
  virtual const char* GetName_cStr() const  override { return "ImpairedAlveolarExchange"; }

  virtual bool HasImpairedSurfaceArea() const;
  virtual SEScalarArea& GetImpairedSurfaceArea();
  virtual double GetImpairedSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasImpairedFraction() const;
  virtual SEScalar0To1& GetImpairedFraction();
  virtual double GetImpairedFraction() const;

  virtual void ToString(std::ostream& str) const override ;

protected:
  SEScalarArea* m_ImpairedSurfaceArea;
  SEScalar0To1* m_ImpairedFraction;
};
}
