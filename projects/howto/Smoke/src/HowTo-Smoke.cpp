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
#include <biogears/string/manipulation.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a smokey environment to the patient
///
/// \details
/// Refer to the SEEnvironmentChange class
//--------------------------------------------------------------------------------------------------
int HowToSmoke()
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
    return 2;
  }
  */

  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
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
  bg->GetLogger()->Info(asprintf("Oxygen Saturation : %f", bg->GetBloodChemistrySystem()->GetOxygenSaturation()));
  bg->GetLogger()->Info(asprintf("CarbonDioxide Saturation : %f", bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation()));
  bg->GetLogger()->Info(asprintf("Carbon Monoxide Saturation : %f", bg->GetBloodChemistrySystem()->GetCarbonMonoxideSaturation()));
  bg->GetLogger()->Info(asprintf("Pulse Oximetry : %f", bg->GetBloodChemistrySystem()->GetPulseOximetry()));
  // There are liquid compartments for each of the gas pulmonary compartments, these track the trasportation of liquid and solid substances through the pulmonary tract, and their deposition
  // Currently, since we have not changed the environment there is no Particulate or CO in the system, so the GetSubstanceQuantity call will return nullptr, so keep this commented
  //bg->GetLogger()->Info(asprintf("Particulate Deposition : %f %s", bg->GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(*Particulate)->GetMassDeposited(MassUnit::ug), "ug"));

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Skin Temperature Torso : %f %s", bg->GetEnergySystem()->GetSkinTemperatureTorso(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("Skin Temperature Head : %f %s", bg->GetEnergySystem()->GetSkinTemperatureHead(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W"));
  bg->GetLogger()->Info(asprintf("Systemic Vascular Resistance : %f %s", bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL), "mmHg_s_Per_mL"));
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

  bg->GetLogger()->Info(asprintf("Oxygen Saturation : %f", bg->GetBloodChemistrySystem()->GetOxygenSaturation()));
  bg->GetLogger()->Info(asprintf("CarbonDioxide Saturation : %f", bg->GetBloodChemistrySystem()->GetCarbonDioxideSaturation()));
  bg->GetLogger()->Info(asprintf("Carbon Monoxide Saturation : %f", bg->GetBloodChemistrySystem()->GetCarbonMonoxideSaturation()));
  bg->GetLogger()->Info(asprintf("Pulse Oximetry : %f", bg->GetBloodChemistrySystem()->GetPulseOximetry()));
  // There are liquid compartments for each of the gas pulmonary compartments, these track the trasportation of liquid and solid substances through the pulmonary tract, and their deposition
  bg->GetLogger()->Info(asprintf("Particulate Deposition : %f %s", bg->GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(*Particulate)->GetMassDeposited(MassUnit::ug), "ug"));

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Skin Temperature Torso : %f %s", bg->GetEnergySystem()->GetSkinTemperatureTorso(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("Skin Temperature Head : %f %s", bg->GetEnergySystem()->GetSkinTemperatureHead(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W"));
  bg->GetLogger()->Info(asprintf("Systemic Vascular Resistance : %f %s", bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL), "mmHg_s_Per_mL"));

  // Here is the amount of particulate

  bg->GetLogger()->Info("Finished");
  return 0;
}

int main ( int argc, char* argv[] ) {
  return HowToSmoke();
}

