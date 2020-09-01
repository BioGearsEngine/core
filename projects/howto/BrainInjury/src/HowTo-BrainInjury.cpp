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
#include <biogears/cdm/patient/actions/SEBrainInjury.h>
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


int GlasgowEstimator(double);

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for the Brain Injury Patient Insult
/// An acute, non-localized traumatic brain injury that results in increased flow resistance in the brain
///
/// \details
/// Refer to the SEBrainInjury class
//--------------------------------------------------------------------------------------------------
void HowToBrainInjury()
{
  std::stringstream ss;
  // Create a BioGears Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToBrainInjury.log");

  bg->GetLogger()->Info("HowToBrainInjury");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalLungVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToBrainInjury.csv");

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << bg->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << bg->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min)));
  // You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated.
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  bg->AdvanceModelTime(30, TimeUnit::s);

  // Create an SEBrainInjury object
  // Set the severity (a fraction between 0 and 1; for maximal injury, use 1.)
  SEBrainInjury tbi;
  tbi.SetType(CDM::enumBrainInjuryType::Diffuse); // Can also be LeftFocal or RightFocal, and you will get pupillary effects in only one eye
  tbi.GetSeverity().SetValue(0.2);
  bg->ProcessAction(tbi);
  bg->GetLogger()->Info("Giving the patient a brain injury.");

  // Advance time to see how the injury affects the patient
  bg->AdvanceModelTime(90, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "The patient has had a brain injury for 90s, not doing well...");
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << bg->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << bg->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min)));
  // You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated.
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  // You can remove a brain injury by setting the severity to 0, this will instantly remove the flow resistance in the brain, and the patient will recover.
  tbi.GetSeverity().SetValue(0.0);
  bg->ProcessAction(tbi);

  bg->GetLogger()->Info("Removing the brain injury.");

  bg->AdvanceModelTime(90, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "The patient's brain injury has been removed for 90s; patient is much better");
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << bg->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << bg->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min))); // You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated.
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  // A more severe injury has more pronounced effects
  tbi.GetSeverity().SetValue(1);
  bg->ProcessAction(tbi);
  bg->GetLogger()->Info("Giving the patient a severe brain injury.");

  bg->AdvanceModelTime(300, TimeUnit::s);

  // You can also get information from the compartment rather than the system, in case you want other metrics
  const SELiquidCompartment* brain = bg->GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Brain);

  bg->GetLogger()->Info(std::stringstream() << "The patient has had a severe brain injury for 5 minutes");
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << brain->GetPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << bg->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << brain->GetInFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(bg->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min))); // You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated.
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  bg->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << bg->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  bg->GetLogger()->Info("Finished");
}

// The Glasgow Coma Scale (GCS) is commonly used to classify patient consciousness after traumatic brain injury.
// The scale runs from 3 to 15, with brain injury classified as:
// Severe (GCS < 9)
// Moderate (GCS 9-12)
// Mild (GCS > 12)
// Since Cerebral Blood Flow directly influences the amount of oxygen delivered to the brain, we can tie CBF to
// an expected GCS outcome. CBF below 8 mL per 100 g of brain tissue per minute results in infarction. CBF between
// 8 and 20 mL per 100 g of brain tissue per minute is the "penumbra" where recovery is possible if treatment is
// quick. From 20 to the normal range of 50 to 65 mL per 100 g of brain tissue per minute is the range where
// irreversible damage isn't as likely. Thus, we can estimate a GCS value using CBF (estimated using the 1450 g
// StandardMale brain):
//
// CBF Value (mL/min)     GCS score
// ------------------     ----------
// <116                   3
// <151                   4
// <186                   5
// <220                   6
// <255                   7
// <290                   8
// <363                   9
// <435                   10
// <508                   11
// <580                   12
// <628                   13
// <725                   14
// 725-943                15
// Note that in Biogears, CBF is pulsatile
int GlasgowEstimator(double cbf)
{
  if (cbf < 116)
    return 3;
  else if (cbf < 151)
    return 4;
  else if (cbf < 186)
    return 5;
  else if (cbf < 220)
    return 6;
  else if (cbf < 255)
    return 7;
  else if (cbf < 290)
    return 8;
  else if (cbf < 363)
    return 9;
  else if (cbf < 435)
    return 10;
  else if (cbf < 508)
    return 11;
  else if (cbf < 580)
    return 12;
  else if (cbf < 628)
    return 13;
  else if (cbf < 725)
    return 14;
  else
    return 15;
}
