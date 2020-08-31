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
#define _USE_MATH_DEFINES



#include <cmath>
// Include the various types you will be using in your code
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
#include <biogears/cdm/patient/actions/SEApnea.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
#include <biogears/cdm/patient/actions/SEBreathHold.h>
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
#include <biogears/cdm/patient/actions/SEForcedExhale.h>
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
// Make a custom event handler that you can connect to your code (See EngineUse for more info)
class MechVentHandler : public SEEventHandler {
private:
  Logger* m_Logger;
public:
  MechVentHandler(Logger* logger)
    : SEEventHandler()
    , m_Logger(logger)
  {
  }
  virtual void HandlePatientEvent(CDM::enumPatientEvent::value type, bool active, const SEScalarTime* time = nullptr)
  {
    switch (type) {
    case CDM::enumPatientEvent::AcuteLungInjury: {
      if (active)
        m_Logger->Info("Do something for MildAcuteRespiratoryDistress");
      else
        m_Logger->Info("Stop doing something for MildAcuteRespiratoryDistress");
      break;
    }
    case CDM::enumPatientEvent::AcuteRespiratoryDistress: {
      if (active)
        m_Logger->Info("Do something for ModerateAcuteRespiratoryDistress");
      else
        m_Logger->Info("Stop doing something for ModerateAcuteRespiratoryDistress");
      break;
    }
    case CDM::enumPatientEvent::SevereAcuteRespiratoryDistress: {
      if (active)
        m_Logger->Info("Do something for SevereAcuteRespiratoryDistress");
      else
        m_Logger->Info("Stop doing something for SevereAcuteRespiratoryDistress");
      break;
    }
    case CDM::enumPatientEvent::CardiogenicShock: {
      if (active)
        m_Logger->Info("Do something for CardiogenicShock");
      else
        m_Logger->Info("Stop doing something for CardiogenicShock");
      break;
    }
    }
  }
  virtual void HandleAnesthesiaMachineEvent(CDM::enumAnesthesiaMachineEvent::value type, bool active, const SEScalarTime* time = nullptr)
  {
  }
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for the Mechanical Ventilation Patient Action
/// Drive respiration with your own driver
///
/// \details
/// Refer to the SEMechanicalVentilation class
//--------------------------------------------------------------------------------------------------
void HowToMechanicalVentialtion()
{
  //Note: Setting circuit values (resistance/compliances/etc.) needs to be done in the engine code - they currently are not directly exposed

  std::stringstream ss;
  // Create a BioGears Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToMechanicalVentilation.log");

  bg->GetLogger()->Info("HowToMechanicalVentilation");

  //Initialize the patient with any conditions
  //Change the following true/false flags to give the patient different conditions
  //If no conditions, just load the serialized healthy state
  std::vector<const SECondition*> conditions;
  if (true) //Healthy - i.e., no chronic conditions
  {
    if (!bg->LoadState("./states/StandardMale@0s.xml")) //Select which patient
    {
      bg->GetLogger()->Error("Could not load state, check the error");
      return;
    }
  } else {
    if (false) //COPD
    {
      SEChronicObstructivePulmonaryDisease COPD;
      COPD.GetBronchitisSeverity().SetValue(0.5);
      COPD.GetEmphysemaSeverity().SetValue(0.7);
      conditions.push_back(&COPD);
    }
    if (false) //LobarPneumonia
    {
      SELobarPneumonia lobarPneumonia;
      lobarPneumonia.GetSeverity().SetValue(0.2);
      lobarPneumonia.GetLeftLungAffected().SetValue(1.0);
      lobarPneumonia.GetRightLungAffected().SetValue(1.0);
      conditions.push_back(&lobarPneumonia);
    }
    if (false) //Generic ImpairedAlveolarExchange (no specified reason)
    {
      SEImpairedAlveolarExchange ImpairedAlveolarExchange;
      ImpairedAlveolarExchange.GetImpairedFraction().SetValue(0.5);
      conditions.push_back(&ImpairedAlveolarExchange);
    }

    //Select the patient and initialize with conditions
    //You can optionally define the patient here - see HowTo-CreateAPatient.cpp
    if (!bg->InitializeEngine("StandardMale.xml", &conditions)) {
      bg->GetLogger()->Error("Could not load initialize engine, check the error");
      return;
    }
  }

  // Let's add our event listener callback
  MechVentHandler myEventHandler(bg->GetLogger());
  bg->SetEventHandler(&myEventHandler);

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  //System data
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalLungVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationMusclePressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryResistance", FlowResistanceUnit::cmH2O_s_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryCompliance", FlowComplianceUnit::L_Per_cmH2O);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryCapillariesWedgePressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryMeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacIndex", VolumePerTimeAreaUnit::L_Per_min_m2);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystemicVascularResistance", FlowResistanceUnit::cmH2O_s_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryVascularResistance", FlowResistanceUnit::cmH2O_s_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryVascularResistanceIndex", PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodPH");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialOxygenPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialCarbonDioxidePressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("VenousOxygenPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("VenousCarbonDioxidePressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("IntracranialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CarricoIndex", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("AlveolarArterialGradient", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SedationLevel");
  //Patient data
  bg->GetEngineTrack()->GetDataRequestManager().CreatePatientDataRequest().Set("FunctionalResidualCapacity", VolumeUnit::L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePatientDataRequest().Set("VitalCapacity", VolumeUnit::L);
  //Compartment data
  //Arteriole bicarbonate
  SESubstance* HCO3 = bg->GetSubstanceManager().GetSubstance("Bicarbonate");
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set(BGE::VascularCompartment::Aorta, *HCO3, "Concentration", MassPerVolumeUnit::ug_Per_mL);
  //Lactate - this should have a relationship to lactic acid
  SESubstance* Lactate = bg->GetSubstanceManager().GetSubstance("Lactate");
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*Lactate, "BloodConcentration", MassPerVolumeUnit::ug_Per_mL);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToMechanicalVentilation.csv");

