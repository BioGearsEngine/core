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

#include "HowToTracker.h"

// Include the various types you will be using in your code
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for adminstering a substance to the patient via a continuous infusion
///
/// \details
/// Refer to the SESubstanceInfusion class
/// Refer to the SESubstanceManager class
//--------------------------------------------------------------------------------------------------
void HowToInfusionDrug()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("VasopressinPD.log");
  bg->GetLogger()->Info("VasopressinPD");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*bg);

  SESubstance* vas = bg->GetSubstanceManager().GetSubstance("Vasopressin");
  vas->GetPlasmaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_L);

  // Create a substance bolus action to administer the substance
  SESubstanceInfusion infuse(*vas);
  infuse.GetConcentration().SetValue(0.102, MassPerVolumeUnit::ug_Per_mL);
  infuse.GetRate().SetValue(0.1, VolumePerTimeUnit::mL_Per_min);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*vas, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineProductionRate", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineOsmolarity", OsmolalityUnit::mOsm_Per_kg);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("VasopressinPD.csv");

  bg->GetLogger()->Info("Beginning PD Scenario");

  bg->ProcessAction(infuse);
  tracker.AdvanceModelTime(3300);

  infuse.GetConcentration().SetValue(0.292, MassPerVolumeUnit::ug_Per_mL);
  bg->ProcessAction(infuse);
  tracker.AdvanceModelTime(3300);

  infuse.GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  bg->ProcessAction(infuse);
  tracker.AdvanceModelTime(600);

  bg->GetLogger()->Info("Finished");

  // Example for TXA infusion after a hemorrhage
  /*
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine>
  bg = CreateBioGearsEngine("TXAPD.log");
  bg->GetLogger()->Info("TXAPD");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*bg);

  SESubstance* txa = bg->GetSubstanceManager().GetSubstance("TranexamicAcid");
  txa->GetPlasmaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_L);

  // Create a substance infusion action to administer the substance
  SESubstanceInfusion infuse(*txa);
  infuse.GetConcentration().SetValue(0.1, MassPerVolumeUnit::g_Per_mL);
  infuse.GetRate().SetValue(0.033, VolumePerTimeUnit::mL_Per_s);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*txa, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("Ground","InFlow",VolumePerTimeUnit::mL_Per_min);

  //Create variables for scenario
  SEHemorrhage hemorrhageSpleen; //hemorrhage object
  std::string location; //location of hemorrhage, valid options are "Aorta", "VenaCava", "Brain", "Myocardium", "LeftLung", "RightLung", "Spleen", "Splanchnic", "SmallIntestine", "LargeIntestine", "LeftKidney", "RightKidney", "Liver", "LeftArm", "RightArm", "LeftLeg", "RightLeg"
  double rate_mL_Per_min = 150.0; //the initial bleeding rate of the hemorrhage
  std::vector<unsigned int> mcisCode; //injury code if using option 2, see ParseMCIS method below for more details
  //Let's create an internal hemorrhage in the spleen (maybe it ruptured...)
  location = "Spleen";
  hemorrhageSpleen.SetCompartment(location);
  hemorrhageSpleen.GetInitialRate().SetValue(rate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  hemorrhageSpleen.SetMCIS();

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("TXAPD.csv");

  bg->GetLogger()->Info("Beginning PD Scenario");

  // Hemorrhage Starts - instantiate a hemorrhage action and have the engine process it.  Note that BioGears will output the injury code regardless of which method was used
  bg->ProcessAction(hemorrhageSpleen);
  bg->ProcessAction(infuse);
  tracker.AdvanceModelTime(300);

  infuse.GetConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_mL);
  bg->ProcessAction(infuse);
  tracker.AdvanceModelTime(7200);

  bg->GetLogger()->Info("Finished");
  */
}
