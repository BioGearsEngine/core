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
#include <biogears/engine/Systems/Environment.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/utils/GeneralMath.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
auto Environment::make_unique(BioGears& bg) -> std::unique_ptr<Environment>
{
  return std::unique_ptr<Environment>(new Environment(bg));
}

Environment::Environment(BioGears& bg)
  : SEEnvironment(bg.GetSubstances())
  , m_data(bg)
{
  Clear();
}

Environment::~Environment()
{
  Clear();
}

void Environment::Clear()
{
  SEEnvironment::Clear();
  m_Patient = nullptr;
  m_PatientActions = nullptr;
  m_EnvironmentActions = nullptr;
  m_AmbientGases = nullptr;
  m_AmbientAerosols = nullptr;
  m_EnvironmentCircuit = nullptr;
  m_ThermalEnvironment = nullptr;
  m_SkinNode = nullptr;
  m_ClothingNode = nullptr;
  m_EnclosureNode = nullptr;
  m_SkinToClothing = nullptr;
  m_ActiveHeatTransferRatePath = nullptr;
  m_ActiveTemperaturePath = nullptr;
  m_ActiveSwitchPath = nullptr;
  m_ClothingToEnclosurePath = nullptr;
  m_GroundToEnclosurePath = nullptr;
  m_ClothingToEnvironmentPath = nullptr;
  m_GroundToEnvironmentPath = nullptr;
  m_EnvironmentSkinToGroundPath = nullptr;
  m_EnvironmentCoreToGroundPath = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Environment::Initialize()
{
  BioGearsSystem::Initialize();

  //Initialize all System Data outputs (inputs should be populated elsewhere)
  GetConvectiveHeatLoss().SetValue(0.0, PowerUnit::W);
  GetEvaporativeHeatLoss().SetValue(0.0, PowerUnit::W);
  GetRadiativeHeatLoss().SetValue(0.0, PowerUnit::W);
  GetRespirationHeatLoss().SetValue(0.0, PowerUnit::W);
  GetSkinHeatLoss().SetValue(0.0, PowerUnit::W);

  GetConvectiveHeatTranferCoefficient().SetValue(0.0, HeatConductancePerAreaUnit::W_Per_m2_K);
  GetEvaporativeHeatTranferCoefficient().SetValue(0.0, HeatConductancePerAreaUnit::W_Per_m2_K);
  GetRadiativeHeatTranferCoefficient().SetValue(0.0, HeatConductancePerAreaUnit::W_Per_m2_K);

  double patientDensity_g_Per_mL = m_Patient->GetBodyDensity(MassPerVolumeUnit::g_Per_mL);
  double patientMass_g = m_Patient->GetWeight(MassUnit::g);
  double patientHeight_m = m_Patient->GetHeight(LengthUnit::m);
  double pi = 3.14159;
  m_PatientEquivalentDiameter_m = std::pow(Convert(patientMass_g / patientDensity_g_Per_mL, VolumeUnit::mL, VolumeUnit::m3) / (pi * patientHeight_m), 0.5);
}

bool Environment::Load(const CDM::BioGearsEnvironmentData& in)
{
  if (!SEEnvironment::Load(in))
    return false;
  BioGearsSystem::LoadState();
  m_PatientEquivalentDiameter_m = in.PatientEquivalentDiameter_m();
  StateChange();
  return true;
}
CDM::BioGearsEnvironmentData* Environment::Unload() const
{
  CDM::BioGearsEnvironmentData* data = new CDM::BioGearsEnvironmentData();
  Unload(*data);
  return data;
}
void Environment::Unload(CDM::BioGearsEnvironmentData& data) const
{
  SEEnvironment::Unload(data);
  data.PatientEquivalentDiameter_m(m_PatientEquivalentDiameter_m);
}

void Environment::SetUp()
{
  // Patient and Actions
  m_Patient = &m_data.GetPatient();
  m_PatientActions = &m_data.GetActions().GetPatientActions();
  m_EnvironmentActions = &m_data.GetActions().GetEnvironmentActions();
  //Circuits
  m_EnvironmentCircuit = &m_data.GetCircuits().GetExternalTemperatureCircuit();
  //Compartments
  m_AmbientGases = m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  m_AmbientAerosols = m_data.GetCompartments().GetLiquidCompartment(BGE::EnvironmentCompartment::Ambient);
  //Nodes
  m_ThermalEnvironment = m_EnvironmentCircuit->GetNode(BGE::ExternalTemperatureNode::Ambient);
  m_SkinNode = m_EnvironmentCircuit->GetNode(BGE::ExternalTemperatureNode::ExternalSkin);
  m_ClothingNode = m_EnvironmentCircuit->GetNode(BGE::ExternalTemperatureNode::Clothing);
  m_EnclosureNode = m_EnvironmentCircuit->GetNode(BGE::ExternalTemperatureNode::Enclosure);
  //Paths
  m_SkinToClothing = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::ExternalSkinToClothing);
  m_ActiveHeatTransferRatePath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::GroundToClothing);
  m_ActiveTemperaturePath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::GroundToActive);
  m_ActiveSwitchPath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::ActiveToClothing);
  m_ClothingToEnclosurePath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::ClothingToEnclosure);
  m_GroundToEnclosurePath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::GroundToEnclosure);
  m_ClothingToEnvironmentPath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::ClothingToEnvironment);
  m_GroundToEnvironmentPath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::GroundToEnvironment);
  m_EnvironmentSkinToGroundPath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::ExternalSkinToGround);
  m_EnvironmentCoreToGroundPath = m_EnvironmentCircuit->GetPath(BGE::ExternalTemperaturePath::ExternalCoreToGround);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Resets environment parameters.
