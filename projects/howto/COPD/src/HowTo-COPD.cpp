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
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a COPD condition to the patient
///
/// \details
/// Refer to the SEChronicObstructivePulmonaryDisease class
//--------------------------------------------------------------------------------------------------
int HowToCOPD()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToCOPD.log");
  bg->GetLogger()->Info("HowToCOPD");

  // Since this is a condition, we do not provide a starting state
  // You will need to initialize the engine to this condition
  // You could then save out your own state and use it in the future

  SEChronicObstructivePulmonaryDisease COPD;
  COPD.GetBronchitisSeverity().SetValue(0.5);
  COPD.GetEmphysemaSeverity().SetValue(0.7);
  std::vector<const SECondition*> conditions;
  conditions.push_back(&COPD);

  if (!bg->InitializeEngine("StandardMale.xml", &conditions)) {
    bg->GetLogger()->Error("Could not load initialize engine, check the error");
    return 1 ;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HemoglobinContent", MassUnit::g);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("InspiratoryExpiratoryRatio");
  bg->GetEngineTrack()->GetDataRequestManager().CreateGasCompartmentDataRequest().Set(BGE::PulmonaryCompartment::Trachea, "InFlow");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToCOPD.csv");

  // Advance some time to get some data
  bg->AdvanceModelTime(500, TimeUnit::s);

  bg->GetLogger()->Info("The patient is not very healthy");
  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Hemoglobin Content : %f %s", bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g), "g"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("InspiratoryExpiratoryRatio : %f", bg->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio()));
  bg->GetLogger()->Info(asprintf("Carina InFlow : %f %s", bg->GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Trachea)->GetInFlow(VolumePerTimeUnit::L_Per_s), "L_Per_s"));
  
  bg->GetLogger()->Info("Finished");
  return 0;
}

int main ( int argc, char* argv[] ) {
  return HowToCOPD();
}

