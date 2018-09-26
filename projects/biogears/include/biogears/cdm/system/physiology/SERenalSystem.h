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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>
#include "biogears/cdm/properties/SEScalarVolumePerTimePressure.h"

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

class BIOGEARS_API SERenalSystem : public SESystem {
public:
  SERenalSystem(Logger* logger);
  virtual ~SERenalSystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::RenalSystemData& in);
  virtual CDM::RenalSystemData* Unload() const;

protected:
  virtual void Unload(CDM::RenalSystemData& data) const;

public:
  virtual bool HasGlomerularFiltrationRate() const;
  virtual SEScalarVolumePerTime& GetGlomerularFiltrationRate();
  virtual double GetGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasFiltrationFraction() const;
  virtual SEScalarFraction& GetFiltrationFraction();
  virtual double GetFiltrationFraction() const;

  virtual bool HasLeftAfferentArterioleResistance() const;
  virtual SEScalarFlowResistance& GetLeftAfferentArterioleResistance();
  virtual double GetLeftAfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasLeftBowmansCapsulesHydrostaticPressure() const;
  virtual SEScalarPressure& GetLeftBowmansCapsulesHydrostaticPressure();
  virtual double GetLeftBowmansCapsulesHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftBowmansCapsulesOsmoticPressure() const;
  virtual SEScalarPressure& GetLeftBowmansCapsulesOsmoticPressure();
  virtual double GetLeftBowmansCapsulesOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftEfferentArterioleResistance() const;
  virtual SEScalarFlowResistance& GetLeftEfferentArterioleResistance();
  virtual double GetLeftEfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasLeftGlomerularCapillariesHydrostaticPressure() const;
  virtual SEScalarPressure& GetLeftGlomerularCapillariesHydrostaticPressure();
  virtual double GetLeftGlomerularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftGlomerularCapillariesOsmoticPressure() const;
  virtual SEScalarPressure& GetLeftGlomerularCapillariesOsmoticPressure();
  virtual double GetLeftGlomerularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftGlomerularFiltrationCoefficient() const;
  virtual SEScalarVolumePerTimePressure& GetLeftGlomerularFiltrationCoefficient();
  virtual double GetLeftGlomerularFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasLeftGlomerularFiltrationRate() const;
  virtual SEScalarVolumePerTime& GetLeftGlomerularFiltrationRate();
  virtual double GetLeftGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasLeftGlomerularFiltrationSurfaceArea() const;
  virtual SEScalarArea& GetLeftGlomerularFiltrationSurfaceArea();
  virtual double GetLeftGlomerularFiltrationSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasLeftGlomerularFluidPermeability() const;
  virtual SEScalarVolumePerTimePressureArea& GetLeftGlomerularFluidPermeability();
  virtual double GetLeftGlomerularFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasLeftFiltrationFraction() const;
  virtual SEScalarFraction& GetLeftFiltrationFraction();
  virtual double GetLeftFiltrationFraction() const;

  virtual bool HasLeftNetFiltrationPressure() const;
  virtual SEScalarPressure& GetLeftNetFiltrationPressure();
  virtual double GetLeftNetFiltrationPressure(const PressureUnit& unit) const;

  virtual bool HasLeftNetReabsorptionPressure() const;
  virtual SEScalarPressure& GetLeftNetReabsorptionPressure();
  virtual double GetLeftNetReabsorptionPressure(const PressureUnit& unit) const;

  virtual bool HasLeftPeritubularCapillariesHydrostaticPressure() const;
  virtual SEScalarPressure& GetLeftPeritubularCapillariesHydrostaticPressure();
  virtual double GetLeftPeritubularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftPeritubularCapillariesOsmoticPressure() const;
  virtual SEScalarPressure& GetLeftPeritubularCapillariesOsmoticPressure();
  virtual double GetLeftPeritubularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftReabsorptionFiltrationCoefficient() const;
  virtual SEScalarVolumePerTimePressure& GetLeftReabsorptionFiltrationCoefficient();
  virtual double GetLeftReabsorptionFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasLeftReabsorptionRate() const;
  virtual SEScalarVolumePerTime& GetLeftReabsorptionRate();
  virtual double GetLeftReabsorptionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasLeftTubularReabsorptionFiltrationSurfaceArea() const;
  virtual SEScalarArea& GetLeftTubularReabsorptionFiltrationSurfaceArea();
  virtual double GetLeftTubularReabsorptionFiltrationSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasLeftTubularReabsorptionFluidPermeability() const;
  virtual SEScalarVolumePerTimePressureArea& GetLeftTubularReabsorptionFluidPermeability();
  virtual double GetLeftTubularReabsorptionFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasLeftTubularHydrostaticPressure() const;
  virtual SEScalarPressure& GetLeftTubularHydrostaticPressure();
  virtual double GetLeftTubularHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasLeftTubularOsmoticPressure() const;
  virtual SEScalarPressure& GetLeftTubularOsmoticPressure();
  virtual double GetLeftTubularOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasRenalBloodFlow() const;
  virtual SEScalarVolumePerTime& GetRenalBloodFlow();
  virtual double GetRenalBloodFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasRenalPlasmaFlow() const;
  virtual SEScalarVolumePerTime& GetRenalPlasmaFlow();
  virtual double GetRenalPlasmaFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasRenalVascularResistance() const;
  virtual SEScalarFlowResistance& GetRenalVascularResistance();
  virtual double GetRenalVascularResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRightAfferentArterioleResistance() const;
  virtual SEScalarFlowResistance& GetRightAfferentArterioleResistance();
  virtual double GetRightAfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRightBowmansCapsulesHydrostaticPressure() const;
  virtual SEScalarPressure& GetRightBowmansCapsulesHydrostaticPressure();
  virtual double GetRightBowmansCapsulesHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasRightBowmansCapsulesOsmoticPressure() const;
  virtual SEScalarPressure& GetRightBowmansCapsulesOsmoticPressure();
  virtual double GetRightBowmansCapsulesOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasRightEfferentArterioleResistance() const;
  virtual SEScalarFlowResistance& GetRightEfferentArterioleResistance();
  virtual double GetRightEfferentArterioleResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRightGlomerularCapillariesHydrostaticPressure() const;
  virtual SEScalarPressure& GetRightGlomerularCapillariesHydrostaticPressure();
  virtual double GetRightGlomerularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasRightGlomerularCapillariesOsmoticPressure() const;
  virtual SEScalarPressure& GetRightGlomerularCapillariesOsmoticPressure();
  virtual double GetRightGlomerularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasRightGlomerularFiltrationCoefficient() const;
  virtual SEScalarVolumePerTimePressure& GetRightGlomerularFiltrationCoefficient();
  virtual double GetRightGlomerularFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasRightGlomerularFiltrationRate() const;
  virtual SEScalarVolumePerTime& GetRightGlomerularFiltrationRate();
  virtual double GetRightGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasRightGlomerularFiltrationSurfaceArea() const;
  virtual SEScalarArea& GetRightGlomerularFiltrationSurfaceArea();
  virtual double GetRightGlomerularFiltrationSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasRightGlomerularFluidPermeability() const;
  virtual SEScalarVolumePerTimePressureArea& GetRightGlomerularFluidPermeability();
  virtual double GetRightGlomerularFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasRightFiltrationFraction() const;
  virtual SEScalarFraction& GetRightFiltrationFraction();
  virtual double GetRightFiltrationFraction() const;

  virtual bool HasRightNetFiltrationPressure() const;
  virtual SEScalarPressure& GetRightNetFiltrationPressure();
  virtual double GetRightNetFiltrationPressure(const PressureUnit& unit) const;

  virtual bool HasRightNetReabsorptionPressure() const;
  virtual SEScalarPressure& GetRightNetReabsorptionPressure();
  virtual double GetRightNetReabsorptionPressure(const PressureUnit& unit) const;

  virtual bool HasRightPeritubularCapillariesHydrostaticPressure() const;
  virtual SEScalarPressure& GetRightPeritubularCapillariesHydrostaticPressure();
  virtual double GetRightPeritubularCapillariesHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasRightPeritubularCapillariesOsmoticPressure() const;
  virtual SEScalarPressure& GetRightPeritubularCapillariesOsmoticPressure();
  virtual double GetRightPeritubularCapillariesOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasRightReabsorptionFiltrationCoefficient() const;
  virtual SEScalarVolumePerTimePressure& GetRightReabsorptionFiltrationCoefficient();
  virtual double GetRightReabsorptionFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasRightReabsorptionRate() const;
  virtual SEScalarVolumePerTime& GetRightReabsorptionRate();
  virtual double GetRightReabsorptionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasRightTubularReabsorptionFiltrationSurfaceArea() const;
  virtual SEScalarArea& GetRightTubularReabsorptionFiltrationSurfaceArea();
  virtual double GetRightTubularReabsorptionFiltrationSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasRightTubularReabsorptionFluidPermeability() const;
  virtual SEScalarVolumePerTimePressureArea& GetRightTubularReabsorptionFluidPermeability();
  virtual double GetRightTubularReabsorptionFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasRightTubularHydrostaticPressure() const;
  virtual SEScalarPressure& GetRightTubularHydrostaticPressure();
  virtual double GetRightTubularHydrostaticPressure(const PressureUnit& unit) const;

  virtual bool HasRightTubularOsmoticPressure() const;
  virtual SEScalarPressure& GetRightTubularOsmoticPressure();
  virtual double GetRightTubularOsmoticPressure(const PressureUnit& unit) const;

  virtual bool HasUrinationRate() const;
  virtual SEScalarVolumePerTime& GetUrinationRate();
  virtual double GetUrinationRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasUrineOsmolality() const;
  virtual SEScalarOsmolality& GetUrineOsmolality();
  virtual double GetUrineOsmolality(const OsmolalityUnit& unit) const;

  virtual bool HasUrineOsmolarity() const;
  virtual SEScalarOsmolarity& GetUrineOsmolarity();
  virtual double GetUrineOsmolarity(const OsmolarityUnit& unit) const;

  virtual bool HasUrineProductionRate() const;
  virtual SEScalarVolumePerTime& GetUrineProductionRate();
  virtual double GetUrineProductionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasUrineSpecificGravity() const;
  virtual SEScalar& GetUrineSpecificGravity();
  virtual double GetUrineSpecificGravity() const;

  virtual bool HasUrineVolume() const;
  virtual SEScalarVolume& GetUrineVolume();
  virtual double GetUrineVolume(const VolumeUnit& unit) const;

  virtual bool HasUrineUreaNitrogenConcentration() const;
  virtual SEScalarMassPerVolume& GetUrineUreaNitrogenConcentration();
  virtual double GetUrineUreaNitrogenConcentration(const MassPerVolumeUnit& unit) const;

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
  SEScalar* m_UrineSpecificGravity;
  SEScalarVolume* m_UrineVolume;
  SEScalarMassPerVolume* m_UrineUreaNitrogenConcentration;
};
}