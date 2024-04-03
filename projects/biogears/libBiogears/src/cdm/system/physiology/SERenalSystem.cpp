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

#include "io/cdm/Physiology.h"

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
constexpr char idBladderPressure[] = "BladderPressure";
constexpr char idGlomerularFiltrationRate[] = "GlomerularFiltrationRate";
constexpr char idFiltrationFraction[] = "FiltrationFraction";
constexpr char idLeftAfferentArterioleResistance[] = "LeftAfferentArterioleResistance";
constexpr char idLeftBowmansCapsulesHydrostaticPressure[] = "LeftBowmansCapsulesHydrostaticPressure";
constexpr char idLeftBowmansCapsulesOsmoticPressure[] = "LeftBowmansCapsulesOsmoticPressure";
constexpr char idLeftEfferentArterioleResistance[] = "LeftEfferentArterioleResistance";
constexpr char idLeftGlomerularCapillariesHydrostaticPressure[] = "LeftGlomerularCapillariesHydrostaticPressure";
constexpr char idLeftGlomerularCapillariesOsmoticPressure[] = "LeftGlomerularCapillariesOsmoticPressure";
constexpr char idLeftGlomerularFiltrationCoefficient[] = "LeftGlomerularFiltrationCoefficient";
constexpr char idLeftGlomerularFiltrationRate[] = "LeftGlomerularFiltrationRate";
constexpr char idLeftGlomerularFiltrationSurfaceArea[] = "LeftGlomerularFiltrationSurfaceArea";
constexpr char idLeftGlomerularFluidPermeability[] = "LeftGlomerularFluidPermeability";
constexpr char idLeftFiltrationFraction[] = "LeftFiltrationFraction";
constexpr char idLeftNetFiltrationPressure[] = "LeftNetFiltrationPressure";
constexpr char idLeftNetReabsorptionPressure[] = "LeftNetReabsorptionPressure";
constexpr char idLeftPeritubularCapillariesHydrostaticPressure[] = "LeftPeritubularCapillariesHydrostaticPressure";
constexpr char idLeftPeritubularCapillariesOsmoticPressure[] = "LeftPeritubularCapillariesOsmoticPressure";
constexpr char idLeftReabsorptionFiltrationCoefficient[] = "LeftReabsorptionFiltrationCoefficient";
constexpr char idLeftReabsorptionRate[] = "LeftReabsorptionRate";
constexpr char idLeftTubularReabsorptionFiltrationSurfaceArea[] = "LeftTubularReabsorptionFiltrationSurfaceArea";
constexpr char idLeftTubularReabsorptionFluidPermeability[] = "LeftTubularReabsorptionFluidPermeability";
constexpr char idLeftTubularHydrostaticPressure[] = "LeftTubularHydrostaticPressure";
constexpr char idLeftTubularOsmoticPressure[] = "LeftTubularOsmoticPressure";
constexpr char idRenalBloodFlow[] = "RenalBloodFlow";
constexpr char idRenalPlasmaFlow[] = "RenalPlasmaFlow";
constexpr char idRenalVascularResistance[] = "RenalVascularResistance";
constexpr char idRightAfferentArterioleResistance[] = "RightAfferentArterioleResistance";
constexpr char idRightBowmansCapsulesHydrostaticPressure[] = "RightBowmansCapsulesHydrostaticPressure";
constexpr char idRightBowmansCapsulesOsmoticPressure[] = "RightBowmansCapsulesOsmoticPressure";
constexpr char idRightEfferentArterioleResistance[] = "RightEfferentArterioleResistance";
constexpr char idRightGlomerularCapillariesHydrostaticPressure[] = "RightGlomerularCapillariesHydrostaticPressure";
constexpr char idRightGlomerularCapillariesOsmoticPressure[] = "RightGlomerularCapillariesOsmoticPressure";
constexpr char idRightGlomerularFiltrationCoefficient[] = "RightGlomerularFiltrationCoefficient";
constexpr char idRightGlomerularFiltrationRate[] = "RightGlomerularFiltrationRate";
constexpr char idRightGlomerularFiltrationSurfaceArea[] = "RightGlomerularFiltrationSurfaceArea";
constexpr char idRightGlomerularFluidPermeability[] = "RightGlomerularFluidPermeability";
constexpr char idRightFiltrationFraction[] = "RightFiltrationFraction";
constexpr char idRightNetFiltrationPressure[] = "RightNetFiltrationPressure";
constexpr char idRightNetReabsorptionPressure[] = "RightNetReabsorptionPressure";
constexpr char idRightPeritubularCapillariesHydrostaticPressure[] = "RightPeritubularCapillariesHydrostaticPressure";
constexpr char idRightPeritubularCapillariesOsmoticPressure[] = "RightPeritubularCapillariesOsmoticPressure";
constexpr char idRightReabsorptionFiltrationCoefficient[] = "RightReabsorptionFiltrationCoefficient";
constexpr char idRightReabsorptionRate[] = "RightReabsorptionRate";
constexpr char idRightTubularReabsorptionFiltrationSurfaceArea[] = "RightTubularReabsorptionFiltrationSurfaceArea";
constexpr char idRightTubularReabsorptionFluidPermeability[] = "RightTubularReabsorptionFluidPermeability";
constexpr char idRightTubularHydrostaticPressure[] = "RightTubularHydrostaticPressure";
constexpr char idRightTubularOsmoticPressure[] = "RightTubularOsmoticPressure";
constexpr char idUrinationRate[] = "UrinationRate";
constexpr char idUrineOsmolality[] = "UrineOsmolality";
constexpr char idUrineOsmolarity[] = "UrineOsmolarity";
constexpr char idUrineProductionRate[] = "UrineProductionRate";
constexpr char idMeanUrineOutput[] = "MeanUrineOutput";
constexpr char idUrineSpecificGravity[] = "UrineSpecificGravity";
constexpr char idUrineVolume[] = "UrineVolume";
constexpr char idUrineUreaNitrogenConcentration[] = "UrineUreaNitrogenConcentration";