///
/// \details
/// This is called any time the environment change action/condition.  It sets the ambient node
/// values needed for the fluid systems.
//--------------------------------------------------------------------------------------------------
void Environment::StateChange()
{
  using namespace std::string_literals;
  if (m_AmbientGases == nullptr || m_AmbientAerosols == nullptr)
    return;

  // Add Gases to the environment
  //Check to make sure fractions sum to 1.0
  double totalFraction = 0.0;
  for (auto s : GetConditions().GetAmbientGases()) {
    SESubstance& sub = s->GetSubstance();
    totalFraction += s->GetFractionAmount().GetValue();
    m_data.GetSubstances().AddActiveSubstance(sub);
  }
  if (std::abs(1.0 - totalFraction) > 1e-6) //Allow for a little bit of numerical error
  {
    /// \error Fatal: Total ambient/environment gas volume fractions must sum to 1.0.
    std::stringstream ss;
    ss << "Total ambient/environment gas volume fractions must sum to 1.0. Current total fraction is " << totalFraction;
    Fatal(ss);
  }
  // Reset what we have
  for (SEGasSubstanceQuantity* subQ : m_AmbientGases->GetSubstanceQuantities())
    subQ->SetToZero();
  //Update the substance values on the Ambient Node based on the Action/File settings
  //We want to set an ambient volume fraction for all active gases
  for (SESubstanceFraction* subFrac : GetConditions().GetAmbientGases()) {
    SEGasSubstanceQuantity* subQ = m_AmbientGases->GetSubstanceQuantity(subFrac->GetSubstance());
    subQ->GetVolumeFraction().Set(subFrac->GetFractionAmount());
    //Set substance volumes to be infinite when compartment/node volume is also infinite
    subQ->GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  }
  m_AmbientGases->GetPressure().Set(GetConditions().GetAtmosphericPressure());
  m_AmbientGases->Balance(BalanceGasBy::VolumeFraction);

  // Add aerosols to the environment
  for (auto s : GetConditions().GetAmbientAerosols()) {
    SESubstance& sub = s->GetSubstance();
    if (!sub.HasAerosolization()) {
      Error("Ignoring environment aerosol as it does not have any aerosol data : "s + sub.GetName());
      continue;
    }
    m_data.GetSubstances().AddActiveSubstance(sub);
    SELiquidSubstanceQuantity* subQ = m_AmbientAerosols->GetSubstanceQuantity(sub);
    subQ->GetConcentration().Set(s->GetConcentration());
  }
}

void Environment::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
    if (m_data.GetConditions().HasInitialEnvironment())
      ProcessChange(*m_data.GetConditions().GetInitialEnvironment());
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess prepares the cardiovascular system for the circuit solver
///
/// \details
/// This function uses feedback to calculate thermal properties and circuit element values for the
/// next engine state.
//--------------------------------------------------------------------------------------------------
void Environment::PreProcess()
{
  if (m_EnvironmentActions->HasChange()) {
    ProcessChange(*m_EnvironmentActions->GetChange());
    m_EnvironmentActions->RemoveChange();
  }

  //Set clothing resistor
  const double dClothingResistance_rsi = GetConditions().GetClothingResistance(HeatResistanceAreaUnit::rsi); //1 rsi = 1 m^2-K/W
  const double dSurfaceArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);
  double skinToClothingResistance = std::max(dClothingResistance_rsi / dSurfaceArea_m2, m_data.GetConfiguration().GetDefaultClosedHeatResistance(HeatResistanceUnit::K_Per_W));

  m_SkinToClothing->GetNextResistance().SetValue(skinToClothingResistance, HeatResistanceUnit::K_Per_W);

  //Set the skin heat loss
  double dSkinHeatLoss_W = 0.0;
  if (m_SkinToClothing->HasHeatTransferRate()) {
    dSkinHeatLoss_W = m_SkinToClothing->GetHeatTransferRate().GetValue(PowerUnit::W);
  }

  GetSkinHeatLoss().SetValue(dSkinHeatLoss_W, PowerUnit::W);

  CalculateSupplementalValues();
  CalculateRadiation();
  CalculateConvection();
  CalculateEvaporation();
  CalculateRespiration();
  ProcessActions();

  //Record heat fluxes after we have processed all actions
  double dTotalHeatLoss_W = 0.0;
  if (m_ClothingToEnclosurePath->HasHeatTransferRate()) {
    dTotalHeatLoss_W = m_ClothingToEnclosurePath->GetHeatTransferRate().GetValue(PowerUnit::W);
  }
  GetRadiativeHeatLoss().SetValue(dTotalHeatLoss_W, PowerUnit::W);
  if (m_ClothingToEnvironmentPath->HasHeatTransferRate()) {
    dTotalHeatLoss_W = m_ClothingToEnvironmentPath->GetHeatTransferRate().GetValue(PowerUnit::W);
  }
  GetConvectiveHeatLoss().SetValue(dTotalHeatLoss_W, PowerUnit::W);
  if (m_EnvironmentSkinToGroundPath->HasHeatTransferRate()) {
    dTotalHeatLoss_W = m_EnvironmentSkinToGroundPath->GetHeatTransferRate().GetValue(PowerUnit::W);
  }
  GetEvaporativeHeatLoss().SetValue(dTotalHeatLoss_W, PowerUnit::W);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// There is nothing to do here.  Processing the combined Energy-Environment circuit is handled by
