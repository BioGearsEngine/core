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

package mil.tatrc.physiology.datamodel.system.physiology;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.RenalSystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SERenalSystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarVolumePerTime glomerularFiltrationRate;
  protected SEScalarFraction filtrationFraction;
  protected SEScalarFlowResistance leftAfferentArterioleResistance;
  protected SEScalarPressure leftBowmansCapsulesHydrostaticPressure;
  protected SEScalarPressure leftBowmansCapsulesOsmoticPressure;
  protected SEScalarFlowResistance leftEfferentArterioleResistance;
  protected SEScalarPressure leftGlomerularCapillariesHydrostaticPressure;
  protected SEScalarPressure leftGlomerularCapillariesOsmoticPressure;
  protected SEScalarVolumePerTimePressure leftGlomerularFiltrationCoefficient;
  protected SEScalarVolumePerTime leftGlomerularFiltrationRate;
  protected SEScalarArea leftGlomerularFiltrationSurfaceArea;
  protected SEScalarVolumePerTimePressureArea leftGlomerularFluidPermeability;
  protected SEScalarFraction leftFiltrationFraction;
  protected SEScalarPressure leftNetFiltrationPressure;
  protected SEScalarPressure leftNetReabsorptionPressure;
  protected SEScalarPressure leftPeritubularCapillariesHydrostaticPressure;
  protected SEScalarPressure leftPeritubularCapillariesOsmoticPressure;
  protected SEScalarVolumePerTimePressure leftReabsorptionFiltrationCoefficient;
  protected SEScalarVolumePerTime leftReabsorptionRate;
  protected SEScalarPressure leftTubularOsmoticPressure;
  protected SEScalarPressure leftTubularHydrostaticPressure;
  protected SEScalarArea leftTubularReabsorptionFiltrationSurfaceArea;
  protected SEScalarVolumePerTimePressureArea leftTubularReabsorptionFluidPermeability;
  protected SEScalarVolumePerTime renalBloodFlow;
  protected SEScalarVolumePerTime renalPlasmaFlow;
  protected SEScalarFlowResistance renalVascularResistance;
  protected SEScalarFlowResistance rightAfferentArterioleResistance;
  protected SEScalarPressure rightBowmansCapsulesHydrostaticPressure;
  protected SEScalarPressure rightBowmansCapsulesOsmoticPressure;
  protected SEScalarFlowResistance rightEfferentArterioleResistance;
  protected SEScalarPressure rightGlomerularCapillariesHydrostaticPressure;
  protected SEScalarPressure rightGlomerularCapillariesOsmoticPressure;
  protected SEScalarVolumePerTimePressure rightGlomerularFiltrationCoefficient;
  protected SEScalarVolumePerTime rightGlomerularFiltrationRate;
  protected SEScalarArea rightGlomerularFiltrationSurfaceArea;
  protected SEScalarVolumePerTimePressureArea rightGlomerularFluidPermeability;
  protected SEScalarFraction rightFiltrationFraction;
  protected SEScalarPressure rightNetFiltrationPressure;
  protected SEScalarPressure rightNetReabsorptionPressure;
  protected SEScalarPressure rightPeritubularCapillariesHydrostaticPressure;
  protected SEScalarPressure rightPeritubularCapillariesOsmoticPressure;
  protected SEScalarVolumePerTimePressure rightReabsorptionFiltrationCoefficient;
  protected SEScalarVolumePerTime rightReabsorptionRate;
  protected SEScalarPressure rightTubularOsmoticPressure;
  protected SEScalarPressure rightTubularHydrostaticPressure;
  protected SEScalarArea rightTubularReabsorptionFiltrationSurfaceArea;
  protected SEScalarVolumePerTimePressureArea rightTubularReabsorptionFluidPermeability;
  protected SEScalarVolumePerTime urinationRate;
  protected SEScalarOsmolality urineOsmolality;
  protected SEScalarOsmolarity urineOsmolarity;
  protected SEScalarVolumePerTime urineProductionRate;
  protected SEScalar urineSpecificGravity;
  protected SEScalarVolume urineVolume;
  protected SEScalarMassPerVolume urineUreaNitrogenConcentration;

  public SERenalSystem()
  {
    glomerularFiltrationRate = null;
    filtrationFraction = null;
    leftAfferentArterioleResistance = null;
    leftBowmansCapsulesHydrostaticPressure = null;
    leftBowmansCapsulesOsmoticPressure = null;
    leftEfferentArterioleResistance = null;
    leftGlomerularCapillariesHydrostaticPressure = null;
    leftGlomerularCapillariesOsmoticPressure = null;
    leftGlomerularFiltrationCoefficient = null;
    leftGlomerularFiltrationRate = null;
    leftGlomerularFiltrationSurfaceArea = null;
    leftGlomerularFluidPermeability = null;
    leftFiltrationFraction = null;
    leftNetFiltrationPressure = null;
    leftNetReabsorptionPressure = null;
    leftPeritubularCapillariesHydrostaticPressure = null;
    leftPeritubularCapillariesOsmoticPressure = null;
    leftReabsorptionFiltrationCoefficient = null;
    leftReabsorptionRate = null;
    leftTubularOsmoticPressure = null;
    leftTubularHydrostaticPressure = null;
    leftTubularReabsorptionFiltrationSurfaceArea = null;
    leftTubularReabsorptionFluidPermeability = null;
    renalBloodFlow = null;
    renalPlasmaFlow = null;
    renalVascularResistance = null;
    rightAfferentArterioleResistance = null;
    rightBowmansCapsulesHydrostaticPressure = null;
    rightBowmansCapsulesOsmoticPressure = null;
    rightEfferentArterioleResistance = null;
    rightGlomerularCapillariesHydrostaticPressure = null;
    rightGlomerularCapillariesOsmoticPressure = null;
    rightGlomerularFiltrationCoefficient = null;
    rightGlomerularFiltrationRate = null;
    rightGlomerularFiltrationSurfaceArea = null;
    rightGlomerularFluidPermeability = null;
    rightFiltrationFraction = null;
    rightNetFiltrationPressure = null;
    rightNetReabsorptionPressure = null;
    rightPeritubularCapillariesHydrostaticPressure = null;
    rightPeritubularCapillariesOsmoticPressure = null;
    rightReabsorptionFiltrationCoefficient = null;
    rightReabsorptionRate = null;
    rightTubularOsmoticPressure = null;
    rightTubularHydrostaticPressure = null;
    rightTubularReabsorptionFiltrationSurfaceArea = null;
    rightTubularReabsorptionFluidPermeability = null;
    urinationRate = null;
    urineOsmolality = null;
    urineOsmolarity = null;
    urineProductionRate = null;
    urineSpecificGravity = null;
    urineVolume = null;
    urineUreaNitrogenConcentration = null;
  }

  public void reset()
  {
    if (glomerularFiltrationRate != null)
      glomerularFiltrationRate.invalidate();
    if (filtrationFraction != null)
      filtrationFraction.invalidate();
    if (leftAfferentArterioleResistance != null)
      leftAfferentArterioleResistance.invalidate();
    if (leftBowmansCapsulesHydrostaticPressure != null)
      leftBowmansCapsulesHydrostaticPressure.invalidate();
    if (leftBowmansCapsulesOsmoticPressure != null)
      leftBowmansCapsulesOsmoticPressure.invalidate();
    if (leftEfferentArterioleResistance != null)
      leftEfferentArterioleResistance.invalidate();
    if (leftGlomerularCapillariesHydrostaticPressure != null)
      leftGlomerularCapillariesHydrostaticPressure.invalidate();
    if (leftGlomerularCapillariesOsmoticPressure != null)
      leftGlomerularCapillariesOsmoticPressure.invalidate();
    if (leftGlomerularFiltrationCoefficient != null)
      leftGlomerularFiltrationCoefficient.invalidate();
    if (leftGlomerularFiltrationRate != null)
      leftGlomerularFiltrationRate.invalidate();
    if (leftGlomerularFiltrationSurfaceArea != null)
      leftGlomerularFiltrationSurfaceArea.invalidate();
    if (leftGlomerularFluidPermeability != null)
      leftGlomerularFluidPermeability.invalidate();
    if (leftFiltrationFraction != null)
      leftFiltrationFraction.invalidate();
    if (leftNetFiltrationPressure != null)
      leftNetFiltrationPressure.invalidate();
    if (leftNetReabsorptionPressure != null)
      leftNetReabsorptionPressure.invalidate();
    if (leftPeritubularCapillariesHydrostaticPressure != null)
      leftPeritubularCapillariesHydrostaticPressure.invalidate();
    if (leftPeritubularCapillariesOsmoticPressure != null)
      leftPeritubularCapillariesOsmoticPressure.invalidate();
    if (leftReabsorptionFiltrationCoefficient != null)
      leftReabsorptionFiltrationCoefficient.invalidate();
    if (leftReabsorptionRate != null)
      leftReabsorptionRate.invalidate();
    if (leftTubularOsmoticPressure != null)
      leftTubularOsmoticPressure.invalidate();
    if (leftTubularHydrostaticPressure != null)
      leftTubularHydrostaticPressure.invalidate();
    if (leftTubularReabsorptionFiltrationSurfaceArea != null)
      leftTubularReabsorptionFiltrationSurfaceArea.invalidate();
    if (leftTubularReabsorptionFluidPermeability != null)
      leftTubularReabsorptionFluidPermeability.invalidate();
    if (renalBloodFlow != null)
      renalBloodFlow.invalidate();
    if (renalPlasmaFlow != null)
      renalPlasmaFlow.invalidate();
    if (renalVascularResistance != null)
      renalVascularResistance.invalidate();
    if (rightAfferentArterioleResistance != null)
      rightAfferentArterioleResistance.invalidate();
    if (rightBowmansCapsulesHydrostaticPressure != null)
      rightBowmansCapsulesHydrostaticPressure.invalidate();
    if (rightBowmansCapsulesOsmoticPressure != null)
      rightBowmansCapsulesOsmoticPressure.invalidate();
    if (rightEfferentArterioleResistance != null)
      rightEfferentArterioleResistance.invalidate();
    if (rightGlomerularCapillariesHydrostaticPressure != null)
      rightGlomerularCapillariesHydrostaticPressure.invalidate();
    if (rightGlomerularCapillariesOsmoticPressure != null)
      rightGlomerularCapillariesOsmoticPressure.invalidate();
    if (rightGlomerularFiltrationCoefficient != null)
      rightGlomerularFiltrationCoefficient.invalidate();
    if (rightGlomerularFiltrationRate != null)
      rightGlomerularFiltrationRate.invalidate();
    if (rightGlomerularFiltrationSurfaceArea != null)
      rightGlomerularFiltrationSurfaceArea.invalidate();
    if (rightGlomerularFluidPermeability != null)
      rightGlomerularFluidPermeability.invalidate();
    if (rightFiltrationFraction != null)
      rightFiltrationFraction.invalidate();
    if (rightNetFiltrationPressure != null)
      rightNetFiltrationPressure.invalidate();
    if (rightNetReabsorptionPressure != null)
      rightNetReabsorptionPressure.invalidate();
    if (rightPeritubularCapillariesHydrostaticPressure != null)
      rightPeritubularCapillariesHydrostaticPressure.invalidate();
    if (rightPeritubularCapillariesOsmoticPressure != null)
      rightPeritubularCapillariesOsmoticPressure.invalidate();
    if (rightReabsorptionFiltrationCoefficient != null)
      rightReabsorptionFiltrationCoefficient.invalidate();
    if (rightReabsorptionRate != null)
      rightReabsorptionRate.invalidate();
    if (rightTubularOsmoticPressure != null)
      rightTubularOsmoticPressure.invalidate();
    if (rightTubularHydrostaticPressure != null)
      rightTubularHydrostaticPressure.invalidate();
    if (rightTubularReabsorptionFiltrationSurfaceArea != null)
      rightTubularReabsorptionFiltrationSurfaceArea.invalidate();
    if (rightTubularReabsorptionFluidPermeability != null)
      rightTubularReabsorptionFluidPermeability.invalidate();
    if (urinationRate != null)
      urinationRate.invalidate();
    if (urineOsmolality != null)
      urineOsmolality.invalidate();
    if (urineOsmolarity != null)
      urineOsmolarity.invalidate();
    if (urineProductionRate != null)
      urineProductionRate.invalidate();
    if (urineSpecificGravity != null)
      urineSpecificGravity.invalidate();
    if (urineVolume != null)
      urineVolume.invalidate();
    if (urineUreaNitrogenConcentration != null)
      urineUreaNitrogenConcentration.invalidate();
  }

  public boolean load(RenalSystemData in)
  {
    if (in.getGlomerularFiltrationRate() != null)
      getGlomerularFiltrationRate().load(in.getGlomerularFiltrationRate());
    if (in.getFiltrationFraction() != null)
      getFiltrationFraction().load(in.getFiltrationFraction());
    if (in.getLeftAfferentArterioleResistance() != null)
      getLeftAfferentArterioleResistance().load(in.getLeftAfferentArterioleResistance());
    if (in.getLeftBowmansCapsulesHydrostaticPressure() != null)
      getLeftBowmansCapsulesHydrostaticPressure().load(in.getLeftBowmansCapsulesHydrostaticPressure());
    if (in.getLeftBowmansCapsulesOsmoticPressure() != null)
      getLeftBowmansCapsulesOsmoticPressure().load(in.getLeftBowmansCapsulesOsmoticPressure());
    if (in.getLeftEfferentArterioleResistance() != null)
      getLeftEfferentArterioleResistance().load(in.getLeftEfferentArterioleResistance());
    if (in.getLeftGlomerularCapillariesHydrostaticPressure() != null)
      getLeftGlomerularCapillariesHydrostaticPressure().load(in.getLeftGlomerularCapillariesHydrostaticPressure());
    if (in.getLeftGlomerularCapillariesOsmoticPressure() != null)
      getLeftGlomerularCapillariesOsmoticPressure().load(in.getLeftGlomerularCapillariesOsmoticPressure());
    if (in.getLeftGlomerularFiltrationCoefficient() != null)
      getLeftGlomerularFiltrationCoefficient().load(in.getLeftGlomerularFiltrationCoefficient());
    if (in.getLeftGlomerularFiltrationRate() != null)
      getLeftGlomerularFiltrationRate().load(in.getLeftGlomerularFiltrationRate());
    if (in.getLeftGlomerularFiltrationSurfaceArea() != null)
      getLeftGlomerularFiltrationSurfaceArea().load(in.getLeftGlomerularFiltrationSurfaceArea());
    if (in.getLeftGlomerularFluidPermeability() != null)
      getLeftGlomerularFluidPermeability().load(in.getLeftGlomerularFluidPermeability());
    if (in.getLeftFiltrationFraction() != null)
      getLeftFiltrationFraction().load(in.getLeftFiltrationFraction());
    if (in.getLeftNetFiltrationPressure() != null)
      getLeftNetFiltrationPressure().load(in.getLeftNetFiltrationPressure());
    if (in.getLeftNetReabsorptionPressure() != null)
      getLeftNetReabsorptionPressure().load(in.getLeftNetReabsorptionPressure());
    if (in.getLeftPeritubularCapillariesHydrostaticPressure() != null)
      getLeftPeritubularCapillariesHydrostaticPressure().load(in.getLeftPeritubularCapillariesHydrostaticPressure());
    if (in.getLeftPeritubularCapillariesOsmoticPressure() != null)
      getLeftPeritubularCapillariesOsmoticPressure().load(in.getLeftPeritubularCapillariesOsmoticPressure());
    if (in.getLeftReabsorptionFiltrationCoefficient() != null)
      getLeftReabsorptionFiltrationCoefficient().load(in.getLeftReabsorptionFiltrationCoefficient());
    if (in.getLeftReabsorptionRate() != null)
      getLeftReabsorptionRate().load(in.getLeftReabsorptionRate());
    if (in.getLeftTubularOsmoticPressure() != null)
      getLeftTubularOsmoticPressure().load(in.getLeftTubularOsmoticPressure());
    if (in.getLeftTubularHydrostaticPressure() != null)
      getLeftTubularHydrostaticPressure().load(in.getLeftTubularHydrostaticPressure());
    if (in.getLeftTubularReabsorptionFiltrationSurfaceArea() != null)
      getLeftTubularReabsorptionFiltrationSurfaceArea().load(in.getLeftTubularReabsorptionFiltrationSurfaceArea());
    if (in.getLeftTubularReabsorptionFluidPermeability() != null)
      getLeftTubularReabsorptionFluidPermeability().load(in.getLeftTubularReabsorptionFluidPermeability());
    if (in.getRenalBloodFlow() != null)
      getRenalBloodFlow().load(in.getRenalBloodFlow());
    if (in.getRenalPlasmaFlow() != null)
      getRenalPlasmaFlow().load(in.getRenalPlasmaFlow());
    if (in.getRenalVascularResistance() != null)
      getRenalVascularResistance().load(in.getRenalVascularResistance());
    if (in.getRightAfferentArterioleResistance() != null)
      getRightAfferentArterioleResistance().load(in.getRightAfferentArterioleResistance());
    if (in.getRightBowmansCapsulesHydrostaticPressure() != null)
      getRightBowmansCapsulesHydrostaticPressure().load(in.getRightBowmansCapsulesHydrostaticPressure());
    if (in.getRightBowmansCapsulesOsmoticPressure() != null)
      getRightBowmansCapsulesOsmoticPressure().load(in.getRightBowmansCapsulesOsmoticPressure());
    if (in.getRightEfferentArterioleResistance() != null)
      getRightEfferentArterioleResistance().load(in.getRightEfferentArterioleResistance());
    if (in.getRightGlomerularCapillariesHydrostaticPressure() != null)
      getRightGlomerularCapillariesHydrostaticPressure().load(in.getRightGlomerularCapillariesHydrostaticPressure());
    if (in.getRightGlomerularCapillariesOsmoticPressure() != null)
      getRightGlomerularCapillariesOsmoticPressure().load(in.getRightGlomerularCapillariesOsmoticPressure());
    if (in.getRightGlomerularFiltrationCoefficient() != null)
      getRightGlomerularFiltrationCoefficient().load(in.getRightGlomerularFiltrationCoefficient());
    if (in.getRightGlomerularFiltrationRate() != null)
      getRightGlomerularFiltrationRate().load(in.getRightGlomerularFiltrationRate());
    if (in.getRightGlomerularFiltrationSurfaceArea() != null)
      getRightGlomerularFiltrationSurfaceArea().load(in.getRightGlomerularFiltrationSurfaceArea());
    if (in.getRightGlomerularFluidPermeability() != null)
      getRightGlomerularFluidPermeability().load(in.getRightGlomerularFluidPermeability());
    if (in.getRightFiltrationFraction() != null)
      getRightFiltrationFraction().load(in.getRightFiltrationFraction());
    if (in.getRightNetFiltrationPressure() != null)
      getRightNetFiltrationPressure().load(in.getRightNetFiltrationPressure());
    if (in.getRightNetReabsorptionPressure() != null)
      getRightNetReabsorptionPressure().load(in.getRightNetReabsorptionPressure());
    if (in.getRightPeritubularCapillariesHydrostaticPressure() != null)
      getRightPeritubularCapillariesHydrostaticPressure().load(in.getRightPeritubularCapillariesHydrostaticPressure());
    if (in.getRightPeritubularCapillariesOsmoticPressure() != null)
      getRightPeritubularCapillariesOsmoticPressure().load(in.getRightPeritubularCapillariesOsmoticPressure());
    if (in.getRightReabsorptionFiltrationCoefficient() != null)
      getRightReabsorptionFiltrationCoefficient().load(in.getRightReabsorptionFiltrationCoefficient());
    if (in.getRightReabsorptionRate() != null)
      getRightReabsorptionRate().load(in.getRightReabsorptionRate());
    if (in.getRightTubularOsmoticPressure() != null)
      getRightTubularOsmoticPressure().load(in.getRightTubularOsmoticPressure());
    if (in.getRightTubularHydrostaticPressure() != null)
      getRightTubularHydrostaticPressure().load(in.getRightTubularHydrostaticPressure());
    if (in.getRightTubularReabsorptionFiltrationSurfaceArea() != null)
      getRightTubularReabsorptionFiltrationSurfaceArea().load(in.getRightTubularReabsorptionFiltrationSurfaceArea());
    if (in.getRightTubularReabsorptionFluidPermeability() != null)
      getRightTubularReabsorptionFluidPermeability().load(in.getRightTubularReabsorptionFluidPermeability());
    if (in.getUrinationRate() != null)
      getUrinationRate().load(in.getUrinationRate());
    if (in.getUrineOsmolality() != null)
      getUrineOsmolality().load(in.getUrineOsmolality());
    if (in.getUrineOsmolarity() != null)
      getUrineOsmolarity().load(in.getUrineOsmolarity());
    if (in.getUrineProductionRate() != null)
      getUrineProductionRate().load(in.getUrineProductionRate());
    if (in.getUrineSpecificGravity() != null)
      getUrineSpecificGravity().load(in.getUrineSpecificGravity());
    if (in.getUrineVolume() != null)
      getUrineVolume().load(in.getUrineVolume());
    if (in.getUrineUreaNitrogenConcentration() != null)
      getUrineUreaNitrogenConcentration().load(in.getUrineUreaNitrogenConcentration());

    return true;
  }

  public RenalSystemData unload()
  {
    RenalSystemData data = CDMSerializer.objFactory.createRenalSystemData();
    unload(data);
    return data;
  }

  protected void unload(RenalSystemData data)
  {
    if (getGlomerularFiltrationRate() != null)
      data.setGlomerularFiltrationRate(glomerularFiltrationRate.unload());
    if (getFiltrationFraction() != null)
      data.setFiltrationFraction(filtrationFraction.unload());
    if (getLeftAfferentArterioleResistance() != null)
      data.setLeftAfferentArterioleResistance(leftAfferentArterioleResistance.unload());
    if (getLeftBowmansCapsulesHydrostaticPressure() != null)
      data.setLeftBowmansCapsulesHydrostaticPressure(leftBowmansCapsulesHydrostaticPressure.unload());
    if (getLeftBowmansCapsulesOsmoticPressure() != null)
      data.setLeftBowmansCapsulesOsmoticPressure(leftBowmansCapsulesOsmoticPressure.unload());
    if (getLeftBowmansCapsulesHydrostaticPressure() != null)
      data.setLeftEfferentArterioleResistance(leftEfferentArterioleResistance.unload());
    if (getLeftEfferentArterioleResistance() != null)
      data.setLeftGlomerularCapillariesHydrostaticPressure(leftGlomerularCapillariesHydrostaticPressure.unload());
    if (getLeftGlomerularCapillariesOsmoticPressure() != null)
      data.setLeftGlomerularCapillariesOsmoticPressure(leftGlomerularCapillariesOsmoticPressure.unload());
    if (getLeftGlomerularFiltrationCoefficient() != null)
      data.setLeftGlomerularFiltrationCoefficient(leftGlomerularFiltrationCoefficient.unload());
    if (getLeftGlomerularFiltrationRate() != null)
      data.setLeftGlomerularFiltrationRate(leftGlomerularFiltrationRate.unload());
    if (getLeftGlomerularFiltrationSurfaceArea() != null)
      data.setLeftGlomerularFiltrationSurfaceArea(leftGlomerularFiltrationSurfaceArea.unload());
    if (getLeftGlomerularFluidPermeability() != null)
      data.setLeftGlomerularFluidPermeability(leftGlomerularFluidPermeability.unload());
    if (getLeftFiltrationFraction() != null)
      data.setLeftFiltrationFraction(leftFiltrationFraction.unload());
    if (getLeftNetFiltrationPressure() != null)
      data.setLeftNetFiltrationPressure(leftNetFiltrationPressure.unload());
    if (getLeftNetReabsorptionPressure() != null)
      data.setLeftNetReabsorptionPressure(leftNetReabsorptionPressure.unload());
    if (getLeftPeritubularCapillariesHydrostaticPressure() != null)
      data.setLeftPeritubularCapillariesHydrostaticPressure(leftPeritubularCapillariesHydrostaticPressure.unload());
    if (getLeftPeritubularCapillariesOsmoticPressure() != null)
      data.setLeftPeritubularCapillariesOsmoticPressure(leftPeritubularCapillariesOsmoticPressure.unload());
    if (getLeftReabsorptionFiltrationCoefficient() != null)
      data.setLeftReabsorptionFiltrationCoefficient(leftReabsorptionFiltrationCoefficient.unload());
    if (getLeftReabsorptionRate() != null)
      data.setLeftReabsorptionRate(leftReabsorptionRate.unload());
    if (getLeftTubularOsmoticPressure() != null)
      data.setLeftTubularOsmoticPressure(leftTubularOsmoticPressure.unload());
    if (getLeftTubularHydrostaticPressure() != null)
      data.setLeftTubularHydrostaticPressure(leftTubularHydrostaticPressure.unload());
    if (getLeftTubularReabsorptionFiltrationSurfaceArea() != null)
      data.setLeftTubularReabsorptionFiltrationSurfaceArea(leftTubularReabsorptionFiltrationSurfaceArea.unload());
    if (getLeftTubularReabsorptionFluidPermeability() != null)
      data.setLeftTubularReabsorptionFluidPermeability(leftTubularReabsorptionFluidPermeability.unload());
    if (getRenalBloodFlow() != null)
      data.setRenalBloodFlow(renalBloodFlow.unload());
    if (getRenalPlasmaFlow() != null)
      data.setRenalPlasmaFlow(renalPlasmaFlow.unload());
    if (getRenalVascularResistance() != null)
      data.setRenalVascularResistance(renalVascularResistance.unload());
    if (getRightAfferentArterioleResistance() != null)
      data.setRightAfferentArterioleResistance(rightAfferentArterioleResistance.unload());
    if (getRightBowmansCapsulesHydrostaticPressure() != null)
      data.setRightBowmansCapsulesHydrostaticPressure(rightBowmansCapsulesHydrostaticPressure.unload());
    if (getRightBowmansCapsulesOsmoticPressure() != null)
      data.setRightEfferentArterioleResistance(rightEfferentArterioleResistance.unload());
    if (getRightEfferentArterioleResistance() != null)
      data.setRightBowmansCapsulesHydrostaticPressure(rightBowmansCapsulesHydrostaticPressure.unload());
    if (getRightGlomerularCapillariesHydrostaticPressure() != null)
      data.setRightGlomerularCapillariesHydrostaticPressure(rightGlomerularCapillariesHydrostaticPressure.unload());
    if (getRightGlomerularCapillariesOsmoticPressure() != null)
      data.setRightGlomerularCapillariesOsmoticPressure(rightGlomerularCapillariesOsmoticPressure.unload());
    if (getRightGlomerularFiltrationCoefficient() != null)
      data.setRightGlomerularFiltrationCoefficient(rightGlomerularFiltrationCoefficient.unload());
    if (getRightGlomerularFiltrationRate() != null)
      data.setRightGlomerularFiltrationRate(rightGlomerularFiltrationRate.unload());
    if (getRightGlomerularFiltrationSurfaceArea() != null)
      data.setRightGlomerularFiltrationSurfaceArea(rightGlomerularFiltrationSurfaceArea.unload());
    if (getRightGlomerularFluidPermeability() != null)
      data.setRightGlomerularFluidPermeability(rightGlomerularFluidPermeability.unload());
    if (getRightFiltrationFraction() != null)
      data.setRightFiltrationFraction(rightFiltrationFraction.unload());
    if (getRightNetFiltrationPressure() != null)
      data.setRightNetFiltrationPressure(rightNetFiltrationPressure.unload());
    if (getRightNetReabsorptionPressure() != null)
      data.setRightNetReabsorptionPressure(rightNetReabsorptionPressure.unload());
    if (getRightPeritubularCapillariesHydrostaticPressure() != null)
      data.setRightPeritubularCapillariesHydrostaticPressure(rightPeritubularCapillariesHydrostaticPressure.unload());
    if (getRightPeritubularCapillariesOsmoticPressure() != null)
      data.setRightPeritubularCapillariesOsmoticPressure(rightPeritubularCapillariesOsmoticPressure.unload());
    if (getRightReabsorptionFiltrationCoefficient() != null)
      data.setRightReabsorptionFiltrationCoefficient(rightReabsorptionFiltrationCoefficient.unload());
    if (getRightReabsorptionRate() != null)
      data.setRightReabsorptionRate(rightReabsorptionRate.unload());
    if (getRightTubularOsmoticPressure() != null)
      data.setRightTubularOsmoticPressure(rightTubularOsmoticPressure.unload());
    if (getRightTubularHydrostaticPressure() != null)
      data.setRightTubularHydrostaticPressure(rightTubularHydrostaticPressure.unload());
    if (getRightTubularReabsorptionFiltrationSurfaceArea() != null)
      data.setRightTubularReabsorptionFiltrationSurfaceArea(rightTubularReabsorptionFiltrationSurfaceArea.unload());
    if (getRightTubularReabsorptionFluidPermeability() != null)
      data.setRightTubularReabsorptionFluidPermeability(rightTubularReabsorptionFluidPermeability.unload());
    if (getUrinationRate() != null)
      data.setUrinationRate(urinationRate.unload());
    if (getUrineOsmolality() != null)
      data.setUrineOsmolality(urineOsmolality.unload());
    if (getUrineOsmolarity() != null)
      data.setUrineOsmolarity(urineOsmolarity.unload());
    if (getUrineProductionRate() != null)
      data.setUrineProductionRate(urineProductionRate.unload());
    if (getUrineSpecificGravity() != null)
      data.setUrineSpecificGravity(urineSpecificGravity.unload());
    if (getUrineVolume() != null)
      data.setUrineVolume(urineVolume.unload());
    if (getUrineUreaNitrogenConcentration() != null)
      data.setUrineUreaNitrogenConcentration(urineUreaNitrogenConcentration.unload());
  }

  public boolean hasGlomerularFiltrationRate()
  {
    return glomerularFiltrationRate == null ? false : glomerularFiltrationRate.isValid();
  }
  public SEScalarVolumePerTime getGlomerularFiltrationRate()
  {
    if (glomerularFiltrationRate == null)
      glomerularFiltrationRate = new SEScalarVolumePerTime();
    return glomerularFiltrationRate;
  }

  public boolean hasFiltrationFraction()
  {
    return filtrationFraction == null ? false : filtrationFraction.isValid();
  }
  public SEScalarFraction getFiltrationFraction()
  {
    if (filtrationFraction == null)
      filtrationFraction = new SEScalarFraction();
    return filtrationFraction;
  }
  
  public boolean hasLeftAfferentArterioleResistance()
  {
    return leftAfferentArterioleResistance == null ? false : leftAfferentArterioleResistance.isValid();
  }
  public SEScalarFlowResistance getLeftAfferentArterioleResistance()
  {
    if (leftAfferentArterioleResistance == null)
      leftAfferentArterioleResistance = new SEScalarFlowResistance();
    return leftAfferentArterioleResistance;
  }

  public boolean hasLeftBowmansCapsulesHydrostaticPressure()
  {
    return leftBowmansCapsulesHydrostaticPressure == null ? false : leftBowmansCapsulesHydrostaticPressure.isValid();
  }
  public SEScalarPressure getLeftBowmansCapsulesHydrostaticPressure()
  {
    if (leftBowmansCapsulesHydrostaticPressure == null)
      leftBowmansCapsulesHydrostaticPressure = new SEScalarPressure();
    return leftBowmansCapsulesHydrostaticPressure;
  }

  public boolean hasLeftBowmansCapsulesOsmoticPressure()
  {
    return leftBowmansCapsulesOsmoticPressure == null ? false : leftBowmansCapsulesOsmoticPressure.isValid();
  }
  public SEScalarPressure getLeftBowmansCapsulesOsmoticPressure()
  {
    if (leftBowmansCapsulesOsmoticPressure == null)
      leftBowmansCapsulesOsmoticPressure = new SEScalarPressure();
    return leftBowmansCapsulesOsmoticPressure;
  }
  
  public boolean hasLeftEfferentArterioleResistance()
  {
    return leftEfferentArterioleResistance == null ? false : leftEfferentArterioleResistance.isValid();
  }
  public SEScalarFlowResistance getLeftEfferentArterioleResistance()
  {
    if (leftEfferentArterioleResistance == null)
      leftEfferentArterioleResistance = new SEScalarFlowResistance();
    return leftEfferentArterioleResistance;
  }

  public boolean hasLeftGlomerularCapillariesHydrostaticPressure()
  {
    return leftGlomerularCapillariesHydrostaticPressure == null ? false : leftGlomerularCapillariesHydrostaticPressure.isValid();
  }
  public SEScalarPressure getLeftGlomerularCapillariesHydrostaticPressure()
  {
    if (leftGlomerularCapillariesHydrostaticPressure == null)
      leftGlomerularCapillariesHydrostaticPressure = new SEScalarPressure();
    return leftGlomerularCapillariesHydrostaticPressure;
  }

  public boolean hasLeftGlomerularCapillariesOsmoticPressure()
  {
    return leftGlomerularCapillariesOsmoticPressure == null ? false : leftGlomerularCapillariesOsmoticPressure.isValid();
  }
  public SEScalarPressure getLeftGlomerularCapillariesOsmoticPressure()
  {
    if (leftGlomerularCapillariesOsmoticPressure == null)
      leftGlomerularCapillariesOsmoticPressure = new SEScalarPressure();
    return leftGlomerularCapillariesOsmoticPressure;
  }

  public boolean hasLeftGlomerularFiltrationCoefficient()
  {
    return leftGlomerularFiltrationCoefficient == null ? false : leftGlomerularFiltrationCoefficient.isValid();
  }
  public SEScalarVolumePerTimePressure getLeftGlomerularFiltrationCoefficient()
  {
    if (leftGlomerularFiltrationCoefficient == null)
      leftGlomerularFiltrationCoefficient = new SEScalarVolumePerTimePressure();
    return leftGlomerularFiltrationCoefficient;
  }

  public boolean hasLeftGlomerularFiltrationRate()
  {
    return leftGlomerularFiltrationRate == null ? false : leftGlomerularFiltrationRate.isValid();
  }
  public SEScalarVolumePerTime getLeftGlomerularFiltrationRate()
  {
    if (leftGlomerularFiltrationRate == null)
      leftGlomerularFiltrationRate = new SEScalarVolumePerTime();
    return leftGlomerularFiltrationRate;
  }

  public boolean hasLeftGlomerularFiltrationSurfaceArea()
  {
    return leftGlomerularFiltrationSurfaceArea == null ? false : leftGlomerularFiltrationSurfaceArea.isValid();
  }
  public SEScalarArea getLeftGlomerularFiltrationSurfaceArea()
  {
    if (leftGlomerularFiltrationSurfaceArea == null)
      leftGlomerularFiltrationSurfaceArea = new SEScalarArea();
    return leftGlomerularFiltrationSurfaceArea;
  }

  public boolean hasLeftGlomerularFluidPermeability()
  {
    return leftGlomerularFluidPermeability == null ? false : leftGlomerularFluidPermeability.isValid();
  }
  public SEScalarVolumePerTimePressureArea getLeftGlomerularFluidPermeability()
  {
    if (leftGlomerularFluidPermeability == null)
      leftGlomerularFluidPermeability = new SEScalarVolumePerTimePressureArea();
    return leftGlomerularFluidPermeability;
  }

  public boolean hasLeftFiltrationFraction()
  {
    return leftFiltrationFraction == null ? false : leftFiltrationFraction.isValid();
  }
  public SEScalarFraction getLeftFiltrationFraction()
  {
    if (leftFiltrationFraction == null)
      leftFiltrationFraction = new SEScalarFraction();
    return leftFiltrationFraction;
  }

  public boolean hasLeftNetFiltrationPressure()
  {
    return leftNetFiltrationPressure == null ? false : leftNetFiltrationPressure.isValid();
  }
  public SEScalarPressure getLeftNetFiltrationPressure()
  {
    if (leftNetFiltrationPressure == null)
      leftNetFiltrationPressure = new SEScalarPressure();
    return leftNetFiltrationPressure;
  }

  public boolean hasLeftNetReabsorptionPressure()
  {
    return leftNetReabsorptionPressure == null ? false : leftNetReabsorptionPressure.isValid();
  }
  public SEScalarPressure getLeftNetReabsorptionPressure()
  {
    if (leftNetReabsorptionPressure == null)
      leftNetReabsorptionPressure = new SEScalarPressure();
    return leftNetReabsorptionPressure;
  }

  public boolean hasLeftPeritubularCapillariesHydrostaticPressure()
  {
    return leftPeritubularCapillariesHydrostaticPressure == null ? false : leftPeritubularCapillariesHydrostaticPressure.isValid();
  }
  public SEScalarPressure getLeftPeritubularCapillariesHydrostaticPressure()
  {
    if (leftPeritubularCapillariesHydrostaticPressure == null)
      leftPeritubularCapillariesHydrostaticPressure = new SEScalarPressure();
    return leftPeritubularCapillariesHydrostaticPressure;
  }

  public boolean hasLeftPeritubularCapillariesOsmoticPressure()
  {
    return leftPeritubularCapillariesOsmoticPressure == null ? false : leftPeritubularCapillariesOsmoticPressure.isValid();
  }
  public SEScalarPressure getLeftPeritubularCapillariesOsmoticPressure()
  {
    if (leftPeritubularCapillariesOsmoticPressure == null)
      leftPeritubularCapillariesOsmoticPressure = new SEScalarPressure();
    return leftPeritubularCapillariesOsmoticPressure;
  }

  public boolean hasLeftReabsorptionFiltrationCoefficient()
  {
    return leftReabsorptionFiltrationCoefficient == null ? false : leftReabsorptionFiltrationCoefficient.isValid();
  }
  public SEScalarVolumePerTimePressure getLeftReabsorptionFiltrationCoefficient()
  {
    if (leftReabsorptionFiltrationCoefficient == null)
      leftReabsorptionFiltrationCoefficient = new SEScalarVolumePerTimePressure();
    return leftReabsorptionFiltrationCoefficient;
  }

  public boolean hasLeftReabsorptionRate()
  {
    return leftReabsorptionRate == null ? false : leftReabsorptionRate.isValid();
  }
  public SEScalarVolumePerTime getLeftReabsorptionRate()
  {
    if (leftReabsorptionRate == null)
      leftReabsorptionRate = new SEScalarVolumePerTime();
    return leftReabsorptionRate;
  }

  public boolean hasLeftTubularOsmoticPressure()
  {
    return leftTubularOsmoticPressure == null ? false : leftTubularOsmoticPressure.isValid();
  }
  public SEScalarPressure getLeftTubularOsmoticPressure()
  {
    if (leftTubularOsmoticPressure == null)
      leftTubularOsmoticPressure = new SEScalarPressure();
    return leftTubularOsmoticPressure;
  }

  public boolean hasLeftTubularHydrostaticPressure()
  {
    return leftTubularHydrostaticPressure == null ? false : leftTubularHydrostaticPressure.isValid();
  }
  public SEScalarPressure getLeftTubularHydrostaticPressure()
  {
    if (leftTubularHydrostaticPressure == null)
      leftTubularHydrostaticPressure = new SEScalarPressure();
    return leftTubularHydrostaticPressure;
  }

  public boolean hasLeftTubularReabsorptionFiltrationSurfaceArea()
  {
    return leftTubularReabsorptionFiltrationSurfaceArea == null ? false : leftTubularReabsorptionFiltrationSurfaceArea.isValid();
  }
  public SEScalarArea getLeftTubularReabsorptionFiltrationSurfaceArea()
  {
    if (leftTubularReabsorptionFiltrationSurfaceArea == null)
      leftTubularReabsorptionFiltrationSurfaceArea = new SEScalarArea();
    return leftTubularReabsorptionFiltrationSurfaceArea;
  }

  public boolean hasLeftTubularReabsorptionFluidPermeability()
  {
    return leftTubularReabsorptionFluidPermeability == null ? false : leftTubularReabsorptionFluidPermeability.isValid();
  }
  public SEScalarVolumePerTimePressureArea getLeftTubularReabsorptionFluidPermeability()
  {
    if (leftTubularReabsorptionFluidPermeability == null)
      leftTubularReabsorptionFluidPermeability = new SEScalarVolumePerTimePressureArea();
    return leftTubularReabsorptionFluidPermeability;
  }

  public boolean hasRenalBloodFlow()
  {
    return renalBloodFlow == null ? false : renalBloodFlow.isValid();
  }
  public SEScalarVolumePerTime getRenalBloodFlow()
  {
    if (renalBloodFlow == null)
      renalBloodFlow = new SEScalarVolumePerTime();
    return renalBloodFlow;
  }

  public boolean hasRenalPlasmaFlow()
  {
    return renalPlasmaFlow == null ? false : renalPlasmaFlow.isValid();
  }
  public SEScalarVolumePerTime getRenalPlasmaFlow()
  {
    if (renalPlasmaFlow == null)
      renalPlasmaFlow = new SEScalarVolumePerTime();
    return renalPlasmaFlow;
  }

  public boolean hasRenalVascularResistance()
  {
    return renalVascularResistance == null ? false : renalVascularResistance.isValid();
  }
  public SEScalarFlowResistance getRenalVascularResistance()
  {
    if (renalVascularResistance == null)
      renalVascularResistance = new SEScalarFlowResistance();
    return renalVascularResistance;
  }
  
  public boolean hasRightAfferentArterioleResistance()
  {
    return rightAfferentArterioleResistance == null ? false : rightAfferentArterioleResistance.isValid();
  }
  public SEScalarFlowResistance getRightAfferentArterioleResistance()
  {
    if (rightAfferentArterioleResistance == null)
      rightAfferentArterioleResistance = new SEScalarFlowResistance();
    return rightAfferentArterioleResistance;
  }

  public boolean hasRightBowmansCapsulesHydrostaticPressure()
  {
    return rightBowmansCapsulesHydrostaticPressure == null ? false : rightBowmansCapsulesHydrostaticPressure.isValid();
  }
  public SEScalarPressure getRightBowmansCapsulesHydrostaticPressure()
  {
    if (rightBowmansCapsulesHydrostaticPressure == null)
      rightBowmansCapsulesHydrostaticPressure = new SEScalarPressure();
    return rightBowmansCapsulesHydrostaticPressure;
  }

  public boolean hasRightBowmansCapsulesOsmoticPressure()
  {
    return rightBowmansCapsulesOsmoticPressure == null ? false : rightBowmansCapsulesOsmoticPressure.isValid();
  }
  public SEScalarPressure getRightBowmansCapsulesOsmoticPressure()
  {
    if (rightBowmansCapsulesOsmoticPressure == null)
      rightBowmansCapsulesOsmoticPressure = new SEScalarPressure();
    return rightBowmansCapsulesOsmoticPressure;
  }
  
  public boolean hasRightEfferentArterioleResistance()
  {
    return rightEfferentArterioleResistance == null ? false : rightEfferentArterioleResistance.isValid();
  }
  public SEScalarFlowResistance getRightEfferentArterioleResistance()
  {
    if (rightEfferentArterioleResistance == null)
      rightEfferentArterioleResistance = new SEScalarFlowResistance();
    return rightEfferentArterioleResistance;
  }

  public boolean hasRightGlomerularCapillariesHydrostaticPressure()
  {
    return rightGlomerularCapillariesHydrostaticPressure == null ? false : rightGlomerularCapillariesHydrostaticPressure.isValid();
  }
  public SEScalarPressure getRightGlomerularCapillariesHydrostaticPressure()
  {
    if (rightGlomerularCapillariesHydrostaticPressure == null)
      rightGlomerularCapillariesHydrostaticPressure = new SEScalarPressure();
    return rightGlomerularCapillariesHydrostaticPressure;
  }

  public boolean hasRightGlomerularCapillariesOsmoticPressure()
  {
    return rightGlomerularCapillariesOsmoticPressure == null ? false : rightGlomerularCapillariesOsmoticPressure.isValid();
  }
  public SEScalarPressure getRightGlomerularCapillariesOsmoticPressure()
  {
    if (rightGlomerularCapillariesOsmoticPressure == null)
      rightGlomerularCapillariesOsmoticPressure = new SEScalarPressure();
    return rightGlomerularCapillariesOsmoticPressure;
  }

  public boolean hasRightGlomerularFiltrationCoefficient()
  {
    return rightGlomerularFiltrationCoefficient == null ? false : rightGlomerularFiltrationCoefficient.isValid();
  }
  public SEScalarVolumePerTimePressure getRightGlomerularFiltrationCoefficient()
  {
    if (rightGlomerularFiltrationCoefficient == null)
      rightGlomerularFiltrationCoefficient = new SEScalarVolumePerTimePressure();
    return rightGlomerularFiltrationCoefficient;
  }

  public boolean hasRightGlomerularFiltrationRate()
  {
    return rightGlomerularFiltrationRate == null ? false : rightGlomerularFiltrationRate.isValid();
  }
  public SEScalarVolumePerTime getRightGlomerularFiltrationRate()
  {
    if (rightGlomerularFiltrationRate == null)
      rightGlomerularFiltrationRate = new SEScalarVolumePerTime();
    return rightGlomerularFiltrationRate;
  }

  public boolean hasRightGlomerularFiltrationSurfaceArea()
  {
    return rightGlomerularFiltrationSurfaceArea == null ? false : rightGlomerularFiltrationSurfaceArea.isValid();
  }
  public SEScalarArea getRightGlomerularFiltrationSurfaceArea()
  {
    if (rightGlomerularFiltrationSurfaceArea == null)
      rightGlomerularFiltrationSurfaceArea = new SEScalarArea();
    return rightGlomerularFiltrationSurfaceArea;
  }

  public boolean hasRightGlomerularFluidPermeability()
  {
    return rightGlomerularFluidPermeability == null ? false : rightGlomerularFluidPermeability.isValid();
  }
  public SEScalarVolumePerTimePressureArea getRightGlomerularFluidPermeability()
  {
    if (rightGlomerularFluidPermeability == null)
      rightGlomerularFluidPermeability = new SEScalarVolumePerTimePressureArea();
    return rightGlomerularFluidPermeability;
  }

  public boolean hasRightFiltrationFraction()
  {
    return rightFiltrationFraction == null ? false : rightFiltrationFraction.isValid();
  }
  public SEScalarFraction getRightFiltrationFraction()
  {
    if (rightFiltrationFraction == null)
      rightFiltrationFraction = new SEScalarFraction();
    return rightFiltrationFraction;
  }

  public boolean hasRightNetFiltrationPressure()
  {
    return rightNetFiltrationPressure == null ? false : rightNetFiltrationPressure.isValid();
  }
  public SEScalarPressure getRightNetFiltrationPressure()
  {
    if (rightNetFiltrationPressure == null)
      rightNetFiltrationPressure = new SEScalarPressure();
    return rightNetFiltrationPressure;
  }

  public boolean hasRightNetReabsorptionPressure()
  {
    return rightNetReabsorptionPressure == null ? false : rightNetReabsorptionPressure.isValid();
  }
  public SEScalarPressure getRightNetReabsorptionPressure()
  {
    if (rightNetReabsorptionPressure == null)
      rightNetReabsorptionPressure = new SEScalarPressure();
    return rightNetReabsorptionPressure;
  }

  public boolean hasRightPeritubularCapillariesHydrostaticPressure()
  {
    return rightPeritubularCapillariesHydrostaticPressure == null ? false : rightPeritubularCapillariesHydrostaticPressure.isValid();
  }
  public SEScalarPressure getRightPeritubularCapillariesHydrostaticPressure()
  {
    if (rightPeritubularCapillariesHydrostaticPressure == null)
      rightPeritubularCapillariesHydrostaticPressure = new SEScalarPressure();
    return rightPeritubularCapillariesHydrostaticPressure;
  }

  public boolean hasRightPeritubularCapillariesOsmoticPressure()
  {
    return rightPeritubularCapillariesOsmoticPressure == null ? false : rightPeritubularCapillariesOsmoticPressure.isValid();
  }
  public SEScalarPressure getRightPeritubularCapillariesOsmoticPressure()
  {
    if (rightPeritubularCapillariesOsmoticPressure == null)
      rightPeritubularCapillariesOsmoticPressure = new SEScalarPressure();
    return rightPeritubularCapillariesOsmoticPressure;
  }

  public boolean hasRightReabsorptionFiltrationCoefficient()
  {
    return rightReabsorptionFiltrationCoefficient == null ? false : rightReabsorptionFiltrationCoefficient.isValid();
  }
  public SEScalarVolumePerTimePressure getRightReabsorptionFiltrationCoefficient()
  {
    if (rightReabsorptionFiltrationCoefficient == null)
      rightReabsorptionFiltrationCoefficient = new SEScalarVolumePerTimePressure();
    return rightReabsorptionFiltrationCoefficient;
  }

  public boolean hasRightReabsorptionRate()
  {
    return rightReabsorptionRate == null ? false : rightReabsorptionRate.isValid();
  }
  public SEScalarVolumePerTime getRightReabsorptionRate()
  {
    if (rightReabsorptionRate == null)
      rightReabsorptionRate = new SEScalarVolumePerTime();
    return rightReabsorptionRate;
  }

  public boolean hasRightTubularOsmoticPressure()
  {
    return rightTubularOsmoticPressure == null ? false : rightTubularOsmoticPressure.isValid();
  }
  public SEScalarPressure getRightTubularOsmoticPressure()
  {
    if (rightTubularOsmoticPressure == null)
      rightTubularOsmoticPressure = new SEScalarPressure();
    return rightTubularOsmoticPressure;
  }

  public boolean hasRightTubularHydrostaticPressure()
  {
    return rightTubularHydrostaticPressure == null ? false : rightTubularHydrostaticPressure.isValid();
  }
  public SEScalarPressure getRightTubularHydrostaticPressure()
  {
    if (rightTubularHydrostaticPressure == null)
      rightTubularHydrostaticPressure = new SEScalarPressure();
    return rightTubularHydrostaticPressure;
  }

  public boolean hasRightTubularReabsorptionFiltrationSurfaceArea()
  {
    return rightTubularReabsorptionFiltrationSurfaceArea == null ? false : rightTubularReabsorptionFiltrationSurfaceArea.isValid();
  }
  public SEScalarArea getRightTubularReabsorptionFiltrationSurfaceArea()
  {
    if (rightTubularReabsorptionFiltrationSurfaceArea == null)
      rightTubularReabsorptionFiltrationSurfaceArea = new SEScalarArea();
    return rightTubularReabsorptionFiltrationSurfaceArea;
  }

  public boolean hasRightTubularReabsorptionFluidPermeability()
  {
    return rightTubularReabsorptionFluidPermeability == null ? false : rightTubularReabsorptionFluidPermeability.isValid();
  }
  public SEScalarVolumePerTimePressureArea getRightTubularReabsorptionFluidPermeability()
  {
    if (rightTubularReabsorptionFluidPermeability == null)
      rightTubularReabsorptionFluidPermeability = new SEScalarVolumePerTimePressureArea();
    return rightTubularReabsorptionFluidPermeability;
  }

  public boolean hasUrinationRate()
  {
    return urinationRate == null ? false : urinationRate.isValid();
  }
  public SEScalarVolumePerTime getUrinationRate()
  {
    if (urinationRate == null)
      urinationRate = new SEScalarVolumePerTime();
    return urinationRate;
  }

  public boolean hasUrineOsmolality()
  {
    return urineOsmolality == null ? false : urineOsmolality.isValid();
  }
  public SEScalarOsmolality getUrineOsmolality()
  {
    if (urineOsmolality == null)
      urineOsmolality = new SEScalarOsmolality();
    return urineOsmolality;
  }

  public boolean hasUrineOsmolarity()
  {
    return urineOsmolarity == null ? false : urineOsmolarity.isValid();
  }
  public SEScalarOsmolarity getUrineOsmolarity()
  {
    if (urineOsmolarity == null)
      urineOsmolarity = new SEScalarOsmolarity();
    return urineOsmolarity;
  }

  public boolean hasUrineProductionRate()
  {
    return urineProductionRate == null ? false : urineProductionRate.isValid();
  }
  public SEScalarVolumePerTime getUrineProductionRate()
  {
    if (urineProductionRate == null)
      urineProductionRate = new SEScalarVolumePerTime();
    return urineProductionRate;
  }

  public boolean hasUrineSpecificGravity()
  {
    return urineSpecificGravity == null ? false : urineSpecificGravity.isValid();
  }
  public SEScalar getUrineSpecificGravity()
  {
    if (urineSpecificGravity == null)
      urineSpecificGravity = new SEScalar();
    return urineSpecificGravity;
  }

  public boolean hasUrineVolume()
  {
    return urineVolume == null ? false : urineVolume.isValid();
  }
  public SEScalarVolume getUrineVolume()
  {
    if (urineVolume == null)
      urineVolume = new SEScalarVolume();
    return urineVolume;
  }

  public boolean hasUrineUreaNitrogenConcentration()
  {
    return urineUreaNitrogenConcentration == null ? false : urineUreaNitrogenConcentration.isValid();
  }
  public SEScalarMassPerVolume getUrineUreaNitrogenConcentration()
  {
    if (urineUreaNitrogenConcentration == null)
      urineUreaNitrogenConcentration = new SEScalarMassPerVolume();
    return urineUreaNitrogenConcentration;
  }
}