SERenalSystem::SERenalSystem(Logger* logger)
  : SESystem(logger)
{
  m_BladderPressure = nullptr;
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
  m_MeanUrineOutput = nullptr;
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

  SAFE_DELETE(m_BladderPressure);
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
  SAFE_DELETE(m_MeanUrineOutput);
  SAFE_DELETE(m_UrineSpecificGravity);
  SAFE_DELETE(m_UrineVolume);
  SAFE_DELETE(m_UrineUreaNitrogenConcentration);
}
//-------------------------------------------------------------------------------
const SEScalar* SERenalSystem::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
const SEScalar* SERenalSystem::GetScalar(const std::string& name)
{
  if (name == idBladderPressure)
    return &GetBladderPressure();
  if (name == idGlomerularFiltrationRate)
    return &GetGlomerularFiltrationRate();
  if (name == idFiltrationFraction)
    return &GetFiltrationFraction();

  if (name == idLeftAfferentArterioleResistance)
    return &GetLeftAfferentArterioleResistance();
  if (name == idLeftBowmansCapsulesHydrostaticPressure)
    return &GetLeftBowmansCapsulesHydrostaticPressure();
  if (name == idLeftBowmansCapsulesOsmoticPressure)
    return &GetLeftBowmansCapsulesOsmoticPressure();
  if (name == idLeftEfferentArterioleResistance)
    return &GetLeftEfferentArterioleResistance();
  if (name == idLeftGlomerularCapillariesHydrostaticPressure)
    return &GetLeftGlomerularCapillariesHydrostaticPressure();
  if (name == idLeftGlomerularCapillariesOsmoticPressure)
    return &GetLeftGlomerularCapillariesOsmoticPressure();
  if (name == idLeftGlomerularFiltrationCoefficient)
    return &GetLeftGlomerularFiltrationCoefficient();
  if (name == idLeftGlomerularFiltrationRate)
    return &GetLeftGlomerularFiltrationRate();
  if (name == idLeftGlomerularFiltrationSurfaceArea)
    return &GetLeftGlomerularFiltrationSurfaceArea();
  if (name == idLeftGlomerularFluidPermeability)
    return &GetLeftGlomerularFluidPermeability();
  if (name == idLeftFiltrationFraction)
    return &GetLeftFiltrationFraction();
  if (name == idLeftNetFiltrationPressure)
    return &GetLeftNetFiltrationPressure();
  if (name == idLeftNetReabsorptionPressure)
    return &GetLeftNetReabsorptionPressure();
  if (name == idLeftPeritubularCapillariesHydrostaticPressure)
    return &GetLeftPeritubularCapillariesHydrostaticPressure();
  if (name == idLeftPeritubularCapillariesOsmoticPressure)
    return &GetLeftPeritubularCapillariesOsmoticPressure();
  if (name == idLeftReabsorptionFiltrationCoefficient)
    return &GetLeftReabsorptionFiltrationCoefficient();
  if (name == idLeftReabsorptionRate)
    return &GetLeftReabsorptionRate();
  if (name == idLeftTubularReabsorptionFiltrationSurfaceArea)
    return &GetLeftTubularReabsorptionFiltrationSurfaceArea();
  if (name == idLeftTubularReabsorptionFluidPermeability)
    return &GetLeftTubularReabsorptionFluidPermeability();
  if (name == idLeftTubularHydrostaticPressure)
    return &GetLeftTubularHydrostaticPressure();
  if (name == idLeftTubularOsmoticPressure)
    return &GetLeftTubularOsmoticPressure();

  if (name == idRenalBloodFlow)
    return &GetRenalBloodFlow();
  if (name == idRenalPlasmaFlow)
    return &GetRenalPlasmaFlow();
  if (name == idRenalVascularResistance)
    return &GetRenalVascularResistance();

  if (name == idRightAfferentArterioleResistance)
    return &GetRightAfferentArterioleResistance();
  if (name == idRightBowmansCapsulesHydrostaticPressure)
    return &GetRightBowmansCapsulesHydrostaticPressure();
  if (name == idRightBowmansCapsulesOsmoticPressure)
    return &GetRightBowmansCapsulesOsmoticPressure();
  if (name == idRightEfferentArterioleResistance)
    return &GetRightEfferentArterioleResistance();
  if (name == idRightGlomerularCapillariesHydrostaticPressure)
    return &GetRightGlomerularCapillariesHydrostaticPressure();
  if (name == idRightGlomerularCapillariesOsmoticPressure)
    return &GetRightGlomerularCapillariesOsmoticPressure();
  if (name == idRightGlomerularFiltrationCoefficient)
    return &GetRightGlomerularFiltrationCoefficient();
  if (name == idRightGlomerularFiltrationRate)
    return &GetRightGlomerularFiltrationRate();
  if (name == idRightGlomerularFiltrationSurfaceArea)
    return &GetRightGlomerularFiltrationSurfaceArea();
  if (name == idRightGlomerularFluidPermeability)
    return &GetRightGlomerularFluidPermeability();
  if (name == idRightFiltrationFraction)
    return &GetRightFiltrationFraction();
  if (name == idRightNetFiltrationPressure)
    return &GetRightNetFiltrationPressure();
  if (name == idRightNetReabsorptionPressure)
    return &GetRightNetReabsorptionPressure();
  if (name == idRightPeritubularCapillariesHydrostaticPressure)
    return &GetRightPeritubularCapillariesHydrostaticPressure();
  if (name == idRightPeritubularCapillariesOsmoticPressure)
    return &GetRightPeritubularCapillariesOsmoticPressure();
  if (name == idRightReabsorptionFiltrationCoefficient)
    return &GetRightReabsorptionFiltrationCoefficient();
  if (name == idRightReabsorptionRate)
    return &GetRightReabsorptionRate();
  if (name == idRightTubularReabsorptionFiltrationSurfaceArea)
    return &GetRightTubularReabsorptionFiltrationSurfaceArea();
  if (name == idRightTubularReabsorptionFluidPermeability)
    return &GetRightTubularReabsorptionFluidPermeability();
  if (name == idRightTubularHydrostaticPressure)
    return &GetRightTubularHydrostaticPressure();
  if (name == idRightTubularOsmoticPressure)
    return &GetRightTubularOsmoticPressure();

  if (name == idUrinationRate)
    return &GetUrinationRate();
  if (name == idUrineOsmolality)
    return &GetUrineOsmolality();
  if (name == idUrineOsmolarity)
    return &GetUrineOsmolarity();
  if (name == idUrineProductionRate)
    return &GetUrineProductionRate();
  if (name == idMeanUrineOutput)
    return &GetMeanUrineOutput();
  if (name == idUrineSpecificGravity)
    return &GetUrineSpecificGravity();
  if (name == idUrineVolume)
    return &GetUrineVolume();
  if (name == idUrineUreaNitrogenConcentration)
    return &GetUrineUreaNitrogenConcentration();

  return nullptr;
}
//-------------------------------------------------------------------------------

