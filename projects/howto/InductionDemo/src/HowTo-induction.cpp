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

#include "HowTo-induction.h"
#include <iostream>

// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/assessments/SEArterialBloodGasAnalysis.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>
#include <filesystem>


namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
  // To run multiple hemorrhages 
  double lowestBloodFlow_mL_Per_min = 50.0;
  double highestBloodFlow_mL_Per_min = 200.0;
  double flowIncrement = 25.0;
  std::filesystem::path states =  "./states";
  std::string state;

  for (const auto& dirEntry : fs::recursive_directory_iterator(states, std::filesystem::directory_options::skip_permission_denied)) {
    std::cout << dirEntry << std::endl;
    state = dirEntry.path().filename().string();
    std::string logName = "./inductionData/HowToHemorrhagepHValidation";
    logName.append(state);
    logName.append(".log");
    InductionThread induction(logName, state);
    induction.AdministerInduction();
    }
  }
}


using namespace biogears;

InductionThread::InductionThread(const std::string logFile, std::string stateFile)
  : m_inductionThread()
{
  //Create the engine and load patient state
  m_bg = CreateBioGearsEngine(logFile);
  m_bg->GetLogger()->Info(asprintf("Initiating %f %s", logFile, "% hemorrhage flow rate, for patient"));
  std::string statePath = "./states/";
  statePath.append(stateFile);
  m_bg->GetLogger()->Info(statePath);
  if (!m_bg->LoadState(statePath)) {
    m_bg->GetLogger()->Error("Could not load state, check the error");
    throw std::runtime_error("Could not load state, check the error");
  }

  //Create CSV results file and set up data that we want to be tracked (tracking done in AdvanceModelTime)
  std::string resultsFileInduction = std::to_string(dochem);
  std::string resultsFile = "./inductionData/HowToInduction";
  resultsFile.append(resultsFileInduction);
  resultsFile.append(stateFile);
  resultsFile.append(".csv");

  //Load substances and compounds
  SESubstance* vas = m_bg->GetSubstanceManager().GetSubstance("Vasopressin");
  SESubstance* lactate = m_bg->GetSubstanceManager().GetSubstance("Lactate");

  //we will perform a 1:1:1 infusion ration during our mtp
  SESubstanceCompound* ringers = m_bg->GetSubstanceManager().GetCompound("RingersLactate");
  SESubstanceCompound* blood = m_bg->GetSubstanceManager().GetCompound("Blood_ONegative");
  SESubstanceCompound* plasma = m_bg->GetSubstanceManager().GetCompound("Plasma");
  SESubstanceCompound* platelets = m_bg->GetSubstanceManager().GetCompound("PlateletsInfusion");


  //Create infusion and bolus actions
  m_ringers = new SESubstanceCompoundInfusion(*ringers);
  m_blood = new SESubstanceCompoundInfusion(*blood);
  m_plasma = new SESubstanceCompoundInfusion(*plasma);
  m_platelet = new SESubstanceCompoundInfusion(*platelets);
  m_VasopressinBolus = new SESubstanceBolus(*vas);

  //initialize trackers to 0
  m_ivBagVolume_mL = 0.0;
  m_ivBagVolumeBlood_mL = 0.0;
  m_ivBagVolumePlasma_mL = 0.0;
  m_ivBagVolumePlatelet_mL = 0.0;

  m_bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename(resultsFile); //deposits in build/runtime
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", "1/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*vas, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialBloodPH", "unitless");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", "mL/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("InflammatoryResponse-TissueIntegrity");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("EndTidalCarbonDioxideFraction", "unitless");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", "1/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("GlomerularFiltrationRate", "mL/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", "mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystemicVascularResistance", "mmHg s/mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("StrongIonDifference", "mmol/L");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", "mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanUrineOutput", "mL/hr");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PainVisualAnalogueScale");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("VenaCava", *lactate, "Molarity", AmountPerVolumeUnit::mmol_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("Aorta", "OutFlow", VolumePerTimeUnit::mL_Per_min);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*lactate, "Clearance-RenalClearance", VolumePerTimeMassUnit::mL_Per_min_kg);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("totalFluid_mL", m_TotalVolume_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolume_mL", m_ivBagVolume_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolumeBlood_mL", m_ivBagVolumeBlood_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolumePlasma_mL", m_ivBagVolumePlasma_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolumePlatelet_mL", m_ivBagVolumePlatelet_mL);

  m_runThread = true;
}

InductionThread::~InductionThread()
{
  m_runThread = false;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  SAFE_DELETE(m_VasopressinBolus);
  SAFE_DELETE(m_ringers);
  SAFE_DELETE(m_blood);
  SAFE_DELETE(m_plasma);
  SAFE_DELETE(m_platelet);
}

void InductionThread::AdministerVasopressin(double& bolus)
{
  m_VasopressinBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  m_VasopressinBolus->GetConcentration().SetValue(1.0, MassPerVolumeUnit::mg_Per_mL);
  m_VasopressinBolus->GetDose().SetValue(bolus, VolumeUnit::mL);
  m_mutex.lock();
  m_bg->ProcessAction(*m_VasopressinBolus);
  m_mutex.unlock();
}

