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



// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a Hemorrhage insult to the patient and also demonstrate how injury code can be used to initiate a hemorrhage if desired
///
/// \details
/// Refer to the SEHemorrhage class
/// Refer to the SESubstanceManager class
/// Refer to the SESubstanceIVFluids class for applying an IV to the patient
//--------------------------------------------------------------------------------------------------
void ParseMCIS(SEHemorrhage& hem, std::vector<unsigned int>& mcis);

void HowToHemorrhage()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToHemorrhage.log");
  bg->GetLogger()->Info("HowToHemorrhage");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HemoglobinContent", MassUnit::g);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToHemorrhage.csv");

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  ;

  //We are going to create a hemorrhage in two different ways.  One way will be to specify the location and a severity on a scale of 0-1.
  //The other way will be to parse an injury code and derive the location and severity
  //Set the choice variable below either to 1 to run with location/severity or to 2 to run with injury code
  int choice = 1;

  //Create variables for scenario
  SEHemorrhage hemorrhageSpleen; //hemorrhage object
  std::string location; //location of hemorrhage, valid options are "Aorta", "VenaCava", "Brain", "Myocardium", "LeftLung", "RightLung", "Spleen", "Splanchnic", "SmallIntestine", "LargeIntestine", "LeftKidney", "RightKidney", "Liver", "LeftArm", "RightArm", "LeftLeg", "RightLeg"
  double rate_mL_Per_min = 150.0; //the initial bleeding rate of the hemorrhage
  std::vector<unsigned int> mcisCode; //injury code if using option 2, see ParseMCIS method below for more details
  //Let's create an internal hemorrhage in the spleen (maybe it ruptured...)
  switch (choice) {
  case 1:
    location = "Spleen";
    hemorrhageSpleen.SetCompartment(location);
    hemorrhageSpleen.GetInitialRate().SetValue(rate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
    hemorrhageSpleen.SetMCIS();
    break;
  case 2:
    mcisCode = { 4, 2, 8, 2, 0 }; //This injury code is a high severity hemorrhage in the spleen
    ParseMCIS(hemorrhageSpleen, mcisCode);
    break;
  }

  // Hemorrhage Starts - instantiate a hemorrhage action and have the engine process it.  Note that BioGears will output the injury code regardless of which method was used
  bg->ProcessAction(hemorrhageSpleen);

  // Advance some time to let the body bleed out a bit
  bg->AdvanceModelTime(300, TimeUnit::s);

  bg->GetLogger()->Info("The patient has been hemorrhaging for 300s");
  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  ;

  //Assume that the hemorrhage has been stopped somehow.  We do this by setting the severity of our hemorrhage object to 0
  hemorrhageSpleen.GetInitialRate().SetValue(0.0, VolumePerTimeUnit::mL_Per_min);
  //Process update to hemorrhage action
  bg->ProcessAction(hemorrhageSpleen);

  // Advance some time while the medic gets the drugs ready
  bg->AdvanceModelTime(100, TimeUnit::s);

  bg->GetLogger()->Info("The patient has NOT been hemorrhaging for 100s");
  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  ;

  // Patient is stabilizing, but not great

  // Let's administer a saline drip, we need to get saline from the substance maganer
  SESubstanceCompound* saline = bg->GetSubstanceManager().GetCompound("Saline");
  SESubstanceCompoundInfusion iVSaline(*saline);
  iVSaline.GetBagVolume().SetValue(500, VolumeUnit::mL); //the total volume in the bag of Saline
  iVSaline.GetRate().SetValue(100, VolumePerTimeUnit::mL_Per_min); //The rate to admnister the compound in the bag in this case saline
  bg->ProcessAction(iVSaline);

  bg->AdvanceModelTime(400, TimeUnit::s);

  bg->GetLogger()->Info("The patient has been getting fluids for the past 400s");
  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  ;
  bg->GetLogger()->Info("Finished");
}

void ParseMCIS(SEHemorrhage& hem, std::vector<unsigned int>& mcis)
{
  //Going to scale flows by 250 mL/min.  This is flow rate that would cause bleed out in 10 minutes assuming blood volume of 5 L
  double flowScale_mL_Per_min = 250.0;
  int sev = mcis[0];
  double initialFlow_mL_Per_min = flowScale_mL_Per_min * mcis[0] / 5.0; //Divide by 5 because mcis is on 1-5 scale
  hem.GetInitialRate().SetValue(initialFlow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);

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
  enum region { Head = 1,
    Torso = 2,
    Arm = 3,
    Leg = 4 }; //This will decide which region to look for compartment based on digit 2 of code
  std::map<std::vector<unsigned int>, std::string> torsoMap; //There are so many compartments in the torso, it is easier to map them
  //Populate torso map (codes with second digit = 2) so that digits 3-4 of code are key to correct compartment
  torsoMap[{ 6, 4 }] = "Aorta";
  torsoMap[{ 6, 6 }] = "VenaCava";
  torsoMap[{ 7, 1 }] = "LeftLung";
  torsoMap[{ 7, 2 }] = "Myocardium";
  torsoMap[{ 8, 1 }] = "Liver";
  torsoMap[{ 8, 2 }] = "Spleen";
  torsoMap[{ 8, 3 }] = "Splanchnic";
  torsoMap[{ 8, 4 }] = "LeftKidney";
  torsoMap[{ 8, 5 }] = "SmallIntestine";
  torsoMap[{ 8, 6 }] = "LargeIntestine";

  int caseKey = mcis[1]; //Need 2nd digit of mcis code to decide in which region to place hemorrhage

  switch (caseKey) {
  case Head:
    comp = "Brain";
    break;
  case Torso:
    if (torsoMap.find({ mcis.begin() + 2, mcis.end() - 1 }) != torsoMap.end()) //Check to see if subvector made from digits 3-4 is in map.
      comp = torsoMap[{ mcis.begin() + 2, mcis.end() - 1 }]; // If yes, get compartment that goes with these digits
    else
      comp = "Aorta"; //If no, we messed up somewhere and we'll put it on the artery so that the sim doesn't crash
    break;
  case Arm:
    comp = "LeftArm";
    break;
  case Leg:
    comp = "LeftLeg";
    break;
  default:
    comp = "Aorta"; //Default to artery in case anything goes wrong
  }

  hem.SetCompartment(comp);
  hem.SetMCIS();
}
