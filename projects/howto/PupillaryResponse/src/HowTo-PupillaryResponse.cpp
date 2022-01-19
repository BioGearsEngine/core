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
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/properties/SEScalarOsmolarity.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for adminstering a substance to the patient via a continuous infusion
///
/// \details
/// Refer to the SESubstanceInfusion class
/// Refer to the SESubstanceManager class
//--------------------------------------------------------------------------------------------------
int HowToPupillaryResponse()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("PupillaryResponseExample.log");
  bg->GetLogger()->Info("PupillaryResponseExample");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step

  SESubstance* morphine = bg->GetSubstanceManager().GetSubstance("Morphine");
  morphine->GetPlasmaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_L);

  // Create a substance bolus action to administer the substance
  SESubstanceInfusion infuse(*morphine);
  infuse.GetConcentration().SetValue(0.102, MassPerVolumeUnit::ug_Per_mL);
  infuse.GetRate().SetValue(0.1, VolumePerTimeUnit::mL_Per_min);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*morphine, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineProductionRate", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineOsmolarity", OsmolarityUnit::mOsm_Per_L);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("PupillaryResponseExample.csv");

  bg->GetLogger()->Info("Beginning Pupillary response Scenario");

  bg->ProcessAction(infuse);
  bg->AdvanceModelTime(60, TimeUnit::s);

  infuse.GetConcentration().SetValue(0.292, MassPerVolumeUnit::ug_Per_mL);
  bg->ProcessAction(infuse);
  bg->AdvanceModelTime(60, TimeUnit::s);

  infuse.GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  bg->ProcessAction(infuse);
  bg->AdvanceModelTime(60, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Has Left Pupillary Response %s", (bg->GetNervousSystem()->HasLeftEyePupillaryResponse()) ? "true" : "false"));
  bg->GetLogger()->Info(asprintf("Has Right Pupillary Response %s", (bg->GetNervousSystem()->HasRightEyePupillaryResponse()) ? "true" : "false"));

  bg->GetLogger()->Info("Finished");
  return 0;
}

int main(int argc, char* argv[])
{
  return HowToPupillaryResponse();
}
