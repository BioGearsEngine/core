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
#define _USE_MATH_DEFINES

#include "HowTo-induction.h"
#include <iostream>

#include <cmath>
#include <math.h>
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
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/patient/actions/SEIntubation.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>
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
  std::filesystem::path states =  "./states";
  std::string state;

  for (const auto& dirEntry : fs::recursive_directory_iterator(states, std::filesystem::directory_options::skip_permission_denied)) {
    std::cout << dirEntry << std::endl;
    state = dirEntry.path().filename().string();
    std::string logName = "./inductionData/HowToHemorrhagepHValidation";
    logName.append(state);
    logName.append(".log");
    InductionThread induction(logName, state);
    induction.FluidLoading();
   }
}


using namespace biogears;

InductionThread::InductionThread(const std::string logFile, std::string stateFile)
  : m_inductionThread()
{
  //Create the engine and load patient state
  int dochem = (int)(2);
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
  SESubstance* fent = m_bg->GetSubstanceManager().GetSubstance("Fentanyl");
  SESubstance* prop = m_bg->GetSubstanceManager().GetSubstance("Propofol");
  SESubstance* roc = m_bg->GetSubstanceManager().GetSubstance("Rocuronium");
  SESubstance* des = m_bg->GetSubstanceManager().GetSubstance("Desflurane");



  //we will perform a 1:1:1 infusion ration during our mtp
  SESubstanceCompound* ringers = m_bg->GetSubstanceManager().GetCompound("RingersLactate");
  SESubstanceCompound* blood = m_bg->GetSubstanceManager().GetCompound("Blood_ONegative");
  SESubstanceCompound* plasma = m_bg->GetSubstanceManager().GetCompound("Plasma");
  SESubstanceCompound* platelets = m_bg->GetSubstanceManager().GetCompound("PlateletsInfusion");


  //Create infusion and bolus actions
  m_RocuroniumBolus = new SESubstanceBolus(*roc);
  m_FentanylBolus = new SESubstanceBolus(*fent);
  m_PropofolBolus = new SESubstanceBolus(*prop);
  m_ringers = new SESubstanceCompoundInfusion(*ringers);
  m_blood = new SESubstanceCompoundInfusion(*blood);
  m_plasma = new SESubstanceCompoundInfusion(*plasma);
  m_platelet = new SESubstanceCompoundInfusion(*platelets);

  //set the hemorrhage
  m_hemorrhage = new SEHemorrhage();
  m_hemorrhage->GetInitialRate().SetValue(200, VolumePerTimeUnit::mL_Per_min);
  m_hemorrhage->SetCompartment("Aorta");
  m_bg->ProcessAction(*m_hemorrhage);

  m_bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename(resultsFile); //deposits in build/runtime
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", "1/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*fent, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*prop, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*roc, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*des, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialBloodPH", "unitless");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", "mL/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("EndTidalCarbonDioxideFraction", "unitless");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationMusclePressure", PressureUnit::mmHg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryResistance", FlowResistanceUnit::cmH2O_s_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PulmonaryCompliance", FlowComplianceUnit::L_Per_cmH2O);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", "1/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", "mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", "mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanUrineOutput", "mL/hr");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("VenaCava", *lactate, "Molarity", AmountPerVolumeUnit::mmol_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set("Aorta", "OutFlow", VolumePerTimeUnit::mL_Per_min);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*prop, "Clearance-RenalClearance", VolumePerTimeMassUnit::mL_Per_min_kg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*fent, "Clearance-RenalClearance", VolumePerTimeMassUnit::mL_Per_min_kg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*roc, "Clearance-RenalClearance", VolumePerTimeMassUnit::mL_Per_min_kg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*des, "Clearance-RenalClearance", VolumePerTimeMassUnit::mL_Per_min_kg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("IntracranialPressure", PressureUnit::mmHg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CarricoIndex", PressureUnit::mmHg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("AlveolarArterialGradient", PressureUnit::mmHg);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SedationLevel");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePatientDataRequest().Set("FunctionalResidualCapacity", VolumeUnit::L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePatientDataRequest().Set("VitalCapacity", VolumeUnit::L);
  m_runThread = true;
}

