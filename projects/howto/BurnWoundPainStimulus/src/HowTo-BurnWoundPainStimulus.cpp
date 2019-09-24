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
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a burn wound to a patient
///
/// \details
/// Refer to the SEBurnWoundData class
/// 
//--------------------------------------------------------------------------------------------------
void HowToBurnWoundPainStimulus()
{
  // Create the engine and load the patient
	std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToBurnWoundPainStimulus.log");
  bg->GetLogger()->Info("HowToBurnWoundPainStimulus");
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
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("NeuromuscularBlockLevel");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToBurnWoundPainStimulus.csv");
  // Capture initial data for healthy patient
	bg->GetLogger()->Info("The patient is nice and healthy");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

	tracker.AdvanceModelTime(50);
  // Apply serious burn wound to patient
  SEBurnWound burnWound;
  burnWound.GetTotalBodySurfaceArea().SetValue(0.33);
  bg->ProcessAction(burnWound);

	tracker.AdvanceModelTime(3600);

	bg->GetLogger()->Info("The patient suffered a burn wound one hour ago");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  // Administer RingersLactate
  SESubstanceCompound* rl = bg->GetSubstanceManager().GetCompound("RingersLactate");
  SESubstanceCompoundInfusion infuse{*rl};
  infuse.GetBagVolume().SetValue(1.0, VolumeUnit::L);
  infuse.GetRate().SetValue(330.0, VolumePerTimeUnit::mL_Per_hr);
  bg->ProcessAction(infuse);

  tracker.AdvanceModelTime(3600);

  infuse.GetRate().SetValue(50.0, VolumePerTimeUnit::mL_Per_hr);
  bg->ProcessAction(infuse);

	bg->GetLogger()->Info("The patient was administered RingersLactate one hour ago");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;
  bg->GetLogger()->Info("Finished");
}
