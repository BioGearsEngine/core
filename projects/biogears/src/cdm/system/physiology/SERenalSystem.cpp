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
#include <biogears/cdm/system/physiology/SERenalSystem.h>

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/properties/SEScalarOsmolarity.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
SERenalSystem::SERenalSystem(Logger* logger)
  : SESystem(logger)
{
  m_GlomerularFiltrationRate = nullptr;
  m_FiltrationFraction = nullptr;

  m_LeftAfferentArterioleResistance = nullptr;
  m_LeftBowmansCapsulesHydrostaticPressure = nullptr;
  m_LeftBowmansCapsulesOsmoticPressure = nullptr;
  m_LeftEfferentArterioleResistance = nullptr;
  m_LeftGlomerularCapillariesHydrostaticPressure = nullptr;
  m_LeftGlomerularCapillariesOsmoticPressure = nullptr;
  m_LeftGlomerularFiltrationCoefficient = nullptr;
  m_LeftGlomerularFiltrationRate = nullptr;
  m_LeftGlomerularFiltrationSurfaceArea = nullptr;
  m_LeftGlomerularFluidPermeability = nullptr;
  m_LeftFiltrationFraction = nullptr;
  m_LeftNetFiltrationPressure = nullptr;
  m_LeftNetReabsorptionPressure = nullptr;
  m_LeftPeritubularCapillariesHydrostaticPressure = nullptr;
  m_LeftPeritubularCapillariesOsmoticPressure = nullptr;
  m_LeftReabsorptionFiltrationCoefficient = nullptr;
  m_LeftReabsorptionRate = nullptr;
  m_LeftTubularReabsorptionFiltrationSurfaceArea = nullptr;
  m_LeftTubularReabsorptionFluidPermeability = nullptr;
  m_LeftTubularHydrostaticPressure = nullptr;
  m_LeftTubularOsmoticPressure = nullptr;

  m_RenalBloodFlow = nullptr;
  m_RenalPlasmaFlow = nullptr;
  m_RenalVascularResistance = nullptr;

  m_RightAfferentArterioleResistance = nullptr;
  m_RightBowmansCapsulesHydrostaticPressure = nullptr;
  m_RightBowmansCapsulesOsmoticPressure = nullptr;
  m_RightEfferentArterioleResistance = nullptr;
  m_RightGlomerularCapillariesHydrostaticPressure = nullptr;
  m_RightGlomerularCapillariesOsmoticPressure = nullptr;
  m_RightGlomerularFiltrationCoefficient = nullptr;
  m_RightGlomerularFiltrationRate = nullptr;
  m_RightGlomerularFiltrationSurfaceArea = nullptr;
  m_RightGlomerularFluidPermeability = nullptr;
  m_RightFiltrationFraction = nullptr;
  m_RightNetFiltrationPressure = nullptr;
  m_RightNetReabsorptionPressure = nullptr;
  m_RightPeritubularCapillariesHydrostaticPressure = nullptr;
  m_RightPeritubularCapillariesOsmoticPressure = nullptr;
  m_RightReabsorptionFiltrationCoefficient = nullptr;
  m_RightReabsorptionRate = nullptr;
  m_RightTubularReabsorptionFiltrationSurfaceArea = nullptr;
  m_RightTubularReabsorptionFluidPermeability = nullptr;
  m_RightTubularHydrostaticPressure = nullptr;
  m_RightTubularOsmoticPressure = nullptr;

  m_UrinationRate = nullptr;
  m_UrineOsmolality = nullptr;
  m_UrineOsmolarity = nullptr;
  m_UrineProductionRate = nullptr;
  m_UrineSpecificGravity = nullptr;
  m_UrineVolume = nullptr;
  m_UrineUreaNitrogenConcentration = nullptr;
}
//-------------------------------------------------------------------------------

SERenalSystem::~SERenalSystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SERenalSystem::Clear()
{
  SESystem::Clear();

  SAFE_DELETE(m_GlomerularFiltrationRate);
  SAFE_DELETE(m_FiltrationFraction);

  SAFE_DELETE(m_LeftAfferentArterioleResistance);
  SAFE_DELETE(m_LeftBowmansCapsulesHydrostaticPressure);
  SAFE_DELETE(m_LeftBowmansCapsulesOsmoticPressure);
  SAFE_DELETE(m_LeftEfferentArterioleResistance);
  SAFE_DELETE(m_LeftGlomerularCapillariesHydrostaticPressure);
  SAFE_DELETE(m_LeftGlomerularCapillariesOsmoticPressure);
  SAFE_DELETE(m_LeftGlomerularFiltrationCoefficient);
  SAFE_DELETE(m_LeftGlomerularFiltrationRate);
  SAFE_DELETE(m_LeftGlomerularFiltrationSurfaceArea);
  SAFE_DELETE(m_LeftGlomerularFluidPermeability);
  SAFE_DELETE(m_LeftFiltrationFraction);
  SAFE_DELETE(m_LeftNetFiltrationPressure);
  SAFE_DELETE(m_LeftNetReabsorptionPressure);
  SAFE_DELETE(m_LeftPeritubularCapillariesHydrostaticPressure);
  SAFE_DELETE(m_LeftPeritubularCapillariesOsmoticPressure);
  SAFE_DELETE(m_LeftReabsorptionFiltrationCoefficient);
  SAFE_DELETE(m_LeftReabsorptionRate);
  SAFE_DELETE(m_LeftTubularReabsorptionFiltrationSurfaceArea);
  SAFE_DELETE(m_LeftTubularReabsorptionFluidPermeability);
  SAFE_DELETE(m_LeftTubularHydrostaticPressure);
  SAFE_DELETE(m_LeftTubularOsmoticPressure);

  SAFE_DELETE(m_RenalBloodFlow);
  SAFE_DELETE(m_RenalPlasmaFlow);
  SAFE_DELETE(m_RenalVascularResistance);

  SAFE_DELETE(m_RightAfferentArterioleResistance);
  SAFE_DELETE(m_RightBowmansCapsulesHydrostaticPressure);
  SAFE_DELETE(m_RightBowmansCapsulesOsmoticPressure);
  SAFE_DELETE(m_RightEfferentArterioleResistance);
  SAFE_DELETE(m_RightGlomerularCapillariesHydrostaticPressure);
  SAFE_DELETE(m_RightGlomerularCapillariesOsmoticPressure);
  SAFE_DELETE(m_RightGlomerularFiltrationCoefficient);
  SAFE_DELETE(m_RightGlomerularFiltrationRate);
  SAFE_DELETE(m_RightGlomerularFiltrationSurfaceArea);
  SAFE_DELETE(m_RightGlomerularFluidPermeability);
  SAFE_DELETE(m_RightFiltrationFraction);
  SAFE_DELETE(m_RightNetFiltrationPressure);
  SAFE_DELETE(m_RightNetReabsorptionPressure);
  SAFE_DELETE(m_RightPeritubularCapillariesHydrostaticPressure);
  SAFE_DELETE(m_RightPeritubularCapillariesOsmoticPressure);
  SAFE_DELETE(m_RightReabsorptionFiltrationCoefficient);
  SAFE_DELETE(m_RightReabsorptionRate);
  SAFE_DELETE(m_RightTubularReabsorptionFiltrationSurfaceArea);
  SAFE_DELETE(m_RightTubularReabsorptionFluidPermeability);
  SAFE_DELETE(m_RightTubularHydrostaticPressure);
  SAFE_DELETE(m_RightTubularOsmoticPressure);

  SAFE_DELETE(m_UrinationRate);
  SAFE_DELETE(m_UrineOsmolality);
  SAFE_DELETE(m_UrineOsmolarity);
  SAFE_DELETE(m_UrineProductionRate);
  SAFE_DELETE(m_UrineSpecificGravity);
  SAFE_DELETE(m_UrineVolume);
  SAFE_DELETE(m_UrineUreaNitrogenConcentration);
}
//-------------------------------------------------------------------------------

