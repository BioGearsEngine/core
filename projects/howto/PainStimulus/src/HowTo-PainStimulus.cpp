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

#include "HowTo-PainStimulus.h"


#include <iostream>
// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
///
///
/// \details
///
//--------------------------------------------------------------------------------------------------
void HowToPainStimulus()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToPain.log");
  bg->GetLogger()->Info("HowToPain");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  //Create variables for scenario
  SEPainStimulus PainStimulus; //pain object
  std::string location; //location of pain stimulus, examples "Arm", "Leg"
  double severity; //severity (scale 0-1)

  //set up the configuration of the pain stimulus
  location = "Arm";
  severity = 0.6;
  PainStimulus.SetLocation(location);
  PainStimulus.GetSeverity().SetValue(severity);

  //grab VAS score  
  double pain;
  double dt = bg->GetTimeStep(TimeUnit::s);

  //Set up substances.  Initialized morphine plasma concentration to 0.  Note that saline is technically a compound--this is
  //so the engine knows to look for multiple components within the same substance file (i.e. Na, Cl, etc)
  SESubstance* epi = bg->GetSubstanceManager().GetSubstance("Epinephrine");
  // Get the Morphine substance from the substance manager
  const SESubstance* morphine = bg->GetSubstanceManager().GetSubstance("Morphine");

  // Create a substance bolus action to administer the substance
  SESubstanceBolus bolus(*morphine);
  bolus.GetConcentration().SetValue(1000, MassPerVolumeUnit::ug_Per_mL);
  bolus.GetDose().SetValue(10.5, VolumeUnit::mL);
  bolus.SetAdminRoute(CDM::enumBolusAdministration::Intravenous);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file,
  //note: can't ask for substance values that aren't currently in the patient (i.e. can't ask for morphine, unless you administer it before advancing time)
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*morphine, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*epi, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PainVisualAnalogueScale");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToPain.txt");

  //lets start the pain
  bg->ProcessAction(PainStimulus);
  //administer morphine

  bg->AdvanceModelTime(3, TimeUnit::s);
  pain = bg->GetNervousSystem()->GetPainVisualAnalogueScale();
  std::cout << pain << std::endl;

  if (pain > 2.0) {
    bg->GetLogger()->Info("The patient is in pain");
  }

  bg->GetLogger()->Info("Giving the patient Morphine.");
  //bg->ProcessAction(bolus);


  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));

  // Advance some time until patient is comfortable
  while (pain > 1.0) {
    //update value
    pain = bg->GetNervousSystem()->GetPainVisualAnalogueScale();
    bg->AdvanceModelTime(dt, TimeUnit::s);
  }

  //after we get out of the while loop patient should be happy now
  bg->GetLogger()->Info("The patient is comfortable, morphine administration buffered effects");

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Hemoglobin Content : %f %s", bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g), "g"));
  bg->GetLogger()->Info(asprintf("Blood Volume : %f %s", bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL), "mL"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  ;

  bg->AdvanceModelTime(400, TimeUnit::s);
  bg->GetLogger()->Info("Finished");
}
