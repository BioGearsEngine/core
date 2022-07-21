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

#include "HowTo-NaloxoneNasal.h"
#include <iostream>

// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>

int main(int argc, char* argv[])
{
  // To run multiple Morphine/Fentanyl overdose values
  std::string overdoseSubstance = "Fentanyl"; // Morphine or fentanyl
  double lowestOverDose = 0.15; // mg/mL
  double highestOverDose = 1.0; // mg/mL
  double doseInc = 0.05;
  for (double opioidDose = lowestOverDose; opioidDose <= highestOverDose; opioidDose += doseInc) {
    //HowToNaloxoneNasal();
    double opDoseug = opioidDose * 1000;
    int docOpioidDose = (int)(opDoseug);
    std::string strOpioidDose = std::to_string(docOpioidDose);
    std::string logName = "./states/NasalNaloxoneStates/HowToNasalNaloxone_";
    logName.append(overdoseSubstance);
    logName.append("_Conc");
    logName.append(strOpioidDose);
    logName.append("ug.log");
    NaloxoneThread nasalNaloxone(logName, opioidDose, overdoseSubstance);
    nasalNaloxone.FluidLoading(overdoseSubstance, opioidDose);
  }
}

using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a burn wound to a patient
///
/// \details
/// Refer to the SEBurnWoundData class
///
//--------------------------------------------------------------------------------------------------
int HowToNaloxoneNasal()
{
  // I do not believe this is needed anymore
  return 0;
}

NaloxoneThread::NaloxoneThread(const std::string logFile, double opioidDose, const std::string opioidName)
  : m_naloxoneThread()
{
  //Create the engine and load patient state
  m_bg = CreateBioGearsEngine(logFile);
  m_bg->GetLogger()->Info(asprintf("Initiating %f %s", opioidDose, "mg/mL or whatever of ", opioidName.c_str(), " for overdose"));
  if (!m_bg->LoadState("./states/StandardMale@0s.xml")) {
    m_bg->GetLogger()->Error("Could not load state, check the error");
    throw std::runtime_error("Could not load state, check the error");
  }
  SESubstance* opioidSub = m_bg->GetSubstanceManager().GetSubstance(opioidName);
  SESubstance* nal = m_bg->GetSubstanceManager().GetSubstance("Naloxone");

  //Create CSV results file and set up data that we want to be tracked (tracking done in AdvanceModelTime)
  double opDose_ug = opioidDose * 1000;
  int docOpioidDose = (int)(opDose_ug);
  std::string resultsFileNasalNaloxone = std::to_string(docOpioidDose);
  std::string resultsFile = "HowToNasalNaloxoneFor_";
  resultsFile.append(opioidName);
  resultsFile.append(resultsFileNasalNaloxone);
  resultsFile.append(".csv");
  m_bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename(resultsFile); //deposits in build/runtime
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", "1/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*opioidSub, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*nal, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialBloodPH", "unitless");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", "mL/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("InflammatoryResponse-TissueIntegrity");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", "mmHg");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", "1/min");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", "mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystemicVascularResistance", "mmHg s/mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", "mL");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanUrineOutput", "mL/hr");
  m_bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PainVisualAnalogueScale");
  m_bg->GetEngineTrack()->GetDataTrack().Probe("totalNaloxone_mg", m_totalNaloxone_mg);

  //Create infusion actions
  m_opioid = new SESubstanceInfusion(*opioidSub);
  m_naloxone = new SESubstanceNasalDose(*nal);
  m_totalNaloxone_mg = 0.0;
  
  //m_genState = new SESerializeState();

  //m_bg->ProcessAction(*m_opioid);

  m_runThread = true;
  // m_NaloxoneThread = std::thread(&NaloxoneThread::AdvanceTime, this);
}

NaloxoneThread::~NaloxoneThread()
{
  m_runThread = false;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  SAFE_DELETE(m_opioid);
  SAFE_DELETE(m_naloxone);
  //SAFE_DELETE(m_genState);
}

