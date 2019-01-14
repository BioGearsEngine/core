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
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class SECircuitManager;
class BIOGEARS_API SEFluidCircuitNode : public SECircuitNode<FLUID_CIRCUIT_NODE> {
  friend class SECircuitManager;

protected:
  SEFluidCircuitNode(const char* name, Logger* logger);
  SEFluidCircuitNode(const std::string& name, Logger* logger);
public:
  virtual ~SEFluidCircuitNode();

  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::FluidCircuitNodeData& in);
  virtual CDM::FluidCircuitNodeData* Unload() const;

protected:
  virtual void Unload(CDM::FluidCircuitNodeData& data) const;

public:
  virtual bool HasPressure() const;
  virtual SEScalarPressure& GetPressure();
  virtual double GetPressure(const PressureUnit& unit) const;

  virtual bool HasNextPressure() const;
  virtual SEScalarPressure& GetNextPressure();
  virtual double GetNextPressure(const PressureUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual bool HasNextVolume() const;
  virtual SEScalarVolume& GetNextVolume();
  virtual double GetNextVolume(const VolumeUnit& unit) const;

  virtual bool HasVolumeBaseline() const;
  virtual SEScalarVolume& GetVolumeBaseline();
  virtual double GetVolumeBaseline(const VolumeUnit& unit) const;
};
}