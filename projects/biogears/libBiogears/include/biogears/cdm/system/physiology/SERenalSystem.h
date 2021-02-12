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
#include "biogears/cdm/properties/SEScalarVolumePerTimePressure.h"
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalarPressure;
class PressureUnit;
class SEScalarFraction;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SEScalarFlowCompliance;
class FlowComplianceUnit;
class SEScalarFlowResistance;
class FlowResistanceUnit;
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarVolume;
class VolumeUnit;
class SEScalarVolumePerTimePressure;
class VolumePerTimePressureUnit;
class SEScalarArea;
class AreaUnit;
class SEScalarVolumePerTimePressureArea;
class VolumePerTimePressureAreaUnit;
class OsmolarityUnit;
class OsmolalityUnit;
class MassPerVolumeUnit;
namespace io {
  class Physiology;
}
class BIOGEARS_API SERenalSystem : public SESystem {
  friend io::Physiology;

public:
  SERenalSystem(Logger* logger);
  ~SERenalSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SERenalSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::RenalSystemData& in);
  CDM::RenalSystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::RenalSystemData& data) const;

public:
  bool HasGlomerularFiltrationRate() const;
  SEScalarVolumePerTime& GetGlomerularFiltrationRate();
  double GetGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const;

  bool HasFiltrationFraction() const;
  SEScalarFraction& GetFiltrationFraction();
  double GetFiltrationFraction() const;

  bool HasLeftAfferentArterioleResistance() const;
  SEScalarFlowResistance& GetLeftAfferentArterioleResistance();
  double GetLeftAfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  bool HasLeftBowmansCapsulesHydrostaticPressure() const;
  SEScalarPressure& GetLeftBowmansCapsulesHydrostaticPressure();
  double GetLeftBowmansCapsulesHydrostaticPressure(const PressureUnit& unit) const;

  bool HasLeftBowmansCapsulesOsmoticPressure() const;
  SEScalarPressure& GetLeftBowmansCapsulesOsmoticPressure();
  double GetLeftBowmansCapsulesOsmoticPressure(const PressureUnit& unit) const;

  bool HasLeftEfferentArterioleResistance() const;
  SEScalarFlowResistance& GetLeftEfferentArterioleResistance();
  double GetLeftEfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  bool HasLeftGlomerularCapillariesHydrostaticPressure() const;
  SEScalarPressure& GetLeftGlomerularCapillariesHydrostaticPressure();
  double GetLeftGlomerularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  bool HasLeftGlomerularCapillariesOsmoticPressure() const;
  SEScalarPressure& GetLeftGlomerularCapillariesOsmoticPressure();
  double GetLeftGlomerularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  bool HasLeftGlomerularFiltrationCoefficient() const;
  SEScalarVolumePerTimePressure& GetLeftGlomerularFiltrationCoefficient();
  double GetLeftGlomerularFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  bool HasLeftGlomerularFiltrationRate() const;
  SEScalarVolumePerTime& GetLeftGlomerularFiltrationRate();
  double GetLeftGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const;

  bool HasLeftGlomerularFiltrationSurfaceArea() const;
  SEScalarArea& GetLeftGlomerularFiltrationSurfaceArea();
  double GetLeftGlomerularFiltrationSurfaceArea(const AreaUnit& unit) const;

  bool HasLeftGlomerularFluidPermeability() const;
  SEScalarVolumePerTimePressureArea& GetLeftGlomerularFluidPermeability();
  double GetLeftGlomerularFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  bool HasLeftFiltrationFraction() const;
  SEScalarFraction& GetLeftFiltrationFraction();
  double GetLeftFiltrationFraction() const;

  bool HasLeftNetFiltrationPressure() const;
  SEScalarPressure& GetLeftNetFiltrationPressure();
  double GetLeftNetFiltrationPressure(const PressureUnit& unit) const;

  bool HasLeftNetReabsorptionPressure() const;
  SEScalarPressure& GetLeftNetReabsorptionPressure();
  double GetLeftNetReabsorptionPressure(const PressureUnit& unit) const;

  bool HasLeftPeritubularCapillariesHydrostaticPressure() const;
  SEScalarPressure& GetLeftPeritubularCapillariesHydrostaticPressure();
  double GetLeftPeritubularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  bool HasLeftPeritubularCapillariesOsmoticPressure() const;
  SEScalarPressure& GetLeftPeritubularCapillariesOsmoticPressure();
  double GetLeftPeritubularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  bool HasLeftReabsorptionFiltrationCoefficient() const;
  SEScalarVolumePerTimePressure& GetLeftReabsorptionFiltrationCoefficient();
  double GetLeftReabsorptionFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  bool HasLeftReabsorptionRate() const;
  SEScalarVolumePerTime& GetLeftReabsorptionRate();
  double GetLeftReabsorptionRate(const VolumePerTimeUnit& unit) const;

  bool HasLeftTubularReabsorptionFiltrationSurfaceArea() const;
  SEScalarArea& GetLeftTubularReabsorptionFiltrationSurfaceArea();
  double GetLeftTubularReabsorptionFiltrationSurfaceArea(const AreaUnit& unit) const;

  bool HasLeftTubularReabsorptionFluidPermeability() const;
  SEScalarVolumePerTimePressureArea& GetLeftTubularReabsorptionFluidPermeability();
  double GetLeftTubularReabsorptionFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  bool HasLeftTubularHydrostaticPressure() const;
  SEScalarPressure& GetLeftTubularHydrostaticPressure();
  double GetLeftTubularHydrostaticPressure(const PressureUnit& unit) const;

  bool HasLeftTubularOsmoticPressure() const;
  SEScalarPressure& GetLeftTubularOsmoticPressure();
  double GetLeftTubularOsmoticPressure(const PressureUnit& unit) const;

  bool HasRenalBloodFlow() const;
  SEScalarVolumePerTime& GetRenalBloodFlow();
  double GetRenalBloodFlow(const VolumePerTimeUnit& unit) const;

  bool HasRenalPlasmaFlow() const;
  SEScalarVolumePerTime& GetRenalPlasmaFlow();
  double GetRenalPlasmaFlow(const VolumePerTimeUnit& unit) const;

  bool HasRenalVascularResistance() const;
  SEScalarFlowResistance& GetRenalVascularResistance();
  double GetRenalVascularResistance(const FlowResistanceUnit& unit) const;

  bool HasRightAfferentArterioleResistance() const;
  SEScalarFlowResistance& GetRightAfferentArterioleResistance();
  double GetRightAfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  bool HasRightBowmansCapsulesHydrostaticPressure() const;
  SEScalarPressure& GetRightBowmansCapsulesHydrostaticPressure();
  double GetRightBowmansCapsulesHydrostaticPressure(const PressureUnit& unit) const;

  bool HasRightBowmansCapsulesOsmoticPressure() const;
  SEScalarPressure& GetRightBowmansCapsulesOsmoticPressure();
  double GetRightBowmansCapsulesOsmoticPressure(const PressureUnit& unit) const;

  bool HasRightEfferentArterioleResistance() const;
  SEScalarFlowResistance& GetRightEfferentArterioleResistance();
  double GetRightEfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  bool HasRightGlomerularCapillariesHydrostaticPressure() const;
  SEScalarPressure& GetRightGlomerularCapillariesHydrostaticPressure();
  double GetRightGlomerularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  bool HasRightGlomerularCapillariesOsmoticPressure() const;
  SEScalarPressure& GetRightGlomerularCapillariesOsmoticPressure();
  double GetRightGlomerularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  bool HasRightGlomerularFiltrationCoefficient() const;
  SEScalarVolumePerTimePressure& GetRightGlomerularFiltrationCoefficient();
  double GetRightGlomerularFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  bool HasRightGlomerularFiltrationRate() const;
  SEScalarVolumePerTime& GetRightGlomerularFiltrationRate();
  double GetRightGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const;

  bool HasRightGlomerularFiltrationSurfaceArea() const;
  SEScalarArea& GetRightGlomerularFiltrationSurfaceArea();
  double GetRightGlomerularFiltrationSurfaceArea(const AreaUnit& unit) const;

  bool HasRightGlomerularFluidPermeability() const;
  SEScalarVolumePerTimePressureArea& GetRightGlomerularFluidPermeability();
  double GetRightGlomerularFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  bool HasRightFiltrationFraction() const;
  SEScalarFraction& GetRightFiltrationFraction();
  double GetRightFiltrationFraction() const;

  bool HasRightNetFiltrationPressure() const;
  SEScalarPressure& GetRightNetFiltrationPressure();
  double GetRightNetFiltrationPressure(const PressureUnit& unit) const;

  bool HasRightNetReabsorptionPressure() const;
  SEScalarPressure& GetRightNetReabsorptionPressure();
  double GetRightNetReabsorptionPressure(const PressureUnit& unit) const;

  bool HasRightPeritubularCapillariesHydrostaticPressure() const;
  SEScalarPressure& GetRightPeritubularCapillariesHydrostaticPressure();
  double GetRightPeritubularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  bool HasRightPeritubularCapillariesOsmoticPressure() const;
  SEScalarPressure& GetRightPeritubularCapillariesOsmoticPressure();
  double GetRightPeritubularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  bool HasRightReabsorptionFiltrationCoefficient() const;
  SEScalarVolumePerTimePressure& GetRightReabsorptionFiltrationCoefficient();
  double GetRightReabsorptionFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  bool HasRightReabsorptionRate() const;
  SEScalarVolumePerTime& GetRightReabsorptionRate();
  double GetRightReabsorptionRate(const VolumePerTimeUnit& unit) const;

  bool HasRightTubularReabsorptionFiltrationSurfaceArea() const;
  SEScalarArea& GetRightTubularReabsorptionFiltrationSurfaceArea();
  double GetRightTubularReabsorptionFiltrationSurfaceArea(const AreaUnit& unit) const;

  bool HasRightTubularReabsorptionFluidPermeability() const;
  SEScalarVolumePerTimePressureArea& GetRightTubularReabsorptionFluidPermeability();
  double GetRightTubularReabsorptionFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  bool HasRightTubularHydrostaticPressure() const;
  SEScalarPressure& GetRightTubularHydrostaticPressure();
  double GetRightTubularHydrostaticPressure(const PressureUnit& unit) const;

  bool HasRightTubularOsmoticPressure() const;
  SEScalarPressure& GetRightTubularOsmoticPressure();
  double GetRightTubularOsmoticPressure(const PressureUnit& unit) const;

  bool HasUrinationRate() const;
  SEScalarVolumePerTime& GetUrinationRate();
  double GetUrinationRate(const VolumePerTimeUnit& unit) const;

  bool HasUrineOsmolality() const;
  SEScalarOsmolality& GetUrineOsmolality();
  double GetUrineOsmolality(const OsmolalityUnit& unit) const;

  bool HasUrineOsmolarity() const;
  SEScalarOsmolarity& GetUrineOsmolarity();
  double GetUrineOsmolarity(const OsmolarityUnit& unit) const;

  bool HasUrineProductionRate() const;
  SEScalarVolumePerTime& GetUrineProductionRate();
  double GetUrineProductionRate(const VolumePerTimeUnit& unit) const;

  bool HasMeanUrineOutput() const;
  SEScalarVolumePerTime& GetMeanUrineOutput();
  double GetMeanUrineOutput(const VolumePerTimeUnit& unit) const;

  bool HasUrineSpecificGravity() const;
  SEScalar& GetUrineSpecificGravity();
  double GetUrineSpecificGravity() const;

  bool HasUrineVolume() const;
  SEScalarVolume& GetUrineVolume();
  double GetUrineVolume(const VolumeUnit& unit) const;

  bool HasUrineUreaNitrogenConcentration() const;
  SEScalarMassPerVolume& GetUrineUreaNitrogenConcentration();
  double GetUrineUreaNitrogenConcentration(const MassPerVolumeUnit& unit) const;