const SEScalar* SERenalSystem::GetScalar(const std::string& name)
{
  if (name.compare("GlomerularFiltrationRate") == 0)
    return &GetGlomerularFiltrationRate();
  if (name.compare("FiltrationFraction") == 0)
    return &GetFiltrationFraction();

  if (name.compare("LeftAfferentArterioleResistance") == 0)
    return &GetLeftAfferentArterioleResistance();
  if (name.compare("LeftBowmansCapsulesHydrostaticPressure") == 0)
    return &GetLeftBowmansCapsulesHydrostaticPressure();
  if (name.compare("LeftBowmansCapsulesOsmoticPressure") == 0)
    return &GetLeftBowmansCapsulesOsmoticPressure();
  if (name.compare("LeftEfferentArterioleResistance") == 0)
    return &GetLeftEfferentArterioleResistance();
  if (name.compare("LeftGlomerularCapillariesHydrostaticPressure") == 0)
    return &GetLeftGlomerularCapillariesHydrostaticPressure();
  if (name.compare("LeftGlomerularCapillariesOsmoticPressure") == 0)
    return &GetLeftGlomerularCapillariesOsmoticPressure();
  if (name.compare("LeftGlomerularFiltrationCoefficient") == 0)
    return &GetLeftGlomerularFiltrationCoefficient();
  if (name.compare("LeftGlomerularFiltrationRate") == 0)
    return &GetLeftGlomerularFiltrationRate();
  if (name.compare("LeftGlomerularFiltrationSurfaceArea") == 0)
    return &GetLeftGlomerularFiltrationSurfaceArea();
  if (name.compare("LeftGlomerularFluidPermeability") == 0)
    return &GetLeftGlomerularFluidPermeability();
  if (name.compare("LeftFiltrationFraction") == 0)
    return &GetLeftFiltrationFraction();
  if (name.compare("LeftNetFiltrationPressure") == 0)
    return &GetLeftNetFiltrationPressure();
  if (name.compare("LeftNetReabsorptionPressure") == 0)
    return &GetLeftNetReabsorptionPressure();
  if (name.compare("LeftPeritubularCapillariesHydrostaticPressure") == 0)
    return &GetLeftPeritubularCapillariesHydrostaticPressure();
  if (name.compare("LeftPeritubularCapillariesOsmoticPressure") == 0)
    return &GetLeftPeritubularCapillariesOsmoticPressure();
  if (name.compare("LeftReabsorptionFiltrationCoefficient") == 0)
    return &GetLeftReabsorptionFiltrationCoefficient();
  if (name.compare("LeftReabsorptionRate") == 0)
    return &GetLeftReabsorptionRate();
  if (name.compare("LeftTubularReabsorptionFiltrationSurfaceArea") == 0)
    return &GetLeftTubularReabsorptionFiltrationSurfaceArea();
  if (name.compare("LeftTubularReabsorptionFluidPermeability") == 0)
    return &GetLeftTubularReabsorptionFluidPermeability();
  if (name.compare("LeftTubularHydrostaticPressure") == 0)
    return &GetLeftTubularHydrostaticPressure();
  if (name.compare("LeftTubularOsmoticPressure") == 0)
    return &GetLeftTubularOsmoticPressure();

  if (name.compare("RenalBloodFlow") == 0)
    return &GetRenalBloodFlow();
  if (name.compare("RenalPlasmaFlow") == 0)
    return &GetRenalPlasmaFlow();
  if (name.compare("RenalVascularResistance") == 0)
    return &GetRenalVascularResistance();

  if (name.compare("RightAfferentArterioleResistance") == 0)
    return &GetRightAfferentArterioleResistance();
  if (name.compare("RightBowmansCapsulesHydrostaticPressure") == 0)
    return &GetRightBowmansCapsulesHydrostaticPressure();
  if (name.compare("RightBowmansCapsulesOsmoticPressure") == 0)
    return &GetRightBowmansCapsulesOsmoticPressure();
  if (name.compare("RightEfferentArterioleResistance") == 0)
    return &GetRightEfferentArterioleResistance();
  if (name.compare("RightGlomerularCapillariesHydrostaticPressure") == 0)
    return &GetRightGlomerularCapillariesHydrostaticPressure();
  if (name.compare("RightGlomerularCapillariesOsmoticPressure") == 0)
    return &GetRightGlomerularCapillariesOsmoticPressure();
  if (name.compare("RightGlomerularFiltrationCoefficient") == 0)
    return &GetRightGlomerularFiltrationCoefficient();
  if (name.compare("RightGlomerularFiltrationRate") == 0)
    return &GetRightGlomerularFiltrationRate();
  if (name.compare("RightGlomerularFiltrationSurfaceArea") == 0)
    return &GetRightGlomerularFiltrationSurfaceArea();
  if (name.compare("RightGlomerularFluidPermeability") == 0)
    return &GetRightGlomerularFluidPermeability();
  if (name.compare("RightFiltrationFraction") == 0)
    return &GetRightFiltrationFraction();
  if (name.compare("RightNetFiltrationPressure") == 0)
    return &GetRightNetFiltrationPressure();
  if (name.compare("RightNetReabsorptionPressure") == 0)
    return &GetRightNetReabsorptionPressure();
  if (name.compare("RightPeritubularCapillariesHydrostaticPressure") == 0)
    return &GetRightPeritubularCapillariesHydrostaticPressure();
  if (name.compare("RightPeritubularCapillariesOsmoticPressure") == 0)
    return &GetRightPeritubularCapillariesOsmoticPressure();
  if (name.compare("RightReabsorptionFiltrationCoefficient") == 0)
    return &GetRightReabsorptionFiltrationCoefficient();
  if (name.compare("RightReabsorptionRate") == 0)
    return &GetRightReabsorptionRate();
  if (name.compare("RightTubularReabsorptionFiltrationSurfaceArea") == 0)
    return &GetRightTubularReabsorptionFiltrationSurfaceArea();
  if (name.compare("RightTubularReabsorptionFluidPermeability") == 0)
    return &GetRightTubularReabsorptionFluidPermeability();
  if (name.compare("RightTubularHydrostaticPressure") == 0)
    return &GetRightTubularHydrostaticPressure();
  if (name.compare("RightTubularOsmoticPressure") == 0)
    return &GetRightTubularOsmoticPressure();

  if (name.compare("UrinationRate") == 0)
    return &GetUrinationRate();
  if (name.compare("UrineOsmolality") == 0)
    return &GetUrineOsmolality();
  if (name.compare("UrineOsmolarity") == 0)
    return &GetUrineOsmolarity();
  if (name.compare("UrineProductionRate") == 0)
    return &GetUrineProductionRate();
  if (name.compare("UrineSpecificGravity") == 0)
    return &GetUrineSpecificGravity();
  if (name.compare("UrineVolume") == 0)
    return &GetUrineVolume();
  if (name.compare("UrineUreaNitrogenConcentration") == 0)
    return &GetUrineUreaNitrogenConcentration();

  return nullptr;
}
//-------------------------------------------------------------------------------