  //Output some random stuff to the log
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

  //Go 1 min before doing anything
  //The patient is just doing spontaneous breathing
  bg->AdvanceModelTime(60.0, TimeUnit::s);

  //Let's do a bunch of different actions at the same time!

  //Use conscious respiration to cough - as expected, this won't do much to the patient's physiology
  //Conscious respiration could be used force certain breathing patterns, but will no longer take into account any feedback
  SEConsciousRespiration consciousRespiration;
  // Create commands in the order you want them processed.
  // Inhale is the first command we want to process
  SEForcedInhale& forcedInhale = consciousRespiration.AddForcedInhale();
  forcedInhale.GetInspiratoryCapacityFraction().SetValue(0.25);
  forcedInhale.GetPeriod().SetValue(0.7, TimeUnit::s);
  // Next we will hold our breath
  consciousRespiration.AddBreathHold().GetPeriod().SetValue(0.25, TimeUnit::s);
  // Then exhale
  SEForcedExhale& forcedExhale = consciousRespiration.AddForcedExhale();
  forcedExhale.GetExpiratoryReserveVolumeFraction().SetValue(0.0);
  forcedExhale.GetPeriod().SetValue(0.05, TimeUnit::s);
  // Then hold our breath again
  consciousRespiration.AddBreathHold().GetPeriod().SetValue(0.5, TimeUnit::s);
  // Once ProcessAction is called, the engine will make a copy of these commands.
  // You cannont modify them,
  //  you will need to either clear out this command and reprocess it,
  //  or process a whole new command.
  // If you plan on reusing this consciousRespiration action, you need to clear it if you want to add a new set of commands.
  bg->ProcessAction(consciousRespiration);
  // NOTE : The engine is going to need  to run for the total sum of the command periods provided above
  // for the action to be completly processed by the engine
  // You can add other actions while this action is being processed.
  // Just be aware that this action is still being processed.
  // It is recommended that you advance time for at least the sum of the command periods.
  bg->AdvanceModelTime(60.0, TimeUnit::s);

  //Airway obstruction
  SEAirwayObstruction obstruction;
  obstruction.GetSeverity().SetValue(0.2);
  bg->ProcessAction(obstruction);

  bg->AdvanceModelTime(60.0, TimeUnit::s);

  //Pneumothorax
  // Create a Tension Pnuemothorax
  // Set the severity (a fraction between 0 and 1)
  SETensionPneumothorax pneumo;
  // You can have a Closed or Open Tension Pneumothorax
  pneumo.SetType(CDM::enumPneumothoraxType::Open);
  //pneumo.SetType(CDM::enumPneumothoraxType::Open);
  pneumo.GetSeverity().SetValue(0.3);
  // It can be on the Left or right side
  pneumo.SetSide(CDM::enumSide::Right);
  //pneumo.SetSide(CDM::enumSide::Left);
  bg->ProcessAction(pneumo);

