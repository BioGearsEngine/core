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

#include "BioGearsEngineHowTo.h"

// Include the various types you will be using in your code
#include "patient/actions/SEPainStimulus.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceCompound.h"
#include "properties/SEScalarFraction.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"
#include "engine/PhysiologyEngineTrack.h"
#include "compartment/SECompartmentManager.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a Hemorrhage insult to the patient and also demonstrate how injury code can be used to initiate a hemorrhage if desired
///
/// \details
/// Refer to the SEHemorrhage class
/// Refer to the SESubstanceManager class
/// Refer to the SESubstanceIVFluids class for applying an IV to the patient
//--------------------------------------------------------------------------------------------------
void HowToPainStimulus() 
{
  // Create the engine and load the patient
	std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToPain.log");
  bg->GetLogger()->Info("HowToPain");
	if (!bg->LoadState("./states/StandardMale@0s.xml"))
  {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
	HowToTracker tracker(*bg);

	//Create variables for scenario
	SEPainStimulus PainStimulus;			//pain object
	std::string location;					//location of pain stimulus, examples "Arm", "Leg"
	double severity;						//severity (scale 0-1)

	//set up the configuration of the pain stimulus
	location = "Arm";
	severity = 0.5;
	PainStimulus.SetLocation(location);
	PainStimulus.GetSeverity().SetValue(severity);

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
	// defined in the Physiology.xsd file
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

	// Advance some time to let the body bleed out a bit
	tracker.AdvanceModelTime(300);

	bg->GetLogger()->Info("The patient has been in moderate pain for 300s");
	bg->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");;

	//administer morphine 
	bg->ProcessAction(bolus);
	bg->GetLogger()->Info("Giving the patient Morphine.");

	//grab VAS score
	double pain = bg->GetNervousSystem()->GetPainVisualAnalogueScale();
	double dt = bg->GetTimeStep(TimeUnit::s);

	// Advance some time until patient is comfortable
	while (pain > 2.0)
	{
		//update value
		pain = bg->GetNervousSystem()->GetPainVisualAnalogueScale();
		tracker.AdvanceModelTime(dt);
	}

	//after we get out of the while loop patient should be happy now
	bg->GetLogger()->Info("The patient is comfortable");

	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
	bg->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");;


	tracker.AdvanceModelTime(400);
	bg->GetLogger()->Info("Finished");
}

