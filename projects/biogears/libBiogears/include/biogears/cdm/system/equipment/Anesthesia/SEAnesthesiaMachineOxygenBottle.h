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
#include <biogears/exports.h>
#include <biogears/schema/cdm/AnesthesiaActions.hxx>

CDM_BIND_DECL(AnesthesiaMachineOxygenBottleData)

namespace biogears {
class SEAnesthesiaMachine;
class SESubstanceManager;
class SEScalar;
class SEScalarVolume;
class VolumeUnit;
namespace io {
  class Anesthesia;
}

class BIOGEARS_API SEAnesthesiaMachineOxygenBottle : Loggable {
protected:
  friend SEAnesthesiaMachine;
  friend io::Anesthesia;

public:
  SEAnesthesiaMachineOxygenBottle(Logger* logger);
  virtual ~SEAnesthesiaMachineOxygenBottle();

  virtual void Clear();

  virtual bool Load(const CDM::AnesthesiaMachineOxygenBottleData& in);
  virtual CDM::AnesthesiaMachineOxygenBottleData* Unload() const;

protected:
  virtual void Unload(CDM::AnesthesiaMachineOxygenBottleData& data) const;

  virtual void Merge(const SEAnesthesiaMachineOxygenBottle& from);

public:
  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual void ToString(std::ostream& str);

protected:
  SEScalarVolume* m_Volume;
};
}