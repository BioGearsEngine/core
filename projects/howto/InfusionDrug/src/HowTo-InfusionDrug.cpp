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
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>

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
	if (!bg->LoadState("./states/StandardMale@0s.xml"))
  {
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

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("VasopressinPD.txt");

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
}
