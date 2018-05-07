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
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a Hemorrhage insult to the patient and also demonstrate how injury code can be used to initiate a hemorrhage if desired
///
/// \details
/// Refer to the SEHemorrhage class
/// Refer to the SESubstanceManager class
/// Refer to the SESubstanceIVFluids class for applying an IV to the patient
//--------------------------------------------------------------------------------------------------
std::string ParseMCIS(std::vector<unsigned int> &mcis);

void HowToHemorrhage() 
{
  // Create the engine and load the patient
	std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToHemorrhage.log");
  bg->GetLogger()->Info("HowToHemorrhage");
	if (!bg->LoadState("./states/StandardMale@0s.xml"))
  {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
	HowToTracker tracker(*bg);

	// Create data requests for each value that should be written to the output log as the engine is executing
	// Physiology System Names are defined on the System Objects 
	// defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HemoglobinContent",MassUnit::g);
	
  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToHemorrhage.txt");
	
	bg->GetLogger()->Info("The patient is nice and healthy");
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
	bg->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");;

	//We are going to create a hemorrhage in two different ways.  One way will be to specify the location and a severity on a scale of 0-1.
	//The other way will be to parse an injury code and derive the location and severity
	//Set the choice variable below either to 1 to run with location/severity or to 2 to run with injury code 
	int choice = 2;

	//Create variables for scenario
	SEHemorrhage hemorrhageSpleen;			//hemorrhage object
	std::string location;					//location of hemorrhage, valid options are "Major Artery", "Vena Cava", "Head", "Myocardium", "Lung", "Spleen", "Splanchnic", "Small Intestine", "Large Intestine", "Kidney", "Liver", "Arm", "Leg"
	double severity;						//severity (scale 0-1)
	std::vector<unsigned int> mcisCode;		//injury code if using option 2, see ParseMCIS method below for more details
	//Let's create an internal hemorrhage in the spleen (maybe it ruptured...)
	switch (choice) {
	case 1:
		location = "Spleen";
		severity = 0.8;
		break;
	case 2:
		mcisCode = { 4,2,8,2,0 };			//This injury code is a high severity hemorrhage in the spleen
		severity = mcisCode[0] / 5.0;		//Digit 1 of mcis is severity on 0-5 scale.  Convert it to a 0-1 scale
		location = ParseMCIS(mcisCode);
		break;
	}
	//Set up hemorrhage with the location and severity info
	hemorrhageSpleen.SetCompartment(location);
	hemorrhageSpleen.GetSeverity().SetValue(severity);
	hemorrhageSpleen.SetBleedPath();		//This is needed to tell engine which circuit pathway to set the hemorrhage on
	
	// Hemorrhage Starts - instantiate a hemorrhage action and have the engine process it.  Note that BioGears will output the injury code regardless of which method was used
	bg->ProcessAction(hemorrhageSpleen);

	// Advance some time to let the body bleed out a bit
	tracker.AdvanceModelTime(300);

	bg->GetLogger()->Info("The patient has been hemorrhaging for 300s");
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
	bg->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");;

	//Assume that the hemorrhage has been stopped somehow.  We do this by setting the severity of our hemorrhage object to 0
	hemorrhageSpleen.GetSeverity().SetValue(0);
	//Process update to hemorrhage action
	bg->ProcessAction(hemorrhageSpleen);
	
	
	// Advance some time while the medic gets the drugs ready
	tracker.AdvanceModelTime(100);

	bg->GetLogger()->Info("The patient has NOT been hemorrhaging for 100s");
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
	bg->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");;

	// Patient is stabilizing, but not great

	// Let's administer a saline drip, we need to get saline from the substance maganer
	SESubstanceCompound* saline = bg->GetSubstanceManager().GetCompound("Saline");
	SESubstanceCompoundInfusion iVSaline(*saline);
	iVSaline.GetBagVolume().SetValue(500,VolumeUnit::mL);//the total volume in the bag of Saline
	iVSaline.GetRate().SetValue(100,VolumePerTimeUnit::mL_Per_min);//The rate to admnister the compound in the bag in this case saline
	bg->ProcessAction(iVSaline);

	tracker.AdvanceModelTime(400);

	bg->GetLogger()->Info("The patient has been getting fluids for the past 400s");
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
	bg->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");;
  bg->GetLogger()->Info("Finished");
}

std::string ParseMCIS(std::vector<unsigned int> &mcis)
{
	/*MCIS Code Brief :
	Digit 1 = Severity
	Digit 2 = Body Region(1 = Head, 2 = Torso, 3 = Arms, 4 = Legs, 5 = Multiple(not currently supported)
	Digit 3 = Subregion(not required for arms or legs)
	In Head : 6 = Vessels
	In Torso : 6 = Vessels, 7 = Chest, 8 = Abdomen, 9 = Pelvis(not currently supported)
	Digit 4
	In Vessels : 1 = Intracranial (if in head), 4 = Major Artery (if in torso), 6 = "Vena Cava" (if in torso)
	In Chest : 1 = Lungs, 2 = Heart In Abdomen : 1 = Liver, 2 = Spleen, 3 = Pancreas(Splanchnic), 4 = Kidney, 5 = SmallIntestine, 6 = LargeIntestine
	Digit 5 : Wound information too specific for BioGears(any number fine, 0 used here) */

	std::string comp = "";
	enum region {Head =1, Torso = 2, Arm = 3, Leg = 4};		//This will decide which region to look for compartment based on digit 2 of code
	std::map<std::vector<unsigned int>, std::string> torsoMap;	//There are so many compartments in the torso, it is easier to map them
	//Populate torso map (codes with second digit = 2) so that digits 3-4 of code are key to correct compartment
	torsoMap[{6, 4}] = "Major Artery";
	torsoMap[{6, 6}] = "Vena Cava";
	torsoMap[{7, 1}] = "Lung";
	torsoMap[{7, 2}] = "Myocardium";
	torsoMap[{8, 1}] = "Liver";
	torsoMap[{8, 2}] = "Spleen";
	torsoMap[{8, 3}] = "Splanchnic";
	torsoMap[{8, 4}] = "Kidney";
	torsoMap[{8, 5}] = "Small Intestine";
	torsoMap[{8, 6}] = "Large Intestine";

	int caseKey = mcis[1];		//Need 2nd digit of mcis code to decide in which region to place hemorrhage

	switch (caseKey) {
	case Head:
		comp = "Head";
		break;
	case Torso:
		if (torsoMap.find({ mcis.begin() + 2,mcis.end() - 1 }) != torsoMap.end())  //Check to see if subvector made from digits 3-4 is in map.
			comp = torsoMap[{mcis.begin() + 2, mcis.end() - 1}];  // If yes, get compartment that goes with these digits
		else
			comp = "Major Artery";	//If no, we messed up somewhere and we'll put it on the artery so that the sim doesn't crash
		break;
	case Arm:
		comp = "Arm";
		break;
	case Leg:
		comp = "Leg";
		break;
	default:
		comp = "Major Artery";	//Default to artery in case anything goes wrong
	}


	return comp;
}