bool SERenalSystem::Load(const CDM::RenalSystemData& in)
{
  SESystem::Load(in);

  if (in.GlomerularFiltrationRate().present())
    GetGlomerularFiltrationRate().Load(in.GlomerularFiltrationRate().get());
  if (in.FiltrationFraction().present())
    GetFiltrationFraction().Load(in.FiltrationFraction().get());

  if (in.LeftAfferentArterioleResistance().present())
    GetLeftAfferentArterioleResistance().Load(in.LeftAfferentArterioleResistance().get());
  if (in.LeftBowmansCapsulesHydrostaticPressure().present())
    GetLeftBowmansCapsulesHydrostaticPressure().Load(in.LeftBowmansCapsulesHydrostaticPressure().get());
  if (in.LeftBowmansCapsulesOsmoticPressure().present())
    GetLeftBowmansCapsulesOsmoticPressure().Load(in.LeftBowmansCapsulesOsmoticPressure().get());
  if (in.LeftEfferentArterioleResistance().present())
    GetLeftEfferentArterioleResistance().Load(in.LeftEfferentArterioleResistance().get());
  if (in.LeftGlomerularCapillariesHydrostaticPressure().present())
    GetLeftGlomerularCapillariesHydrostaticPressure().Load(in.LeftGlomerularCapillariesHydrostaticPressure().get());
  if (in.LeftGlomerularCapillariesOsmoticPressure().present())
    GetLeftGlomerularCapillariesOsmoticPressure().Load(in.LeftGlomerularCapillariesOsmoticPressure().get());
  if (in.LeftGlomerularFiltrationCoefficient().present())
    GetLeftGlomerularFiltrationCoefficient().Load(in.LeftGlomerularFiltrationCoefficient().get());
  if (in.LeftGlomerularFiltrationRate().present())
    GetLeftGlomerularFiltrationRate().Load(in.LeftGlomerularFiltrationRate().get());
  if (in.LeftGlomerularFiltrationSurfaceArea().present())
    GetLeftGlomerularFiltrationSurfaceArea().Load(in.LeftGlomerularFiltrationSurfaceArea().get());
  if (in.LeftGlomerularFluidPermeability().present())
    GetLeftGlomerularFluidPermeability().Load(in.LeftGlomerularFluidPermeability().get());
  if (in.LeftFiltrationFraction().present())
    GetLeftFiltrationFraction().Load(in.LeftFiltrationFraction().get());
  if (in.LeftNetFiltrationPressure().present())
    GetLeftNetFiltrationPressure().Load(in.LeftNetFiltrationPressure().get());
  if (in.LeftNetReabsorptionPressure().present())
    GetLeftNetReabsorptionPressure().Load(in.LeftNetReabsorptionPressure().get());
  if (in.LeftPeritubularCapillariesHydrostaticPressure().present())
    GetLeftPeritubularCapillariesHydrostaticPressure().Load(in.LeftPeritubularCapillariesHydrostaticPressure().get());
  if (in.LeftPeritubularCapillariesOsmoticPressure().present())
    GetLeftPeritubularCapillariesOsmoticPressure().Load(in.LeftPeritubularCapillariesOsmoticPressure().get());
  if (in.LeftReabsorptionFiltrationCoefficient().present())
    GetLeftReabsorptionFiltrationCoefficient().Load(in.LeftReabsorptionFiltrationCoefficient().get());
  if (in.LeftReabsorptionRate().present())
    GetLeftReabsorptionRate().Load(in.LeftReabsorptionRate().get());
  if (in.LeftTubularReabsorptionFiltrationSurfaceArea().present())
    GetLeftTubularReabsorptionFiltrationSurfaceArea().Load(in.LeftTubularReabsorptionFiltrationSurfaceArea().get());
  if (in.LeftTubularReabsorptionFluidPermeability().present())
    GetLeftTubularReabsorptionFluidPermeability().Load(in.LeftTubularReabsorptionFluidPermeability().get());
  if (in.LeftTubularHydrostaticPressure().present())
    GetLeftTubularHydrostaticPressure().Load(in.LeftTubularHydrostaticPressure().get());
  if (in.LeftTubularOsmoticPressure().present())
    GetLeftTubularOsmoticPressure().Load(in.LeftTubularOsmoticPressure().get());

  if (in.RenalBloodFlow().present())
    GetRenalBloodFlow().Load(in.RenalBloodFlow().get());
  if (in.RenalPlasmaFlow().present())
    GetRenalPlasmaFlow().Load(in.RenalPlasmaFlow().get());
  if (in.RenalVascularResistance().present())
    GetRenalVascularResistance().Load(in.RenalVascularResistance().get());

  if (in.RightAfferentArterioleResistance().present())
    GetRightAfferentArterioleResistance().Load(in.RightAfferentArterioleResistance().get());
  if (in.RightBowmansCapsulesHydrostaticPressure().present())
    GetRightBowmansCapsulesHydrostaticPressure().Load(in.RightBowmansCapsulesHydrostaticPressure().get());
  if (in.RightBowmansCapsulesOsmoticPressure().present())
    GetRightBowmansCapsulesOsmoticPressure().Load(in.RightBowmansCapsulesOsmoticPressure().get());
  if (in.RightEfferentArterioleResistance().present())
    GetRightEfferentArterioleResistance().Load(in.RightEfferentArterioleResistance().get());
  if (in.RightGlomerularCapillariesHydrostaticPressure().present())
    GetRightGlomerularCapillariesHydrostaticPressure().Load(in.RightGlomerularCapillariesHydrostaticPressure().get());
  if (in.RightGlomerularCapillariesOsmoticPressure().present())
    GetRightGlomerularCapillariesOsmoticPressure().Load(in.RightGlomerularCapillariesOsmoticPressure().get());
  if (in.RightGlomerularFiltrationCoefficient().present())
    GetRightGlomerularFiltrationCoefficient().Load(in.RightGlomerularFiltrationCoefficient().get());
  if (in.RightGlomerularFiltrationRate().present())
    GetRightGlomerularFiltrationRate().Load(in.RightGlomerularFiltrationRate().get());
  if (in.RightGlomerularFiltrationSurfaceArea().present())
    GetRightGlomerularFiltrationSurfaceArea().Load(in.RightGlomerularFiltrationSurfaceArea().get());
  if (in.RightGlomerularFluidPermeability().present())
    GetRightGlomerularFluidPermeability().Load(in.RightGlomerularFluidPermeability().get());
  if (in.RightFiltrationFraction().present())
    GetRightFiltrationFraction().Load(in.RightFiltrationFraction().get());
  if (in.RightNetFiltrationPressure().present())
    GetRightNetFiltrationPressure().Load(in.RightNetFiltrationPressure().get());
  if (in.RightNetReabsorptionPressure().present())
    GetRightNetReabsorptionPressure().Load(in.RightNetReabsorptionPressure().get());
  if (in.RightPeritubularCapillariesHydrostaticPressure().present())
    GetRightPeritubularCapillariesHydrostaticPressure().Load(in.RightPeritubularCapillariesHydrostaticPressure().get());
  if (in.RightPeritubularCapillariesOsmoticPressure().present())
    GetRightPeritubularCapillariesOsmoticPressure().Load(in.RightPeritubularCapillariesOsmoticPressure().get());
  if (in.RightReabsorptionFiltrationCoefficient().present())
    GetRightReabsorptionFiltrationCoefficient().Load(in.RightReabsorptionFiltrationCoefficient().get());
  if (in.RightReabsorptionRate().present())
    GetRightReabsorptionRate().Load(in.RightReabsorptionRate().get());
  if (in.RightTubularReabsorptionFiltrationSurfaceArea().present())
    GetRightTubularReabsorptionFiltrationSurfaceArea().Load(in.RightTubularReabsorptionFiltrationSurfaceArea().get());
  if (in.RightTubularReabsorptionFluidPermeability().present())
    GetRightTubularReabsorptionFluidPermeability().Load(in.RightTubularReabsorptionFluidPermeability().get());
  if (in.RightTubularHydrostaticPressure().present())
    GetRightTubularHydrostaticPressure().Load(in.RightTubularHydrostaticPressure().get());
  if (in.RightTubularOsmoticPressure().present())
    GetRightTubularOsmoticPressure().Load(in.RightTubularOsmoticPressure().get());

  if (in.UrinationRate().present())
    GetUrinationRate().Load(in.UrinationRate().get());
  if (in.UrineOsmolality().present())
    GetUrineOsmolality().Load(in.UrineOsmolality().get());
  if (in.UrineOsmolarity().present())
    GetUrineOsmolarity().Load(in.UrineOsmolarity().get());
  if (in.UrineProductionRate().present())
    GetUrineProductionRate().Load(in.UrineProductionRate().get());
  if (in.UrineSpecificGravity().present())
    GetUrineSpecificGravity().Load(in.UrineSpecificGravity().get());
  if (in.UrineVolume().present())
    GetUrineVolume().Load(in.UrineVolume().get());
  if (in.UrineUreaNitrogenConcentration().present())
    GetUrineUreaNitrogenConcentration().Load(in.UrineUreaNitrogenConcentration().get());

  return true;
}
//-------------------------------------------------------------------------------