protected:
  SEScalarVolumePerTime* m_GlomerularFiltrationRate;
  SEScalarFraction* m_FiltrationFraction;

  SEScalarFlowResistance* m_LeftAfferentArterioleResistance;
  SEScalarPressure* m_LeftBowmansCapsulesHydrostaticPressure;
  SEScalarPressure* m_LeftBowmansCapsulesOsmoticPressure;
  SEScalarFlowResistance* m_LeftEfferentArterioleResistance;
  SEScalarPressure* m_LeftGlomerularCapillariesHydrostaticPressure;
  SEScalarPressure* m_LeftGlomerularCapillariesOsmoticPressure;
  SEScalarVolumePerTimePressure* m_LeftGlomerularFiltrationCoefficient;
  SEScalarVolumePerTime* m_LeftGlomerularFiltrationRate;
  SEScalarArea* m_LeftGlomerularFiltrationSurfaceArea;
  SEScalarVolumePerTimePressureArea* m_LeftGlomerularFluidPermeability;
  SEScalarFraction* m_LeftFiltrationFraction;
  SEScalarPressure* m_LeftNetFiltrationPressure;
  SEScalarPressure* m_LeftNetReabsorptionPressure;
  SEScalarPressure* m_LeftPeritubularCapillariesHydrostaticPressure;
  SEScalarPressure* m_LeftPeritubularCapillariesOsmoticPressure;
  SEScalarVolumePerTimePressure* m_LeftReabsorptionFiltrationCoefficient;
  SEScalarVolumePerTime* m_LeftReabsorptionRate;
  SEScalarArea* m_LeftTubularReabsorptionFiltrationSurfaceArea;
  SEScalarVolumePerTimePressureArea* m_LeftTubularReabsorptionFluidPermeability;
  SEScalarPressure* m_LeftTubularHydrostaticPressure;
  SEScalarPressure* m_LeftTubularOsmoticPressure;

  SEScalarVolumePerTime* m_RenalBloodFlow;
  SEScalarVolumePerTime* m_RenalPlasmaFlow;
  SEScalarFlowResistance* m_RenalVascularResistance;

  SEScalarFlowResistance* m_RightAfferentArterioleResistance;
  SEScalarPressure* m_RightBowmansCapsulesHydrostaticPressure;
  SEScalarPressure* m_RightBowmansCapsulesOsmoticPressure;
  SEScalarFlowResistance* m_RightEfferentArterioleResistance;
  SEScalarPressure* m_RightGlomerularCapillariesHydrostaticPressure;
  SEScalarPressure* m_RightGlomerularCapillariesOsmoticPressure;
  SEScalarVolumePerTimePressure* m_RightGlomerularFiltrationCoefficient;
  SEScalarVolumePerTime* m_RightGlomerularFiltrationRate;
  SEScalarArea* m_RightGlomerularFiltrationSurfaceArea;
  SEScalarVolumePerTimePressureArea* m_RightGlomerularFluidPermeability;
  SEScalarFraction* m_RightFiltrationFraction;
  SEScalarPressure* m_RightNetFiltrationPressure;
  SEScalarPressure* m_RightNetReabsorptionPressure;
  SEScalarPressure* m_RightPeritubularCapillariesHydrostaticPressure;
  SEScalarPressure* m_RightPeritubularCapillariesOsmoticPressure;
  SEScalarVolumePerTimePressure* m_RightReabsorptionFiltrationCoefficient;
  SEScalarVolumePerTime* m_RightReabsorptionRate;
  SEScalarArea* m_RightTubularReabsorptionFiltrationSurfaceArea;
  SEScalarVolumePerTimePressureArea* m_RightTubularReabsorptionFluidPermeability;
  SEScalarPressure* m_RightTubularHydrostaticPressure;
  SEScalarPressure* m_RightTubularOsmoticPressure;

  SEScalarVolumePerTime* m_UrinationRate;
  SEScalarOsmolality* m_UrineOsmolality;
  SEScalarOsmolarity* m_UrineOsmolarity;
  SEScalarVolumePerTime* m_UrineProductionRate;
  SEScalarVolumePerTime* m_MeanUrineOutput;
  SEScalar* m_UrineSpecificGravity;
  SEScalarVolume* m_UrineVolume;
  SEScalarMassPerVolume* m_UrineUreaNitrogenConcentration;
};
}