/// the Energy system.
//--------------------------------------------------------------------------------------------------
void Environment::Process()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// There is nothing to do here.  Postprocessing the combined Energy-Environment circuit is handled by
/// the Energy system.
//--------------------------------------------------------------------------------------------------
void Environment::PostProcess()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Apply environment specific actions.
///
/// \details
/// Handle active heating and/or cooling.  This will set the active heat flow source and/or the
/// active temperature source.  The applied area or fraction is used to determine the average
/// patient application, since we're using a one dimensional model.  They can be
/// called individually or will sum together if called in combination.  Note that the action manager
/// will handle removing the actions (i.e. will return false for has calls).
//--------------------------------------------------------------------------------------------------
void Environment::ProcessActions()
{
  //Begin by assuming nothing is active
  //Set the power source to zero
  m_ActiveHeatTransferRatePath->GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  //Set the temperature source to zero
  m_ActiveTemperaturePath->GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  //Open the switch
  m_ActiveSwitchPath->SetNextSwitch(CDM::enumOpenClosed::Open);

  //Check for actions that modify environment resistances
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Burn)) {
    //In the event of a burn, modify the skin to clothing, clothing to enclosure (radiation), and clothing to environment (convection)
    //paths based on the burn surface area fraction.  Reducing skin to clothing resistance assumes that burn patient will have large
    //surface area uncovered. We calculate change in evaporative resistance in CalcEvaporation since that involves adjustment of funtion level parameters
    const double burnSurfaceAreaFraction = m_data.GetActions().GetPatientActions().GetBurnWound()->GetTotalBodySurfaceArea().GetValue();

    //The "next" resistance are those just calculated in PreProcess, CalcRadiation, and CalcConvection -- these are essentially baselines
    // since they are calculated from the baseline clothing and environment variables.  We can't grab baseline resistances because
    //they are not always set depending on the environment.
    double nextSkinToClothingResistance = m_SkinToClothing->GetNextResistance(HeatResistanceUnit::K_Per_W);
    double nextRadiativeResistance = m_ClothingToEnclosurePath->GetNextResistance(HeatResistanceUnit::K_Per_W);
    double nextConvectiveResistance = m_ClothingToEnvironmentPath->GetNextResistance(HeatResistanceUnit::K_Per_W);

    //The "last" resistance are from the previous time step and are used to help ramp responses to final values
    const double lastSkinToClothingResistance = m_SkinToClothing->GetResistance(HeatResistanceUnit::K_Per_W);
    const double lastRadiativeResistance = m_ClothingToEnclosurePath->GetResistance(HeatResistanceUnit::K_Per_W);
    const double lastConvectiveResistance = m_ClothingToEnvironmentPath->GetResistance(HeatResistanceUnit::K_Per_W);

    //Target resistance are based on burn surface area (using "next" values since they were just set to basal values earlier in PreProcess)
    const double targetSkinToClothingResistance = nextSkinToClothingResistance * std::pow(1.0 - burnSurfaceAreaFraction, 4.0);
    const double targetRadiativeResistance = nextRadiativeResistance * std::pow(1.0 - burnSurfaceAreaFraction, 4.0);
    const double targetConvectiveResistance = nextConvectiveResistance * std::pow(1.0 - burnSurfaceAreaFraction, 4.0);

    const double rampGain = 1.0e-5; //Smooth the response so that we reach targets over several minutes

    nextSkinToClothingResistance = lastSkinToClothingResistance + rampGain * (targetSkinToClothingResistance - lastSkinToClothingResistance);
    nextRadiativeResistance = lastRadiativeResistance + rampGain * (targetRadiativeResistance - lastRadiativeResistance);
    nextConvectiveResistance = lastConvectiveResistance + rampGain * (targetConvectiveResistance - lastConvectiveResistance);

    m_SkinToClothing->GetNextResistance().SetValue(nextSkinToClothingResistance, HeatResistanceUnit::K_Per_W);
    m_ClothingToEnclosurePath->GetNextResistance().SetValue(nextRadiativeResistance, HeatResistanceUnit::K_Per_W);
    m_ClothingToEnvironmentPath->GetNextResistance().SetValue(nextConvectiveResistance, HeatResistanceUnit::K_Per_W);
  }

  if (!m_EnvironmentActions->HasThermalApplication()) {
    //No action
    return;
  }

  //We'll allow heating, cooling, and temperature setting to be done simultaneously

  SEThermalApplication* ta = m_EnvironmentActions->GetThermalApplication();
  double dEffectiveAreaFraction = 0.0;
  double dSurfaceArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);

  //Handle active heating and cooling
  //We'll allow heating and cooling to be done simultaneously by just summing the effects

  double dTotalEffect_W = 0.0;

  if (ta->HasActiveHeating()) {
    SEActiveHeating& ah = ta->GetActiveHeating();
    if (ah.HasSurfaceArea() && ah.HasSurfaceAreaFraction()) {
      ///\error Warning: SurfaceArea and SurfaceAreaFraction are both set. The largest fraction will be used.
      Warning("SurfaceArea and SurfaceAreaFraction are both set. The largest fraction will be used.");
      dEffectiveAreaFraction = std::max(ah.GetSurfaceArea(AreaUnit::m2) / dSurfaceArea_m2, ah.GetSurfaceAreaFraction().GetValue());
      ah.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
      ah.GetSurfaceArea().Invalidate();
    } else if (ah.HasSurfaceAreaFraction()) {
      dEffectiveAreaFraction = ah.GetSurfaceAreaFraction().GetValue();
    } else if (ah.HasSurfaceArea()) {
      dEffectiveAreaFraction = ah.GetSurfaceArea(AreaUnit::m2) / dSurfaceArea_m2;
    } else {
      ///\error Warning: Neither SurfaceArea nor SurfaceAreaFraction are set. A fraction of 1 will be used.
      Warning("Neither SurfaceArea nor SurfaceAreaFraction are set. A fraction of 1 will be used.");
      dEffectiveAreaFraction = 1.0;
      ah.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
    }

    if (dEffectiveAreaFraction > 1.0) {
      ///\error Warning: Thermal application effective area exceeds the total skin surface area. A fraction of 1 will be used.
      Warning("Thermal application effective area exceeds the total skin surface area. A fraction of 1 will be used.");
      dEffectiveAreaFraction = 1.0;
      ah.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
    }

    dTotalEffect_W += ah.GetPower(PowerUnit::W) * dEffectiveAreaFraction;
  }

  dEffectiveAreaFraction = 0.0;
  if (ta->HasActiveCooling()) {
    SEActiveCooling& ac = ta->GetActiveCooling();
    if (ac.HasSurfaceArea() && ac.HasSurfaceAreaFraction()) {
      ///\error Warning: SurfaceArea and SurfaceAreaFraction are both set. The largest fraction will be used.
      Warning("SurfaceArea and SurfaceAreaFraction are both set. The largest fraction will be used.");
      dEffectiveAreaFraction = std::max(ac.GetSurfaceArea(AreaUnit::m2) / dSurfaceArea_m2, ac.GetSurfaceAreaFraction().GetValue());
      ac.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
      ac.GetSurfaceArea().Invalidate();
    } else if (ac.HasSurfaceAreaFraction()) {
      dEffectiveAreaFraction = ac.GetSurfaceAreaFraction().GetValue();
    } else if (ac.HasSurfaceArea()) {
      dEffectiveAreaFraction = ac.GetSurfaceArea(AreaUnit::m2) / dSurfaceArea_m2;
    } else {
      ///\error Warning: Neither SurfaceArea nor SurfaceAreaFraction are set. A fraction of 1 will be used.
      Warning("Neither SurfaceArea nor SurfaceAreaFraction are set. A fraction of 1 will be used.");
      dEffectiveAreaFraction = 1.0;
      ac.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
    }

    if (dEffectiveAreaFraction > 1.0) {
      ///\error Warning: Thermal application effective area exceeds the total skin surface area. A fraction of 1 will be used.
      Warning("Thermal application effective area exceeds the total skin surface area. A fraction of 1 will be used.");
      dEffectiveAreaFraction = 1.0;
      ac.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
    }

    dTotalEffect_W -= ac.GetPower(PowerUnit::W) * dEffectiveAreaFraction;
  }

  //Set the the power source
  m_ActiveHeatTransferRatePath->GetNextHeatSource().SetValue(dTotalEffect_W, PowerUnit::W);

  //Handle active temperature

  dEffectiveAreaFraction = 0.0;
  if (ta->HasAppliedTemperature()) {
    SEAppliedTemperature& ap = ta->GetAppliedTemperature();
    if (ap.HasSurfaceArea() && ap.HasSurfaceAreaFraction()) {
      ///\error Warning: AppliedSurfaceArea and AppliedSurfaceAreaFraction are both set. The largest fraction will be used.
      Warning("AppliedSurfaceArea and AppliedSurfaceAreaFraction are both set. The largest fraction will be used.");
      dEffectiveAreaFraction = std::max(ap.GetSurfaceArea(AreaUnit::m2) / dSurfaceArea_m2, ap.GetSurfaceAreaFraction().GetValue());
      ap.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
      ap.GetSurfaceArea().Invalidate();
    } else if (ap.HasSurfaceAreaFraction()) {
      dEffectiveAreaFraction = ap.GetSurfaceAreaFraction().GetValue();
    } else if (ap.HasSurfaceArea()) {
      dEffectiveAreaFraction = ap.GetSurfaceArea().GetValue(AreaUnit::m2) / dSurfaceArea_m2;
    } else {
      ///\error Warning: Neither AppliedSurfaceArea nor AppliedSurfaceAreaFraction are set. A fraction of 1 will be used.
      Warning("Neither AppliedSurfaceArea nor AppliedSurfaceAreaFraction are set. A fraction of 1 will be used.");
      dEffectiveAreaFraction = 1.0;
      ap.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
    }

    if (dEffectiveAreaFraction > 1.0) {
      ///\error Warning: Thermal application effective area exceeds the total skin surface area. A fraction of 1 will be used.
      Warning("Thermal application effective area exceeds the total skin surface area. A fraction of 1 will be used.");
      dEffectiveAreaFraction = 1.0;
      ap.GetSurfaceAreaFraction().SetValue(dEffectiveAreaFraction);
    }

    //Average the active temperature with ambient temperature by using the fraction of the body covered
    double dAmbientTemperature_K = GetConditions().GetAmbientTemperature(TemperatureUnit::K);
    double dAppliedTemperature_K = ap.GetTemperature(TemperatureUnit::K);
    dAppliedTemperature_K = dAppliedTemperature_K * dEffectiveAreaFraction + dAmbientTemperature_K * (1.0 - dEffectiveAreaFraction);

    //Set the temperature source
    m_ActiveTemperaturePath->GetNextTemperatureSource().SetValue(dAppliedTemperature_K, TemperatureUnit::K);

    //Close the switch
    m_ActiveSwitchPath->SetNextSwitch(CDM::enumOpenClosed::Closed);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate intermediate values to be used later.
///
/// \details
/// These computed values will be used in the other feedback methods.
//--------------------------------------------------------------------------------------------------
void Environment::CalculateSupplementalValues()
{
  //Use the Antoine Equation for to determine water vapor pressures
  //It's a piecewise function
  //Ambient Air
  double dAirTemperature_C = 0.0;
  if (!m_ThermalEnvironment->HasTemperature()) {
    dAirTemperature_C = GetConditions().GetAmbientTemperature(TemperatureUnit::C);
  } else {
    dAirTemperature_C = m_ThermalEnvironment->GetTemperature().GetValue(TemperatureUnit::C);
  }

  double dWaterVaporPressureInAmbientAir_mmHg = GeneralMath::AntoineEquation(dAirTemperature_C);
  m_dWaterVaporPressureInAmbientAir_Pa = Convert(dWaterVaporPressureInAmbientAir_mmHg, PressureUnit::mmHg, PressureUnit::Pa);
  //Skin
  double dSkinTemperature_C = m_SkinNode->GetTemperature().GetValue(TemperatureUnit::C);
  double dWaterVaporPressureAtSkin_mmHg = GeneralMath::AntoineEquation(dSkinTemperature_C);
  m_dWaterVaporPressureAtSkin_Pa = Convert(dWaterVaporPressureAtSkin_mmHg, PressureUnit::mmHg, PressureUnit::Pa);

  //Now use that to solve for the density of air
  double dUniversalGasConstant_JPerK_Mol = m_data.GetConfiguration().GetUniversalGasConstant(HeatCapacitancePerAmountUnit::J_Per_K_mol);
  double dMolarMassOfDryAir_KgPerMol = m_data.GetConfiguration().GetMolarMassOfDryAir(MassPerAmountUnit::kg_Per_mol);
  double dMolarMassOfWaterVapor_KgPerMol = m_data.GetConfiguration().GetMolarMassOfWaterVapor(MassPerAmountUnit::kg_Per_mol);

  double dAmbientTemperature_K = GetConditions().GetAmbientTemperature().GetValue(TemperatureUnit::K);

  double dPressureOfWaterVapor_Pa = GetConditions().GetRelativeHumidity().GetValue() * m_dWaterVaporPressureInAmbientAir_Pa;
  double dPartialPressureOfDryAir_Pa = GetConditions().GetAtmosphericPressure().GetValue(PressureUnit::Pa) - dPressureOfWaterVapor_Pa;

  double dAirDensity_kgPerm3 = (dPartialPressureOfDryAir_Pa * dMolarMassOfDryAir_KgPerMol + dPressureOfWaterVapor_Pa * dMolarMassOfWaterVapor_KgPerMol) / (dUniversalGasConstant_JPerK_Mol * dAmbientTemperature_K);
  GetConditions().GetAirDensity().SetValue(dAirDensity_kgPerm3, MassPerVolumeUnit::kg_Per_m3);

  //Now use that to determine the Lewis Relation
  double dAirSpecificHeat_JPerK_kg = m_data.GetConfiguration().GetAirSpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg);
  m_dLewisRelation = 1.0 / (dAirDensity_kgPerm3 * dAirSpecificHeat_JPerK_kg);

  //Determine the heat vaporization of water
  //This equation was determined using a best fit from experimental data
  double dTemperature_K = Convert(dAirTemperature_C, TemperatureUnit::C, TemperatureUnit::K);
  double dHeatOfVaporizationOfWater_JPerMol = -0.1004 * std::pow(dTemperature_K, 2) + 22.173 * dTemperature_K + 46375.0;
  //Convert moles of water to kg
  m_dHeatOfVaporizationOfWater_J_Per_kg = dHeatOfVaporizationOfWater_JPerMol / 0.0180153; //1 mol of water = 0.0180153 kg

  //Water convective heat transfer properties
  if (GetConditions().GetSurroundingType() == CDM::enumSurroundingType::Water) {
    double dWaterTemperature_C = GetConditions().GetAmbientTemperature(TemperatureUnit::C);
    double dT = Convert(dWaterTemperature_C, TemperatureUnit::C, TemperatureUnit::K) / 298.15;

    m_WaterSpecificHeat_J_Per_kg_K = 0.001 * ((-1.0E-7) * std::pow(dWaterTemperature_C, 3.0) + (3.0E-5) * std::pow(dWaterTemperature_C, 2.0) - 0.0018 * dWaterTemperature_C + 4.2093);
    m_WaterViscosity_N_s_Per_m2 = 0.001 * ((-3.0E-6) * std::pow(dWaterTemperature_C, 3.0) + 0.0006 * std::pow(dWaterTemperature_C, 2.0) - 0.0462 * dWaterTemperature_C + 1.7412);
    m_WaterThermalConductivity_W_Per_m_K = 0.6065 * (-1.48446 + 4.12292 * dT + -1.63866 * std::pow(dT, 2));
    m_ThermalExpansion_Per_K = (6.0E-7) * std::pow(dWaterTemperature_C, 3.0) - 0.0001 * std::pow(dWaterTemperature_C, 2.0) + 0.016 * dWaterTemperature_C - 0.0632;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the effects of radiation.
///
/// \details
/// This determines the circuit element values and system data outputs associated with radiation
/// heat transfer based on feedback.
//--------------------------------------------------------------------------------------------------
void Environment::CalculateRadiation()
{
  if (GetConditions().GetSurroundingType() == CDM::enumSurroundingType::Water) {
    //Submerged - therefore, no radiation

    //Invalidate the coefficient
    GetRadiativeHeatTranferCoefficient().Invalidate();

    //Set the resistance
    double dOpenSwitchResistance_KPerW = m_data.GetConfiguration().GetDefaultOpenHeatResistance(HeatResistanceUnit::K_Per_W);
    m_ClothingToEnclosurePath->GetNextResistance().SetValue(dOpenSwitchResistance_KPerW, HeatResistanceUnit::K_Per_W);

    //Set the source
    m_GroundToEnclosurePath->GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  } else //Air
  {
    //Calculate the coefficient
    double dEmissivity = GetConditions().GetEmissivity().GetValue();

    double dStefanBoltzmann_WPerM2_K4 = m_data.GetConfiguration().GetStefanBoltzmann(PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4);
    double dEffectiveAreaOverSurfaceArea = 0.73; //Standing
    double dClothingTemperature_K = m_ClothingNode->GetTemperature().GetValue(TemperatureUnit::K);
    double dMeanRadiantTemperature_K = m_EnclosureNode->GetTemperature().GetValue(TemperatureUnit::K);
    double dRadiativeHeatTransferCoefficient_WPerM2_K = std::abs(4.0 * dEmissivity * dStefanBoltzmann_WPerM2_K4 * dEffectiveAreaOverSurfaceArea * std::pow((dClothingTemperature_K + dMeanRadiantTemperature_K) / 2.0, 3));
    GetRadiativeHeatTranferCoefficient().SetValue(dRadiativeHeatTransferCoefficient_WPerM2_K, HeatConductancePerAreaUnit::W_Per_m2_K);

    //Calculate the resistance
    double dSurfaceArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);
    double dResistance_K_Per_W = 0.0;
    if (dRadiativeHeatTransferCoefficient_WPerM2_K == 0) {
      //Infinite resistance
      dResistance_K_Per_W = m_data.GetConfiguration().GetDefaultOpenHeatResistance(HeatResistanceUnit::K_Per_W);
    } else {
      dResistance_K_Per_W = dSurfaceArea_m2 / dRadiativeHeatTransferCoefficient_WPerM2_K;
    }

    auto max_result = std::max(dResistance_K_Per_W, m_data.GetConfiguration().GetDefaultClosedHeatResistance(HeatResistanceUnit::K_Per_W));
    m_ClothingToEnclosurePath->GetNextResistance().SetValue(max_result, HeatResistanceUnit::K_Per_W);

    //Set the source
    dMeanRadiantTemperature_K = GetConditions().GetMeanRadiantTemperature(TemperatureUnit::K);
    m_GroundToEnclosurePath->GetNextTemperatureSource().SetValue(dMeanRadiantTemperature_K, TemperatureUnit::K);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the effects of convection.
///
/// \details
/// This determines the circuit element values and system data outputs associated with convection
/// heat transfer based on feedback.
//--------------------------------------------------------------------------------------------------
void Environment::CalculateConvection()
{
  double dConvectiveHeatTransferCoefficient_WPerM2_K = 0.0;

  if (GetConditions().GetSurroundingType() == CDM::enumSurroundingType::Water) {
    //Submerged - therefore, convection is most important
    double dClothingTemperature_K = m_ClothingNode->GetTemperature().GetValue(TemperatureUnit::K);
    double dWaterTemperature_K = GetConditions().GetAmbientTemperature(TemperatureUnit::K);
    SEScalarMassPerVolume dWaterDensity;
    GeneralMath::CalculateWaterDensity(GetConditions().GetAmbientTemperature(), dWaterDensity);
    double dGravity_m_Per_s2 = 9.81;

    //Calculate the coefficient
    //Heat transfer coefficient for submerged water convection. C. Boutelier et al. Experimental study of convective heat transfer coefficient for the human body in water. Journal of Applied Physiology. 1977. Vol. 42. p.93-100
    double dGrashofNumber = dGravity_m_Per_s2 * m_ThermalExpansion_Per_K * (std::abs(dClothingTemperature_K - dWaterTemperature_K)) * std::pow(m_PatientEquivalentDiameter_m, 3.0) / (m_WaterViscosity_N_s_Per_m2 / dWaterDensity.GetValue(MassPerVolumeUnit::kg_Per_m3));
    double dPrandtlNumber = m_WaterSpecificHeat_J_Per_kg_K * m_WaterViscosity_N_s_Per_m2 / m_WaterThermalConductivity_W_Per_m_K;
    dConvectiveHeatTransferCoefficient_WPerM2_K = 0.09 * (dGrashofNumber - dPrandtlNumber) * 0.275;
  } else //Air
  {
    //Calculate the coefficient
    //Velocity should take into account wind and patient movement combined
    double dAirVelocity_MPerS = GetConditions().GetAirVelocity(LengthPerTimeUnit::m_Per_s);
    dConvectiveHeatTransferCoefficient_WPerM2_K = 10.3 * std::pow(dAirVelocity_MPerS, 0.6);
    if (dAirVelocity_MPerS < 0.2) {
      //Equation above is valid between 0.2 and 0.8 m/s air speed.  Assume value of coefficient = f(0.2) @ air speed < 0.2
      dConvectiveHeatTransferCoefficient_WPerM2_K = 10.3 * std::pow(0.2, 0.6);
    }
  }

  //Set the coefficient
  GetConvectiveHeatTranferCoefficient().SetValue(dConvectiveHeatTransferCoefficient_WPerM2_K, HeatConductancePerAreaUnit::W_Per_m2_K);
  //Calculate the resistance
  double dSurfaceArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);
  double dResistance_K_Per_W = 0.0;
  if (dConvectiveHeatTransferCoefficient_WPerM2_K == 0) {
    //Infinite resistance
    dResistance_K_Per_W = m_data.GetConfiguration().GetDefaultOpenHeatResistance(HeatResistanceUnit::K_Per_W);
  } else {
    dResistance_K_Per_W = dSurfaceArea_m2 / dConvectiveHeatTransferCoefficient_WPerM2_K;
  }

  auto max_result = std::max(dResistance_K_Per_W, m_data.GetConfiguration().GetDefaultClosedHeatResistance(HeatResistanceUnit::K_Per_W));
  m_ClothingToEnvironmentPath->GetNextResistance().SetValue(max_result, HeatResistanceUnit::K_Per_W);

  //Set the source
  double dAmbientTemperature_K = GetConditions().GetAmbientTemperature(TemperatureUnit::K);
  m_GroundToEnvironmentPath->GetNextTemperatureSource().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the effects of evaporation.
///
/// \details
/// This determines the circuit element values and system data outputs associated with evaporation
/// heat transfer based on feedback.
//--------------------------------------------------------------------------------------------------
void Environment::CalculateEvaporation()
{
  if (GetConditions().GetSurroundingType() == CDM::enumSurroundingType::Water) {
    //Submerged - therefore, no evaporation

    //Invalidate the coefficient
    GetEvaporativeHeatTranferCoefficient().Invalidate();

    //Set the source
    m_EnvironmentSkinToGroundPath->GetNextHeatSource().SetValue(0.0, PowerUnit::W);

    //Set the total heat lost
    GetEvaporativeHeatLoss().SetValue(0.0, PowerUnit::W);
  } else //Air
  {
    //Calculate the coefficient
    double dConvectiveTransferCoefficient_W_Per_m2_K = GetConvectiveHeatTranferCoefficient(HeatConductancePerAreaUnit::W_Per_m2_K);
    const double dLewisRelation_K_Per_kPa = 16.5;
    const double dEvaporativeHeatTransferCoefficient_W_Per_m2_kPa = dConvectiveTransferCoefficient_W_Per_m2_K * dLewisRelation_K_Per_kPa;
    const double dClothingResistance_clo = GetConditions().GetClothingResistance(HeatResistanceAreaUnit::clo);
    const double clo_To_m2_K_Per_W = 0.155;
    const double iCl = 0.35;
    double dClothingResistance_m2_kPa_Per_W = clo_To_m2_K_Per_W * dClothingResistance_clo / (iCl * dLewisRelation_K_Per_kPa);
    double fCl = 1.0 + 0.3 * dClothingResistance_clo;
    double skinWettednessDiffusion = 0.06;
    if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Burn)) {
      skinWettednessDiffusion = m_data.GetActions().GetPatientActions().GetBurnWound()->GetTotalBodySurfaceArea().GetValue();
      dClothingResistance_m2_kPa_Per_W = 0.0;
      fCl = 1.0;
    }

	///\ToDo:  The units of this constant are incorrect on the CDM--they should be W_Per_m2_Pa (no support for this unit currently)
    GetEvaporativeHeatTranferCoefficient().SetValue(dEvaporativeHeatTransferCoefficient_W_Per_m2_kPa, HeatConductancePerAreaUnit::W_Per_m2_K);

    double dMaxEvaporativePotential = (1.0 / 1000.0) * (m_dWaterVaporPressureAtSkin_Pa - m_dWaterVaporPressureInAmbientAir_Pa) / (dClothingResistance_m2_kPa_Per_W + 1.0 / (fCl * dEvaporativeHeatTransferCoefficient_W_Per_m2_kPa));
    double dSurfaceArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);

    double dSweatRate_kgPers = 0.0;
    if (m_data.GetEnergy().HasSweatRate()) {
      dSweatRate_kgPers = m_data.GetEnergy().GetSweatRate(MassPerTimeUnit::kg_Per_s) / dSurfaceArea_m2;
    }
    double dSweatingControlMechanisms = dSweatRate_kgPers * m_dHeatOfVaporizationOfWater_J_Per_kg;
    double dWettedPortion = 0.0;
    if (dMaxEvaporativePotential != 0) {
      dWettedPortion = dSweatingControlMechanisms / dMaxEvaporativePotential;
    }
    double dDiffusionOfWater = (1.0 - dWettedPortion) * skinWettednessDiffusion * dMaxEvaporativePotential;
    double EvaporativeHeatLossFromSkin_W = dSweatingControlMechanisms + dDiffusionOfWater;

    //Set the source
    m_EnvironmentSkinToGroundPath->GetNextHeatSource().SetValue(dSurfaceArea_m2 * EvaporativeHeatLossFromSkin_W, PowerUnit::W);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the effects of respiration.
///
/// \details
/// This determines the circuit element values and system data outputs associated with respiration
/// heat transfer based on feedback.
//--------------------------------------------------------------------------------------------------
void Environment::CalculateRespiration()
{
  //Calculate the heat loss due to breathing
  //This is the same for submerged - assume head stays above water - would be zero anyway, if holding breath (Ventilation Rate = 0)

  //Convection
  double dTempOfRespAir_K = GetConditions().GetRespirationAmbientTemperature(TemperatureUnit::K);
  double dTempOfRespTract_K = 310.15; // = 37C = 98.6F
  if (m_data.GetEnergy().HasCoreTemperature()) {
    dTempOfRespTract_K = m_data.GetEnergy().GetCoreTemperature(TemperatureUnit::K);
  }
  double dPulmonaryVentilationRate_M3PerS = m_data.GetRespiratory().GetTotalPulmonaryVentilation(VolumePerTimeUnit::m3_Per_s);
  double dAirDensity_kgPerM3 = GetConditions().GetAirDensity(MassPerVolumeUnit::kg_Per_m3);
  double dAirSpecificHeat_JPerK_kg = m_data.GetConfiguration().GetAirSpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg);
  double dSensibleHeatLoss_W = dPulmonaryVentilationRate_M3PerS * dAirDensity_kgPerM3 * dAirSpecificHeat_JPerK_kg * (dTempOfRespTract_K - dTempOfRespAir_K);

  //Evaporation
  double dTempOfRespAir_F = GetConditions().GetRespirationAmbientTemperature(TemperatureUnit::F);
  double dRelativeHumidity = GetConditions().GetRelativeHumidity().GetValue();
  double dPressure_Pa = GetConditions().GetAtmosphericPressure(PressureUnit::Pa);
  double dSpecificHumidity = (dRelativeHumidity * 100.0) / (0.263 * dPressure_Pa) * (std::exp(17.67 * (dTempOfRespAir_K - 273.16) / (dTempOfRespAir_K - 29.65)));
  double dHumidityDiff = 0.02645 + 0.0000361 * dTempOfRespAir_F - 0.798 * dSpecificHumidity;
  double dLatentHeatLoss_W = m_dHeatOfVaporizationOfWater_J_Per_kg * dPulmonaryVentilationRate_M3PerS * dAirDensity_kgPerM3 * dHumidityDiff;

  //Total
  double dTotalHeatLoss_W = dSensibleHeatLoss_W + dLatentHeatLoss_W;

  //Set the source
  m_EnvironmentCoreToGroundPath->GetNextHeatSource().SetValue(dTotalHeatLoss_W, PowerUnit::W);

  //Set the total heat lost
  GetRespirationHeatLoss().SetValue(dTotalHeatLoss_W, PowerUnit::W);
}
}