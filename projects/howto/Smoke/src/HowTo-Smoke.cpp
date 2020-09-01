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
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a smokey environment to the patient
///
/// \details
/// Refer to the SEEnvironmentChange class
//--------------------------------------------------------------------------------------------------
void HowToSmoke()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToSmoke.log");
  bg->GetLogger()->Info("HowToSmoke");
  /*
  // Smoke is made up of many things.
  // You will need to add 2 things to the environement to effectively model a smokey environment
  // A solid particle substance, and CarbonMonoxide
  // You can create your own environment file with these, the following would work

  SEInitialEnvironment ienv(bg->GetSubstanceManager());
  ienv.SetConditionsFile("./environments/CheyenneMountainFireFighter.xml");
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
  SESubstance* CO = bg->GetSubstanceManager().GetSubstance("CarbonMonoxide");
  SESubstance* Particulate = bg->GetSubstanceManager().GetSubstance("ForestFireParticulate");

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
  bg->AdvanceModelTime(5, TimeUnit::s);

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "CarbonDioxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Carbon Monoxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonMonoxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Pulse Oximetry : " << bg->GetBloodChemistrySystem()->GetPulseOximetry());
  // There are liquid compartments for each of the gas pulmonary compartments, these track the trasportation of liquid and solid substances through the pulmonary tract, and their deposition
  // Currently, since we have not changed the environment there is no Particulate or CO in the system, so the GetSubstanceQuantity call will return nullptr, so keep this commented
  //bg->GetLogger()->Info(std::stringstream() << "Particulate Deposition : " << bg->GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(*Particulate)->GetMassDeposited(MassUnit::ug) << MassUnit::ug);

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

  // Here we will put this healty patient into a smokey environment.
  SEEnvironmentChange envChange(bg->GetSubstanceManager());
  // NOTE YOUR FRACTIONS MUST ADD UP TO 1.0
  envChange.GetConditions().GetAmbientGas(*N2).GetFractionAmount().SetValue(0.79008);
  envChange.GetConditions().GetAmbientGas(*O2).GetFractionAmount().SetValue(0.2095);
  envChange.GetConditions().GetAmbientGas(*CO2).GetFractionAmount().SetValue(4.0E-4);
  envChange.GetConditions().GetAmbientGas(*CO).GetFractionAmount().SetValue(2.0E-5);
  // Concentrations are independent and do not need to add up to 1.0
  envChange.GetConditions().GetAmbientAerosol(*Particulate).GetConcentration().SetValue(2.9, MassPerVolumeUnit::mg_Per_m3);
  bg->ProcessAction(envChange);
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "CarbonDioxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Carbon Monoxide Saturation : " << bg->GetBloodChemistrySystem()->GetCarbonMonoxideSaturation());
  bg->GetLogger()->Info(std::stringstream() << "Pulse Oximetry : " << bg->GetBloodChemistrySystem()->GetPulseOximetry());
  // There are liquid compartments for each of the gas pulmonary compartments, these track the trasportation of liquid and solid substances through the pulmonary tract, and their deposition
  bg->GetLogger()->Info(std::stringstream() << "Particulate Deposition : " << bg->GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(*Particulate)->GetMassDeposited(MassUnit::ug) << MassUnit::ug);

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

  // Here is the amount of particulate

  bg->GetLogger()->Info("Finished");
}
