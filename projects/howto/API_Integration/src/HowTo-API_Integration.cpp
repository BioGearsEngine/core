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

#include "HowTo-API_Integration.h"

#include <atomic>
#include <memory>
#include <thread>
// Include the various types you will be using in your code

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SEUrinate.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/schema/cdm/PatientAssessments.hxx>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


#include <biogears/string/manipulation.h>
//!
//! All action functions take in an entine and a verying number of params based on the SEAction type being applied
//!
//! For threading control it is better to move the engine to the function and return it on the other side to simulate ownership
//! passing.  BioGears is single threaded and so no two threads can operate on the same engine.
//!
//! Current signiture of foo(unique_ptr<BioGearsEngine>&) -> bool allows an easy error check for if all data was relevant
//! but does not make any promises that the ptr to engine isn't copied or used outside

//!
//!  Applies an SEHemmorhage to the given engine.
//!
//!  SEHemorrhage has two initial values needed for this function
//!  std::string compartment -- Where the tourniquet will be applied [LeftLeg,RightLeg,LeftArm,RightArm]
//!  double -- Initial flow rate we assume its given in ml_Per_min
bool action_apply_hemorrhage(std::unique_ptr<biogears::BioGearsEngine>& engine,
                             std::string compartment, double initial_rate_ml_Per_min)
{
  auto hemorrhage = biogears::SEHemorrhage();
  hemorrhage.SetCompartment(compartment);
  hemorrhage.GetInitialRate().SetValue(10., biogears::VolumePerTimeUnit::mL_Per_min);
  if (hemorrhage.IsValid()) {
    engine->ProcessAction(hemorrhage);
    return true;
  } else {
    return false;
  }
}
//!
//!  Applies an SETourniquet to the given engine.
//!
//!  SETourniquet has two members
//!  std::string compartment -- Where the tourniquet will be applied [LeftLeg,RightLeg,LeftArm,RightArm]
//!  CDM::enumTourniquetApplicationLevel -- Applied, Misapplied, None
bool action_apply_tourniquet(std::unique_ptr<biogears::BioGearsEngine>& engine,
                             std::string compartment, CDM::enumTourniquetApplicationLevel application)
{
  auto tourniquet = biogears::SETourniquet();
  tourniquet.SetCompartment(compartment);
  tourniquet.SetTourniquetLevel(application);

  if (tourniquet.IsValid()) {
    engine->ProcessAction(tourniquet);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!  Applies an environment change to a running BioGearsEngine
//!
//!  SEEnvironmentalConditions has several members.
//!
bool action_env_change(std::unique_ptr<biogears::BioGearsEngine>& engine, biogears::SEEnvironmentalConditions& new_conditions)
{
  auto environment = biogears::SEEnvironmentChange(engine->GetSubstanceManager());

  //This example only uses three partial gasses. You could submit a vector of name/value pairs.
  //Then this section could be modified to Get every substance in the pair list.
  //
  auto* N2 = engine->GetSubstanceManager().GetSubstance("Nitrogen");
  auto* O2 = engine->GetSubstanceManager().GetSubstance("Oxygen");
  auto* CO2 = engine->GetSubstanceManager().GetSubstance("CarbonDioxide");

  auto conditions = environment.GetConditions();
  conditions.Clear(); //Reset he existing conditions
  conditions.SetSurroundingType(new_conditions.GetSurroundingType());
  conditions.GetAirVelocity().SetValue(new_conditions.GetAirVelocity().GetValue(biogears::LengthPerTimeUnit::m_Per_s), biogears::LengthPerTimeUnit::m_Per_s);
  conditions.GetAmbientTemperature().SetValue(new_conditions.GetAmbientTemperature().GetValue(biogears::TemperatureUnit::C), biogears::TemperatureUnit::C);
  conditions.GetAtmosphericPressure().SetValue(new_conditions.GetAtmosphericPressure().GetValue(biogears::PressureUnit::mmHg), biogears::PressureUnit::mmHg);
  conditions.GetClothingResistance().SetValue(new_conditions.GetClothingResistance().GetValue(biogears::HeatResistanceAreaUnit::clo), biogears::HeatResistanceAreaUnit::clo);
  conditions.GetEmissivity().SetValue(new_conditions.GetEmissivity().GetValue());
  conditions.GetMeanRadiantTemperature().SetValue(new_conditions.GetMeanRadiantTemperature().GetValue(biogears::TemperatureUnit::C), biogears::TemperatureUnit::C);
  conditions.GetRelativeHumidity().SetValue(new_conditions.GetRelativeHumidity().GetValue());
  conditions.GetRespirationAmbientTemperature().SetValue(new_conditions.GetRespirationAmbientTemperature().GetValue(biogears::TemperatureUnit::C), biogears::TemperatureUnit::C);

  conditions.GetAmbientGas(*N2).GetFractionAmount().SetValue(new_conditions.GetAmbientGas(*N2).GetFractionAmount().GetValue());
  conditions.GetAmbientGas(*O2).GetFractionAmount().SetValue(new_conditions.GetAmbientGas(*O2).GetFractionAmount().GetValue());
  conditions.GetAmbientGas(*CO2).GetFractionAmount().SetValue(new_conditions.GetAmbientGas(*CO2).GetFractionAmount().GetValue());

  if (environment.IsValid()) {
    engine->ProcessAction(environment);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!
//! Applies a Tension Pneumothorax
//!
//! \param enumSide Left, Right
//! \param enumPneumothoraxType  Open, Closed
//! \param double [0.0, 1.0]
bool action_tension_pneumothorax(std::unique_ptr<biogears::BioGearsEngine>& engine, CDM::enumSide side, CDM::enumPneumothoraxType type, double severity)
{
  auto pneumothorax = biogears::SETensionPneumothorax();
  pneumothorax.SetSide(side);
  pneumothorax.SetType(type);
  pneumothorax.GetSeverity().SetValue(severity);
  if (pneumothorax.IsValid()) {
    engine->ProcessAction(pneumothorax);
    return true;
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------
//!  Applies an needle decompression to the patient
//!
//!  SENeedleDecompression has several members.
//! \param enumSide Left, Right
//! \param bool  Active/Not-Active  {On/Off}

bool action_needle_decompression(std::unique_ptr<biogears::BioGearsEngine>& engine, CDM::enumSide side, bool active)
{
  auto needleD = biogears::SENeedleDecompression();
  needleD.SetSide(side);
  needleD.SetActive(active);
  if (needleD.IsValid()) {
    engine->ProcessAction(needleD);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!  Applies an O2 Mask for the patient
//!
//!  O2 Mask is part of the Biogears Anesthesia Machine
//!  We first supply a Anesthesia Machine configuration then activate the mask
//!
//! \param o2_fraction -- Percentage of pure O^2 supplied by the mask
//! \param tank 1 volume -- Assumed in letters
//! \param tank 2 volume -- Assumed in letters
bool action_o2_mask(std::unique_ptr<biogears::BioGearsEngine>& engine, double o2_fraction, double o2_volume1, double o2_volume2)
{

  auto AMConfig = biogears::SEAnesthesiaMachineConfiguration(engine->GetSubstanceManager());
  auto& config = AMConfig.GetConfiguration();
  config.GetOxygenFraction().SetValue(o2_fraction);
  config.GetOxygenBottleOne().GetVolume().SetValue(o2_volume1, biogears::VolumeUnit::L);
  config.GetOxygenBottleTwo().GetVolume().SetValue(o2_volume2, biogears::VolumeUnit::L);

  //Any of these values could auso be adjusted, but I don't think its required
  //for this example
  config.SetConnection(CDM::enumAnesthesiaMachineConnection::Mask);
  config.GetInletFlow().SetValue(2.0, biogears::VolumePerTimeUnit::L_Per_min);
  config.GetInspiratoryExpiratoryRatio().SetValue(.5);
  config.SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::Wall);
  config.GetPositiveEndExpiredPressure().SetValue(0.0, biogears::PressureUnit::cmH2O);
  config.SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen);
  config.GetReliefValvePressure().SetValue(20.0, biogears::PressureUnit::cmH2O);
  config.GetRespiratoryRate().SetValue(12, biogears::FrequencyUnit::Per_min);
  config.GetVentilatorPressure().SetValue(0.0, biogears::PressureUnit::cmH2O);

  if (AMConfig.IsValid()) {
    engine->ProcessAction(AMConfig);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!  Applies an infection to the patient
//!
//!  Untreated Infections lead to Septic conditions, Antibotics do affect
//!  the infection based on the mic_mg_Per_l given for an individual infection
//!
//! \param severity  -- Eliminated, Mild, Moderate, Severe
//! \param location -- Compartmant Key on where the Infection Starts
//! \param mic_mg_Per_l -- Minimal Inhibitory Condition for the given infection
bool action_infection(std::unique_ptr<biogears::BioGearsEngine>& engine, CDM::enumInfectionSeverity severity, std::string location, double mic_mg_Per_l)
{
  auto sepsis = biogears::SEInfection();
  sepsis.SetSeverity(severity);
  sepsis.SetLocation(location);
  sepsis.GetMinimumInhibitoryConcentration().SetValue(mic_mg_Per_l, biogears::MassPerVolumeUnit::mg_Per_L);
  if (sepsis.IsValid()) {
    engine->ProcessAction(sepsis);
    return true;
  } else {
    return false;
  }
}
//!
//!  A end user might want to create a call back function for this one.  b
//!  Based on a timer
//!
//!  \param blood_volume_ml -- Amount of blood in the bag
//!  \param flowrate_ml_Per_min -- how fast the blood leaves the bag
//!
//!  You must recall this function with a 0 flowrate to terminate the transfusion
//-------------------------------------------------------------------------------
bool action_bloodtransfuction(std::unique_ptr<biogears::BioGearsEngine>& engine, double blood_volume_ml, double flowrate_ml_Per_min)
{
  auto* o_negative = engine->GetSubstanceManager().GetCompound("Blood_ONegative");
  auto transfusion = biogears::SESubstanceCompoundInfusion(*o_negative);
  transfusion.GetBagVolume().SetValue(blood_volume_ml, biogears::VolumeUnit::mL); //the total volume in the bag
  transfusion.GetRate().SetValue(flowrate_ml_Per_min, biogears::VolumePerTimeUnit::mL_Per_min); //The rate to admnister the compound in the bag
  if (transfusion.IsValid()) {
    engine->ProcessAction(transfusion);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!
//!  Applies an Acrtive heating element to the  environment
//!
//!  \param wattage --  Number of watts provided to the patient by the heating element
//!  \param surface_ara_fraction -- What % [0.0,1.0] of the patients surface area is covered by the heating element
//!
//!  Terminating this function requires applying it with 0 Wattage
bool action_active_heating(std::unique_ptr<biogears::BioGearsEngine>& engine, double watt, double surface_area_fraction)
{
  auto thermalApplication = biogears::SEThermalApplication();
  auto& blanket = thermalApplication.GetActiveHeating();
  blanket.GetPower().SetValue(watt, biogears::PowerUnit::W);
  blanket.GetSurfaceAreaFraction().SetValue(surface_area_fraction);
  if (thermalApplication.IsValid()) {
    engine->ProcessAction(thermalApplication);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!
//!  Applies an Active cooling element to the  environment
//!
//!  \param wattage --  Number of watts provided to the patient by the cooling element
//!  \param surface_ara_fraction -- What % [0.0,1.0] of the patients surface area is covered by the cooling element
//!
//! Terminating this function requires applying it with 0 wattage
bool action_active_cooling(std::unique_ptr<biogears::BioGearsEngine>& engine, double watt, double surface_area_fraction)
{
  auto thermalApplication = biogears::SEThermalApplication();
  auto& icepack = thermalApplication.GetActiveCooling();
  icepack.GetPower().SetValue(watt, biogears::PowerUnit::W);
  icepack.GetSurfaceAreaFraction().SetValue(surface_area_fraction);
  if (thermalApplication.IsValid()) {
    engine->ProcessAction(thermalApplication);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!
//!  Applies an AppliedTemperature element to the  environment
//!
//!  \param degrees --  YThe surface temperature of the temperature being applied to the bodies surface.
//!  \param surface_ara_fraction -- What % [0.0,1.0] of the patients surface area is covered by the applied temp
//!
//!  This function can not be undone.  SEThermalApplication supports a State member which can be set to off to terminate
//!  an active applied_temperature, you would simply need to pass the CDM::enumOnOff as an additional paramater to this function.
bool action_applied_temperature(std::unique_ptr<biogears::BioGearsEngine>& engine, double degrees_c, double surface_area_fraction)
{
  auto thermalApplication = biogears::SEThermalApplication();
  auto& heated_car_seat = thermalApplication.GetAppliedTemperature();
  heated_car_seat.GetTemperature().SetValue(degrees_c, biogears::TemperatureUnit::C);
  heated_car_seat.GetSurfaceAreaFraction().SetValue(surface_area_fraction);
  if (thermalApplication.IsValid()) {
    engine->ProcessAction(thermalApplication);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!
//!  Applies an Pain Stimulus to the Patient
//!
//!  \param location -- A Key to where the pain stimulus occured can be any value
//!  \param severity -- [0.0,1.0] of how bad the pain is
//!
//!  You must recall this function with a 0 severity and the same location to terminate the stimulus
bool action_pain_stimulus(std::unique_ptr<biogears::BioGearsEngine>& engine, std::string location, double severity)
{
  auto pain = biogears::SEPainStimulus();
  pain.SetLocation(location);
  pain.GetSeverity().SetValue(severity);
  if (pain.IsValid()) {
    engine->ProcessAction(pain);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------

//!
//! Patient will urinate until the blader nears zero volume
//!
bool action_urinate(std::unique_ptr<biogears::BioGearsEngine>& engine)
{
  auto relief = biogears::SEUrinate();
  if (relief.IsValid()) {
    engine->ProcessAction(relief);
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
//!
//!  Pimpl Implementation Structure.
//!  demonstrates how to get the urine color
//-------------------------------------------------------------------------------

bool action_get_urine_color(std::unique_ptr<biogears::BioGearsEngine>& engine)
{
  auto urineAnalysis = biogears::SEUrinalysis();
  const biogears::Renal* constRenalSystem = dynamic_cast<const biogears::Renal*>(engine->GetRenalSystem());
  biogears::Renal* renalSystem = const_cast<biogears::Renal*>(constRenalSystem);

  renalSystem->CalculateUrinalysis(urineAnalysis);
  if (urineAnalysis.HasColorResult()) {
    mil::tatrc::physiology::datamodel::enumUrineColor eColor = urineAnalysis.GetColorResult();

    switch (eColor) {
    case mil::tatrc::physiology::datamodel::enumUrineColor::DarkYellow:
      std::cout << "Urine Color: Dark Yellow";
      return true;
    case mil::tatrc::physiology::datamodel::enumUrineColor::PaleYellow:
      std::cout << "Urine Color: Pale Yellow";
      return true;
    case mil::tatrc::physiology::datamodel::enumUrineColor::Pink:
      std::cout << "Urine Color: Pink";
      return true;
    case mil::tatrc::physiology::datamodel::enumUrineColor::Yellow:
      std::cout << "Urine Color: Yellow";
      return true;
    default:
      std::cout << "Urine Color: Default";
      return true;
    }
  }

  return false;
}
//-------------------------------------------------------------------------------
//
//Substance Management Functions
//
//-------------------------------------------------------------------------------
std::unique_ptr<biogears::SESubstanceCompound> substance_make_compound(std::unique_ptr<biogears::BioGearsEngine>& engine)
{
  // The biggest concern with substances is unique names for storage in the map
  // In this example we make some trivial thread safe approach of naming all substances Substance_X
  //
  // Any approach is acceptable provided you can create a map of the UUID to the substance you want to fetch/
  // Second concern is that adding substances to simulation invalidates substance iterators so you need to make sure no
  // one is holding on to a substance iterator or substance ptr nad that time is not advancing.
  //
  static std::atomic<int> counter { 0 };
  std::stringstream ss;
  ss << "Substance_" << ++counter;
  return biogears::SESubstanceCompound::make_unique(ss.str().c_str(), engine->GetLogger());
}

bool addCompoundComponents(std::unique_ptr<biogears::BioGearsEngine>& engine, std::unique_ptr<biogears::SESubstanceCompound>& compound, const char* substance_str, double concentration, biogears::MassPerVolumeUnit unit)
{
  
  if (biogears::SESubstance* substance = engine->GetSubstanceManager().GetSubstance(substance_str)) {
    auto substance_concentration = biogears::SESubstanceConcentration(*substance);
    substance_concentration.GetConcentration().SetValue(10.0, biogears::MassPerVolumeUnit::mg_Per_mL);
    compound->GetComponents().push_back(substance_concentration);

    //Because the substance isn't register atat this point, it is possible to adjust the name based on the current mix.
    //Active infusions are stored by substance name so once registered and infuse you can no longer modify the mix name;
    return true;

  } else if (biogears::SESubstanceCompound* source_compound = engine->GetSubstanceManager().GetCompound(substance_str)) {
    for (auto& source_substance : source_compound->GetComponents()) {
      auto& destination_concentration = compound->GetComponent(source_substance.GetSubstance());

      //The mixing of substanceCompounds is non trivial and this is mearly an example of feasability
      //If one does not simply want to overwrite the existing concentration one may need to concider the
      //relative volumes of the two compounds to determine the new concentration.
      //Example Mixing Water 1:1 with Seawater  creates a very different sodium concentration then a 1:10 mix
      
      destination_concentration.GetConcentration() = source_substance.GetConcentration();

    }
    return true;
  }
  return false;
}
std::string substance_register_compound(std::unique_ptr<biogears::BioGearsEngine>& engine, std::unique_ptr<biogears::SESubstanceCompound>&& compound)
{
  //This call registeres the substance with the substance manager. At this point we are going to release control of the memory to the biogears library who will
  //delete it at the end of the run.  DLL boundries being what they are we may need to introduce a make_substancecompound function to the API.
  std::string compound_name = compound->GetName_cStr();
  engine->GetSubstanceManager().AddCompound(*compound.release());
  return compound_name;
}

bool action_infuse_compound(std::unique_ptr<biogears::BioGearsEngine>& engine, std::string compound_name, double volume, biogears::VolumeUnit volume_unit, double rate, biogears::VolumePerTimeUnit rate_unit)
{
  auto& compound = *engine->GetSubstanceManager().GetCompound(compound_name);
  biogears::SESubstanceCompoundInfusion infusion { compound };
  infusion.GetBagVolume().SetValue(volume, volume_unit);
  infusion.GetRate().SetValue(rate, rate_unit);
  return engine->ProcessAction(infusion);
}
//-------------------------------------------------------------------------------
//!
//!  Pimpl Implementation Structure.
//!
//!  std::thread for parallel computation of the biogears engine
//!  std::atomic_bool for threading loop control
//!  biogears::BioGearsEngine simulation thread for api control.
//-------------------------------------------------------------------------------
struct BioGearsPlugin::Implementation {
  std::unique_ptr<biogears::BioGearsEngine> engine;

  std::atomic_bool simulation_running { false };
  std::atomic_bool simulation_started { false };
  std::atomic_bool simulation_finished { false };
  std::thread simulation_thread;
};
//-------------------------------------------------------------------------------
BioGearsPlugin::BioGearsPlugin(std::string name)
  : _pimpl(std::make_unique<Implementation>())
{
  using namespace biogears;

  _pimpl->engine = std::make_unique<BioGearsEngine>(name + ".log");
  _pimpl->engine->GetLogger()->Info("HowToAPI_Integration");
  if (!_pimpl->engine->LoadState("states/StandardMale@0s.xml")) {
    _pimpl->engine->GetLogger()->Warning("Could not load patient state falling back to patient file.");

    if (!_pimpl->engine->InitializeEngine("patients/StandardMale.xml")) {
      _pimpl->engine->GetLogger()->Warning("Could not load patient falling back to manually creating a patient.");
      biogears::SEPatient patient { _pimpl->engine->GetLogger() };
      patient.SetName("StandardMale");
      patient.SetGender(CDM::enumSex::Male);
      patient.GetAge().SetValue(44, biogears::TimeUnit::yr);
      patient.GetWeight().SetValue(170, biogears::MassUnit::lb);
      patient.GetHeight().SetValue(71, biogears::LengthUnit::in);
      patient.GetBodyFatFraction().SetValue(0.21);
      patient.SetBloodType(CDM::enumBloodType::AB);
      patient.SetBloodRh(true);
      patient.GetHeartRateBaseline().SetValue(72, biogears::FrequencyUnit::Per_min);
      patient.GetRespirationRateBaseline().SetValue(16, biogears::FrequencyUnit::Per_min);
      patient.GetDiastolicArterialPressureBaseline().SetValue(73.5, biogears::PressureUnit::mmHg);
      patient.GetSystolicArterialPressureBaseline().SetValue(114, biogears::PressureUnit::mmHg);
      _pimpl->engine->InitializeEngine(patient);
    }
    _pimpl->engine->GetLogger()->Info("Initializing Patient. This could take a moment.");
  }
}
//-------------------------------------------------------------------------------
BioGearsPlugin ::~BioGearsPlugin()
{
  join();
  _pimpl = nullptr;
}
//-------------------------------------------------------------------------------
BioGearsPlugin& BioGearsPlugin::operator=(BioGearsPlugin&& rhs)
{
  _pimpl = std::move(rhs._pimpl);
  rhs._pimpl = std::make_unique<Implementation>();
  return *this;
}
//-------------------------------------------------------------------------------
void BioGearsPlugin::run()
{
  using namespace biogears;
  using namespace std::chrono_literals;

  if (!_pimpl->simulation_thread.joinable()) {
    _pimpl->simulation_running.store(true);
    _pimpl->simulation_thread = std::thread {
      [&]() {
        _pimpl->simulation_started.store(true);
        while (_pimpl->simulation_running) {
          action_apply_hemorrhage(_pimpl->engine, "LeftLeg", 5.);
          action_apply_tourniquet(_pimpl->engine, "LeftLeg", CDM::enumTourniquetApplicationLevel::Applied);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_apply_hemorrhage(_pimpl->engine, "LeftLeg", 5.);
          action_apply_tourniquet(_pimpl->engine, "LeftLeg", CDM::enumTourniquetApplicationLevel::Misapplied);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_apply_tourniquet(_pimpl->engine, "LeftLeg", CDM::enumTourniquetApplicationLevel::Applied);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_apply_tourniquet(_pimpl->engine, "LeftLeg", CDM::enumTourniquetApplicationLevel::None);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          //The effectivness of this helper-function is called in to doubt, for such a simple tutorial
          //But you could imagine creating a vector of common conditions and then pushing and poping them in to
          //This function as the patient changes locations.
          
          auto conditions = biogears::SEEnvironmentalConditions(_pimpl->engine->GetSubstanceManager());
          auto* N2 = _pimpl->engine->GetSubstanceManager().GetSubstance("Nitrogen");
          auto* O2 = _pimpl->engine->GetSubstanceManager().GetSubstance("Oxygen");
          auto* CO2 = _pimpl->engine->GetSubstanceManager().GetSubstance("CarbonDioxide");
          
          conditions.SetSurroundingType(CDM::enumSurroundingType::Water);
          conditions.GetAirVelocity().SetValue(0, LengthPerTimeUnit::m_Per_s);
          conditions.GetAmbientTemperature().SetValue(10.0, TemperatureUnit::C);
          conditions.GetAtmosphericPressure().SetValue(760., PressureUnit::mmHg);
          conditions.GetClothingResistance().SetValue(0.01, HeatResistanceAreaUnit::clo);
          conditions.GetEmissivity().SetValue(0.0);
          conditions.GetMeanRadiantTemperature().SetValue(22.0, TemperatureUnit::C);
          conditions.GetRelativeHumidity().SetValue(1.0);
          conditions.GetRespirationAmbientTemperature().SetValue(22.0, TemperatureUnit::C);
          conditions.GetAmbientGas(*N2).GetFractionAmount().SetValue(0.7901);
          conditions.GetAmbientGas(*O2).GetFractionAmount().SetValue(0.2095);
          conditions.GetAmbientGas(*CO2).GetFractionAmount().SetValue(4.0E-4);
          
          action_env_change(_pimpl->engine, conditions);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_tension_pneumothorax(_pimpl->engine, CDM::enumSide::Left, CDM::enumPneumothoraxType::Open, 0.5);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_needle_decompression(_pimpl->engine, CDM::enumSide::Left, true);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_o2_mask(_pimpl->engine, .5, 3., 0.);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_infection(_pimpl->engine, CDM::enumInfectionSeverity::Mild, "LeftLeg", 0.2);
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_bloodtransfuction(_pimpl->engine, 500, 100);

          ///////////////////////////////////////

          ///DRUGS
          auto customCompound = substance_make_compound(_pimpl->engine);
          auto mixSuccess = addCompoundComponents(_pimpl->engine, customCompound, "Saline", 2.17, MassPerVolumeUnit::kg_Per_L);
          mixSuccess &= addCompoundComponents(_pimpl->engine, customCompound, "Albumin", 10.0, MassPerVolumeUnit::mg_Per_mL);
          mixSuccess &= addCompoundComponents(_pimpl->engine, customCompound, "Morphine", 10.0, MassPerVolumeUnit::mg_Per_mL);

          if (mixSuccess) {
            for (auto& component : customCompound->GetComponents()) {
              //Example of creating data request for tracking the components of the new substance.
              //Note: String based DataRequest will cause runtime instability if no Scalar matches the input string.
              if (component.GetSubstance().HasPK()) {
                _pimpl->engine->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*_pimpl->engine->GetSubstanceManager().GetSubstance(component.GetSubstance().GetName()), "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
              }
            }
            auto compound_name = substance_register_compound(_pimpl->engine, std::move(customCompound)); //<  _pimpl->engine takes ownership of customCompound here and will delete it once it is removed (DLL Boundry issue need to create a make_compound function to avoid that in biogears)
            action_infuse_compound(_pimpl->engine, compound_name, 1.0, VolumeUnit::L, 100., VolumePerTimeUnit::mL_Per_hr);

            _pimpl->engine->AdvanceModelTime(1, TimeUnit::s);

            action_infuse_compound(_pimpl->engine, compound_name, 1.0, VolumeUnit::L, 0, VolumePerTimeUnit::mL_Per_hr);

            _pimpl->engine->AdvanceModelTime(1, TimeUnit::s);

          } else {
            _pimpl->engine->GetLogger()->Error("Saline,Albumin, and Morphine definitions required for this HowTo please ensure they exist and try again.");
          }

          ////////////////////////////////////////
          if (_pimpl->engine->GetActions().GetEnvironmentActions().HasThermalApplication()) {
            _pimpl->engine->GetLogger()->Info(asprintf("ActiveHeating[%s]", _pimpl->engine->GetActions().GetEnvironmentActions().GetThermalApplication()->HasActiveHeating() ? "true" : "false")); // false
          }
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_active_heating(_pimpl->engine, 500, 0.5);
          _pimpl->engine->GetLogger()->Info(asprintf("ActiveHeating[%s]", _pimpl->engine->GetActions().GetEnvironmentActions().GetThermalApplication()->HasActiveHeating() ? "true" : "false")); // true
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          
          _pimpl->engine->GetLogger()->Info(asprintf("ActiveCooling[%s]", _pimpl->engine->GetActions().GetEnvironmentActions().GetThermalApplication()->HasActiveCooling() ? "true" : "false"));
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_active_cooling(_pimpl->engine, 500, 0.5);
          _pimpl->engine->GetLogger()->Info(asprintf("ActiveCooling[%s]", _pimpl->engine->GetActions().GetEnvironmentActions().GetThermalApplication()->HasActiveCooling() ? "true" : "false"));
          
          _pimpl->engine->GetLogger()->Info(asprintf("AppliedTempeture[%s]", _pimpl->engine->GetActions().GetEnvironmentActions().GetThermalApplication()->HasAppliedTemperature() ? "true" : "false"));
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_applied_temperature(_pimpl->engine, 37, 0.5);
          _pimpl->engine->GetLogger()->Info(asprintf("AppliedTempeture[%s]", _pimpl->engine->GetActions().GetEnvironmentActions().GetThermalApplication()->HasAppliedTemperature() ? "true" : "false"));
          _pimpl->engine->AdvanceModelTime(1, biogears::TimeUnit::s);
          action_get_urine_color(_pimpl->engine);
          action_urinate(_pimpl->engine);
          try {
          _pimpl->engine->SaveState("HowTo-API_Integration_FinalState.xml");
            
          } catch ( xsd::cxx::exception e) {
            std::cout << e.what();
          }
          _pimpl->simulation_finished.store(true);
        }
      }
    };
  }
}
//-------------------------------------------------------------------------------
void BioGearsPlugin::join()
{
  if (_pimpl->simulation_thread.joinable()) {
    _pimpl->simulation_thread.join();
  }
}
//-------------------------------------------------------------------------------
void BioGearsPlugin::stop()
{
  _pimpl->simulation_running.store(false);
}
//-------------------------------------------------------------------------------
bool BioGearsPlugin::isRunning()
{
  return _pimpl && _pimpl->simulation_started && !_pimpl->simulation_finished;
}
