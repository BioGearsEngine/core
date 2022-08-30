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

#include "HowTo-SepsisDynamic.h"

#include <iostream>
#include <stdexcept>

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/string/manipulation.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

using namespace biogears;
//This method uses the Threaded BioGears functionality
int HowToDynamicSepsis()
{
  int infectionState;
  std::cout << "This function loads a BioGears engine state generated from an active infection action." << std::endl;
  std::cout << "There are two points of entry to this simulation: " << std::endl;
  std::cout << "\t [1] Early sepsis:  Patient showing signs of systemic inflamamtion (36 hrs post infection action start)" << std::endl;
  std::cout << "\t [2] Shock onset:  Patient will imminently enter shock without treatment (48 hrs post infection action start)" << std::endl;
  std::cout << "Please type 1 or 2 to begin your simulation, followed by ENTER" << std::endl;
  std::cin >> infectionState;

  // Create the engine and load the infection state selected:  1 = InfectionSevere_36hr, 2 = InfectionSevere_48hr
  // This call will block while the engine stabilizes
  std::unique_ptr<DynamicSepsis> sepThread;
  
  try{
    sepThread = std::make_unique<DynamicSepsis>("./DynamicSepsis.log", infectionState);
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  // When it comes back, the engine will be running, waiting for your input

  int action;
  int fluid;
  bool active = true;
  double rate;
  double concentration;
  double mic;

  do {
    sepThread->GetLogger()->Info("Enter Integer for Action to Perform : \n\t[1] Status \n\t[2] IVFluids \n\t[3] Antibiotic (Piperacillin/Tazobactam) Admin \n\t[4] Norepinephrine Admin \n\t[5] Update bacteria minimum inhibitory concentration (MIC) \n\t[6] Quit \n");
    std::cin >> action;
    switch (action) {
    case 1:
      sepThread->Status();
      break;
    case 2:
      sepThread->GetLogger()->Info("Type 1 for Saline or 2 for Ringers Lactate, followed by ENTER : ");
      std::cin >> fluid;
      if (fluid < 1 || fluid > 2) {
        sepThread->GetLogger()->Info("Invalid selection, select 1 or 2 \n");
        break;
      }
      sepThread->GetLogger()->Info("Enter IV Fluids Rate in mL/min (bag volume is 500 mL), followed by ENTER : ");
      std::cin >> rate;
      sepThread->SetIVFluids(fluid, rate);
      break;
    case 3:
      sepThread->GetLogger()->Info("Administering piperacillin/tazobactam (4.5 g over 30 min \n");
      sepThread->SetAntibiotic();
      break;
    case 4:
      sepThread->GetLogger()->Info("Enter a concentration in ug/mL (will be delivered at 1 mL/min), followed by ENTER : ");
      std::cin >> concentration;
      sepThread->SetNorepinephrine(concentration);
      break;
    case 5:
      sepThread->GetLogger()->Info("Default bactera MIC is 8.0 mg/L.  Increase or decrease bacteria resistance to antibiotics by entering a new MIC (in mg/L), followed by ENTER : ");
      std::cin >> mic;
      sepThread->UpdateMIC(mic);
      break;
    case 6:
      active = false;
      break;
    default:
      sepThread->GetLogger()->Info("Not a valid choice \n");
    }
  } while (active);
  return 0;
}

DynamicSepsis::DynamicSepsis(const std::string& logfile, int infectionInput)
  : m_sepsisThread()
{
  //Create the engine
  m_bg = std::make_unique<BioGearsEngine>(logfile);
  //Load appropriate patient state
  std::string patientState = "./states/InfectionStates/SevereInfection";
  if (infectionInput == 1) {
    patientState += "@36hr.xml";
    m_StartTime_min = 2160.0;
  } else {
    patientState += "@48hr.xml";
    m_StartTime_min = 2880.0;
  }
  if (!m_bg->LoadState(patientState)) {
    m_bg->GetLogger()->Error("Could not load state, check to make sure it exists");
    throw std::runtime_error("Could not load state, check to make sure it exists");
  }

  //Create CSV results file and set up data that we want to be tracked (tracking done in AdvanceModelTime)
  m_bg->GetEngineTrack().GetDataRequestManager().SetResultsFilename("HowToSepsisDynamic.csv");
  //We do not need to add any data requests to the manager because they have already been initialized in the simulations that generated the septic patient states.


  //Load substances we might use
  SESubstanceCompound* antibiotic = m_bg->GetSubstanceManager().GetCompound("PiperacillinTazobactam");
  SESubstanceCompound* saline = m_bg->GetSubstanceManager().GetCompound("Saline");
  SESubstanceCompound* ringers = m_bg->GetSubstanceManager().GetCompound("RingersLactate");
  SESubstance* norepinephrine = m_bg->GetSubstanceManager().GetSubstance("Norepinephrine");
  // Create and initialize our actions (infusion constructed in BioGearsThread)
  m_antibiotic = new SESubstanceCompoundInfusion(*antibiotic);
  m_saline = new SESubstanceCompoundInfusion(*saline);
  m_ringers = new SESubstanceCompoundInfusion(*ringers);
  m_pressor = new SESubstanceInfusion(*norepinephrine);

  m_runThread = true;
  m_sepsisThread = std::thread(&DynamicSepsis::AdvanceTime, this);
}

DynamicSepsis::~DynamicSepsis()
{
  m_runThread = false;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  SAFE_DELETE(m_antibiotic);
  SAFE_DELETE(m_pressor);
  SAFE_DELETE(m_saline);
  SAFE_DELETE(m_ringers);
}

void DynamicSepsis::SetAntibiotic()
{
  //Pip/Taz substance compound defined so that 20 mL vial has 4.0 g Pip (0.2 g/mL) and 0.5 g Taz (0.025 g/mL)
  m_antibiotic->GetBagVolume().SetValue(20.0, VolumeUnit::mL);
  m_antibiotic->GetRate().SetValue(0.667, VolumePerTimeUnit::mL_Per_min);
  m_mutex.lock();
  m_bg->ProcessAction(*m_antibiotic);
  m_mutex.unlock();
}

void DynamicSepsis::SetIVFluids(int compound, double rate)
{
  if (compound == 1) {
    m_saline->GetBagVolume().SetValue(500.0, VolumeUnit::mL);
    m_saline->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_min);
    m_mutex.lock();
    m_bg->ProcessAction(*m_saline);
    m_mutex.unlock();
  } else {
    m_ringers->GetBagVolume().SetValue(500.0, VolumeUnit::mL);
    m_ringers->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_min);
    m_mutex.lock();
    m_bg->ProcessAction(*m_ringers);
    m_mutex.unlock();
  }
}

