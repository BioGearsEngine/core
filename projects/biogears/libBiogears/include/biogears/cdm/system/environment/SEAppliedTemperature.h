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
#include "biogears/cdm/properties/SEScalarFraction.h"
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>

CDM_BIND_DECL(AppliedTemperatureData)

namespace biogears {
class SEScalar;
class SEScalarTemperature;
class TemperatureUnit;
class SEScalarArea;
class AreaUnit;
namespace io {
  class Environment;
}
class BIOGEARS_API SEAppliedTemperature : public Loggable {
public:
  friend io::Environment;
  SEAppliedTemperature(Logger* logger);
  SEAppliedTemperature(const SEAppliedTemperature& other);
  virtual ~SEAppliedTemperature();

  virtual void Clear();

  virtual bool Load(const CDM::AppliedTemperatureData& in);
  virtual CDM::AppliedTemperatureData* Unload() const;

protected:
  virtual void Unload(CDM::AppliedTemperatureData& data) const;

public:
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasTemperature() const;
  virtual SEScalarTemperature& GetTemperature();
  virtual double GetTemperature(const TemperatureUnit& unit) const;

  virtual bool HasSurfaceArea() const;
  virtual SEScalarArea& GetSurfaceArea();
  virtual double GetSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasSurfaceAreaFraction() const;
  virtual SEScalarFraction& GetSurfaceAreaFraction();
  virtual double GetSurfaceAreaFraction() const;

  virtual CDM::enumOnOff::value GetState() const;
  virtual void SetState(CDM::enumOnOff::value onOff);

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalarTemperature* m_Temperature;
  SEScalarArea* m_SurfaceArea;
  SEScalarFraction* m_SurfaceAreaFraction;
  CDM::enumOnOff::value m_State;
};
}