void InductionThread::SetRingersInfusionRate(double& volume, double& rate)
{
  m_ringers->GetBagVolume().SetValue(volume, VolumeUnit::mL);
  m_ringers->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_hr);
  m_ivBagVolume_mL = volume;
  m_mutex.lock();
  m_bg->ProcessAction(*m_ringers);
  m_mutex.unlock();
}

void InductionThread::SetBloodInfusionRate(double& volume, double& rate)
{
  m_blood->GetBagVolume().SetValue(volume, VolumeUnit::mL);
  m_blood->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_hr);
  m_ivBagVolumeBlood_mL = volume;
  m_mutex.lock();
  m_bg->ProcessAction(*m_blood);
  m_mutex.unlock();
}

void InductionThread::SetPlasmaInfusionRate(double& volume, double& rate)
{
  m_plasma->GetBagVolume().SetValue(volume, VolumeUnit::mL);
  m_plasma->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_min);
  m_ivBagVolumePlasma_mL = volume;
  m_mutex.lock();
  m_bg->ProcessAction(*m_plasma);
  m_mutex.unlock();
}

void InductionThread::SetPlateletInfusionRate(double& volume, double& rate)
{
  m_platelet->GetBagVolume().SetValue(volume, VolumeUnit::mL);
  m_platelet->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_hr);
  m_ivBagVolumePlatelet_mL = volume;
  m_mutex.lock();
  m_bg->ProcessAction(*m_blood);
  m_mutex.unlock();
}

