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



#include <biogears/schema/cdm/Properties.hxx>

// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Demonstrates how to set concentrations of ionic compounds in bloodstream and tissue and call events
/// based on changes in concentrations
///
/// \details
/// Refer to the SEEnvironmentChange class
/// Refer to the SEDrug Class
//--------------------------------------------------------------------------------------------------

void HowToFaciculation()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToFasciculation.log");
  bg->GetLogger()->Info("HowToFasciculation");

  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  //---Initialize all variables needed for scenario
  SESubstance* Na = bg->GetSubstanceManager().GetSubstance("Sodium");
  SESubstance* K = bg->GetSubstanceManager().GetSubstance("Potassium");
  SESubstance* Cl = bg->GetSubstanceManager().GetSubstance("Chloride");
  SESubstance* Ca = bg->GetSubstanceManager().GetSubstance("Calcium");

  double monitorTime = 200.0; //how long we're going to be on the look out for ion imbalances in the blood
  CDM::enumOnOff::value lowKActive;
  lowKActive = CDM::enumOnOff::Off;

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("VenaCava", *Na, "Molarity", AmountPerVolumeUnit::mmol_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("VenaCava", *K, "Molarity", AmountPerVolumeUnit::mmol_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("VenaCava", *Cl, "Molarity", AmountPerVolumeUnit::mmol_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("VenaCava", *Ca, "Molarity", AmountPerVolumeUnit::mmol_Per_L);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToFasciculation.csv");

  // Advance some time to get some resting data
  bg->AdvanceModelTime(60, TimeUnit::s);

  bg->GetLogger()->Info("The patient is nice and healthy");
  std::string message = "";
  message = "Increase membrane resistance to potassium";
  bg->GetLogger()->Info(message);
  K->GetMembraneResistance().SetValue(5.0, ElectricResistanceUnit::Ohm);

  while (bg->GetSimulationTime(TimeUnit::s) < monitorTime + 60.0) {
    if (bg->GetPatient().IsEventActive(CDM::enumPatientEvent::MildHypokalemia) && (lowKActive == CDM::enumOnOff::Off)) {
      lowKActive = CDM::enumOnOff::On;
      message = "Patient has low serum potassium, muscle fasciculation may occur";
      bg->GetLogger()->Info(message);
    }
    bg->AdvanceModelTime(10.0, TimeUnit::s);
  }
  message = "Return membrane resistance to potassium to baseline";
  bg->GetLogger()->Info(message);
  K->GetMembraneResistance().SetValue(0.248, ElectricResistanceUnit::Ohm);

  bg->AdvanceModelTime(300, TimeUnit::s);
}
