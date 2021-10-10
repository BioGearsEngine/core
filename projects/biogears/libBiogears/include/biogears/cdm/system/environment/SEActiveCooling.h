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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/Environment.hxx>

namespace biogears {
class SEScalar;
class SEScalarFraction;
class SEScalarPower;
class PowerUnit;
class SEScalarArea;
class AreaUnit;
namespace io {
  class Environment;
}
class BIOGEARS_API SEActiveCooling : public Loggable {
public:
  friend io::Environment;
  SEActiveCooling(Logger* logger);
  virtual ~SEActiveCooling();

  virtual void Clear();
  virtual void Reset();

  virtual bool Load(const CDM::ActiveCoolingData& in);
  virtual CDM::ActiveCoolingData* Unload() const;

protected:
  virtual void Unload(CDM::ActiveCoolingData& data) const;

public:
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasPower() const;
  virtual SEScalarPower& GetPower();
  virtual double GetPower(const PowerUnit& unit) const;

  virtual bool HasSurfaceArea() const;
  virtual SEScalarArea& GetSurfaceArea();
  virtual double GetSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasSurfaceAreaFraction() const;
  virtual SEScalarFraction& GetSurfaceAreaFraction();
  virtual double GetSurfaceAreaFraction() const;

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalarPower* m_Power;
  SEScalarArea* m_SurfaceArea;
  SEScalarFraction* m_SurfaceAreaFraction;
};
}