InductionThread::~InductionThread()
{
  m_runThread = false;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  SAFE_DELETE(m_RocuroniumBolus);
  SAFE_DELETE(m_FentanylBolus);
  SAFE_DELETE(m_PropofolBolus);
  SAFE_DELETE(m_hemorrhage);
}

void InductionThread::AdministerInduction()
{
    //set up bolus
  m_RocuroniumBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  m_FentanylBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  m_PropofolBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);

  //pull patient weight
  const SEPatient& patient = m_bg->GetPatient();
  double weight_kg = patient.GetWeight(MassUnit::kg);

  double fentbolus_mg = 0.002*weight_kg;
  double propofolBolus_mg = 2*weight_kg;
  double rocuroniumBolus_mg = 0.6*weight_kg;

  double bolus = 10.0;

  //set concentrations
  m_FentanylBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  m_FentanylBolus->GetConcentration().SetValue(fentbolus_mg / 10, MassPerVolumeUnit::mg_Per_mL);
  m_FentanylBolus->GetDose().SetValue(bolus, VolumeUnit::mL);

  m_PropofolBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  m_PropofolBolus->GetConcentration().SetValue(propofolBolus_mg / 10, MassPerVolumeUnit::mg_Per_mL);
  m_PropofolBolus->GetDose().SetValue(bolus, VolumeUnit::mL);

  m_RocuroniumBolus->SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
  m_RocuroniumBolus->GetConcentration().SetValue(rocuroniumBolus_mg / 10, MassPerVolumeUnit::mg_Per_mL);
  m_RocuroniumBolus->GetDose().SetValue(bolus, VolumeUnit::mL);

  //process the actions
  m_mutex.lock();
  m_bg->ProcessAction(*m_FentanylBolus);
  m_mutex.unlock();
  m_mutex.lock();
  m_bg->ProcessAction(*m_PropofolBolus);
  m_mutex.unlock();
  m_mutex.lock();
  m_bg->ProcessAction(*m_RocuroniumBolus);
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

void InductionThread::Ventilation()
{
  SEAnesthesiaMachineConfiguration AMConfig(m_bg->GetSubstanceManager());
  // You can set configuration by modifing the configuration class directly
  // Or you can point to an XML with configuration data.
  // Modifying the class will keep any old settings that are not provided in the config
  // Using a xml will set the anesthesia machine to only the property states specified in the file
  SEIntubation intubate;
  intubate.SetType(CDM::enumIntubationType::Tracheal);
  m_bg->ProcessAction(intubate);

  SEAnesthesiaMachine& config = AMConfig.GetConfiguration();
  config.SetConnection(CDM::enumAnesthesiaMachineConnection::Tube);
  config.GetInletFlow().SetValue(2.0, VolumePerTimeUnit::L_Per_min);
  config.GetInspiratoryExpiratoryRatio().SetValue(.5);
  config.GetOxygenFraction().SetValue(.6);
  config.SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::Wall);
  config.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  config.SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen);
  config.GetReliefValvePressure().SetValue(20.0, PressureUnit::cmH2O);
  config.GetRespiratoryRate().SetValue(12, FrequencyUnit::Per_min);
  config.GetVentilatorPressure().SetValue(22.0, PressureUnit::cmH2O);
  config.GetOxygenBottleOne().GetVolume().SetValue(660.0, VolumeUnit::L);
  config.GetOxygenBottleTwo().GetVolume().SetValue(660.0, VolumeUnit::L);

  // Process the action to propagate state into the engine
  m_bg->ProcessAction(AMConfig);
  m_bg->AdvanceModelTime(5.0, TimeUnit::s);

  //introcuce desfulrane
  SEAnesthesiaMachineChamber& rightChamber = config.GetLeftChamber();
  rightChamber.SetState(CDM::enumOnOff::On);
  rightChamber.GetSubstanceFraction().SetValue(0.02);
  rightChamber.SetSubstance(*m_bg->GetSubstanceManager().GetSubstance("Desflurane"));
  m_bg->ProcessAction(AMConfig);

  m_bg->AdvanceModelTime(5.0, TimeUnit::s);

  m_bg->GetLogger()->Info(asprintf("Turning on the Anesthesia Machine and placing mask on patient for spontaneous breathing with machine connection."));
}