CDM::RenalSystemData* SERenalSystem::Unload() const
{
  CDM::RenalSystemData* data = new CDM::RenalSystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SERenalSystem::Unload(CDM::RenalSystemData& data) const
{
  SESystem::Unload(data);
  if (m_GlomerularFiltrationRate != nullptr)
    data.GlomerularFiltrationRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_GlomerularFiltrationRate->Unload()));
  if (m_FiltrationFraction != nullptr)
    data.FiltrationFraction(std::unique_ptr<CDM::ScalarFractionData>(m_FiltrationFraction->Unload()));

  if (m_LeftAfferentArterioleResistance != nullptr)
    data.LeftAfferentArterioleResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_LeftAfferentArterioleResistance->Unload()));
  if (m_LeftBowmansCapsulesHydrostaticPressure != nullptr)
    data.LeftBowmansCapsulesHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftBowmansCapsulesHydrostaticPressure->Unload()));
  if (m_LeftBowmansCapsulesOsmoticPressure != nullptr)
    data.LeftBowmansCapsulesOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftBowmansCapsulesOsmoticPressure->Unload()));
  if (m_LeftEfferentArterioleResistance != nullptr)
    data.LeftEfferentArterioleResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_LeftEfferentArterioleResistance->Unload()));
  if (m_LeftGlomerularCapillariesHydrostaticPressure != nullptr)
    data.LeftGlomerularCapillariesHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftGlomerularCapillariesHydrostaticPressure->Unload()));
  if (m_LeftGlomerularCapillariesOsmoticPressure != nullptr)
    data.LeftGlomerularCapillariesOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftGlomerularCapillariesOsmoticPressure->Unload()));
  if (m_LeftGlomerularFiltrationCoefficient != nullptr)
    data.LeftGlomerularFiltrationCoefficient(std::unique_ptr<CDM::ScalarVolumePerTimePressureData>(m_LeftGlomerularFiltrationCoefficient->Unload()));
  if (m_LeftGlomerularFiltrationRate != nullptr)
    data.LeftGlomerularFiltrationRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_LeftGlomerularFiltrationRate->Unload()));
  if (m_LeftGlomerularFiltrationSurfaceArea != nullptr)
    data.LeftGlomerularFiltrationSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_LeftGlomerularFiltrationSurfaceArea->Unload()));
  if (m_LeftGlomerularFluidPermeability != nullptr)
    data.LeftGlomerularFluidPermeability(std::unique_ptr<CDM::ScalarVolumePerTimePressureAreaData>(m_LeftGlomerularFluidPermeability->Unload()));
  if (m_LeftFiltrationFraction != nullptr)
    data.LeftFiltrationFraction(std::unique_ptr<CDM::ScalarFractionData>(m_LeftFiltrationFraction->Unload()));
  if (m_LeftNetFiltrationPressure != nullptr)
    data.LeftNetFiltrationPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftNetFiltrationPressure->Unload()));
  if (m_LeftNetReabsorptionPressure != nullptr)
    data.LeftNetReabsorptionPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftNetReabsorptionPressure->Unload()));
  if (m_LeftPeritubularCapillariesHydrostaticPressure != nullptr)
    data.LeftPeritubularCapillariesHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftPeritubularCapillariesHydrostaticPressure->Unload()));
  if (m_LeftPeritubularCapillariesOsmoticPressure != nullptr)
    data.LeftPeritubularCapillariesOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftPeritubularCapillariesOsmoticPressure->Unload()));
  if (m_LeftReabsorptionFiltrationCoefficient != nullptr)
    data.LeftReabsorptionFiltrationCoefficient(std::unique_ptr<CDM::ScalarVolumePerTimePressureData>(m_LeftReabsorptionFiltrationCoefficient->Unload()));
  if (m_LeftReabsorptionRate != nullptr)
    data.LeftReabsorptionRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_LeftReabsorptionRate->Unload()));
  if (m_LeftTubularReabsorptionFiltrationSurfaceArea != nullptr)
    data.LeftTubularReabsorptionFiltrationSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_LeftTubularReabsorptionFiltrationSurfaceArea->Unload()));
  if (m_LeftTubularReabsorptionFluidPermeability != nullptr)
    data.LeftTubularReabsorptionFluidPermeability(std::unique_ptr<CDM::ScalarVolumePerTimePressureAreaData>(m_LeftTubularReabsorptionFluidPermeability->Unload()));
  if (m_LeftTubularHydrostaticPressure != nullptr)
    data.LeftTubularHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftTubularHydrostaticPressure->Unload()));
  if (m_LeftTubularOsmoticPressure != nullptr)
    data.LeftTubularOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_LeftTubularOsmoticPressure->Unload()));

  if (m_RenalBloodFlow != nullptr)
    data.RenalBloodFlow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RenalBloodFlow->Unload()));
  if (m_RenalPlasmaFlow != nullptr)
    data.RenalPlasmaFlow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RenalPlasmaFlow->Unload()));
  if (m_RenalVascularResistance != nullptr)
    data.RenalVascularResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_RenalVascularResistance->Unload()));

  if (m_RightAfferentArterioleResistance != nullptr)
    data.RightAfferentArterioleResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_RightAfferentArterioleResistance->Unload()));
  if (m_RightBowmansCapsulesHydrostaticPressure != nullptr)
    data.RightBowmansCapsulesHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightBowmansCapsulesHydrostaticPressure->Unload()));
  if (m_RightBowmansCapsulesOsmoticPressure != nullptr)
    data.RightBowmansCapsulesOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightBowmansCapsulesOsmoticPressure->Unload()));
  if (m_RightEfferentArterioleResistance != nullptr)
    data.RightEfferentArterioleResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_RightEfferentArterioleResistance->Unload()));
  if (m_RightGlomerularCapillariesHydrostaticPressure != nullptr)
    data.RightGlomerularCapillariesHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightGlomerularCapillariesHydrostaticPressure->Unload()));
  if (m_RightGlomerularCapillariesOsmoticPressure != nullptr)
    data.RightGlomerularCapillariesOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightGlomerularCapillariesOsmoticPressure->Unload()));
  if (m_RightGlomerularFiltrationCoefficient != nullptr)
    data.RightGlomerularFiltrationCoefficient(std::unique_ptr<CDM::ScalarVolumePerTimePressureData>(m_RightGlomerularFiltrationCoefficient->Unload()));
  if (m_RightGlomerularFiltrationRate != nullptr)
    data.RightGlomerularFiltrationRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RightGlomerularFiltrationRate->Unload()));
  if (m_RightGlomerularFiltrationSurfaceArea != nullptr)
    data.RightGlomerularFiltrationSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_RightGlomerularFiltrationSurfaceArea->Unload()));
  if (m_RightGlomerularFluidPermeability != nullptr)
    data.RightGlomerularFluidPermeability(std::unique_ptr<CDM::ScalarVolumePerTimePressureAreaData>(m_RightGlomerularFluidPermeability->Unload()));
  if (m_RightFiltrationFraction != nullptr)
    data.RightFiltrationFraction(std::unique_ptr<CDM::ScalarFractionData>(m_RightFiltrationFraction->Unload()));
  if (m_RightNetFiltrationPressure != nullptr)
    data.RightNetFiltrationPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightNetFiltrationPressure->Unload()));
  if (m_RightNetReabsorptionPressure != nullptr)
    data.RightNetReabsorptionPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightNetReabsorptionPressure->Unload()));
  if (m_RightPeritubularCapillariesHydrostaticPressure != nullptr)
    data.RightPeritubularCapillariesHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightPeritubularCapillariesHydrostaticPressure->Unload()));
  if (m_RightPeritubularCapillariesOsmoticPressure != nullptr)
    data.RightPeritubularCapillariesOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightPeritubularCapillariesOsmoticPressure->Unload()));
  if (m_RightReabsorptionFiltrationCoefficient != nullptr)
    data.RightReabsorptionFiltrationCoefficient(std::unique_ptr<CDM::ScalarVolumePerTimePressureData>(m_RightReabsorptionFiltrationCoefficient->Unload()));
  if (m_RightReabsorptionRate != nullptr)
    data.RightReabsorptionRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_RightReabsorptionRate->Unload()));
  if (m_RightTubularReabsorptionFiltrationSurfaceArea != nullptr)
    data.RightTubularReabsorptionFiltrationSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_RightTubularReabsorptionFiltrationSurfaceArea->Unload()));
  if (m_RightTubularReabsorptionFluidPermeability != nullptr)
    data.RightTubularReabsorptionFluidPermeability(std::unique_ptr<CDM::ScalarVolumePerTimePressureAreaData>(m_RightTubularReabsorptionFluidPermeability->Unload()));
  if (m_RightTubularHydrostaticPressure != nullptr)
    data.RightTubularHydrostaticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightTubularHydrostaticPressure->Unload()));
  if (m_RightTubularOsmoticPressure != nullptr)
    data.RightTubularOsmoticPressure(std::unique_ptr<CDM::ScalarPressureData>(m_RightTubularOsmoticPressure->Unload()));

  if (m_UrinationRate != nullptr)
    data.UrinationRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_UrinationRate->Unload()));
  if (m_UrineOsmolality != nullptr)
    data.UrineOsmolality(std::unique_ptr<CDM::ScalarOsmolalityData>(m_UrineOsmolality->Unload()));
  if (m_UrineOsmolarity != nullptr)
    data.UrineOsmolarity(std::unique_ptr<CDM::ScalarOsmolarityData>(m_UrineOsmolarity->Unload()));
  if (m_UrineProductionRate != nullptr)
    data.UrineProductionRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_UrineProductionRate->Unload()));
  if (m_UrineSpecificGravity != nullptr)
    data.UrineSpecificGravity(std::unique_ptr<CDM::ScalarData>(m_UrineSpecificGravity->Unload()));
  if (m_UrineVolume != nullptr)
    data.UrineVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_UrineVolume->Unload()));
  if (m_UrineUreaNitrogenConcentration != nullptr)
    data.UrineUreaNitrogenConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_UrineUreaNitrogenConcentration->Unload()));
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasGlomerularFiltrationRate() const
{
  return m_GlomerularFiltrationRate == nullptr ? false : m_GlomerularFiltrationRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetGlomerularFiltrationRate()
{
  if (m_GlomerularFiltrationRate == nullptr)
    m_GlomerularFiltrationRate = new SEScalarVolumePerTime();
  return *m_GlomerularFiltrationRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const
{
  if (m_GlomerularFiltrationRate == nullptr)
    return SEScalar::dNaN();
  return m_GlomerularFiltrationRate->GetValue(unit);
}

//-------------------------------------------------------------------------------
bool SERenalSystem::HasFiltrationFraction() const
{
  return m_FiltrationFraction == nullptr ? false : m_FiltrationFraction->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SERenalSystem::GetFiltrationFraction()
{
  if (m_FiltrationFraction == nullptr)
    m_FiltrationFraction = new SEScalarFraction();
  return *m_FiltrationFraction;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetFiltrationFraction() const
{
  if (m_FiltrationFraction == nullptr)
    return SEScalar::dNaN();
  return m_FiltrationFraction->GetValue();
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftAfferentArterioleResistance() const
{
  return m_LeftAfferentArterioleResistance == nullptr ? false : m_LeftAfferentArterioleResistance->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFlowResistance& SERenalSystem::GetLeftAfferentArterioleResistance()
{
  if (m_LeftAfferentArterioleResistance == nullptr)
    m_LeftAfferentArterioleResistance = new SEScalarFlowResistance();
  return *m_LeftAfferentArterioleResistance;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftAfferentArterioleResistance(const FlowResistanceUnit& unit) const
{
  if (m_LeftAfferentArterioleResistance == nullptr)
    return SEScalar::dNaN();
  return m_LeftAfferentArterioleResistance->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftBowmansCapsulesHydrostaticPressure() const
{
  return m_LeftBowmansCapsulesHydrostaticPressure == nullptr ? false : m_LeftBowmansCapsulesHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftBowmansCapsulesHydrostaticPressure()
{
  if (m_LeftBowmansCapsulesHydrostaticPressure == nullptr)
    m_LeftBowmansCapsulesHydrostaticPressure = new SEScalarPressure();
  return *m_LeftBowmansCapsulesHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftBowmansCapsulesHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_LeftBowmansCapsulesHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftBowmansCapsulesHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftBowmansCapsulesOsmoticPressure() const
{
  return m_LeftBowmansCapsulesOsmoticPressure == nullptr ? false : m_LeftBowmansCapsulesOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftBowmansCapsulesOsmoticPressure()
{
  if (m_LeftBowmansCapsulesOsmoticPressure == nullptr)
    m_LeftBowmansCapsulesOsmoticPressure = new SEScalarPressure();
  return *m_LeftBowmansCapsulesOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftBowmansCapsulesOsmoticPressure(const PressureUnit& unit) const
{
  if (m_LeftBowmansCapsulesOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftBowmansCapsulesOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftEfferentArterioleResistance() const
{
  return m_LeftEfferentArterioleResistance == nullptr ? false : m_LeftEfferentArterioleResistance->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFlowResistance& SERenalSystem::GetLeftEfferentArterioleResistance()
{
  if (m_LeftEfferentArterioleResistance == nullptr)
    m_LeftEfferentArterioleResistance = new SEScalarFlowResistance();
  return *m_LeftEfferentArterioleResistance;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftEfferentArterioleResistance(const FlowResistanceUnit& unit) const
{
  if (m_LeftEfferentArterioleResistance == nullptr)
    return SEScalar::dNaN();
  return m_LeftEfferentArterioleResistance->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftGlomerularCapillariesHydrostaticPressure() const
{
  return m_LeftGlomerularCapillariesHydrostaticPressure == nullptr ? false : m_LeftGlomerularCapillariesHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftGlomerularCapillariesHydrostaticPressure()
{
  if (m_LeftGlomerularCapillariesHydrostaticPressure == nullptr)
    m_LeftGlomerularCapillariesHydrostaticPressure = new SEScalarPressure();
  return *m_LeftGlomerularCapillariesHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftGlomerularCapillariesHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_LeftGlomerularCapillariesHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftGlomerularCapillariesHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftGlomerularCapillariesOsmoticPressure() const
{
  return m_LeftGlomerularCapillariesOsmoticPressure == nullptr ? false : m_LeftGlomerularCapillariesOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftGlomerularCapillariesOsmoticPressure()
{
  if (m_LeftGlomerularCapillariesOsmoticPressure == nullptr)
    m_LeftGlomerularCapillariesOsmoticPressure = new SEScalarPressure();
  return *m_LeftGlomerularCapillariesOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftGlomerularCapillariesOsmoticPressure(const PressureUnit& unit) const
{
  if (m_LeftGlomerularCapillariesOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftGlomerularCapillariesOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftGlomerularFiltrationCoefficient() const
{
  return m_LeftGlomerularFiltrationCoefficient == nullptr ? false : m_LeftGlomerularFiltrationCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressure& SERenalSystem::GetLeftGlomerularFiltrationCoefficient()
{
  if (m_LeftGlomerularFiltrationCoefficient == nullptr)
    m_LeftGlomerularFiltrationCoefficient = new SEScalarVolumePerTimePressure();
  return *m_LeftGlomerularFiltrationCoefficient;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftGlomerularFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const
{
  if (m_LeftGlomerularFiltrationCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_LeftGlomerularFiltrationCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftGlomerularFiltrationRate() const
{
  return m_LeftGlomerularFiltrationRate == nullptr ? false : m_LeftGlomerularFiltrationRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetLeftGlomerularFiltrationRate()
{
  if (m_LeftGlomerularFiltrationRate == nullptr)
    m_LeftGlomerularFiltrationRate = new SEScalarVolumePerTime();
  return *m_LeftGlomerularFiltrationRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const
{
  if (m_LeftGlomerularFiltrationRate == nullptr)
    return SEScalar::dNaN();
  return m_LeftGlomerularFiltrationRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftGlomerularFiltrationSurfaceArea() const
{
  return m_LeftGlomerularFiltrationSurfaceArea == nullptr ? false : m_LeftGlomerularFiltrationSurfaceArea->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarArea& SERenalSystem::GetLeftGlomerularFiltrationSurfaceArea()
{
  if (m_LeftGlomerularFiltrationSurfaceArea == nullptr)
    m_LeftGlomerularFiltrationSurfaceArea = new SEScalarArea();
  return *m_LeftGlomerularFiltrationSurfaceArea;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftGlomerularFiltrationSurfaceArea(const AreaUnit& unit) const
{
  if (m_LeftGlomerularFiltrationSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_LeftGlomerularFiltrationSurfaceArea->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftGlomerularFluidPermeability() const
{
  return m_LeftGlomerularFluidPermeability == nullptr ? false : m_LeftGlomerularFluidPermeability->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressureArea& SERenalSystem::GetLeftGlomerularFluidPermeability()
{
  if (m_LeftGlomerularFluidPermeability == nullptr)
    m_LeftGlomerularFluidPermeability = new SEScalarVolumePerTimePressureArea();
  return *m_LeftGlomerularFluidPermeability;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftGlomerularFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const
{
  if (m_LeftGlomerularFluidPermeability == nullptr)
    return SEScalar::dNaN();
  return m_LeftGlomerularFluidPermeability->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftFiltrationFraction() const
{
  return m_LeftFiltrationFraction == nullptr ? false : m_LeftFiltrationFraction->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SERenalSystem::GetLeftFiltrationFraction()
{
  if (m_LeftFiltrationFraction == nullptr)
    m_LeftFiltrationFraction = new SEScalarFraction();
  return *m_LeftFiltrationFraction;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftFiltrationFraction() const
{
  if (m_LeftFiltrationFraction == nullptr)
    return SEScalar::dNaN();
  return m_LeftFiltrationFraction->GetValue();
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftNetFiltrationPressure() const
{
  return m_LeftNetFiltrationPressure == nullptr ? false : m_LeftNetFiltrationPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftNetFiltrationPressure()
{
  if (m_LeftNetFiltrationPressure == nullptr)
    m_LeftNetFiltrationPressure = new SEScalarPressure();
  return *m_LeftNetFiltrationPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftNetFiltrationPressure(const PressureUnit& unit) const
{
  if (m_LeftNetFiltrationPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftNetFiltrationPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftNetReabsorptionPressure() const
{
  return m_LeftNetReabsorptionPressure == nullptr ? false : m_LeftNetReabsorptionPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftNetReabsorptionPressure()
{
  if (m_LeftNetReabsorptionPressure == nullptr)
    m_LeftNetReabsorptionPressure = new SEScalarPressure();
  return *m_LeftNetReabsorptionPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftNetReabsorptionPressure(const PressureUnit& unit) const
{
  if (m_LeftNetReabsorptionPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftNetReabsorptionPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftPeritubularCapillariesHydrostaticPressure() const
{
  return m_LeftPeritubularCapillariesHydrostaticPressure == nullptr ? false : m_LeftPeritubularCapillariesHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftPeritubularCapillariesHydrostaticPressure()
{
  if (m_LeftPeritubularCapillariesHydrostaticPressure == nullptr)
    m_LeftPeritubularCapillariesHydrostaticPressure = new SEScalarPressure();
  return *m_LeftPeritubularCapillariesHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftPeritubularCapillariesHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_LeftPeritubularCapillariesHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftPeritubularCapillariesHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftPeritubularCapillariesOsmoticPressure() const
{
  return m_LeftPeritubularCapillariesOsmoticPressure == nullptr ? false : m_LeftPeritubularCapillariesOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftPeritubularCapillariesOsmoticPressure()
{
  if (m_LeftPeritubularCapillariesOsmoticPressure == nullptr)
    m_LeftPeritubularCapillariesOsmoticPressure = new SEScalarPressure();
  return *m_LeftPeritubularCapillariesOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftPeritubularCapillariesOsmoticPressure(const PressureUnit& unit) const
{
  if (m_LeftPeritubularCapillariesOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftPeritubularCapillariesOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftReabsorptionFiltrationCoefficient() const
{
  return m_LeftReabsorptionFiltrationCoefficient == nullptr ? false : m_LeftReabsorptionFiltrationCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressure& SERenalSystem::GetLeftReabsorptionFiltrationCoefficient()
{
  if (m_LeftReabsorptionFiltrationCoefficient == nullptr)
    m_LeftReabsorptionFiltrationCoefficient = new SEScalarVolumePerTimePressure();
  return *m_LeftReabsorptionFiltrationCoefficient;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftReabsorptionFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const
{
  if (m_LeftReabsorptionFiltrationCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_LeftReabsorptionFiltrationCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftReabsorptionRate() const
{
  return m_LeftReabsorptionRate == nullptr ? false : m_LeftReabsorptionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetLeftReabsorptionRate()
{
  if (m_LeftReabsorptionRate == nullptr)
    m_LeftReabsorptionRate = new SEScalarVolumePerTime();
  return *m_LeftReabsorptionRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftReabsorptionRate(const VolumePerTimeUnit& unit) const
{
  if (m_LeftReabsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_LeftReabsorptionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftTubularReabsorptionFiltrationSurfaceArea() const
{
  return m_LeftTubularReabsorptionFiltrationSurfaceArea == nullptr ? false : m_LeftTubularReabsorptionFiltrationSurfaceArea->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarArea& SERenalSystem::GetLeftTubularReabsorptionFiltrationSurfaceArea()
{
  if (m_LeftTubularReabsorptionFiltrationSurfaceArea == nullptr)
    m_LeftTubularReabsorptionFiltrationSurfaceArea = new SEScalarArea();
  return *m_LeftTubularReabsorptionFiltrationSurfaceArea;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftTubularReabsorptionFiltrationSurfaceArea(const AreaUnit& unit) const
{
  if (m_LeftTubularReabsorptionFiltrationSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_LeftTubularReabsorptionFiltrationSurfaceArea->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftTubularReabsorptionFluidPermeability() const
{
  return m_LeftTubularReabsorptionFluidPermeability == nullptr ? false : m_LeftTubularReabsorptionFluidPermeability->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressureArea& SERenalSystem::GetLeftTubularReabsorptionFluidPermeability()
{
  if (m_LeftTubularReabsorptionFluidPermeability == nullptr)
    m_LeftTubularReabsorptionFluidPermeability = new SEScalarVolumePerTimePressureArea();
  return *m_LeftTubularReabsorptionFluidPermeability;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftTubularReabsorptionFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const
{
  if (m_LeftTubularReabsorptionFluidPermeability == nullptr)
    return SEScalar::dNaN();
  return m_LeftTubularReabsorptionFluidPermeability->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftTubularHydrostaticPressure() const
{
  return m_LeftTubularHydrostaticPressure == nullptr ? false : m_LeftTubularHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftTubularHydrostaticPressure()
{
  if (m_LeftTubularHydrostaticPressure == nullptr)
    m_LeftTubularHydrostaticPressure = new SEScalarPressure();
  return *m_LeftTubularHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftTubularHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_LeftTubularHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftTubularHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasLeftTubularOsmoticPressure() const
{
  return m_LeftTubularOsmoticPressure == nullptr ? false : m_LeftTubularOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetLeftTubularOsmoticPressure()
{
  if (m_LeftTubularOsmoticPressure == nullptr)
    m_LeftTubularOsmoticPressure = new SEScalarPressure();
  return *m_LeftTubularOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetLeftTubularOsmoticPressure(const PressureUnit& unit) const
{
  if (m_LeftTubularOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_LeftTubularOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRenalBloodFlow() const
{
  return m_RenalBloodFlow == nullptr ? false : m_RenalBloodFlow->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetRenalBloodFlow()
{
  if (m_RenalBloodFlow == nullptr)
    m_RenalBloodFlow = new SEScalarVolumePerTime();
  return *m_RenalBloodFlow;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRenalBloodFlow(const VolumePerTimeUnit& unit) const
{
  if (m_RenalBloodFlow == nullptr)
    return SEScalar::dNaN();
  return m_RenalBloodFlow->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRenalPlasmaFlow() const
{
  return m_RenalPlasmaFlow == nullptr ? false : m_RenalPlasmaFlow->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetRenalPlasmaFlow()
{
  if (m_RenalPlasmaFlow == nullptr)
    m_RenalPlasmaFlow = new SEScalarVolumePerTime();
  return *m_RenalPlasmaFlow;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRenalPlasmaFlow(const VolumePerTimeUnit& unit) const
{
  if (m_RenalPlasmaFlow == nullptr)
    return SEScalar::dNaN();
  return m_RenalPlasmaFlow->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRenalVascularResistance() const
{
  return m_RenalVascularResistance == nullptr ? false : m_RenalVascularResistance->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFlowResistance& SERenalSystem::GetRenalVascularResistance()
{
  if (m_RenalVascularResistance == nullptr)
    m_RenalVascularResistance = new SEScalarFlowResistance();
  return *m_RenalVascularResistance;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRenalVascularResistance(const FlowResistanceUnit& unit) const
{
  if (m_RenalVascularResistance == nullptr)
    return SEScalar::dNaN();
  return m_RenalVascularResistance->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightAfferentArterioleResistance() const
{
  return m_RightAfferentArterioleResistance == nullptr ? false : m_RightAfferentArterioleResistance->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFlowResistance& SERenalSystem::GetRightAfferentArterioleResistance()
{
  if (m_RightAfferentArterioleResistance == nullptr)
    m_RightAfferentArterioleResistance = new SEScalarFlowResistance();
  return *m_RightAfferentArterioleResistance;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightAfferentArterioleResistance(const FlowResistanceUnit& unit) const
{
  if (m_RightAfferentArterioleResistance == nullptr)
    return SEScalar::dNaN();
  return m_RightAfferentArterioleResistance->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightBowmansCapsulesHydrostaticPressure() const
{
  return m_RightBowmansCapsulesHydrostaticPressure == nullptr ? false : m_RightBowmansCapsulesHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightBowmansCapsulesHydrostaticPressure()
{
  if (m_RightBowmansCapsulesHydrostaticPressure == nullptr)
    m_RightBowmansCapsulesHydrostaticPressure = new SEScalarPressure();
  return *m_RightBowmansCapsulesHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightBowmansCapsulesHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_RightBowmansCapsulesHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightBowmansCapsulesHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightBowmansCapsulesOsmoticPressure() const
{
  return m_RightBowmansCapsulesOsmoticPressure == nullptr ? false : m_RightBowmansCapsulesOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightBowmansCapsulesOsmoticPressure()
{
  if (m_RightBowmansCapsulesOsmoticPressure == nullptr)
    m_RightBowmansCapsulesOsmoticPressure = new SEScalarPressure();
  return *m_RightBowmansCapsulesOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightBowmansCapsulesOsmoticPressure(const PressureUnit& unit) const
{
  if (m_RightBowmansCapsulesOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightBowmansCapsulesOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightEfferentArterioleResistance() const
{
  return m_RightEfferentArterioleResistance == nullptr ? false : m_RightEfferentArterioleResistance->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFlowResistance& SERenalSystem::GetRightEfferentArterioleResistance()
{
  if (m_RightEfferentArterioleResistance == nullptr)
    m_RightEfferentArterioleResistance = new SEScalarFlowResistance();
  return *m_RightEfferentArterioleResistance;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightEfferentArterioleResistance(const FlowResistanceUnit& unit) const
{
  if (m_RightEfferentArterioleResistance == nullptr)
    return SEScalar::dNaN();
  return m_RightEfferentArterioleResistance->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightGlomerularCapillariesHydrostaticPressure() const
{
  return m_RightGlomerularCapillariesHydrostaticPressure == nullptr ? false : m_RightGlomerularCapillariesHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightGlomerularCapillariesHydrostaticPressure()
{
  if (m_RightGlomerularCapillariesHydrostaticPressure == nullptr)
    m_RightGlomerularCapillariesHydrostaticPressure = new SEScalarPressure();
  return *m_RightGlomerularCapillariesHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightGlomerularCapillariesHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_RightGlomerularCapillariesHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightGlomerularCapillariesHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightGlomerularCapillariesOsmoticPressure() const
{
  return m_RightGlomerularCapillariesOsmoticPressure == nullptr ? false : m_RightGlomerularCapillariesOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightGlomerularCapillariesOsmoticPressure()
{
  if (m_RightGlomerularCapillariesOsmoticPressure == nullptr)
    m_RightGlomerularCapillariesOsmoticPressure = new SEScalarPressure();
  return *m_RightGlomerularCapillariesOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightGlomerularCapillariesOsmoticPressure(const PressureUnit& unit) const
{
  if (m_RightGlomerularCapillariesOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightGlomerularCapillariesOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightGlomerularFiltrationCoefficient() const
{
  return m_RightGlomerularFiltrationCoefficient == nullptr ? false : m_RightGlomerularFiltrationCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressure& SERenalSystem::GetRightGlomerularFiltrationCoefficient()
{
  if (m_RightGlomerularFiltrationCoefficient == nullptr)
    m_RightGlomerularFiltrationCoefficient = new SEScalarVolumePerTimePressure();
  return *m_RightGlomerularFiltrationCoefficient;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightGlomerularFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const
{
  if (m_RightGlomerularFiltrationCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_RightGlomerularFiltrationCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightGlomerularFiltrationRate() const
{
  return m_RightGlomerularFiltrationRate == nullptr ? false : m_RightGlomerularFiltrationRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetRightGlomerularFiltrationRate()
{
  if (m_RightGlomerularFiltrationRate == nullptr)
    m_RightGlomerularFiltrationRate = new SEScalarVolumePerTime();
  return *m_RightGlomerularFiltrationRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightGlomerularFiltrationRate(const VolumePerTimeUnit& unit) const
{
  if (m_RightGlomerularFiltrationRate == nullptr)
    return SEScalar::dNaN();
  return m_RightGlomerularFiltrationRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightGlomerularFiltrationSurfaceArea() const
{
  return m_RightGlomerularFiltrationSurfaceArea == nullptr ? false : m_RightGlomerularFiltrationSurfaceArea->IsValid();
}//-------------------------------------------------------------------------------
SEScalarArea& SERenalSystem::GetRightGlomerularFiltrationSurfaceArea()
{
  if (m_RightGlomerularFiltrationSurfaceArea == nullptr)
    m_RightGlomerularFiltrationSurfaceArea = new SEScalarArea();
  return *m_RightGlomerularFiltrationSurfaceArea;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightGlomerularFiltrationSurfaceArea(const AreaUnit& unit) const
{
  if (m_RightGlomerularFiltrationSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_RightGlomerularFiltrationSurfaceArea->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightGlomerularFluidPermeability() const
{
  return m_RightGlomerularFluidPermeability == nullptr ? false : m_RightGlomerularFluidPermeability->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressureArea& SERenalSystem::GetRightGlomerularFluidPermeability()
{
  if (m_RightGlomerularFluidPermeability == nullptr)
    m_RightGlomerularFluidPermeability = new SEScalarVolumePerTimePressureArea();
  return *m_RightGlomerularFluidPermeability;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightGlomerularFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const
{
  if (m_RightGlomerularFluidPermeability == nullptr)
    return SEScalar::dNaN();
  return m_RightGlomerularFluidPermeability->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightFiltrationFraction() const
{
  return m_RightFiltrationFraction == nullptr ? false : m_RightFiltrationFraction->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SERenalSystem::GetRightFiltrationFraction()
{
  if (m_RightFiltrationFraction == nullptr)
    m_RightFiltrationFraction = new SEScalarFraction();
  return *m_RightFiltrationFraction;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightFiltrationFraction() const
{
  if (m_RightFiltrationFraction == nullptr)
    return SEScalar::dNaN();
  return m_RightFiltrationFraction->GetValue();
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightNetFiltrationPressure() const
{
  return m_RightNetFiltrationPressure == nullptr ? false : m_RightNetFiltrationPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightNetFiltrationPressure()
{
  if (m_RightNetFiltrationPressure == nullptr)
    m_RightNetFiltrationPressure = new SEScalarPressure();
  return *m_RightNetFiltrationPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightNetFiltrationPressure(const PressureUnit& unit) const
{
  if (m_RightNetFiltrationPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightNetFiltrationPressure->GetValue(unit);
}


bool SERenalSystem::HasRightNetReabsorptionPressure() const
{
  return m_RightNetReabsorptionPressure == nullptr ? false : m_RightNetReabsorptionPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightNetReabsorptionPressure()
{
  if (m_RightNetReabsorptionPressure == nullptr)
    m_RightNetReabsorptionPressure = new SEScalarPressure();
  return *m_RightNetReabsorptionPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightNetReabsorptionPressure(const PressureUnit& unit) const
{
  if (m_RightNetReabsorptionPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightNetReabsorptionPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightPeritubularCapillariesHydrostaticPressure() const
{
  return m_RightPeritubularCapillariesHydrostaticPressure == nullptr ? false : m_RightPeritubularCapillariesHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightPeritubularCapillariesHydrostaticPressure()
{
  if (m_RightPeritubularCapillariesHydrostaticPressure == nullptr)
    m_RightPeritubularCapillariesHydrostaticPressure = new SEScalarPressure();
  return *m_RightPeritubularCapillariesHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightPeritubularCapillariesHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_RightPeritubularCapillariesHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightPeritubularCapillariesHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightPeritubularCapillariesOsmoticPressure() const
{
  return m_RightPeritubularCapillariesOsmoticPressure == nullptr ? false : m_RightPeritubularCapillariesOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightPeritubularCapillariesOsmoticPressure()
{
  if (m_RightPeritubularCapillariesOsmoticPressure == nullptr)
    m_RightPeritubularCapillariesOsmoticPressure = new SEScalarPressure();
  return *m_RightPeritubularCapillariesOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightPeritubularCapillariesOsmoticPressure(const PressureUnit& unit) const
{
  if (m_RightPeritubularCapillariesOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightPeritubularCapillariesOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightReabsorptionFiltrationCoefficient() const
{
  return m_RightReabsorptionFiltrationCoefficient == nullptr ? false : m_RightReabsorptionFiltrationCoefficient->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressure& SERenalSystem::GetRightReabsorptionFiltrationCoefficient()
{
  if (m_RightReabsorptionFiltrationCoefficient == nullptr)
    m_RightReabsorptionFiltrationCoefficient = new SEScalarVolumePerTimePressure();
  return *m_RightReabsorptionFiltrationCoefficient;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightReabsorptionFiltrationCoefficient(const VolumePerTimePressureUnit& unit) const
{
  if (m_RightReabsorptionFiltrationCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_RightReabsorptionFiltrationCoefficient->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightReabsorptionRate() const
{
  return m_RightReabsorptionRate == nullptr ? false : m_RightReabsorptionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetRightReabsorptionRate()
{
  if (m_RightReabsorptionRate == nullptr)
    m_RightReabsorptionRate = new SEScalarVolumePerTime();
  return *m_RightReabsorptionRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightReabsorptionRate(const VolumePerTimeUnit& unit) const
{
  if (m_RightReabsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_RightReabsorptionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightTubularReabsorptionFiltrationSurfaceArea() const
{
  return m_RightTubularReabsorptionFiltrationSurfaceArea == nullptr ? false : m_RightTubularReabsorptionFiltrationSurfaceArea->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarArea& SERenalSystem::GetRightTubularReabsorptionFiltrationSurfaceArea()
{
  if (m_RightTubularReabsorptionFiltrationSurfaceArea == nullptr)
    m_RightTubularReabsorptionFiltrationSurfaceArea = new SEScalarArea();
  return *m_RightTubularReabsorptionFiltrationSurfaceArea;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightTubularReabsorptionFiltrationSurfaceArea(const AreaUnit& unit) const
{
  if (m_RightTubularReabsorptionFiltrationSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_RightTubularReabsorptionFiltrationSurfaceArea->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightTubularReabsorptionFluidPermeability() const
{
  return m_RightTubularReabsorptionFluidPermeability == nullptr ? false : m_RightTubularReabsorptionFluidPermeability->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTimePressureArea& SERenalSystem::GetRightTubularReabsorptionFluidPermeability()
{
  if (m_RightTubularReabsorptionFluidPermeability == nullptr)
    m_RightTubularReabsorptionFluidPermeability = new SEScalarVolumePerTimePressureArea();
  return *m_RightTubularReabsorptionFluidPermeability;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightTubularReabsorptionFluidPermeability(const VolumePerTimePressureAreaUnit& unit) const
{
  if (m_RightTubularReabsorptionFluidPermeability == nullptr)
    return SEScalar::dNaN();
  return m_RightTubularReabsorptionFluidPermeability->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightTubularHydrostaticPressure() const
{
  return m_RightTubularHydrostaticPressure == nullptr ? false : m_RightTubularHydrostaticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightTubularHydrostaticPressure()
{
  if (m_RightTubularHydrostaticPressure == nullptr)
    m_RightTubularHydrostaticPressure = new SEScalarPressure();
  return *m_RightTubularHydrostaticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightTubularHydrostaticPressure(const PressureUnit& unit) const
{
  if (m_RightTubularHydrostaticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightTubularHydrostaticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasRightTubularOsmoticPressure() const
{
  return m_RightTubularOsmoticPressure == nullptr ? false : m_RightTubularOsmoticPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetRightTubularOsmoticPressure()
{
  if (m_RightTubularOsmoticPressure == nullptr)
    m_RightTubularOsmoticPressure = new SEScalarPressure();
  return *m_RightTubularOsmoticPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetRightTubularOsmoticPressure(const PressureUnit& unit) const
{
  if (m_RightTubularOsmoticPressure == nullptr)
    return SEScalar::dNaN();
  return m_RightTubularOsmoticPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrinationRate() const
{
  return m_UrinationRate == nullptr ? false : m_UrinationRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetUrinationRate()
{
  if (m_UrinationRate == nullptr)
    m_UrinationRate = new SEScalarVolumePerTime();
  return *m_UrinationRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrinationRate(const VolumePerTimeUnit& unit) const
{
  if (m_UrinationRate == nullptr)
    return SEScalar::dNaN();
  return m_UrinationRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrineOsmolality() const
{
  return m_UrineOsmolality == nullptr ? false : m_UrineOsmolality->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarOsmolality& SERenalSystem::GetUrineOsmolality()
{
  if (m_UrineOsmolality == nullptr)
    m_UrineOsmolality = new SEScalarOsmolality();
  return *m_UrineOsmolality;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrineOsmolality(const OsmolalityUnit& unit) const
{
  if (m_UrineOsmolality == nullptr)
    return SEScalar::dNaN();
  return m_UrineOsmolality->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrineOsmolarity() const
{
  return m_UrineOsmolarity == nullptr ? false : m_UrineOsmolarity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarOsmolarity& SERenalSystem::GetUrineOsmolarity()
{
  if (m_UrineOsmolarity == nullptr)
    m_UrineOsmolarity = new SEScalarOsmolarity();
  return *m_UrineOsmolarity;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrineOsmolarity(const OsmolarityUnit& unit) const
{
  if (m_UrineOsmolarity == nullptr)
    return SEScalar::dNaN();
  return m_UrineOsmolarity->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrineProductionRate() const
{
  return m_UrineProductionRate == nullptr ? false : m_UrineProductionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetUrineProductionRate()
{
  if (m_UrineProductionRate == nullptr)
    m_UrineProductionRate = new SEScalarVolumePerTime();
  return *m_UrineProductionRate;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrineProductionRate(const VolumePerTimeUnit& unit) const
{
  if (m_UrineProductionRate == nullptr)
    return SEScalar::dNaN();
  return m_UrineProductionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrineSpecificGravity() const
{
  return m_UrineSpecificGravity == nullptr ? false : m_UrineSpecificGravity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SERenalSystem::GetUrineSpecificGravity()
{
  if (m_UrineSpecificGravity == nullptr)
    m_UrineSpecificGravity = new SEScalar();
  return *m_UrineSpecificGravity;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrineSpecificGravity() const
{
  if (m_UrineSpecificGravity == nullptr)
    return SEScalar::dNaN();
  return m_UrineSpecificGravity->GetValue();
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrineVolume() const
{
  return m_UrineVolume == nullptr ? false : m_UrineVolume->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolume& SERenalSystem::GetUrineVolume()
{
  if (m_UrineVolume == nullptr)
    m_UrineVolume = new SEScalarVolume();
  return *m_UrineVolume;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrineVolume(const VolumeUnit& unit) const
{
  if (m_UrineVolume == nullptr)
    return SEScalar::dNaN();
  return m_UrineVolume->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasUrineUreaNitrogenConcentration() const
{
  return m_UrineUreaNitrogenConcentration == nullptr ? false : m_UrineUreaNitrogenConcentration->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SERenalSystem::GetUrineUreaNitrogenConcentration()
{
  if (m_UrineUreaNitrogenConcentration == nullptr)
    m_UrineUreaNitrogenConcentration = new SEScalarMassPerVolume();
  return *m_UrineUreaNitrogenConcentration;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetUrineUreaNitrogenConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_UrineUreaNitrogenConcentration == nullptr)
    return SEScalar::dNaN();
  return m_UrineUreaNitrogenConcentration->GetValue(unit);
}
//-------------------------------------------------------------------------------

Tree<std::string> SERenalSystem::GetPhysiologyRequestGraph() const
{
  return {};
}
}