void InductionThread::AdvanceTimeFluids()
{
  m_mutex.lock();
  m_bg->AdvanceModelTime(1.0, TimeUnit::s);
  if (m_blood->HasBagVolume() && m_blood->HasRate()) {
    m_TotalVolume_mL += (m_blood->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
    m_TotalVolumeBlood_mL += (m_blood->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
    m_ivBagVolumeBlood_mL += (-m_blood->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
    if (m_ivBagVolumeBlood_mL < 0.0) {
      m_bg->GetLogger()->Info("blood bag is empty \n");
      m_ivBagVolumeBlood_mL = 0.0;
      m_blood->Clear();
    }
  }
  //repeat for plasma
  if (m_plasma->HasBagVolume() && m_plasma->HasRate()) {
    m_TotalVolume_mL += (m_plasma->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
    m_TotalVolumePlasma_mL += (m_plasma->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
    m_ivBagVolumePlasma_mL += (-m_plasma->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
    if (m_ivBagVolumePlasma_mL < 0.0) {
      m_bg->GetLogger()->Info("blood bag is empty \n");
      m_ivBagVolumePlasma_mL = 0.0;
      m_plasma->Clear();
    }
  }
  m_bg->GetEngineTrack()->GetDataTrack().Probe("totalFluid_mL", m_TotalVolume_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolume_mL", m_ivBagVolume_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("totalFluidBlood_mL", m_TotalVolumeBlood_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolumeBlood_mL", m_ivBagVolumeBlood_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("totalFluidPlasma_mL", m_TotalVolumePlasma_mL);
  m_bg->GetEngineTrack()->GetDataTrack().Probe("bagVolumePlasma_mL", m_ivBagVolumePlasma_mL);
  m_bg->GetEngineTrack()->TrackData(m_bg->GetSimulationTime(TimeUnit::s));
  m_mutex.unlock();
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void InductionThread::Status()
{
  m_mutex.lock();
  m_bg->GetLogger()->Info(asprintf("The patient suffered a hemorrhage wound %f %s", m_bg->GetSimulationTime(TimeUnit::min), " min ago"));
  m_bg->GetLogger()->Info(asprintf("Tidal Volume : %f %s", m_bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL), "mL"));
  m_bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", m_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  m_bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", m_bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  m_bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", m_bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  m_bg->GetLogger()->Info(asprintf("Respiration Rate : %f %s", m_bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  m_bg->GetLogger()->Info(asprintf("Oxygen Saturation : %f", m_bg->GetBloodChemistrySystem()->GetOxygenSaturation()));
  m_bg->GetLogger()->Info(asprintf("Blood Volume: %f %s", m_bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL), "mL"));
  m_bg->GetLogger()->Info(asprintf("Systemic Vascular Resistance : %f %s", m_bg->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL), "mmHg_s_Per_mL"));
  m_bg->GetLogger()->Info(asprintf("Mean Urine Output : %f %s", m_bg->GetRenalSystem()->GetMeanUrineOutput(VolumePerTimeUnit::mL_Per_hr), "mL_Per_hr"));
  if (m_ringers->HasBagVolume()) {
    m_bg->GetLogger()->Info(asprintf("Remaining blood Volume in bag: %f %s", m_ivBagVolumeBlood_mL, "mL"));
  }

  std::cout << std::endl;
  m_mutex.unlock();
}

void AdministerInduction();
{
  const SEPatient& patient = m_bg->GetPatient();
  double weight_kg = patient.GetWeight(MassUnit::kg);
}


//routine to administer fluids with a goal directed therapy for urine ourput
void InductionThread::FluidLoading()
{
  //redblood cell unit volume: 
  double pRBCVolume_mL = 350;

  //plasma unit volume 
  double plasmaVolume_mL = 200;

  //platlet volume
  double platletVolume_mL = 300;

  //rates: 
  double bloodRate_mL_Per_hr = 250;
  double plasmaRate_mL_Per_min = 15;

  //we will activate the protocol when shock index reaches 1:
  double shockIndex = 0.0;

  //shock index requires the heart rate and the systolic pressure: 
  double heartRate_min = m_bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min);
  double systolicPressure_mmHg = m_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);

  //update shock index: 
  shockIndex = heartRate_min / systolicPressure_mmHg;

  //time keeping
  int checkTime_s = 3600;
  int LabTime_s = 1800; // 60 mins
  double volume = 0.0;

  //how long do we want to run for?
  double maxSimTime_hr = 2.0;
  double hemTime_min = 20.0;

  //compute urine production and max fluid requirements, per parkland formula
  const SEPatient& patient = m_bg->GetPatient();
  double weight_kg = patient.GetWeight(MassUnit::kg);

  double initialInfustion_mL_Per_hr = 0.0; //tbsa * 10.0; // Should start at 10*tbsa // (DayLimit_mL / 0.5) / 8.0; //half of the fluid should be loaded in the first 8 hours;
  double initialInfustionAlbumin_mL_Per_hr = 50.0; //tbsa * 10.0; // Should start at 10*tbsa // (DayLimit_mL / 0.5) / 8.0; //half of the fluid should be loaded in the first 8 hours;
  double temp = 0.0;
  bool Labs = true;
  bool fluidOn = false;

  m_bg->GetLogger()->Info(asprintf("Beginning care"));

  while (m_runThread) {
     //pull the data

    //check for bleeding intervention: 
    if (m_bg->GetSimulationTime(TimeUnit::min) > hemTime_min && m_hemorrhage->GetInitialRate().GetValue(VolumePerTimeUnit::mL_Per_min) > ZERO_APPROX) {
      m_hemorrhage->GetInitialRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
      m_bg->ProcessAction(*m_hemorrhage);
    }
    heartRate_min = m_bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min);
    systolicPressure_mmHg = m_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);

    shockIndex = heartRate_min / systolicPressure_mmHg;

    if (shockIndex >= 1.0) {
      fluidOn = true;
    }

    //initial fluids: 
    if (fluidOn && m_ivBagVolumeBlood_mL == 0.0 && m_ivBagVolumePlasma_mL == 0.0) {
      //set 1:1 fluid infusion:
      SetPlasmaInfusionRate(plasmaVolume_mL, plasmaRate_mL_Per_min);
      SetBloodInfusionRate(pRBCVolume_mL, bloodRate_mL_Per_hr);
      m_bg->GetLogger()->Info(asprintf("Beginning Intervention with infusion at %f %s", bloodRate_mL_Per_hr, "mL_Per_hr"));
    }

    // Generate blood gas amount of time
    if (((int)m_bg->GetSimulationTime(TimeUnit::s) + 1) % LabTime_s == 0 && Labs == true) {
        //run our blood gas assesment every hour: 
      m_bg->GetPatientAssessment(bga);
      m_bg->GetLogger()->Info(asprintf("Checked blood/gas Labs"));
    }

    //check status every hour, reset the volume while we are at it
    if (((int)m_bg->GetSimulationTime(TimeUnit::s) + 1) % checkTime_s == 0) {
      Status();
    }

    // make sure that the bag is full
    if (m_ivBagVolumeBlood_mL < 1.0 && fluidOn) {
      m_blood->GetBagVolume().SetValue(pRBCVolume_mL, VolumeUnit::mL);
      m_bg->GetLogger()->Info("blood IV bag is low, refilling bag \n");
      m_bg->ProcessAction(*m_blood);
      m_ivBagVolumeBlood_mL = pRBCVolume_mL; //tracking purposes
    }
    // make sure that the plasma bag is full
    if (m_ivBagVolumePlasma_mL < 1.0 && fluidOn) {
      m_plasma->GetBagVolume().SetValue(plasmaVolume_mL, VolumeUnit::mL);
      m_bg->GetLogger()->Info("blood IV bag is low, refilling bag \n");
      m_bg->ProcessAction(*m_blood);
      m_ivBagVolumePlasma_mL = plasmaVolume_mL; //tracking purposes
    }

    //exit checks:
    if (m_bg->GetPatient().IsEventActive(CDM::enumPatientEvent::IrreversibleState)) {
      //m_bg->GetLogger()->Info(std::stringstream() << "oh no!");
      m_bg->GetLogger()->Info("///////////////////////////////////////////////////////////////");
      m_runThread = false;
    }

    if (m_bg->GetSimulationTime(TimeUnit::hr) > maxSimTime_hr) {
      m_bg->GetLogger()->Info("This simulation has gone on too long");
      m_runThread = false;
    }

    //advance time
    AdvanceTime();
  }
}

