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



#include <biogears/schema/cdm/Properties.hxx>
// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for exposing the patient to Sarin, a toxic nerve agent
///
/// \details
/// Refer to the SEEnvironmentChange class
/// Refer to the SEDrug Class
//--------------------------------------------------------------------------------------------------
void OutputState(std::unique_ptr<PhysiologyEngine>& bgOut);

void HowToSarinExposure()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToSarinExposure.log");
  bg->GetLogger()->Info("HowToSarinExposure");

  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  //---Initialize all variables needed for scenario

  //Substances
  SESubstance* Sarin = bg->GetSubstanceManager().GetSubstance("Sarin");
  SESubstance* CO2 = bg->GetSubstanceManager().GetSubstance("CarbonDioxide");
  SESubstance* O2 = bg->GetSubstanceManager().GetSubstance("Oxygen");

  //Numerical values
  double exposureTime = 5.0; //Establish how long the patient will be exposed to Sarin
  double simulationTime = 20.0; //Establish the length of the simulation
  double SarinAerosol_mg_Per_m3 = 15.0; //Establish concentration of ambient Sarin in mg/m^3
  double BaselineRbcAche_nM = 8.0; //The average baseline concentration of red blood cell acetylcholinesterase in the body
  double RbcAcheConversionFactor_M_to_nM = 1e9; //Use to convert data calls in M to nM because nM is used in kinetics equations in BioGears Drugs file for Sarin
  double FractionRbcAcheInhibited = 0.0; //The fraction of red blood cell acetylcholinesterase that has been inhibited by Sarin binding.  This value will be used to trigger events.

  //System states
  CDM::enumOnOff::value SarinActive; //Track whether the patient is currently being exposed to Sarin
  std::vector<std::string> eventList = { "Rhinorrhea", "Nausea", "Gastrointestinal Sounds" }; //The list of patient states you want to monitor--note that BioGears track many events already; this is a way to track other states you might want to add
  std::map<std::string, std::pair<double, CDM::enumOnOff::value>> eventMap; //Each state has a threshold level at which it is triggered and an associated on/off switch
  eventMap["Rhinorrhea"] = std::make_pair(0.1, CDM::enumOnOff::Off); //Put the states in the map with the threshold inhibition at which they start and make them all inactive initially
  eventMap["Nausea"] = std::make_pair(0.25, CDM::enumOnOff::Off);
  eventMap["Gastrointestinal Sounds"] = std::make_pair(0.5, CDM::enumOnOff::Off);

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file

  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalAlveolarVentilation", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("Aorta", *CO2, "PartialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("Aorta", *O2, "PartialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RedBloodCellAcetylcholinesterase", AmountPerVolumeUnit::mol_Per_L);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToSarinExposure.csv");

  // Advance some time to get some resting data
  bg->AdvanceModelTime(60, TimeUnit::s);

  bg->GetLogger()->Info("The patient is nice and healthy");
  OutputState(bg);

  // We assume that Sarin is inhaled as an aerosol, therefore we have to instantiate an environmental conditions change (see HowTo-EnvironmentChange for more details)
  // Sarin is introduced as an ambient aerosol and the concentration is set with units of mg/m^3
  SEEnvironmentChange env(bg->GetSubstanceManager());
  SEEnvironmentalConditions& conditions = env.GetConditions();
  conditions.GetAmbientAerosol(*Sarin).GetConcentration().SetValue(SarinAerosol_mg_Per_m3, MassPerVolumeUnit::mg_Per_m3);
  SarinActive = CDM::enumOnOff::On; //Sarin exposure is active
  bg->ProcessAction(env);

  // We maintain this loop so that we can periodically check events while advancing data tracker
  while (bg->GetSimulationTime(TimeUnit::min) < simulationTime) {
    //Break out of loop if patient enters irreversible state
    if (bg->GetPatient().IsEventActive(CDM::enumPatientEvent::IrreversibleState))
      break;

    //Check if we have reached the end of the exposure time to Sarin.  If so, remove it from the environment and deactivate it
    if ((bg->GetSimulationTime(TimeUnit::min) > exposureTime + 1) && (SarinActive == CDM::enumOnOff::On)) {
      conditions.RemoveAmbientAerosol(*Sarin);
      bg->ProcessAction(env);
      bg->GetLogger()->Info(std::stringstream() << std::string{ Sarin->GetName() } +" removed from environment");
      SarinActive = CDM::enumOnOff::Off;
    }

    //Get the fraction of inactive red blood cell acetylcholinesterase by converting active M concentration to nM, dividing by baseline, and subtracting from 1
    FractionRbcAcheInhibited = 1.0 - (bg->GetBloodChemistrySystem()->GetRedBloodCellAcetylcholinesterase(AmountPerVolumeUnit::mol_Per_L) * RbcAcheConversionFactor_M_to_nM / BaselineRbcAche_nM);

    //Cout statement outputs Fraction of RBC Inhibited in case you want to track that during simulation
    std::cout << "Fraction of Red Blood Cell AChE Inhibited: " << FractionRbcAcheInhibited << std::endl;

    //Check each event in the event list specified above.  An event is active if the fraction of red blood cell acetylcholinesterase inhibited
    //is greater than the threshold provided for the event in the event map
    for (auto e : eventList) {
      if ((FractionRbcAcheInhibited > eventMap[e].first) && (eventMap[e].second == CDM::enumOnOff::Off)) //Only display state when it is triggered, not every time point when the event is active
      {
        bg->GetLogger()->Info(std::stringstream() << "Patient is exhibiting " + e);
        eventMap[e].second = CDM::enumOnOff::On;
      }
      if ((FractionRbcAcheInhibited < eventMap[e].first) && (eventMap[e].second == CDM::enumOnOff::On)) //Remove state if levels drop below threshold.  Probably won't happen for Sarin because the red blood cell acetylcholinesterase is inhibited for a very long time (days to weeks)
      {
        bg->GetLogger()->Info(std::stringstream() << "Patient no longer exhibiting " + e);
        eventMap[e].second = CDM::enumOnOff::Off;
      }
    }

    //Advance the tracker forward by 30 seconds.  Change this time to adjust how frequently the simulation checks for events (will not effect how the engine runs)

    bg->AdvanceModelTime(30.0, TimeUnit::s);
  }

  OutputState(bg);

  bg->AdvanceModelTime(60, TimeUnit::s);

  bg->GetLogger()->Info("Finished");
}

void OutputState(std::unique_ptr<PhysiologyEngine>& bgOut)
{
  //Since we'll want this data repeatedly, make a function call so that we don't copy and paste like crazy
  bgOut->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bgOut->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "breaths/min");
  bgOut->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bgOut->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bgOut->GetLogger()->Info(std::stringstream() << "C02 Partial Pressure in Aorta : " << bgOut->GetBloodChemistrySystem()->GetArterialCarbonDioxidePressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bgOut->GetLogger()->Info(std::stringstream() << "O2 Partial Pressure in Aorta : " << bgOut->GetBloodChemistrySystem()->GetArterialOxygenPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bgOut->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bgOut->GetBloodChemistrySystem()->GetOxygenSaturation() << "%");
  bgOut->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bgOut->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bgOut->GetLogger()->Info(std::stringstream() << "Red Blood Cell Acetylcholinesterase Level : " << bgOut->GetBloodChemistrySystem()->GetRedBloodCellAcetylcholinesterase(AmountPerVolumeUnit::mol_Per_L) << AmountPerVolumeUnit::mol_Per_L);
}
