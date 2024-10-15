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
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>

#include <sstream>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a ConsumeNutrients insult to the patient
///
/// \details
/// Refer to the SEConsumeNutrients class
//--------------------------------------------------------------------------------------------------
int HowToConsumeNutrients()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToConsumeNutrients.log");
  bg->GetLogger()->Info("HowToConsumeNutrients");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
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
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("StomachContents-Water", VolumeUnit::mL);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToConsumeNutrients.csv");

  // Advance some time to get some resting data
  bg->AdvanceModelTime(50, TimeUnit::s);

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Blood Volume : %f %s", bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL), "mL"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Stomach Water : %f %s", bg->GetGastrointestinalSystem()->GetStomachContents()->GetWater(VolumeUnit::mL), "mL"));
  ;

  // ConsumeNutrients Starts - instantiate a ConsumeNutrients action and have the engine process it
  // Have some water
  SEConsumeNutrients eat;
  eat.GetNutrition().GetWater().SetValue(50, VolumeUnit::mL);
  bg->ProcessAction(eat);

  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Blood Volume : %f %s", bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL), "mL"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Stomach Water : %f %s", bg->GetGastrointestinalSystem()->GetStomachContents()->GetWater(VolumeUnit::mL), "mL"));
  ;
  bg->GetLogger()->Info("Finished");
  return 0;
}

int main ( int argc, char* argv[] ) {
  return HowToConsumeNutrients();
}