void InductionThread::AdvanceTime()
{
  m_mutex.lock();
  m_bg->AdvanceModelTime(1.0, TimeUnit::s);
  //if (m_blood->HasBagVolume() && m_blood->HasRate()) {
  //  m_TotalVolume_mL += (m_blood->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
  //  m_TotalVolumeBlood_mL += (m_blood->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
  //  m_ivBagVolumeBlood_mL += (-m_blood->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
  //  if (m_ivBagVolumeBlood_mL < 0.0) {
  //    m_bg->GetLogger()->Info("blood bag is empty \n");
  //    m_ivBagVolumeBlood_mL = 0.0;
  //    m_blood->Clear();
  //  }
  //}
  //repeat for plasma
  //if (m_plasma->HasBagVolume() && m_plasma->HasRate()) {
  //  m_TotalVolume_mL += (m_plasma->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
  //  m_TotalVolumePlasma_mL += (m_plasma->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
  //  m_ivBagVolumePlasma_mL += (-m_plasma->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s));
  //  if (m_ivBagVolumePlasma_mL < 0.0) {
  //    m_bg->GetLogger()->Info("blood bag is empty \n");
  //    m_ivBagVolumePlasma_mL = 0.0;
  //    m_plasma->Clear();
  //  }
  //}
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

  //how long do we want to run for?
  double maxSimTime_min = 40.0;
  double hemTime_min = 20.0;
  double venTime_min = 15.0;
  double transitTime_min = 5.0;

  //compute urine production and max fluid requirements, per parkland formula
  const SEPatient& patient = m_bg->GetPatient();
  double weight_kg = patient.GetWeight(MassUnit::kg);

  double initialInfustion_mL_Per_hr = 0.0; //tbsa * 10.0; // Should start at 10*tbsa // (DayLimit_mL / 0.5) / 8.0; //half of the fluid should be loaded in the first 8 hours;
  double initialInfustionAlbumin_mL_Per_hr = 50.0; //tbsa * 10.0; // Should start at 10*tbsa // (DayLimit_mL / 0.5) / 8.0; //half of the fluid should be loaded in the first 8 hours;
  double temp = 0.0;
  bool Labs = true;
  bool fluidOn = false;

  //induce patient 
  m_bg->AdvanceModelTime(transitTime_min, TimeUnit::min);

  m_bg->GetLogger()->Info(asprintf("Beginning care"));
  //INDUCE PATIENT
  AdministerInduction();
  m_bg->AdvanceModelTime(30, TimeUnit::s);

  SetPlasmaInfusionRate(plasmaVolume_mL, plasmaRate_mL_Per_min);
  SetBloodInfusionRate(pRBCVolume_mL, bloodRate_mL_Per_hr);

  m_bg->AdvanceModelTime(30, TimeUnit::s);

  //vent patient
  Ventilation();

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

    //check vent time 
    if (m_bg->GetSimulationTime(TimeUnit::min) > venTime_min) {

    }

    //check status every hour, reset the volume while we are at it
    if (((int)m_bg->GetSimulationTime(TimeUnit::s) + 1) % checkTime_s == 0) {
      Status();
    }

    //exit checks:
    if (m_bg->GetPatient().IsEventActive(CDM::enumPatientEvent::IrreversibleState)) {
      //m_bg->GetLogger()->Info(std::stringstream() << "oh no!");
      m_bg->GetLogger()->Info("///////////////////////////////////////////////////////////////");
      m_runThread = false;
    }

    if (m_bg->GetSimulationTime(TimeUnit::min) > maxSimTime_min) {
      m_bg->GetLogger()->Info("This simulation has gone on too long");
      m_runThread = false;
    }

    //advance time
    AdvanceTime();
  }
}

