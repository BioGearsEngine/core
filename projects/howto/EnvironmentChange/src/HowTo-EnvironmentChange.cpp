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
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying an Environment Change to the patient
///
/// \details
/// Refer to the SEEnvironmentChange class
/// Refer to the SEEnvironmentHeating class
//--------------------------------------------------------------------------------------------------
void HowToEnvironmentChange()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToEnvironmentChange.log");
  bg->GetLogger()->Info("HowToEnvironmentChange");
  /*
  // You have the option to change the enviornmental conditions of the patient 
  // with a condition or an action. By default the standard environment file is used
  // when stabilizing the patient, all initial states provided are with that environment.
  // You will need to initialize the engine to this condition
  // You could then save out your own state and use it in the future

  SEInitialEnvironment ienv(bg->GetSubstanceManager());
  ienv.SetConditionsFile("./environments/AnchorageDecember.xml");
  // You can set a file or the conditions object just like is shown below
  std::vector<const SECondition*> conditions;
  conditions.push_back(&ienv);

  if (!bg->InitializeEngine("StandardMale.xml", &conditions))
  {
    bg->GetLogger()->Error("Could not load initialize engine, check the error");
    return;
  }
  */

  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Get some substances out we will use
  SESubstance* N2 = bg->GetSubstanceManager().GetSubstance("Nitrogen");
  SESubstance* O2 = bg->GetSubstanceManager().GetSubstance("Oxygen");
  SESubstance* CO2 = bg->GetSubstanceManager().GetSubstance("CarbonDioxide");

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SkinTemperature", TemperatureUnit::C);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CoreTemperature", TemperatureUnit::C);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalMetabolicRate", PowerUnit::W);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystemicVascularResistance", FlowResistanceUnit::mmHg_s_Per_mL);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToEnvironmentChange.csv");

  // Advance some time to get some resting data
  bg->AdvanceModelTime(50, TimeUnit::s);

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Skin Temperature : " << bg->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Core Temperature : " << bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Total Metabolic Rate : " << bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W) << PowerUnit::W);
  bg->GetLogger()->Info(std::stringstream() << "Systemic Vascular Resistance : " << bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL) << FlowResistanceUnit::mmHg_s_Per_mL);
  ;

  // EnvironmentChange Starts - instantiate a EnvironmentChange action and have the engine process it
  // The patient is instantly submerged in 10 degree Celsius water.
  // This causes an immediate drop in the skin temperature due to increased convective heat transfer from the skin surface.
  // The core temperature follows the skin temperature, and the metabolic rate increases due to shivering.
  SEEnvironmentChange env(bg->GetSubstanceManager());
  SEEnvironmentalConditions& conditions = env.GetConditions();
  conditions.SetSurroundingType(CDM::enumSurroundingType::Water);
  conditions.GetAirVelocity().SetValue(0, LengthPerTimeUnit::m_Per_s);
  conditions.GetAmbientTemperature().SetValue(10.0, TemperatureUnit::C);
  conditions.GetAtmosphericPressure().SetValue(760., PressureUnit::mmHg);
  conditions.GetClothingResistance().SetValue(0.01, HeatResistanceAreaUnit::clo);
  conditions.GetEmissivity().SetValue(0.0);
  conditions.GetMeanRadiantTemperature().SetValue(22.0, TemperatureUnit::C);
  conditions.GetRelativeHumidity().SetValue(1.0);
  conditions.GetRespirationAmbientTemperature().SetValue(22.0, TemperatureUnit::C);
  conditions.GetAmbientGas(*N2).GetFractionAmount().SetValue(0.7901);
  conditions.GetAmbientGas(*O2).GetFractionAmount().SetValue(0.2095);
  conditions.GetAmbientGas(*CO2).GetFractionAmount().SetValue(4.0E-4);
  bg->ProcessAction(env);
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Skin Temperature : " << bg->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Core Temperature : " << bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Total Metabolic Rate : " << bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W) << PowerUnit::W);
  bg->GetLogger()->Info(std::stringstream() << "Systemic Vascular Resistance : " << bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL) << FlowResistanceUnit::mmHg_s_Per_mL);
  ;

  // The patient is removed from the water and returns to the normal environment.
  // The skin temperature rises, leading to a subsequent rise in core temperature
  conditions.SetSurroundingType(CDM::enumSurroundingType::Air);
  conditions.GetAirVelocity().SetValue(0.1, LengthPerTimeUnit::m_Per_s);
  conditions.GetAmbientTemperature().SetValue(22.0, TemperatureUnit::C);
  conditions.GetAtmosphericPressure().SetValue(760., PressureUnit::mmHg);
  conditions.GetClothingResistance().SetValue(0.01, HeatResistanceAreaUnit::clo);
  conditions.GetEmissivity().SetValue(0.95);
  conditions.GetMeanRadiantTemperature().SetValue(22.0, TemperatureUnit::C);
  conditions.GetRelativeHumidity().SetValue(0.6);
  conditions.GetRespirationAmbientTemperature().SetValue(19, TemperatureUnit::C);
  conditions.GetAmbientGas(*N2).GetFractionAmount().SetValue(0.7896);
  conditions.GetAmbientGas(*O2).GetFractionAmount().SetValue(0.21);
  conditions.GetAmbientGas(*CO2).GetFractionAmount().SetValue(4.0E-4);
  bg->ProcessAction(env);
  bg->AdvanceModelTime(60, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Skin Temperature : " << bg->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Core Temperature : " << bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Total Metabolic Rate : " << bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W) << PowerUnit::W);
  bg->GetLogger()->Info(std::stringstream() << "Systemic Vascular Resistance : " << bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL) << FlowResistanceUnit::mmHg_s_Per_mL);
  bg->GetLogger()->Info("");

  //The patient’s skin temperature continues to rise as they are actively heated at 340 btu/hr. Both core temperature and skin
  // temperature increase as a result of this.  The patient’s metabolic rate has returned to normal since shivering has ceased.
  SEThermalApplication heat;
  heat.GetActiveHeating().GetPower().SetValue(340, PowerUnit::BTU_Per_hr);
  bg->ProcessAction(heat);
  bg->AdvanceModelTime(120, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Skin Temperature : " << bg->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Core Temperature : " << bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  bg->GetLogger()->Info(std::stringstream() << "Total Metabolic Rate : " << bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W) << PowerUnit::W);
  bg->GetLogger()->Info(std::stringstream() << "Systemic Vascular Resistance : " << bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL) << FlowResistanceUnit::mmHg_s_Per_mL);
  ;
  bg->GetLogger()->Info("Finished");
}