bool SERenalSystem::Load(const CDM::RenalSystemData& in)
{
  io::Physiology::UnMarshall(in, *this);

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
  io::Physiology::Marshall(*this, data);
}
//-------------------------------------------------------------------------------

bool SERenalSystem::HasBladderPressure() const
{
  return m_BladderPressure == nullptr ? false : m_BladderPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SERenalSystem::GetBladderPressure()
{
  if (m_BladderPressure == nullptr)
    m_BladderPressure = new SEScalarPressure();
  return *m_BladderPressure;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetBladderPressure(const PressureUnit& unit) const
{
  if (m_BladderPressure == nullptr)
    return SEScalar::dNaN();
  return m_BladderPressure->GetValue(unit);
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
} //-------------------------------------------------------------------------------
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
bool SERenalSystem::HasMeanUrineOutput() const
{
  return m_MeanUrineOutput == nullptr ? false : m_MeanUrineOutput->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SERenalSystem::GetMeanUrineOutput()
{
  if (m_MeanUrineOutput == nullptr)
    m_MeanUrineOutput = new SEScalarVolumePerTime();
  return *m_MeanUrineOutput;
}
//-------------------------------------------------------------------------------
double SERenalSystem::GetMeanUrineOutput(const VolumePerTimeUnit& unit) const
{
  if (m_MeanUrineOutput == nullptr)
    return SEScalar::dNaN();
  return m_MeanUrineOutput->GetValue(unit);
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

Tree<const char*> SERenalSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*> { classname() }
    .emplace_back(idGlomerularFiltrationRate)
    .emplace_back(idFiltrationFraction)
    .emplace_back(idLeftAfferentArterioleResistance)
    .emplace_back(idLeftBowmansCapsulesHydrostaticPressure)
    .emplace_back(idLeftBowmansCapsulesOsmoticPressure)
    .emplace_back(idLeftEfferentArterioleResistance)
    .emplace_back(idLeftGlomerularCapillariesHydrostaticPressure)
    .emplace_back(idLeftGlomerularCapillariesOsmoticPressure)
    .emplace_back(idLeftGlomerularFiltrationCoefficient)
    .emplace_back(idLeftGlomerularFiltrationRate)
    .emplace_back(idLeftGlomerularFiltrationSurfaceArea)
    .emplace_back(idLeftGlomerularFluidPermeability)
    .emplace_back(idLeftFiltrationFraction)
    .emplace_back(idLeftNetFiltrationPressure)
    .emplace_back(idLeftNetReabsorptionPressure)
    .emplace_back(idLeftPeritubularCapillariesHydrostaticPressure)
    .emplace_back(idLeftPeritubularCapillariesOsmoticPressure)
    .emplace_back(idLeftReabsorptionFiltrationCoefficient)
    .emplace_back(idLeftReabsorptionRate)
    .emplace_back(idLeftTubularReabsorptionFiltrationSurfaceArea)
    .emplace_back(idLeftTubularReabsorptionFluidPermeability)
    .emplace_back(idLeftTubularHydrostaticPressure)
    .emplace_back(idLeftTubularOsmoticPressure)
    .emplace_back(idRenalBloodFlow)
    .emplace_back(idRenalPlasmaFlow)
    .emplace_back(idRenalVascularResistance)
    .emplace_back(idRightAfferentArterioleResistance)
    .emplace_back(idRightBowmansCapsulesHydrostaticPressure)
    .emplace_back(idRightBowmansCapsulesOsmoticPressure)
    .emplace_back(idRightEfferentArterioleResistance)
    .emplace_back(idRightGlomerularCapillariesHydrostaticPressure)
    .emplace_back(idRightGlomerularCapillariesOsmoticPressure)
    .emplace_back(idRightGlomerularFiltrationCoefficient)
    .emplace_back(idRightGlomerularFiltrationRate)
    .emplace_back(idRightGlomerularFiltrationSurfaceArea)
    .emplace_back(idRightGlomerularFluidPermeability)
    .emplace_back(idRightFiltrationFraction)
    .emplace_back(idRightNetFiltrationPressure)
    .emplace_back(idRightNetReabsorptionPressure)
    .emplace_back(idRightPeritubularCapillariesHydrostaticPressure)
    .emplace_back(idRightPeritubularCapillariesOsmoticPressure)
    .emplace_back(idRightReabsorptionFiltrationCoefficient)
    .emplace_back(idRightReabsorptionRate)
    .emplace_back(idRightTubularReabsorptionFiltrationSurfaceArea)
    .emplace_back(idRightTubularReabsorptionFluidPermeability)
    .emplace_back(idRightTubularHydrostaticPressure)
    .emplace_back(idRightTubularOsmoticPressure)
    .emplace_back(idUrinationRate)
    .emplace_back(idUrineOsmolality)
    .emplace_back(idUrineOsmolarity)
    .emplace_back(idUrineProductionRate)
    .emplace_back(idMeanUrineOutput)
    .emplace_back(idUrineSpecificGravity)
    .emplace_back(idUrineVolume)
    .emplace_back(idUrineUreaNitrogenConcentration);
}
//-------------------------------------------------------------------------------
bool SERenalSystem::operator==(SERenalSystem const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_BladderPressure && rhs.m_BladderPressure) ? m_BladderPressure->operator==(*rhs.m_BladderPressure) : m_BladderPressure == rhs.m_BladderPressure)
    && ((m_GlomerularFiltrationRate && rhs.m_GlomerularFiltrationRate) ? m_GlomerularFiltrationRate->operator==(*rhs.m_GlomerularFiltrationRate) : m_GlomerularFiltrationRate == rhs.m_GlomerularFiltrationRate)
    && ((m_FiltrationFraction && rhs.m_FiltrationFraction) ? m_FiltrationFraction->operator==(*rhs.m_FiltrationFraction) : m_FiltrationFraction == rhs.m_FiltrationFraction)

    && ((m_LeftAfferentArterioleResistance && rhs.m_LeftAfferentArterioleResistance) ? m_LeftAfferentArterioleResistance->operator==(*rhs.m_LeftAfferentArterioleResistance) : m_LeftAfferentArterioleResistance == rhs.m_LeftAfferentArterioleResistance)
    && ((m_LeftBowmansCapsulesHydrostaticPressure && rhs.m_LeftBowmansCapsulesHydrostaticPressure) ? m_LeftBowmansCapsulesHydrostaticPressure->operator==(*rhs.m_LeftBowmansCapsulesHydrostaticPressure) : m_LeftBowmansCapsulesHydrostaticPressure == rhs.m_LeftBowmansCapsulesHydrostaticPressure)
    && ((m_LeftBowmansCapsulesOsmoticPressure && rhs.m_LeftBowmansCapsulesOsmoticPressure) ? m_LeftBowmansCapsulesOsmoticPressure->operator==(*rhs.m_LeftBowmansCapsulesOsmoticPressure) : m_LeftBowmansCapsulesOsmoticPressure == rhs.m_LeftBowmansCapsulesOsmoticPressure)
    && ((m_LeftEfferentArterioleResistance && rhs.m_LeftEfferentArterioleResistance) ? m_LeftEfferentArterioleResistance->operator==(*rhs.m_LeftEfferentArterioleResistance) : m_LeftEfferentArterioleResistance == rhs.m_LeftEfferentArterioleResistance)
    && ((m_LeftGlomerularCapillariesHydrostaticPressure && rhs.m_LeftGlomerularCapillariesHydrostaticPressure) ? m_LeftGlomerularCapillariesHydrostaticPressure->operator==(*rhs.m_LeftGlomerularCapillariesHydrostaticPressure) : m_LeftGlomerularCapillariesHydrostaticPressure == rhs.m_LeftGlomerularCapillariesHydrostaticPressure)
    && ((m_LeftGlomerularCapillariesOsmoticPressure && rhs.m_LeftGlomerularCapillariesOsmoticPressure) ? m_LeftGlomerularCapillariesOsmoticPressure->operator==(*rhs.m_LeftGlomerularCapillariesOsmoticPressure) : m_LeftGlomerularCapillariesOsmoticPressure == rhs.m_LeftGlomerularCapillariesOsmoticPressure)
    && ((m_LeftGlomerularFiltrationCoefficient && rhs.m_LeftGlomerularFiltrationCoefficient) ? m_LeftGlomerularFiltrationCoefficient->operator==(*rhs.m_LeftGlomerularFiltrationCoefficient) : m_LeftGlomerularFiltrationCoefficient == rhs.m_LeftGlomerularFiltrationCoefficient)
    && ((m_LeftGlomerularFiltrationRate && rhs.m_LeftGlomerularFiltrationRate) ? m_LeftGlomerularFiltrationRate->operator==(*rhs.m_LeftGlomerularFiltrationRate) : m_LeftGlomerularFiltrationRate == rhs.m_LeftGlomerularFiltrationRate)
    && ((m_LeftGlomerularFiltrationSurfaceArea && rhs.m_LeftGlomerularFiltrationSurfaceArea) ? m_LeftGlomerularFiltrationSurfaceArea->operator==(*rhs.m_LeftGlomerularFiltrationSurfaceArea) : m_LeftGlomerularFiltrationSurfaceArea == rhs.m_LeftGlomerularFiltrationSurfaceArea)
    && ((m_LeftGlomerularFluidPermeability && rhs.m_LeftGlomerularFluidPermeability) ? m_LeftGlomerularFluidPermeability->operator==(*rhs.m_LeftGlomerularFluidPermeability) : m_LeftGlomerularFluidPermeability == rhs.m_LeftGlomerularFluidPermeability)
    && ((m_LeftFiltrationFraction && rhs.m_LeftFiltrationFraction) ? m_LeftFiltrationFraction->operator==(*rhs.m_LeftFiltrationFraction) : m_LeftFiltrationFraction == rhs.m_LeftFiltrationFraction)
    && ((m_LeftNetFiltrationPressure && rhs.m_LeftNetFiltrationPressure) ? m_LeftNetFiltrationPressure->operator==(*rhs.m_LeftNetFiltrationPressure) : m_LeftNetFiltrationPressure == rhs.m_LeftNetFiltrationPressure)
    && ((m_LeftNetReabsorptionPressure && rhs.m_LeftNetReabsorptionPressure) ? m_LeftNetReabsorptionPressure->operator==(*rhs.m_LeftNetReabsorptionPressure) : m_LeftNetReabsorptionPressure == rhs.m_LeftNetReabsorptionPressure)
    && ((m_LeftPeritubularCapillariesHydrostaticPressure && rhs.m_LeftPeritubularCapillariesHydrostaticPressure) ? m_LeftPeritubularCapillariesHydrostaticPressure->operator==(*rhs.m_LeftPeritubularCapillariesHydrostaticPressure) : m_LeftPeritubularCapillariesHydrostaticPressure == rhs.m_LeftPeritubularCapillariesHydrostaticPressure)
    && ((m_LeftPeritubularCapillariesOsmoticPressure && rhs.m_LeftPeritubularCapillariesOsmoticPressure) ? m_LeftPeritubularCapillariesOsmoticPressure->operator==(*rhs.m_LeftPeritubularCapillariesOsmoticPressure) : m_LeftPeritubularCapillariesOsmoticPressure == rhs.m_LeftPeritubularCapillariesOsmoticPressure)
    && ((m_LeftReabsorptionFiltrationCoefficient && rhs.m_LeftReabsorptionFiltrationCoefficient) ? m_LeftReabsorptionFiltrationCoefficient->operator==(*rhs.m_LeftReabsorptionFiltrationCoefficient) : m_LeftReabsorptionFiltrationCoefficient == rhs.m_LeftReabsorptionFiltrationCoefficient)
    && ((m_LeftReabsorptionRate && rhs.m_LeftReabsorptionRate) ? m_LeftReabsorptionRate->operator==(*rhs.m_LeftReabsorptionRate) : m_LeftReabsorptionRate == rhs.m_LeftReabsorptionRate)
    && ((m_LeftTubularReabsorptionFiltrationSurfaceArea && rhs.m_LeftTubularReabsorptionFiltrationSurfaceArea) ? m_LeftTubularReabsorptionFiltrationSurfaceArea->operator==(*rhs.m_LeftTubularReabsorptionFiltrationSurfaceArea) : m_LeftTubularReabsorptionFiltrationSurfaceArea == rhs.m_LeftTubularReabsorptionFiltrationSurfaceArea)
    && ((m_LeftTubularReabsorptionFluidPermeability && rhs.m_LeftTubularReabsorptionFluidPermeability) ? m_LeftTubularReabsorptionFluidPermeability->operator==(*rhs.m_LeftTubularReabsorptionFluidPermeability) : m_LeftTubularReabsorptionFluidPermeability == rhs.m_LeftTubularReabsorptionFluidPermeability)
    && ((m_LeftTubularHydrostaticPressure && rhs.m_LeftTubularHydrostaticPressure) ? m_LeftTubularHydrostaticPressure->operator==(*rhs.m_LeftTubularHydrostaticPressure) : m_LeftTubularHydrostaticPressure == rhs.m_LeftTubularHydrostaticPressure)
    && ((m_LeftTubularOsmoticPressure && rhs.m_LeftTubularOsmoticPressure) ? m_LeftTubularOsmoticPressure->operator==(*rhs.m_LeftTubularOsmoticPressure) : m_LeftTubularOsmoticPressure == rhs.m_LeftTubularOsmoticPressure)

    && ((m_RenalBloodFlow && rhs.m_RenalBloodFlow) ? m_RenalBloodFlow->operator==(*rhs.m_RenalBloodFlow) : m_RenalBloodFlow == rhs.m_RenalBloodFlow)
    && ((m_RenalPlasmaFlow && rhs.m_RenalPlasmaFlow) ? m_RenalPlasmaFlow->operator==(*rhs.m_RenalPlasmaFlow) : m_RenalPlasmaFlow == rhs.m_RenalPlasmaFlow)
    && ((m_RenalVascularResistance && rhs.m_RenalVascularResistance) ? m_RenalVascularResistance->operator==(*rhs.m_RenalVascularResistance) : m_RenalVascularResistance == rhs.m_RenalVascularResistance)

    && ((m_RightAfferentArterioleResistance && rhs.m_RightAfferentArterioleResistance) ? m_RightAfferentArterioleResistance->operator==(*rhs.m_RightAfferentArterioleResistance) : m_RightAfferentArterioleResistance == rhs.m_RightAfferentArterioleResistance)
    && ((m_RightBowmansCapsulesHydrostaticPressure && rhs.m_RightBowmansCapsulesHydrostaticPressure) ? m_RightBowmansCapsulesHydrostaticPressure->operator==(*rhs.m_RightBowmansCapsulesHydrostaticPressure) : m_RightBowmansCapsulesHydrostaticPressure == rhs.m_RightBowmansCapsulesHydrostaticPressure)
    && ((m_RightBowmansCapsulesOsmoticPressure && rhs.m_RightBowmansCapsulesOsmoticPressure) ? m_RightBowmansCapsulesOsmoticPressure->operator==(*rhs.m_RightBowmansCapsulesOsmoticPressure) : m_RightBowmansCapsulesOsmoticPressure == rhs.m_RightBowmansCapsulesOsmoticPressure)
    && ((m_RightEfferentArterioleResistance && rhs.m_RightEfferentArterioleResistance) ? m_RightEfferentArterioleResistance->operator==(*rhs.m_RightEfferentArterioleResistance) : m_RightEfferentArterioleResistance == rhs.m_RightEfferentArterioleResistance)
    && ((m_RightGlomerularCapillariesHydrostaticPressure && rhs.m_RightGlomerularCapillariesHydrostaticPressure) ? m_RightGlomerularCapillariesHydrostaticPressure->operator==(*rhs.m_RightGlomerularCapillariesHydrostaticPressure) : m_RightGlomerularCapillariesHydrostaticPressure == rhs.m_RightGlomerularCapillariesHydrostaticPressure)
    && ((m_RightGlomerularCapillariesOsmoticPressure && rhs.m_RightGlomerularCapillariesOsmoticPressure) ? m_RightGlomerularCapillariesOsmoticPressure->operator==(*rhs.m_RightGlomerularCapillariesOsmoticPressure) : m_RightGlomerularCapillariesOsmoticPressure == rhs.m_RightGlomerularCapillariesOsmoticPressure)
    && ((m_RightGlomerularFiltrationCoefficient && rhs.m_RightGlomerularFiltrationCoefficient) ? m_RightGlomerularFiltrationCoefficient->operator==(*rhs.m_RightGlomerularFiltrationCoefficient) : m_RightGlomerularFiltrationCoefficient == rhs.m_RightGlomerularFiltrationCoefficient)
    && ((m_RightGlomerularFiltrationRate && rhs.m_RightGlomerularFiltrationRate) ? m_RightGlomerularFiltrationRate->operator==(*rhs.m_RightGlomerularFiltrationRate) : m_RightGlomerularFiltrationRate == rhs.m_RightGlomerularFiltrationRate)
    && ((m_RightGlomerularFiltrationSurfaceArea && rhs.m_RightGlomerularFiltrationSurfaceArea) ? m_RightGlomerularFiltrationSurfaceArea->operator==(*rhs.m_RightGlomerularFiltrationSurfaceArea) : m_RightGlomerularFiltrationSurfaceArea == rhs.m_RightGlomerularFiltrationSurfaceArea)
    && ((m_RightGlomerularFluidPermeability && rhs.m_RightGlomerularFluidPermeability) ? m_RightGlomerularFluidPermeability->operator==(*rhs.m_RightGlomerularFluidPermeability) : m_RightGlomerularFluidPermeability == rhs.m_RightGlomerularFluidPermeability)
    && ((m_RightFiltrationFraction && rhs.m_RightFiltrationFraction) ? m_RightFiltrationFraction->operator==(*rhs.m_RightFiltrationFraction) : m_RightFiltrationFraction == rhs.m_RightFiltrationFraction)
    && ((m_RightNetFiltrationPressure && rhs.m_RightNetFiltrationPressure) ? m_RightNetFiltrationPressure->operator==(*rhs.m_RightNetFiltrationPressure) : m_RightNetFiltrationPressure == rhs.m_RightNetFiltrationPressure)
    && ((m_RightNetReabsorptionPressure && rhs.m_RightNetReabsorptionPressure) ? m_RightNetReabsorptionPressure->operator==(*rhs.m_RightNetReabsorptionPressure) : m_RightNetReabsorptionPressure == rhs.m_RightNetReabsorptionPressure)
    && ((m_RightPeritubularCapillariesHydrostaticPressure && rhs.m_RightPeritubularCapillariesHydrostaticPressure) ? m_RightPeritubularCapillariesHydrostaticPressure->operator==(*rhs.m_RightPeritubularCapillariesHydrostaticPressure) : m_RightPeritubularCapillariesHydrostaticPressure == rhs.m_RightPeritubularCapillariesHydrostaticPressure)
    && ((m_RightPeritubularCapillariesOsmoticPressure && rhs.m_RightPeritubularCapillariesOsmoticPressure) ? m_RightPeritubularCapillariesOsmoticPressure->operator==(*rhs.m_RightPeritubularCapillariesOsmoticPressure) : m_RightPeritubularCapillariesOsmoticPressure == rhs.m_RightPeritubularCapillariesOsmoticPressure)
    && ((m_RightReabsorptionFiltrationCoefficient && rhs.m_RightReabsorptionFiltrationCoefficient) ? m_RightReabsorptionFiltrationCoefficient->operator==(*rhs.m_RightReabsorptionFiltrationCoefficient) : m_RightReabsorptionFiltrationCoefficient == rhs.m_RightReabsorptionFiltrationCoefficient)
    && ((m_RightReabsorptionRate && rhs.m_RightReabsorptionRate) ? m_RightReabsorptionRate->operator==(*rhs.m_RightReabsorptionRate) : m_RightReabsorptionRate == rhs.m_RightReabsorptionRate)
    && ((m_RightTubularReabsorptionFiltrationSurfaceArea && rhs.m_RightTubularReabsorptionFiltrationSurfaceArea) ? m_RightTubularReabsorptionFiltrationSurfaceArea->operator==(*rhs.m_RightTubularReabsorptionFiltrationSurfaceArea) : m_RightTubularReabsorptionFiltrationSurfaceArea == rhs.m_RightTubularReabsorptionFiltrationSurfaceArea)
    && ((m_RightTubularReabsorptionFluidPermeability && rhs.m_RightTubularReabsorptionFluidPermeability) ? m_RightTubularReabsorptionFluidPermeability->operator==(*rhs.m_RightTubularReabsorptionFluidPermeability) : m_RightTubularReabsorptionFluidPermeability == rhs.m_RightTubularReabsorptionFluidPermeability)
    && ((m_RightTubularHydrostaticPressure && rhs.m_RightTubularHydrostaticPressure) ? m_RightTubularHydrostaticPressure->operator==(*rhs.m_RightTubularHydrostaticPressure) : m_RightTubularHydrostaticPressure == rhs.m_RightTubularHydrostaticPressure)
    && ((m_RightTubularOsmoticPressure && rhs.m_RightTubularOsmoticPressure) ? m_RightTubularOsmoticPressure->operator==(*rhs.m_RightTubularOsmoticPressure) : m_RightTubularOsmoticPressure == rhs.m_RightTubularOsmoticPressure)

    && ((m_UrinationRate && rhs.m_UrinationRate) ? m_UrinationRate->operator==(*rhs.m_UrinationRate) : m_UrinationRate == rhs.m_UrinationRate)
    && ((m_UrineOsmolality && rhs.m_UrineOsmolality) ? m_UrineOsmolality->operator==(*rhs.m_UrineOsmolality) : m_UrineOsmolality == rhs.m_UrineOsmolality)
    && ((m_UrineOsmolarity && rhs.m_UrineOsmolarity) ? m_UrineOsmolarity->operator==(*rhs.m_UrineOsmolarity) : m_UrineOsmolarity == rhs.m_UrineOsmolarity)
    && ((m_UrineProductionRate && rhs.m_UrineProductionRate) ? m_UrineProductionRate->operator==(*rhs.m_UrineProductionRate) : m_UrineProductionRate == rhs.m_UrineProductionRate)
    && ((m_MeanUrineOutput && rhs.m_MeanUrineOutput) ? m_MeanUrineOutput->operator==(*rhs.m_MeanUrineOutput) : m_MeanUrineOutput == rhs.m_MeanUrineOutput)
    && ((m_UrineSpecificGravity && rhs.m_UrineSpecificGravity) ? m_UrineSpecificGravity->operator==(*rhs.m_UrineSpecificGravity) : m_UrineSpecificGravity == rhs.m_UrineSpecificGravity)
    && ((m_UrineVolume && rhs.m_UrineVolume) ? m_UrineVolume->operator==(*rhs.m_UrineVolume) : m_UrineVolume == rhs.m_UrineVolume)
    && ((m_UrineUreaNitrogenConcentration && rhs.m_UrineUreaNitrogenConcentration) ? m_UrineUreaNitrogenConcentration->operator==(*rhs.m_UrineUreaNitrogenConcentration) : m_UrineUreaNitrogenConcentration == rhs.m_UrineUreaNitrogenConcentration);
}
bool SERenalSystem::operator!=(SERenalSystem const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}