void DynamicSepsis::SetNorepinephrine(double& concentration)
{
  m_pressor->GetRate().SetValue(1.0, VolumePerTimeUnit::mL_Per_min);
  m_pressor->GetConcentration().SetValue(concentration, MassPerVolumeUnit::ug_Per_mL);
  m_mutex.lock();
  m_bg->ProcessAction(*m_pressor);
  m_mutex.unlock();
}

void DynamicSepsis::UpdateMIC(double mic)
{
  m_septicInfection = new SEInfection(); //This will not override current infection because inflammation model is already active
  m_septicInfection->SetLocation("Gut"); //Assume stomach infection--will not change outcome
  m_septicInfection->SetSeverity(CDM::enumInfectionSeverity::Severe); //Again, this will not change model outcome, as inflammation is already active
  m_septicInfection->GetMinimumInhibitoryConcentration().SetValue(mic, MassPerVolumeUnit::mg_Per_L);
  m_mutex.lock();
  m_bg->ProcessAction(*m_septicInfection);
  m_mutex.unlock();
}

void DynamicSepsis::Status()
{ // On demand call to print vitals to the screen
  m_mutex.lock();
  m_bg->GetLogger()->Info("");
  m_bg->GetLogger()->Info(asprintf("Simulation Time  : %f %s", m_bg->GetSimulationTime(TimeUnit::min) - m_StartTime_min, "min"));
  m_bg->GetLogger()->Info(asprintf("Time Since Infection Start  : %f %s", m_bg->GetSimulationTime(TimeUnit::min), "min"));
  m_bg->GetLogger()->Info(asprintf("Blood Volume : %f %s", m_bg->GetCardiovascularSystem().GetBloodVolume(VolumeUnit::mL), "mL"));
  m_bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", m_bg->GetCardiovascularSystem().GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  m_bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", m_bg->GetCardiovascularSystem().GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  m_bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", m_bg->GetCardiovascularSystem().GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  m_bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", m_bg->GetCardiovascularSystem().GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  m_bg->GetLogger()->Info(asprintf("Respiration Rate : %f %s", m_bg->GetRespiratorySystem().GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  m_bg->GetLogger()->Info(asprintf("Mean Urine Output : %f %s", m_bg->GetRenalSystem().GetMeanUrineOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  m_bg->GetLogger()->Info(asprintf("Temperature : %f %s", m_bg->GetEnergySystem().GetCoreTemperature(TemperatureUnit::C), "deg C"));
  m_bg->GetLogger()->Info(asprintf("Blood Lactate : %f %s", m_bg->GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta)->GetSubstanceQuantity(*m_bg->GetSubstanceManager().GetSubstance("Lactate"))->GetMolarity(AmountPerVolumeUnit::mmol_Per_L), "mmol_Per_L"));
  m_bg->GetLogger()->Info(asprintf("Bacteria Count (Blood) : %f", m_bg->GetBloodChemistrySystem().GetInflammatoryResponse().GetBloodPathogen().GetValue()));
  m_bg->GetLogger()->Info(asprintf("Antibiotic Activity : %f", m_bg->GetDrugsSystem().GetAntibioticActivity().GetValue()));

  std::cout << std::endl;
  m_mutex.unlock();
}

void DynamicSepsis::AdvanceTime()
{
  while (m_runThread) {
    m_mutex.lock();
    m_bg->AdvanceModelTime(1.0, TimeUnit::s);
    m_bg->GetEngineTrack().TrackData(m_bg->GetSimulationTime(TimeUnit::s));
    m_mutex.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
  }
}

int main ( int argc, char* argv[] ) {
  return HowToDynamicSepsis();
}

