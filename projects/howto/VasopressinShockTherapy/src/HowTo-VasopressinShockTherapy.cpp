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
#include "HowTo-VasopressinShockTherapy.h"


// Include the various types you will be using in your code
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

using namespace biogears;
void HowToVasopressinShockTherapy()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HemorrhageScenario_Control.log");
  bg->GetLogger()->Info("HemorrhageScenario_Control");

  if (!bg->InitializeEngine("StandardMale.xml")) {
    //See Patients folder for other possible patient configurations
    bg->GetLogger()->Error("Could not load initialize engine, check the error");
    return;
  } else {
    bg->GetLogger()->Info("Engine stabilization complete");
  }

  //The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  //Set up substances.  Initialized vasopressin plasma concentration to 0.  Note that saline is technically a compound--this is
  //so the engine knows to look for multiple components within the same substance file (i.e. Na, Cl, etc)
  SESubstance* vas = bg->GetSubstanceManager().GetSubstance("Vasopressin");
  vas->GetPlasmaConcentration().SetValue(0.0, MassPerVolumeUnit::ug_Per_L);
  SESubstanceCompound* sal = bg->GetSubstanceManager().GetCompound("Saline");

  //Each infusion is managed by a separate object
  //This object is the vasopressin infusion.  It requires a concentration and an admin rate.  The infusion will continue unabated until the
  //rate of infusion is reset to 0.0
  SESubstanceInfusion vasInfuse(*vas);
  vasInfuse.GetConcentration().SetValue(0.75, MassPerVolumeUnit::ug_Per_mL);
  vasInfuse.GetRate().SetValue(3.33, VolumePerTimeUnit::mL_Per_min);

  //This is the saline infusion given to the control group.  It is given a large bag volume to ensure that it will not run out during trial.
  SESubstanceCompoundInfusion controlInfuse(*sal);
  controlInfuse.GetBagVolume().SetValue(2000.0, VolumeUnit::mL);
  controlInfuse.GetRate().SetValue(3.33, VolumePerTimeUnit::mL_Per_min);

  //The hemorrhage object is created below.  Hemorrhages are stopped by setting the bleeding rate to 0.0
  SEHemorrhage venaCavaBleed;
  venaCavaBleed.SetCompartment("VenaCava");
  venaCavaBleed.GetInitialRate().SetValue(350.0, VolumePerTimeUnit::mL_Per_min);
  venaCavaBleed.SetMCIS(); //BioGears supports Military Combat Injury Scale for Hemorrhages, but this is not relevant to current simulation

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*vas, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartStrokeVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("GlomerularFiltrationRate", VolumePerTimeUnit::mL_Per_s);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineProductionRate", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineOsmolality", OsmolalityUnit::mOsm_Per_kg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("GlomerlarFiltrationRate", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CerebralPerfusionPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("IntracranialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystemicVascularResistance", FlowResistanceUnit::mmHg_s_Per_mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("SkinVasculature", "InFlow", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("BrainVasculature", "InFlow", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("MyocardiumVasculature", "InFlow", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("MuscleVasculature", "InFlow", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("SmallIntestineVasculature", "InFlow", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("LeftKidneyVasculature", "InFlow", VolumePerTimeUnit::mL_Per_s);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("Ground", "InFlow", VolumePerTimeUnit::mL_Per_min);

  //Create the text file to output results with all of the data requests called above
  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HemorrhageScenario_Control.csv");

  bool control = true; //Use to flag which variation you want to run

  bg->GetLogger()->Info("Beginning Scenario");

  //Initiate the hemorrhage action and advance engine eight minutes
  bg->ProcessAction(venaCavaBleed);
  bg->AdvanceModelTime(480, TimeUnit::s);

  //Remove bleeding
  venaCavaBleed.GetInitialRate().SetValue(0.0, VolumePerTimeUnit::mL_Per_min);
  bg->ProcessAction(venaCavaBleed);

  //Begin either the control treatment (control = true) or experimental treatment (control = false). Toggle between using boolean above
  if (control) {
    bg->ProcessAction(controlInfuse);
  } else {
    bg->ProcessAction(vasInfuse);
  }

  //Allow fluid administration for 30 minutes
  bg->AdvanceModelTime(1800, TimeUnit::s);

  bg->GetLogger()->Info("Finished");
}