  bg->AdvanceModelTime(60.0, TimeUnit::s);

  //Asthma attack
  SEAsthmaAttack asthmaAttack;
  asthmaAttack.GetSeverity().SetValue(0.3);
  bg->ProcessAction(asthmaAttack);

  //Stress response - release epinephrine
  SEAcuteStress acuteStress;
  acuteStress.GetSeverity().SetValue(0.3);
  bg->ProcessAction(acuteStress);

  //TBI
  //See HowTo-BrainInjury for an example of getting the Glasgow Scale
  SEBrainInjury tbi;
  tbi.SetType(CDM::enumBrainInjuryType::Diffuse); // Can also be LeftFocal or RightFocal, and you will get pupillary effects in only one eye
  tbi.GetSeverity().SetValue(0.2);
  bg->ProcessAction(tbi);

  //Environment change
  SEEnvironmentChange env(bg->GetSubstanceManager());
  SEEnvironmentalConditions& envConditions = env.GetConditions();
  envConditions.GetAirVelocity().SetValue(2.0, LengthPerTimeUnit::m_Per_s);
  envConditions.GetAmbientTemperature().SetValue(15.0, TemperatureUnit::C);
  envConditions.GetAtmosphericPressure().SetValue(740., PressureUnit::mmHg);
  envConditions.GetMeanRadiantTemperature().SetValue(15.0, TemperatureUnit::C);
  bg->ProcessAction(env);

  bg->AdvanceModelTime(60.0, TimeUnit::s);

  //Apnea
  //Maybe the muscles are getting weak?
  SEApnea apnea;
  apnea.GetSeverity().SetValue(0.3);
  bg->ProcessAction(apnea);

  //Succs
  //Make the patient stop breathing
  // Get the Succinylcholine substance from the substance manager
  const SESubstance* succs = bg->GetSubstanceManager().GetSubstance("Succinylcholine");
  // Create a substance bolus action to administer the substance
  SESubstanceBolus bolus(*succs);
  bolus.GetConcentration().SetValue(4820, MassPerVolumeUnit::ug_Per_mL);
  bolus.GetDose().SetValue(20, VolumeUnit::mL);
  bolus.SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  bg->ProcessAction(bolus);

  bg->AdvanceModelTime(60.0, TimeUnit::s);

  //Mechanical Ventilation
  // Create an SEMechanicalVentilation object
  SEMechanicalVentilation mechVent;
  mechVent.SetState(CDM::enumOnOff::On); // Turn it on
    // Grab the substance fractions so we can quickly modify them
  SESubstanceFraction& O2frac = mechVent.GetGasFraction(*bg->GetSubstanceManager().GetSubstance("Oxygen"));
  SESubstanceFraction& CO2frac = mechVent.GetGasFraction(*bg->GetSubstanceManager().GetSubstance("CarbonDioxide"));
  SESubstanceFraction& N2frac = mechVent.GetGasFraction(*bg->GetSubstanceManager().GetSubstance("Nitrogen"));

  //We'll mimic inputs from real-time sensors by just driving the mechanical ventilation pressure using a sinusoid
  //Pressure waveform parameters
  double period = 5.0;
  double alpha = (2 * M_PI) / (period);
  double inputPressure_cmH2O = 0.0;
  double amplitude_cmH2O = 6.0;
  double yOffset = 10.0;

  // Drive the system for 5 mins
  for (unsigned int time_s = 0; time_s < 300; time_s++) {
    // Going to update values every second
    //The tracker with write to the results file every time-step

    //Difference from ambient pressure
    inputPressure_cmH2O = yOffset + amplitude_cmH2O * sin(alpha * time_s); //compute new pressure

    mechVent.GetPressure().SetValue(inputPressure_cmH2O, PressureUnit::cmH2O);
    //You can set flow, but we aren't
    O2frac.GetFractionAmount().SetValue(0.21);
    CO2frac.GetFractionAmount().SetValue(4.0E-4);
    N2frac.GetFractionAmount().SetValue(0.7896);
    bg->ProcessAction(mechVent);

    bg->AdvanceModelTime(1, TimeUnit::s);

    //Output some random stuff to the log
    bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
    bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
    bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
    bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
    bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
    bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  }

  bg->GetLogger()->Info("Finished");
}