void NaloxoneThread::AdministerOpioid(double& conc, double& rate)
{
  m_opioid->GetConcentration().SetValue(conc, MassPerVolumeUnit::mg_Per_mL);
  m_opioid->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_min);

  m_mutex.lock();
  m_bg->ProcessAction(*m_opioid);
  m_mutex.unlock();
}

void NaloxoneThread::SetNaloxoneInfusionRate(double& dose)
{
  m_naloxone->GetDose().SetValue(dose, MassUnit::mg);
  m_totalNaloxone_mg += dose;

  m_mutex.lock();
  m_bg->ProcessAction(*m_naloxone);
  m_mutex.unlock();
}

void NaloxoneThread::AdvanceTime()
{
  while (m_runThread) {
    m_mutex.lock();
    m_bg->AdvanceModelTime(1.0, TimeUnit::s);
    // Increment m_totalNaloxone here?
    // Need to check for O2 below 0.9 and readmin naloxone

    m_bg->GetEngineTrack()->GetDataTrack().Probe("totalNaloxone1_mg", m_totalNaloxone_mg);
    m_bg->GetEngineTrack()->TrackData(m_bg->GetSimulationTime(TimeUnit::s));
    m_mutex.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
  }
}

void NaloxoneThread::AdvanceTimeFluids()
{
  m_mutex.lock();
  m_bg->AdvanceModelTime(1.0, TimeUnit::s);
  // Increment m_totalNaloxone here?
  // Need to check for O2 below 0.9 and readmin naloxone

  m_bg->GetEngineTrack()->GetDataTrack().Probe("totalNaloxone2_mg", m_totalNaloxone_mg);
  m_bg->GetEngineTrack()->TrackData(m_bg->GetSimulationTime(TimeUnit::s));
  m_mutex.unlock();
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void NaloxoneThread::Status()
{
  m_mutex.lock();
  /*m_bg->GetLogger()->Info(asprintf("The patient suffered a burn wound %f %s", m_bg->GetSimulationTime(TimeUnit::min), " min ago"));
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
    m_bg->GetLogger()->Info(asprintf("Remaining LR Volume : %f %s", m_ivBagVolume_mL, "mL"));
  }*/

  std::cout << std::endl;
  m_mutex.unlock();
}

//routine to administer drugs with a goal directed therapy for o2 sat
void NaloxoneThread::FluidLoading(std::string overdoseSubstance, double opioidDose)
{
  double urineProduction = 0.0;
  int checkTime_s = 300; // 5 mins
  int stateTime_s = 3600; // 60 mins

  m_runThread = true;
  //Create variables for scenario

  //compute urine production and max fluid requirements, per parkland formula
  const SEPatient& patient = m_bg->GetPatient();
  double weight_kg = patient.GetWeight(MassUnit::kg);
  double targetLowO2Saturation = 0.9;
  //set drug infusion rate:
  //Infuse 3 ug / kg of fentanyl over 90 s.Effects should be large
  //Fentanyl overdose at 150 ug/mL at rate of 1 mL/min for 1.5 mins
  //Morphine overdose at 1 mg/mL at rate of 8.2 mL/min for 24 mins
  double opioidConc_mg_Per_mL = opioidDose;
  double opioidRate_mL_Per_min = 1; // 8.2 for morphine; //1 for fentanyl
  double timeOfOpioidDose_min = 2; //24 for morphine; //min 2 for fentanyl
  //double DayLimit_mL = X; // Ask Austin if there is a max dosage of naloxone
  double initialInfustion_mL_Per_hr = 0.0;
  double hrsBeforeIntervention = 0.02 + (timeOfOpioidDose_min/60); // 2-3 mins, just make sure patient does not die before or right after treatment aka savable
  double secsBeforeIntervention = hrsBeforeIntervention * 3600;
  double standardNaloxoneDose_mg = 4.0;
  double minTimeBetweenDoses_s = 30.0;
  double timeOfLastDose = 0.0;
  //double DayLimit_Hr = DayLimit_mL / 25.0;
  double maxSimTime = 4.0 + hrsBeforeIntervention + (timeOfOpioidDose_min / 60.0);
  double temp = 0.0;
  bool saveState = true;
  bool fluidOn = true;

  
  AdministerOpioid(opioidConc_mg_Per_mL, opioidRate_mL_Per_min);


  while (m_runThread) {
    // Generate State Every X amount of time
   /* if ((((int)m_bg->GetSimulationTime(TimeUnit::s) + 1) % stateTime_s == 0 && saveState == true) || ((int)m_bg->GetSimulationTime(TimeUnit::s) == 0)) {
      //Create CSV results file and set up data that we want to be tracked (tracking done in AdvanceModelTime)
      double opDose_ug = opioidDose * 1000;
      int docOpioidDose = (int)(opDose_ug);
      std::string resultsFileNasalNaloxone = std::to_string(docOpioidDose);
      std::string resultsFile = "HowToNasalNaloxoneFor_";
      resultsFile.append(overdoseSubstance);
      resultsFile.append(resultsFileNasalNaloxone);
      resultsFile.append("ug.xml");

      m_bg->GetLogger()->Info(asprintf("Saved state"));
      m_bg->SaveStateToFile(resultsFile);
    }*/

    //Stop opioid dose at specified time
    if ((int)m_bg->GetSimulationTime(TimeUnit::s) == ((int)timeOfOpioidDose_min * 60)) {
      m_bg->GetLogger()->Info(asprintf("Stopping Opioid"));
      double stopOpioidConc = 0.0;
      double stopOpioidRate = 0.0;
      AdministerOpioid(stopOpioidConc, stopOpioidRate);
    }

    //administer initial nasal naloxone dose at specified time
    if ((int)m_bg->GetSimulationTime(TimeUnit::s) == ((int)secsBeforeIntervention)) {
      m_bg->GetLogger()->Info(asprintf("administering nasal naloxone"));
      m_bg->GetLogger()->Info(asprintf("Beginning Intervention with infusion at %f %s", standardNaloxoneDose_mg, "mg"));
      m_bg->GetLogger()->Info(asprintf("Current O2 Saturation is %f %s", m_bg->GetBloodChemistrySystem()->GetOxygenSaturation()));
      SetNaloxoneInfusionRate(standardNaloxoneDose_mg);
    }


    //check o2 
    if (((int)m_bg->GetSimulationTime(TimeUnit::s) + 1) > ((int)secsBeforeIntervention)) {
        Status();
        double o2Sat = m_bg->GetBloodChemistrySystem()->GetOxygenSaturation();

        if (((int)m_bg->GetSimulationTime(TimeUnit::s) + 1) > ((int)(minTimeBetweenDoses_s + timeOfLastDose))) {
            if (o2Sat < targetLowO2Saturation) {
                m_bg->GetLogger()->Info(asprintf("O2 Saturation is too low at %f %s", o2Sat));
                SetNaloxoneInfusionRate(standardNaloxoneDose_mg);
                timeOfLastDose = m_bg->GetSimulationTime(TimeUnit::s) + 1;
            }
        }
    }

    //exit checks:
    if (m_bg->GetPatient().IsEventActive(CDM::enumPatientEvent::IrreversibleState)) {
      //m_bg->GetLogger()->Info(std::stringstream() << "oh no!");
      m_bg->GetLogger()->Info("///////////////////////////////////////////////////////////////");
      m_runThread = false;
    }

    /*if (m_TotalVolume_mL > DayLimit_mL) {
      m_bg->GetLogger()->Info(asprintf("We have given too many fluids, per guidelines: %f %s", m_TotalVolume_mL, "mL"));
      DayLimit_mL *= 2.0;
      //m_runThread = false;
    }*/

    if (m_bg->GetSimulationTime(TimeUnit::hr) > maxSimTime) {
      m_bg->GetLogger()->Info("This simulation has gone on too long");
      m_runThread = false;
    }

    //advance time

    AdvanceTimeFluids();
  }
}
