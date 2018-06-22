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

#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESepsis.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>

//This method uses the Threaded BioGears functionality

void HowToDynamicSepsis()
{
  // Create the engine and have it run in it's own thread
  // This call will block while the engine stabilizes
  DynamicSepsis sepThread("./DynamicSepsis.csv");
  // When it comes back, the engine will be running, waiting for your input

  int action;
  double sev;
  std::string location;
  bool active = true;
  double rate;
  double concentration;
  std::string out;

  do {
    sepThread.GetLogger()->Info("Enter Integer for Action to Perform : [1]Status, [2]Initiate Sepsis, [3]IVFluids, [4] Antibiotic Admin, [5], Norepinephrine Admin, [6]Quit");
    std::cin >> action;
    switch (action) {
    case 1:
      sepThread.Status();
      break;
    case 2:
      sepThread.GetLogger()->Info("Type a location, then hit ENTER: ");
      std::cin >> location;
      sepThread.GetLogger()->Info("Type a severity (0-1), then hit ENTER: ");
      std::cin >> sev;
      if (sev < 0 || sev > 1.0) {
        out = "Severity out of bounds, default to 0.5";
        sev = 0.5;
      } else if (sev <= ZERO_APPROX)
        out = "Sepsis action disabled in  " + location;
      else
        out = "Sepsis in " + location + " with severity = " + std::to_string(sev);
      sepThread.GetLogger()->Info(out);
      sepThread.SetSepsis(location, sev);
      break;
    case 3:
      sepThread.GetLogger()->Info("Enter IV Fluids Rate in mL/min (bag volume is 500 mL), then hit ENTER : ");
      std::cin >> rate;
      sepThread.GetLogger()->Info(std::stringstream() << rate);
      sepThread.SetIVFluidsFlow_mL_Per_min(rate);
      break;
    case 4:
      sepThread.GetLogger()->Info("Administering antibiotics (4.5 g over 4 hrs");
      sepThread.SetAntibiotic();
      break;
    case 5:
      sepThread.GetLogger()->Info("Enter a concentration in ug/mL (will be delivered at 1 mL/min), then hit ENTER : ");
      std::cin >> concentration;
      sepThread.GetLogger()->Info(std::stringstream() << concentration);
      sepThread.SetNorepinephrine(concentration);
      break;
    case 6:
      active = false;
    }
  } while (active);
}

DynamicSepsis::DynamicSepsis(const std::string& logfile)
  : BioGearsThread(logfile)
{
  //BioGearsThread constructor takes care of engine initialization, patient state loading
  //Saline constructed in BioGearsThread
  SESubstanceCompound* antibiotic = m_bg->GetSubstanceManager().GetCompound("Antibiotic");
  SESubstance* norepinephrine = m_bg->GetSubstanceManager().GetSubstance("Norepinephrine");
  // Create and initialize our actions (infusion constructed in BioGearsThread)
  m_antibiotic = new SESubstanceCompoundInfusion(*antibiotic);
  m_pressor = new SESubstanceInfusion(*norepinephrine);
  m_sepsis = new SESepsis();
}

DynamicSepsis::~DynamicSepsis()
{
  SAFE_DELETE(m_sepsis);
  SAFE_DELETE(m_antibiotic);
  SAFE_DELETE(m_pressor);
}

void DynamicSepsis::SetSepsis(std::string& location, double& sev)
{
  m_sepsis->SetCompartment(location);
  m_sepsis->GetSeverity().SetValue(sev);
  m_sepsis->BuildTissueResistorMap();
  m_mutex.lock();
  m_bg->ProcessAction(*m_sepsis);
  m_mutex.unlock();
}

void DynamicSepsis::SetAntibiotic()
{
  //Administering 4.5 g / 4 hrs, assuming that the 4.5 g are in 150 mL (FDA piperacilllin/tazobactam info sheet)
  m_antibiotic->GetBagVolume().SetValue(150.0, VolumeUnit::mL);
  m_antibiotic->GetRate().SetValue(0.625, VolumePerTimeUnit::mL_Per_min);
  m_mutex.lock();
  m_bg->ProcessAction(*m_antibiotic);
  m_mutex.unlock();
}

void DynamicSepsis::SetNorepinephrine(double& concentration)
{
  m_pressor->GetRate().SetValue(1.0, VolumePerTimeUnit::mL_Per_min);
  m_pressor->GetConcentration().SetValue(concentration, MassPerVolumeUnit::ug_Per_mL);
  m_mutex.lock();
  m_bg->ProcessAction(*m_sepsis);
  m_mutex.unlock();
}

void DynamicSepsis::Status()
{ // On demand call to print vitals to the screen
  m_mutex.lock();
  m_bg->GetLogger()->Info("");
  m_bg->GetLogger()->Info(std::stringstream() << "Simulation Time : " << m_bg->GetSimulationTime(TimeUnit::s) << "s");
  m_bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << m_bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  m_bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << m_bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  m_bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << m_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  m_bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << m_bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  m_bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << m_bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  m_bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << m_bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  m_bg->GetLogger()->Info(std::stringstream() << "Temperature : " << m_bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << "deg C");
  m_bg->GetLogger()->Info(std::stringstream() << "Total Bilirubin : " << m_bg->GetBloodChemistrySystem()->GetTotalBilirubin(MassPerVolumeUnit::mg_Per_dL) << "mg/dL");
  m_bg->GetLogger()->Info(std::stringstream() << "Blood Lactate : " << m_bg->GetSubstanceManager().GetSubstance("Lactate")->GetBloodConcentration(MassPerVolumeUnit::mg_Per_dL) << "mg/dL");

  m_bg->GetLogger()->Info("");
  m_mutex.unlock();
}
