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
#include <biogears/cdm/patient/SEPatient.h>

#include "io/cdm/Patient.h"
#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/io/io-manager.h>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/patients.h>
#endif

#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/schema/cdm/Properties.hxx>

namespace std {
template class map<biogears::SEPatientEventType, bool>;
template class map<biogears::SEPatientEventType, void (*)(bool)>;
template class map<biogears::SEPatientEventType, double>;
}
namespace biogears {
SEPatient::SEPatient(Logger* logger)
  : Loggable(logger)
{
  m_EventHandler = nullptr;

  m_Name = "";
  m_Sex = SESex::Invalid;
  m_Age = nullptr;
  m_Weight = nullptr;
  m_Height = nullptr;

  m_AlveoliSurfaceArea = nullptr;
  m_BasalMetabolicRate = nullptr;
  m_BloodRh = true;
  m_BloodType = SEBloodType::Invalid;
  m_BloodVolumeBaseline = nullptr;
  m_BodyDensity = nullptr;
  m_BodyFatFraction = nullptr;
  m_DiastolicArterialPressureBaseline = nullptr;
  m_ExpiratoryReserveVolume = nullptr;
  m_FunctionalResidualCapacity = nullptr;
  m_HeartRateBaseline = nullptr;
  m_HeartRateMaximum = nullptr;
  m_HeartRateMinimum = nullptr;
  m_Hyperhidrosis = nullptr;
  m_InspiratoryCapacity = nullptr;
  m_InspiratoryReserveVolume = nullptr;
  m_LeanBodyMass = nullptr;
  m_MaxWorkRate = nullptr;
  m_MuscleMass = nullptr;
  m_MeanArterialPressureBaseline = nullptr;
  m_PainSusceptibility = nullptr;
  m_ResidualVolume = nullptr;
  m_RespiratoryDriverAmplitudeBaseline = nullptr;
  m_RespirationRateBaseline = nullptr;
  m_RightLungRatio = nullptr;
  m_SleepAmount = nullptr;
  m_SkinSurfaceArea = nullptr;
  m_SystolicArterialPressureBaseline = nullptr;
  m_TotalVentilationBaseline = nullptr;
  m_TidalVolumeBaseline = nullptr;
  m_TotalLungCapacity = nullptr;
  m_VitalCapacity = nullptr;

  for (SEPatientEventType key = SEPatientEventType::_begin;
       key < SEPatientEventType::_end;
       key = static_cast<SEPatientEventType>((size_t)key + 1)) {
    // SEPatientEventType is a code generated C style enum.
    // To my knowladge it must be conitnguous as XSD does not support assinigng manual values to XML Enums that are use dto generated C++ counterpart
    // Short story is the goal is to set every possible enum to a value of false and accomidate future extensions ot the enum.
    m_EventState[key] = false;
    m_EventDuration_s[key] = 0;
  }
}
//-----------------------------------------------------------------------------
SEPatient::~SEPatient()
{
  Clear();
}
//-----------------------------------------------------------------------------
bool SEPatient::Load(const char* patientFile)
{
  return Load(std::string { patientFile });
}
//-----------------------------------------------------------------------------
bool SEPatient::Load(const std::string& patientFile)
{
  CDM::PatientData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->FindPatientFile(patientFile.c_str());
  if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
    size_t content_size;
    auto resource = filesystem::path(patientFile).basename();
    auto content = io::get_embedded_patients_file(resource.c_str(), content_size);
    if (content_size > 0) {
      data = Serializer::ReadBuffer(reinterpret_cast<XMLByte const*>(content), content_size, m_Logger);
    }
#endif
  } else {
    data = Serializer::ReadFile(possible_path, m_Logger);
  }

  pData = dynamic_cast<CDM::PatientData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Patient file could not be read : " << patientFile << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}
//-----------------------------------------------------------------------------
void SEPatient::Clear()
{
  m_EventHandler = nullptr;

  for (SEPatientEventType key = SEPatientEventType::_begin;
       key < SEPatientEventType::_end;
       key = static_cast<SEPatientEventType>((size_t)key + 1)) {
    // SEPatientEventType is a code generated C style enum.
    // To my knowladge it must be conitnguous as XSD does not support assinigng manual values to XML Enums that are use dto generated C++ counterpart
    // Short story is the goal is to set every possible enum to a value of false and accomidate future extensions ot the enum.
    m_EventState[key] = false;
    m_EventDuration_s[key] = 0;
  }

  m_Name = "";
  m_Sex = SESex::Invalid;
  SAFE_DELETE(m_Age);
  SAFE_DELETE(m_Weight);
  SAFE_DELETE(m_Height);

  SAFE_DELETE(m_AlveoliSurfaceArea);
  SAFE_DELETE(m_BasalMetabolicRate);
  m_BloodRh = true;
  m_BloodType = SEBloodType::Invalid;
  SAFE_DELETE(m_BloodVolumeBaseline);
  SAFE_DELETE(m_BodyDensity);
  SAFE_DELETE(m_BodyFatFraction);
  SAFE_DELETE(m_DiastolicArterialPressureBaseline);
  SAFE_DELETE(m_ExpiratoryReserveVolume);
  SAFE_DELETE(m_FunctionalResidualCapacity);
  SAFE_DELETE(m_HeartRateBaseline);
  SAFE_DELETE(m_HeartRateMaximum);
  SAFE_DELETE(m_HeartRateMinimum);
  SAFE_DELETE(m_Hyperhidrosis);
  SAFE_DELETE(m_InspiratoryCapacity);
  SAFE_DELETE(m_InspiratoryReserveVolume);
  SAFE_DELETE(m_LeanBodyMass);
  SAFE_DELETE(m_MaxWorkRate);
  SAFE_DELETE(m_MuscleMass);
  SAFE_DELETE(m_MeanArterialPressureBaseline);
  SAFE_DELETE(m_PainSusceptibility);
  SAFE_DELETE(m_ResidualVolume);
  SAFE_DELETE(m_RespirationRateBaseline);
  SAFE_DELETE(m_RespiratoryDriverAmplitudeBaseline);
  SAFE_DELETE(m_RightLungRatio);
  SAFE_DELETE(m_SkinSurfaceArea);
  SAFE_DELETE(m_SleepAmount);
  SAFE_DELETE(m_SystolicArterialPressureBaseline);
  SAFE_DELETE(m_TotalVentilationBaseline);
  SAFE_DELETE(m_TidalVolumeBaseline);
  SAFE_DELETE(m_TotalLungCapacity);
  SAFE_DELETE(m_VitalCapacity);
}
//-----------------------------------------------------------------------------
const SEScalar* SEPatient::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEPatient::GetScalar(const std::string& name)
{
  if (name.compare("Age") == 0)
    return &GetAge();
  if (name.compare("Weight") == 0)
    return &GetWeight();
  if (name.compare("Height") == 0)
    return &GetHeight();
  if (name.compare("AlveoliSurfaceArea") == 0)
    return &GetAlveoliSurfaceArea();
  if (name.compare("BasalMetabolicRate") == 0)
    return &GetBasalMetabolicRate();
  if (name.compare("BloodVolumeBaseline") == 0)
    return &GetBloodVolumeBaseline();
  if (name.compare("BodyDensity") == 0)
    return &GetBodyDensity();
  if (name.compare("BodyFatFraction") == 0)
    return &GetBodyFatFraction();
  if (name.compare("DiastolicArterialPressureBaseline") == 0)
    return &GetDiastolicArterialPressureBaseline();
  if (name.compare("ExpiratoryReserveVolume") == 0)
    return &GetExpiratoryReserveVolume();
  if (name.compare("FunctionalResidualCapacity") == 0)
    return &GetFunctionalResidualCapacity();
  if (name.compare("HeartRateBaseline") == 0)
    return &GetHeartRateBaseline();
  if (name.compare("HeartRateMaximum") == 0)
    return &GetHeartRateMaximum();
  if (name.compare("HeartRateMinimum") == 0)
    return &GetHeartRateMinimum();
  if (name.compare("Hyperhidrosis") == 0)
    return &GetHyperhidrosis();
  if (name.compare("InspiratoryCapacity") == 0)
    return &GetInspiratoryCapacity();
  if (name.compare("InspiratoryReserveVolume") == 0)
    return &GetInspiratoryReserveVolume();
  if (name.compare("LeanBodyMass") == 0)
    return &GetLeanBodyMass();
  if (name.compare("MaxWorkRate") == 0)
    return &GetMaxWorkRate();
  if (name.compare("MuscleMass") == 0)
    return &GetMuscleMass();
  if (name.compare("MeanArterialPressureBaseline") == 0)
    return &GetMeanArterialPressureBaseline();
  if (name.compare("PainSusceptibility") == 0)
    return &GetPainSusceptibility();
  if (name.compare("ResidualVolume") == 0)
    return &GetResidualVolume();
  if (name.compare("RespirationRateBaseline") == 0)
    return &GetRespirationRateBaseline();
  if (name.compare("RespiratoryDriverAmplitudeBaseline") == 0)
    return &GetRespiratoryDriverAmplitudeBaseline();
  if (name.compare("RightLungRatio") == 0)
    return &GetRightLungRatio();
  if (name.compare("SkinSurfaceArea") == 0)
    return &GetSkinSurfaceArea();
  if (name.compare("SleepAmount") == 0)
    return &GetSleepAmount();
  if (name.compare("SystolicArterialPressureBaseline") == 0)
    return &GetSystolicArterialPressureBaseline();
  if (name.compare("TotalVentilationBaseline") == 0)
    return &GetTotalVentilationBaseline();
  if (name.compare("TidalVolumeBaseline") == 0)
    return &GetTidalVolumeBaseline();
  if (name.compare("TotalLungCapacity") == 0)
    return &GetTotalLungCapacity();
  if (name.compare("VitalCapacity") == 0)
    return &GetVitalCapacity();

  return nullptr;
}
//-----------------------------------------------------------------------------
bool SEPatient::Load(const CDM::PatientData& in)
{
  io::Patient::UnMarshall(in, *this);

  return true;
}
//-----------------------------------------------------------------------------
CDM::PatientData* SEPatient::Unload() const
{
  CDM::PatientData* data = new CDM::PatientData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEPatient::Unload(CDM::PatientData& data) const
{
  io::Patient::Marshall(*this, data);
};
void SEPatient::SetEventCallback(SEPatientEventType type, void (*callback)(bool))
{
  m_EventCallbacks[type] = callback;
}
//-----------------------------------------------------------------------------
void SEPatient::SetEvent(SEPatientEventType type, bool active, const SEScalarTime& time)
{
  if (m_EventState[type] == active) {
    return; // No Change
  } else {
    m_ss.str("");
    m_ss << "[Event] " << time << ", ";
    if (active) {
      switch (type) {
      case SEPatientEventType::AcuteLungInjury:
        m_ss << " Carrico Index < 300 mmHg:  Patient has Acute Lung Injury";
        break;
      case SEPatientEventType::AcuteRespiratoryDistress:
        m_ss << " Carrico Index < 200 mmHg: Patient has Acute Respiratory Distress";
        break;
      case SEPatientEventType::Antidiuresis:
        m_ss << " Patient has Antidiuresis";
        break;
      case SEPatientEventType::Asystole:
        m_ss << " Patient has Asystole";
        break;
      case SEPatientEventType::Bradycardia:
        m_ss << " Patient has Bradycardia";
        break;
      case SEPatientEventType::Bradypnea:
        m_ss << " Patient has Bradypnea";
        break;
      case SEPatientEventType::BrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain is dangerously low";
        break;
      case SEPatientEventType::CardiacArrest:
        m_ss << " Patient has Cardiac Arrest";
        break;
      case SEPatientEventType::CardiogenicShock:
        m_ss << " Patient has Cardiogenic Shock";
        break;
      case SEPatientEventType::CompartmentSyndromeAbdominal:
        m_ss << " Patient has Abdominal Compartment Syndrome";
        break;
      case SEPatientEventType::CompartmentSyndromeLeftArm:
        m_ss << " Patient has Compartment Syndrome in the Left Arm";
        break;
      case SEPatientEventType::CompartmentSyndromeLeftLeg:
        m_ss << " Patient has Compartment Syndrome in the Left Leg";
        break;
      case SEPatientEventType::CompartmentSyndromeRightArm:
        m_ss << " Patient has Compartment Syndrome in the Right Arm";
        break;
      case SEPatientEventType::CompartmentSyndromeRightLeg:
        m_ss << " Patient has Compartment Syndrome in the Right Leg";
        break;
      case SEPatientEventType::CriticalBrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain is critically low";
        break;
      case SEPatientEventType::Dehydration:
        m_ss << " Patient has entered state of Dehydration";
        break;
      case SEPatientEventType::MildDiarrhea:
        m_ss << " Patient has mild diarrhea";
        break;
      case SEPatientEventType::SevereDiarrhea:
        m_ss << " Patient has severe diarrhea";
        break;
      case SEPatientEventType::Diuresis:
        m_ss << " Patient has entered Diuresis";
        break;
      case SEPatientEventType::Fasciculation:
        m_ss << "Patient has Fasciculation";
        break;
      case SEPatientEventType::FlaccidParalysis:
        m_ss << "Patient has Flaccid Paralysis";
        break;
      case SEPatientEventType::FunctionalIncontinence:
        m_ss << " Patient has involuntarily emptied their bladder";
        break;
      case SEPatientEventType::MildHeadache:
        m_ss << " Patient has a mild headache";
        break;
      case SEPatientEventType::SevereHeadache:
        m_ss << " Patient has a severe headache";
        break;
      case SEPatientEventType::HemolyticTransfusionReaction:
        m_ss << " Patient is undergoing an incompatible blood transfusion";
        break;
      case SEPatientEventType::Hypercapnia:
        m_ss << " Patient has Hypercapnia";
        break;
      case SEPatientEventType::Hyperglycemia:
        m_ss << " Patient has Hyperglycemia";
        break;
      case SEPatientEventType::Hyperthermia:
        m_ss << " Patient is Hyperthermic";
        break;
      case SEPatientEventType::Hypoglycemia:
        m_ss << " Patient has Hypoglycemia";
        break;
      case SEPatientEventType::HypoglycemicShock:
        m_ss << " Patient is in Hypoglycemic Shock";
        break;
      case SEPatientEventType::HypoglycemicComa:
        m_ss << " Patient has fallen into Hypoglycemic Coma";
        break;
      case SEPatientEventType::MildHypothermia:
        m_ss << " Patient is mildly Hypothermic";
        break;
      case SEPatientEventType::ModerateHypothermia:
        m_ss << " Patient is moderately Hypothermic";
        break;
      case SEPatientEventType::SevereHypothermia:
        m_ss << " Patient is Severely Hypothermic";
        break;
      case SEPatientEventType::Hypoxia:
        m_ss << " Patient has Hypoxia";
        break;
      case SEPatientEventType::IntracranialHypertension:
        m_ss << " Patient has Intracranial Hypertension";
        break;
      case SEPatientEventType::IntracranialHypotension:
        m_ss << " Patient has Intracranial Hypotension";
        break;
      case SEPatientEventType::HypovolemicShock:
        m_ss << " Patient is in Hypovolemic Shock";
        break;
      case SEPatientEventType::IrreversibleState:
        m_ss << " Patient has entered irreversible state";
        break;
      case SEPatientEventType::Ketoacidosis:
        m_ss << " Patient has Ketoacidosis";
        break;
      case SEPatientEventType::LacticAcidosis:
        m_ss << " Patient has Lactic Acidosis";
        break;
      case SEPatientEventType::LiverGlycogenDepleted:
        m_ss << " Patient's liver glycogen is depleted";
        break;
      case SEPatientEventType::MaximumPulmonaryVentilationRate:
        m_ss << " Patient's Respiratory Driver has exceeded the maximum target pulmonary ventilation rate, setting value to the maximum allowable rate";
        break;
      case SEPatientEventType::MetabolicAcidosis:
        m_ss << " The patient is in a state of metabolic acidosis";
        break;
      case SEPatientEventType::MetabolicAlkalosis:
        m_ss << " The patient is in a state of metabolic alkalosis";
        break;
      case SEPatientEventType::MildDiaphoresis:
        m_ss << " Patient has Mild Diaphoresis";
        break;
      case SEPatientEventType::ModerateDiaphoresis:
        m_ss << " Patient has Moderate Diaphoresis";
        break;
      case SEPatientEventType::SevereDiaphoresis:
        m_ss << " Patient has Severe Diaphoresis";
        break;
      case SEPatientEventType::MildSecretions:
        m_ss << " Patient has Mild Secretions";
        break;
      case SEPatientEventType::ModerateSecretions:
        m_ss << " Patient has Moderate Secretions";
        break;
      case SEPatientEventType::MildWeakness:
        m_ss << " Patient has Mild Weakness";
        break;
      case SEPatientEventType::ModerateWeakness:
        m_ss << " Patient has Moderate Weakness";
        break;
      case SEPatientEventType::MuscleCatabolism:
        m_ss << " Patient has begun muscle catabolism";
        break;
      case SEPatientEventType::MuscleGlycogenDepleted:
        m_ss << " Patient's muscle glycogen is depleted";
        break;
      case SEPatientEventType::MyocardiumOxygenDeficit:
        m_ss << " The patient's heart is not receiving enough oxygen";
        break;
      case SEPatientEventType::Natriuresis:
        m_ss << " Patient has Natriuresis";
        break;
      case SEPatientEventType::Nausea:
        m_ss << " Patient is Nauseous";
        break;
      case SEPatientEventType::NutritionDepleted:
        m_ss << " Patient has depleted all nutrition in body";
        break;
      case SEPatientEventType::PulselessRhythm:
        m_ss << " Patient has a Pulseless Rhythm";
        break;
      case SEPatientEventType::RenalHypoperfusion:
        m_ss << " Patient has Renal Hypoperfusion";
        break;
      case SEPatientEventType::Seizures:
        m_ss << " The patient is having a seizure";
        break;
      case SEPatientEventType::SevereAcuteRespiratoryDistress:
        m_ss << " Carrico Index < 100 : Patient has Severe Acute Respiratory Distress";
        break;
      case SEPatientEventType::SevereSecretions:
        m_ss << " The patient has severe Secretions";
        break;
      case SEPatientEventType::SevereSepsis:
        m_ss << " The patient has severe sepsis";
        break;
      case SEPatientEventType::Shivering:
        m_ss << " The patient is shivering";
        break;
      case SEPatientEventType::Tachycardia:
        m_ss << " Patient has Tachycardia";
        break;
      case SEPatientEventType::Tachypnea:
        m_ss << " Patient has Tachypnea";
        break;
      case SEPatientEventType::Fatigue:
        m_ss << " Patient has fatigue";
        break;
      case SEPatientEventType::Vomiting:
        m_ss << " Patient is Vomiting";
        break;
      case SEPatientEventType::StartOfCardiacCycle:
      case SEPatientEventType::StartOfExhale:
      case SEPatientEventType::StartOfInhale:
        m_ss.str(""); // make m_ss empty and nothing will be logged, this event does not need to get logged each activation
        break;
      default:
        m_ss << " Patient has entered state : " << type; // TODO SEPatientEventType::_xsd_enumPatientEvent_literals_[type];
      }
    } else {
      switch (type) {
      case SEPatientEventType::AcuteLungInjury:
        m_ss << " Patient no longer has Acute Lung Injury";
        break;
      case SEPatientEventType::AcuteRespiratoryDistress:
        m_ss << " Patient no longer has Acute Respiratory Distress";
        break;
      case SEPatientEventType::Antidiuresis:
        m_ss << " Patient no longer is in Antidiuresis";
        break;
      case SEPatientEventType::Asystole:
        m_ss << " Patient no longer is in Asystole";
        break;
      case SEPatientEventType::Bradycardia:
        m_ss << " Patient no longer has Bradycardia";
        break;
      case SEPatientEventType::Bradypnea:
        m_ss << " Patient no longer has Bradypnea";
        break;
      case SEPatientEventType::BrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain has increased above the danger threshold";
        break;
      case SEPatientEventType::CardiacArrest:
        m_ss << " Patient no longer has Cardiac Arrest";
        break;
      case SEPatientEventType::CardiogenicShock:
        m_ss << " Patient no longer has Cardiogenic Shock";
        break;
      case SEPatientEventType::CompartmentSyndromeAbdominal:
        m_ss << " Patient no longer has Abdominal Compartment Syndrome";
        break;
      case SEPatientEventType::CompartmentSyndromeLeftArm:
        m_ss << " Patient no longer has Compartment Syndrome in the Left Arm";
        break;
      case SEPatientEventType::CompartmentSyndromeLeftLeg:
        m_ss << " Patient no longer has Compartment Syndrome in the Left Leg";
        break;
      case SEPatientEventType::CompartmentSyndromeRightArm:
        m_ss << " Patient no longer has Compartment Syndrome in the Right Arm";
        break;
      case SEPatientEventType::CompartmentSyndromeRightLeg:
        m_ss << " Patient no longer has Compartment Syndrome in the Right Leg";
        break;
      case SEPatientEventType::CriticalBrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain has increased above the critical threshold";
        break;
      case SEPatientEventType::Dehydration:
        m_ss << " Patient no longer is in Dehydration state";
        break;
      case SEPatientEventType::Diuresis:
        m_ss << " Patient no longer has Diuresis";
        break;
      case SEPatientEventType::Fasciculation:
        m_ss << "Patient no longer has Fasciculation";
        break;
      case SEPatientEventType::FunctionalIncontinence:
        m_ss << " Patient has an empty bladder";
        break;
      case SEPatientEventType::HemolyticTransfusionReaction:
        m_ss << " Patient is no longer undergoing a hemolytic transfusion reaction";
        break;
      case SEPatientEventType::Hypercapnia:
        m_ss << " Patient no longer has Hypercapnia";
        break;
      case SEPatientEventType::Hyperglycemia:
        m_ss << " Patient no longer has Hyperglycemia";
        break;
      case SEPatientEventType::Hyperthermia:
        m_ss << " Patient is no longer has Hyperthermic";
        break;
      case SEPatientEventType::Hypoglycemia:
        m_ss << " Patient no longer has Hypoglycemia";
        break;
      case SEPatientEventType::HypoglycemicShock:
        m_ss << " Patient is no longer in Hypoglycemic Shock";
        break;
      case SEPatientEventType::HypoglycemicComa:
        m_ss << " Patient is no longer in a Hypoglycemic Coma";
        break;
      case SEPatientEventType::MildHypothermia:
        m_ss << " Patient is no longer Mildly Hypothermic";
        break;
      case SEPatientEventType::ModerateHypothermia:
        m_ss << " Patient is no longer Moderately Hypothermic";
        break;
      case SEPatientEventType::SevereHypothermia:
        m_ss << " Patient is no longer Severely Hypothermic";
        break;
      case SEPatientEventType::Hypoxia:
        m_ss << " Patient no longer has Hypoxia";
        break;
      case SEPatientEventType::HypovolemicShock:
        m_ss << " Patient is no longer in Hypovolemic Shock";
        break;
      case SEPatientEventType::IntracranialHypertension:
        m_ss << " Patient no longer has Intracranial Hypertension";
        break;
      case SEPatientEventType::IntracranialHypotension:
        m_ss << " Patient no longer has Intracranial Hypotension";
        break;
      case SEPatientEventType::IrreversibleState:
        m_ss << " Patient no longer is in irreversible state?!";
        break;
      case SEPatientEventType::Ketoacidosis:
        m_ss << " Patient no longer has Ketoacidosis";
        break;
      case SEPatientEventType::LacticAcidosis:
        m_ss << " Patient no longer has Lactic Acidosis";
        break;
      case SEPatientEventType::LiverGlycogenDepleted:
        m_ss << " Patient's liver glycogen is no longer depleted";
        break;
      case SEPatientEventType::MaximumPulmonaryVentilationRate:
        m_ss << " Patient's Respiratory Driver is no longer exceeding the maximum target pulmonary ventilation rate";
        break;
      case SEPatientEventType::MetabolicAcidosis:
        m_ss << " The patient is no longer in a state of metabolic acidosis";
        break;
      case SEPatientEventType::MetabolicAlkalosis:
        m_ss << " The patient is no longer in a state of metabolic alkalosis";
        break;
      case SEPatientEventType::MuscleGlycogenDepleted:
        m_ss << " Patient's muscle glycogen is no longer depleted";
        break;
      case SEPatientEventType::MyocardiumOxygenDeficit:
        m_ss << " Patient no longer has a Myocardium Oxygen Deficit";
        break;
      case SEPatientEventType::Natriuresis:
        m_ss << " Patient no longer has Natriuresis";
        break;
      case SEPatientEventType::NutritionDepleted:
        m_ss << " Patient has nutrition in body";
        break;
      case SEPatientEventType::PulselessRhythm:
        m_ss << " Patient no longer has a Pulseless Rhythm";
        break;
      case SEPatientEventType::RenalHypoperfusion:
        m_ss << " Patient no longer has Renal Hypoperfusion";
        break;
      case SEPatientEventType::SevereAcuteRespiratoryDistress:
        m_ss << " Patient no longer has a Severe Acute Respiratory Distress";
        break;
      case SEPatientEventType::SevereSepsis:
        m_ss << " Patient no longer has severe sepsis";
        break;
      case SEPatientEventType::Shivering:
        m_ss << " Patient is longer shivering";
        break;
      case SEPatientEventType::Tachycardia:
        m_ss << " Patient no longer has Tachycardia";
        break;
      case SEPatientEventType::Tachypnea:
        m_ss << " Patient no longer has Tachypnea";
        break;
      case SEPatientEventType::Fatigue:
        m_ss << " Patient is no longer fatigued";
        break;
      case SEPatientEventType::StartOfCardiacCycle:
      case SEPatientEventType::StartOfExhale:
      case SEPatientEventType::StartOfInhale:
        m_ss.str(""); // make m_ss empty and nothing will be logged, this event does not need to get logged each activation
        break;
      default:
        m_ss << " Patient has exited state : " << type; // TODO SEPatientEventType::_xsd_enumPatientEvent_literals_[type];
      }
    }
    if (!m_ss.str().empty())
      Info(m_ss);
  }
  m_EventState[type] = active;
  m_EventDuration_s[type] = 0;
  if (m_EventHandler != nullptr) {
    m_EventHandler->HandlePatientEvent(type, active, &time);
  }
  if (m_EventCallbacks[type] != nullptr) {
    m_EventCallbacks[type](active);
  }
}
//-----------------------------------------------------------------------------
bool SEPatient::IsEventActive(SEPatientEventType type) const
{
  return m_EventState.at(type);
}
//-----------------------------------------------------------------------------
double SEPatient::GetEventDuration(SEPatientEventType type, const TimeUnit& unit) const
{
  auto i = m_EventDuration_s.find(type);
  if (i == m_EventDuration_s.end()) {
    return 0;
  }
  return Convert(i->second, TimeUnit::s, unit);
}
//-----------------------------------------------------------------------------
void SEPatient::UpdateEvents(const SEScalarTime& timeStep)
{
  for (auto& itr : m_EventDuration_s) {
    itr.second += timeStep.GetValue(TimeUnit::s);
  }
}
//-----------------------------------------------------------------------------
void SEPatient::ForwardEvents(SEEventHandler* handler) const
{
  m_EventHandler = handler;
}
//-----------------------------------------------------------------------------
std::string SEPatient::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SEPatient::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SEPatient::SetName(const char* name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
void SEPatient::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
std::string SEPatient::GetAnnotation() const
{
  return m_Annotation;
}
//-----------------------------------------------------------------------------
const char* SEPatient::GetAnnotation_cStr() const
{
  return m_Annotation.c_str();
}
//-----------------------------------------------------------------------------
void SEPatient::SetAnnotation(const char* Annotation)
{
  m_Annotation = Annotation;
}
//-----------------------------------------------------------------------------
void SEPatient::SetAnnotation(const std::string& Annotation)
{
  m_Annotation = Annotation;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasAnnotation() const
{
  return m_Annotation.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateAnnotation()
{
  m_Annotation = "";
}
//-----------------------------------------------------------------------------
SESex SEPatient::GetSex() const
{
  return m_Sex;
}
//-----------------------------------------------------------------------------
void SEPatient::SetSex(SESex gender)
{
  m_Sex = gender;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasSex() const
{
  return m_Sex == (SESex::Invalid) ? false : true;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateSex()
{
  m_Sex = SESex::Invalid;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasAge() const
{
  return m_Age == nullptr ? false : m_Age->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& SEPatient::GetAge()
{
  if (m_Age == nullptr) {
    m_Age = new SEScalarTime();
  }
  return *m_Age;
}
//-----------------------------------------------------------------------------
double SEPatient::GetAge(const TimeUnit& unit) const
{
  if (m_Age == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Age->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasWeight() const
{
  return m_Weight == nullptr ? false : m_Weight->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SEPatient::GetWeight()
{
  if (m_Weight == nullptr) {
    m_Weight = new SEScalarMass();
  }
  return *m_Weight;
}
//-----------------------------------------------------------------------------
double SEPatient::GetWeight(const MassUnit& unit) const
{
  if (m_Weight == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Weight->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasHeight() const
{
  return m_Height == nullptr ? false : m_Height->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarLength& SEPatient::GetHeight()
{
  if (m_Height == nullptr) {
    m_Height = new SEScalarLength();
  }
  return *m_Height;
}
//-----------------------------------------------------------------------------
double SEPatient::GetHeight(const LengthUnit& unit) const
{
  if (m_Height == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Height->GetValue(unit);
}
//-----------------------------------------------------------------------------
void SEPatient::CalculateWeightByBMI(const CDM::ScalarData& bmi)
{
  if (!HasWeight()) {
    m_Weight = new SEScalarMass();
    double height_m = GetHeight(LengthUnit::m);
    double weightByBMI_kg = bmi.value() * std::pow(height_m, 2.);

    m_Weight->SetValue(weightByBMI_kg, MassUnit::kg);
    std::stringstream ss;
    ss << "Based on BMI input, the patient weight is being set at " << weightByBMI_kg << "kg." << std::endl;
    Info(ss);
  }
}
//-----------------------------------------------------------------------------
void SEPatient::CalculateHeightByBMI(const CDM::ScalarData& bmi)
{
  if (!HasHeight()) {
    m_Height = new SEScalarLength();
    double weight_kg = GetWeight(MassUnit::kg);
    double heightByBMI_m = std::sqrt(weight_kg / bmi.value());

    m_Height->SetValue(heightByBMI_m, LengthUnit::m);
    std::stringstream ss;
    ss << "Based on BMI input, the patient height is being set at " << heightByBMI_m << "m." << std::endl;
    Info(ss);
  }
}
//-----------------------------------------------------------------------------
bool SEPatient::HasAlveoliSurfaceArea() const
{
  return m_AlveoliSurfaceArea == nullptr ? false : m_AlveoliSurfaceArea->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarArea& SEPatient::GetAlveoliSurfaceArea()
{
  if (m_AlveoliSurfaceArea == nullptr) {
    m_AlveoliSurfaceArea = new SEScalarArea();
  }
  return *m_AlveoliSurfaceArea;
}
//-----------------------------------------------------------------------------
double SEPatient::GetAlveoliSurfaceArea(const AreaUnit& unit) const
{
  if (m_AlveoliSurfaceArea == nullptr) {
    return SEScalar::dNaN();
  }
  return m_AlveoliSurfaceArea->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasBasalMetabolicRate() const
{
  return m_BasalMetabolicRate == nullptr ? false : m_BasalMetabolicRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPower& SEPatient::GetBasalMetabolicRate()
{
  if (m_BasalMetabolicRate == nullptr) {
    m_BasalMetabolicRate = new SEScalarPower();
  }
  return *m_BasalMetabolicRate;
}
//-----------------------------------------------------------------------------
double SEPatient::GetBasalMetabolicRate(const PowerUnit& unit) const
{
  if (m_BasalMetabolicRate == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BasalMetabolicRate->GetValue(unit);
}
//------------------------------------------------------------------------------
SEBloodType SEPatient::GetBloodType() const
{
  return m_BloodType;
}
//-----------------------------------------------------------------------------
void SEPatient::SetBloodType(SEBloodType bloodAntigen)
{
  m_BloodType = bloodAntigen;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasBloodType() const
{
  return m_BloodType == SEBloodType::Invalid ? false : true;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateBloodType()
{
  m_BloodType = SEBloodType::Invalid;
}
//-----------------------------------------------------------------------------
bool SEPatient::GetBloodRh() const
{
  return m_BloodRh;
}
//-----------------------------------------------------------------------------
void SEPatient::SetBloodRh(bool bloodRh)
{
  m_BloodRh = bloodRh;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasBloodRh() const
{
  return (m_BloodRh == true || m_BloodRh == false) ? true : false;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateBloodRh()
{
  m_BloodRh = true;
}
//---------------------------------------------------------------------------
bool SEPatient::HasBloodVolumeBaseline() const
{
  return m_BloodVolumeBaseline == nullptr ? false : m_BloodVolumeBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetBloodVolumeBaseline()
{
  if (m_BloodVolumeBaseline == nullptr) {
    m_BloodVolumeBaseline = new SEScalarVolume();
  }
  return *m_BloodVolumeBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetBloodVolumeBaseline(const VolumeUnit& unit) const
{
  if (m_BloodVolumeBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodVolumeBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasBodyDensity() const
{
  return m_BodyDensity == nullptr ? false : m_BodyDensity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMassPerVolume& SEPatient::GetBodyDensity()
{
  if (m_BodyDensity == nullptr) {
    m_BodyDensity = new SEScalarMassPerVolume();
  }
  return *m_BodyDensity;
}
//-----------------------------------------------------------------------------
double SEPatient::GetBodyDensity(const MassPerVolumeUnit& unit) const
{
  if (m_BodyDensity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BodyDensity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasBodyFatFraction() const
{
  return m_BodyFatFraction == nullptr ? false : m_BodyFatFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEPatient::GetBodyFatFraction()
{
  if (m_BodyFatFraction == nullptr) {
    m_BodyFatFraction = new SEScalarFraction();
  }
  return *m_BodyFatFraction;
}
//-----------------------------------------------------------------------------
double SEPatient::GetBodyFatFraction() const
{
  if (m_BodyFatFraction == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BodyFatFraction->GetValue();
}
//-----------------------------------------------------------------------------
bool SEPatient::HasDiastolicArterialPressureBaseline() const
{
  return m_DiastolicArterialPressureBaseline == nullptr ? false : m_DiastolicArterialPressureBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEPatient::GetDiastolicArterialPressureBaseline()
{
  if (m_DiastolicArterialPressureBaseline == nullptr) {
    m_DiastolicArterialPressureBaseline = new SEScalarPressure();
  }
  return *m_DiastolicArterialPressureBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetDiastolicArterialPressureBaseline(const PressureUnit& unit) const
{
  if (m_DiastolicArterialPressureBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_DiastolicArterialPressureBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasExpiratoryReserveVolume() const
{
  return m_ExpiratoryReserveVolume == nullptr ? false : m_ExpiratoryReserveVolume->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetExpiratoryReserveVolume()
{
  if (m_ExpiratoryReserveVolume == nullptr) {
    m_ExpiratoryReserveVolume = new SEScalarVolume();
  }
  return *m_ExpiratoryReserveVolume;
}
//-----------------------------------------------------------------------------
double SEPatient::GetExpiratoryReserveVolume(const VolumeUnit& unit) const
{
  if (m_ExpiratoryReserveVolume == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ExpiratoryReserveVolume->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasFunctionalResidualCapacity() const
{
  return m_FunctionalResidualCapacity == nullptr ? false : m_FunctionalResidualCapacity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetFunctionalResidualCapacity()
{
  if (m_FunctionalResidualCapacity == nullptr) {
    m_FunctionalResidualCapacity = new SEScalarVolume();
  }
  return *m_FunctionalResidualCapacity;
}
//-----------------------------------------------------------------------------
double SEPatient::GetFunctionalResidualCapacity(const VolumeUnit& unit) const
{
  if (m_FunctionalResidualCapacity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_FunctionalResidualCapacity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasHeartRateBaseline() const
{
  return m_HeartRateBaseline == nullptr ? false : m_HeartRateBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SEPatient::GetHeartRateBaseline()
{
  if (m_HeartRateBaseline == nullptr) {
    m_HeartRateBaseline = new SEScalarFrequency();
  }
  return *m_HeartRateBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetHeartRateBaseline(const FrequencyUnit& unit) const
{
  if (m_HeartRateBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartRateBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasHeartRateMaximum() const
{
  return m_HeartRateMaximum == nullptr ? false : m_HeartRateMaximum->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SEPatient::GetHeartRateMaximum()
{
  if (m_HeartRateMaximum == nullptr) {
    m_HeartRateMaximum = new SEScalarFrequency();
  }
  return *m_HeartRateMaximum;
}
//-----------------------------------------------------------------------------
double SEPatient::GetHeartRateMaximum(const FrequencyUnit& unit) const
{
  if (m_HeartRateMaximum == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartRateMaximum->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasHeartRateMinimum() const
{
  return m_HeartRateMinimum == nullptr ? false : m_HeartRateMinimum->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SEPatient::GetHeartRateMinimum()
{
  if (m_HeartRateMinimum == nullptr) {
    m_HeartRateMinimum = new SEScalarFrequency();
  }
  return *m_HeartRateMinimum;
}
//-----------------------------------------------------------------------------
double SEPatient::GetHeartRateMinimum(const FrequencyUnit& unit) const
{
  if (m_HeartRateMinimum == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HeartRateMinimum->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasHyperhidrosis() const
{
  return m_Hyperhidrosis == nullptr ? false : m_Hyperhidrosis->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarNeg1To1& SEPatient::GetHyperhidrosis()
{
  if (m_Hyperhidrosis == nullptr)
    m_Hyperhidrosis = new SEScalarNeg1To1();
  return *m_Hyperhidrosis;
}
//-----------------------------------------------------------------------------
double SEPatient::GetHyperhidrosis() const
{
  if (m_Hyperhidrosis == nullptr)
    return SEScalar::dNaN();
  return m_Hyperhidrosis->GetValue();
}
//-----------------------------------------------------------------------------
bool SEPatient::HasInspiratoryCapacity() const
{
  return m_InspiratoryCapacity == nullptr ? false : m_InspiratoryCapacity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetInspiratoryCapacity()
{
  if (m_InspiratoryCapacity == nullptr) {
    m_InspiratoryCapacity = new SEScalarVolume();
  }
  return *m_InspiratoryCapacity;
}
//-----------------------------------------------------------------------------
double SEPatient::GetInspiratoryCapacity(const VolumeUnit& unit) const
{
  if (m_InspiratoryCapacity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InspiratoryCapacity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasInspiratoryReserveVolume() const
{
  return m_InspiratoryReserveVolume == nullptr ? false : m_InspiratoryReserveVolume->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetInspiratoryReserveVolume()
{
  if (m_InspiratoryReserveVolume == nullptr) {
    m_InspiratoryReserveVolume = new SEScalarVolume();
  }
  return *m_InspiratoryReserveVolume;
}
//-----------------------------------------------------------------------------
double SEPatient::GetInspiratoryReserveVolume(const VolumeUnit& unit) const
{
  if (m_InspiratoryReserveVolume == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InspiratoryReserveVolume->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasLeanBodyMass() const
{
  return m_LeanBodyMass == nullptr ? false : m_LeanBodyMass->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SEPatient::GetLeanBodyMass()
{
  if (m_LeanBodyMass == nullptr) {
    m_LeanBodyMass = new SEScalarMass();
  }
  return *m_LeanBodyMass;
}
//-----------------------------------------------------------------------------
double SEPatient::GetLeanBodyMass(const MassUnit& unit) const
{
  if (m_LeanBodyMass == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LeanBodyMass->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasMaxWorkRate() const
{
  return m_MaxWorkRate == nullptr ? false : m_MaxWorkRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPower& SEPatient::GetMaxWorkRate()
{
  if (m_MaxWorkRate == nullptr) {
    m_MaxWorkRate = new SEScalarPower();
  }
  return *m_MaxWorkRate;
}
//-----------------------------------------------------------------------------
double SEPatient::GetMaxWorkRate(const PowerUnit& unit) const
{
  if (m_MaxWorkRate == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MaxWorkRate->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasMuscleMass() const
{
  return m_MuscleMass == nullptr ? false : m_MuscleMass->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SEPatient::GetMuscleMass()
{
  if (m_MuscleMass == nullptr) {
    m_MuscleMass = new SEScalarMass();
  }
  return *m_MuscleMass;
}
//-----------------------------------------------------------------------------
double SEPatient::GetMuscleMass(const MassUnit& unit) const
{
  if (m_MuscleMass == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MuscleMass->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasMeanArterialPressureBaseline() const
{
  return m_MeanArterialPressureBaseline == nullptr ? false : m_MeanArterialPressureBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEPatient::GetMeanArterialPressureBaseline()
{
  if (m_MeanArterialPressureBaseline == nullptr) {
    m_MeanArterialPressureBaseline = new SEScalarPressure();
  }
  return *m_MeanArterialPressureBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetMeanArterialPressureBaseline(const PressureUnit& unit) const
{
  if (m_MeanArterialPressureBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MeanArterialPressureBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasPainSusceptibility() const
{
  return m_PainSusceptibility == nullptr ? false : m_PainSusceptibility->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarNeg1To1& SEPatient::GetPainSusceptibility()
{
  if (m_PainSusceptibility == nullptr)
    m_PainSusceptibility = new SEScalarNeg1To1();
  return *m_PainSusceptibility;
}
//-----------------------------------------------------------------------------
double SEPatient::GetPainSusceptibility() const
{
  if (m_PainSusceptibility == nullptr)
    return SEScalar::dNaN();
  return m_PainSusceptibility->GetValue();
}
//-----------------------------------------------------------------------------
bool SEPatient::HasResidualVolume() const
{
  return m_ResidualVolume == nullptr ? false : m_ResidualVolume->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetResidualVolume()
{
  if (m_ResidualVolume == nullptr) {
    m_ResidualVolume = new SEScalarVolume();
  }
  return *m_ResidualVolume;
}
//-----------------------------------------------------------------------------
double SEPatient::GetResidualVolume(const VolumeUnit& unit) const
{
  if (m_ResidualVolume == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ResidualVolume->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasRespirationRateBaseline() const
{
  return m_RespirationRateBaseline == nullptr ? false : m_RespirationRateBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SEPatient::GetRespirationRateBaseline()
{
  if (m_RespirationRateBaseline == nullptr) {
    m_RespirationRateBaseline = new SEScalarFrequency();
  }
  return *m_RespirationRateBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetRespirationRateBaseline(const FrequencyUnit& unit) const
{
  if (m_RespirationRateBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RespirationRateBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasRespiratoryDriverAmplitudeBaseline() const
{
  return m_RespiratoryDriverAmplitudeBaseline == nullptr ? false : m_RespiratoryDriverAmplitudeBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEPatient::GetRespiratoryDriverAmplitudeBaseline()
{
  if (m_RespiratoryDriverAmplitudeBaseline == nullptr) {
    m_RespiratoryDriverAmplitudeBaseline = new SEScalarPressure();
  }
  return *m_RespiratoryDriverAmplitudeBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetRespiratoryDriverAmplitudeBaseline(const PressureUnit& unit) const
{
  if (m_RespiratoryDriverAmplitudeBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RespiratoryDriverAmplitudeBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasRightLungRatio() const
{
  return m_RightLungRatio == nullptr ? false : m_RightLungRatio->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEPatient::GetRightLungRatio()
{
  if (m_RightLungRatio == nullptr) {
    m_RightLungRatio = new SEScalarFraction();
  }
  return *m_RightLungRatio;
}
//-----------------------------------------------------------------------------
double SEPatient::GetRightLungRatio() const
{
  if (m_RightLungRatio == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RightLungRatio->GetValue();
}
//-----------------------------------------------------------------------------
bool SEPatient::HasSkinSurfaceArea() const
{
  return m_SkinSurfaceArea == nullptr ? false : m_SkinSurfaceArea->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarArea& SEPatient::GetSkinSurfaceArea()
{
  if (m_SkinSurfaceArea == nullptr) {
    m_SkinSurfaceArea = new SEScalarArea();
  }
  return *m_SkinSurfaceArea;
}
//-----------------------------------------------------------------------------
double SEPatient::GetSkinSurfaceArea(const AreaUnit& unit) const
{
  if (m_SkinSurfaceArea == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SkinSurfaceArea->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasSleepAmount() const
{
  return m_SleepAmount == nullptr ? false : m_SleepAmount->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& SEPatient::GetSleepAmount()
{
  if (m_SleepAmount == nullptr) {
    m_SleepAmount = new SEScalarTime();
  }
  return *m_SleepAmount;
}
//-----------------------------------------------------------------------------
double SEPatient::GetSleepAmount(const TimeUnit& unit) const
{
  if (m_SleepAmount == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SleepAmount->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasSystolicArterialPressureBaseline() const
{
  return m_SystolicArterialPressureBaseline == nullptr ? false : m_SystolicArterialPressureBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEPatient::GetSystolicArterialPressureBaseline()
{
  if (m_SystolicArterialPressureBaseline == nullptr) {
    m_SystolicArterialPressureBaseline = new SEScalarPressure();
  }
  return *m_SystolicArterialPressureBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetSystolicArterialPressureBaseline(const PressureUnit& unit) const
{
  if (m_SystolicArterialPressureBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_SystolicArterialPressureBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasTotalVentilationBaseline() const
{
  return m_TotalVentilationBaseline == nullptr ? false : m_TotalVentilationBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTime& SEPatient::GetTotalVentilationBaseline()
{
  if (m_TotalVentilationBaseline == nullptr) {
    m_TotalVentilationBaseline = new SEScalarVolumePerTime();
  }
  return *m_TotalVentilationBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetTotalVentilationBaseline(const VolumePerTimeUnit& unit) const
{
  if (m_TotalVentilationBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalVentilationBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasTidalVolumeBaseline() const
{
  return m_TidalVolumeBaseline == nullptr ? false : m_TidalVolumeBaseline->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetTidalVolumeBaseline()
{
  if (m_TidalVolumeBaseline == nullptr) {
    m_TidalVolumeBaseline = new SEScalarVolume();
  }
  return *m_TidalVolumeBaseline;
}
//-----------------------------------------------------------------------------
double SEPatient::GetTidalVolumeBaseline(const VolumeUnit& unit) const
{
  if (m_TidalVolumeBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TidalVolumeBaseline->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasTotalLungCapacity() const
{
  return m_TotalLungCapacity == nullptr ? false : m_TotalLungCapacity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetTotalLungCapacity()
{
  if (m_TotalLungCapacity == nullptr) {
    m_TotalLungCapacity = new SEScalarVolume();
  }
  return *m_TotalLungCapacity;
}
//-----------------------------------------------------------------------------
double SEPatient::GetTotalLungCapacity(const VolumeUnit& unit) const
{
  if (m_TotalLungCapacity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalLungCapacity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::HasVitalCapacity() const
{
  return m_VitalCapacity == nullptr ? false : m_VitalCapacity->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SEPatient::GetVitalCapacity()
{
  if (m_VitalCapacity == nullptr) {
    m_VitalCapacity = new SEScalarVolume();
  }
  return *m_VitalCapacity;
}
//-----------------------------------------------------------------------------
double SEPatient::GetVitalCapacity(const VolumeUnit& unit) const
{
  if (m_VitalCapacity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VitalCapacity->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEPatient::operator==(SEPatient const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_EventDuration_s == rhs.m_EventDuration_s
    && m_Name == rhs.m_Name
    && m_Annotation == rhs.m_Annotation
    && m_Sex == rhs.m_Sex
    && ((m_Age && rhs.m_Age)
          ? m_Age->operator==(*rhs.m_Age)
          : m_Age == rhs.m_Age)
    && ((m_Weight && rhs.m_Weight)
          ? m_Weight->operator==(*rhs.m_Weight)
          : m_Weight == rhs.m_Weight)
    && ((m_Height && rhs.m_Height)
          ? m_Height->operator==(*rhs.m_Height)
          : m_Height == rhs.m_Height)
    && ((m_BodyDensity && rhs.m_BodyDensity)
          ? m_BodyDensity->operator==(*rhs.m_BodyDensity)
          : m_BodyDensity == rhs.m_BodyDensity)
    && ((m_BodyFatFraction && rhs.m_BodyFatFraction)
          ? m_BodyFatFraction->operator==(*rhs.m_BodyFatFraction)
          : m_BodyFatFraction == rhs.m_BodyFatFraction)
    && ((m_LeanBodyMass && rhs.m_LeanBodyMass)
          ? m_LeanBodyMass->operator==(*rhs.m_LeanBodyMass)
          : m_LeanBodyMass == rhs.m_LeanBodyMass)
    && ((m_MaxWorkRate && rhs.m_MaxWorkRate)
          ? m_MaxWorkRate->operator==(*rhs.m_MaxWorkRate)
          : m_MaxWorkRate == rhs.m_MaxWorkRate)
    && ((m_MuscleMass && rhs.m_MuscleMass)
          ? m_MuscleMass->operator==(*rhs.m_MuscleMass)
          : m_MuscleMass == rhs.m_MuscleMass)
    && m_BloodType == rhs.m_BloodType
    && m_BloodRh == rhs.m_BloodRh
    && ((m_AlveoliSurfaceArea && rhs.m_AlveoliSurfaceArea)
          ? m_AlveoliSurfaceArea->operator==(*rhs.m_AlveoliSurfaceArea)
          : m_AlveoliSurfaceArea == rhs.m_AlveoliSurfaceArea)
    && ((m_RightLungRatio && rhs.m_RightLungRatio)
          ? m_RightLungRatio->operator==(*rhs.m_RightLungRatio)
          : m_RightLungRatio == rhs.m_RightLungRatio)
    && ((m_SkinSurfaceArea && rhs.m_SkinSurfaceArea)
          ? m_SkinSurfaceArea->operator==(*rhs.m_SkinSurfaceArea)
          : m_SkinSurfaceArea == rhs.m_SkinSurfaceArea)
    && ((m_BasalMetabolicRate && rhs.m_BasalMetabolicRate)
          ? m_BasalMetabolicRate->operator==(*rhs.m_BasalMetabolicRate)
          : m_BasalMetabolicRate == rhs.m_BasalMetabolicRate)
    && ((m_BloodVolumeBaseline && rhs.m_BloodVolumeBaseline)
          ? m_BloodVolumeBaseline->operator==(*rhs.m_BloodVolumeBaseline)
          : m_BloodVolumeBaseline == rhs.m_BloodVolumeBaseline)
    && ((m_DiastolicArterialPressureBaseline && rhs.m_DiastolicArterialPressureBaseline)
          ? m_DiastolicArterialPressureBaseline->operator==(*rhs.m_DiastolicArterialPressureBaseline)
          : m_DiastolicArterialPressureBaseline == rhs.m_DiastolicArterialPressureBaseline)
    && ((m_HeartRateBaseline && rhs.m_HeartRateBaseline)
          ? m_HeartRateBaseline->operator==(*rhs.m_HeartRateBaseline)
          : m_HeartRateBaseline == rhs.m_HeartRateBaseline)
    && ((m_Hyperhidrosis && rhs.m_Hyperhidrosis)
          ? m_Hyperhidrosis->operator==(*rhs.m_Hyperhidrosis)
          : m_Hyperhidrosis == rhs.m_Hyperhidrosis)
    && ((m_MeanArterialPressureBaseline && rhs.m_MeanArterialPressureBaseline)
          ? m_MeanArterialPressureBaseline->operator==(*rhs.m_MeanArterialPressureBaseline)
          : m_MeanArterialPressureBaseline == rhs.m_MeanArterialPressureBaseline)
    && ((m_PainSusceptibility && rhs.m_PainSusceptibility)
          ? m_PainSusceptibility->operator==(*rhs.m_PainSusceptibility)
          : m_PainSusceptibility == rhs.m_PainSusceptibility)
    && ((m_RespirationRateBaseline && rhs.m_RespirationRateBaseline)
          ? m_RespirationRateBaseline->operator==(*rhs.m_RespirationRateBaseline)
          : m_RespirationRateBaseline == rhs.m_RespirationRateBaseline)
    && ((m_RespiratoryDriverAmplitudeBaseline && rhs.m_RespiratoryDriverAmplitudeBaseline)
          ? m_RespiratoryDriverAmplitudeBaseline->operator==(*rhs.m_RespiratoryDriverAmplitudeBaseline)
          : m_RespiratoryDriverAmplitudeBaseline == rhs.m_RespiratoryDriverAmplitudeBaseline)
    && ((m_SleepAmount && rhs.m_SleepAmount)
          ? m_SleepAmount->operator==(*rhs.m_SleepAmount)
          : m_SleepAmount == rhs.m_SleepAmount)
    && ((m_SystolicArterialPressureBaseline && rhs.m_SystolicArterialPressureBaseline)
          ? m_SystolicArterialPressureBaseline->operator==(*rhs.m_SystolicArterialPressureBaseline)
          : m_SystolicArterialPressureBaseline == rhs.m_SystolicArterialPressureBaseline)
    && ((m_TotalVentilationBaseline && rhs.m_TotalVentilationBaseline)
          ? m_TotalVentilationBaseline->operator==(*rhs.m_TotalVentilationBaseline)
          : m_TotalVentilationBaseline == rhs.m_TotalVentilationBaseline)
    && ((m_TidalVolumeBaseline && rhs.m_TidalVolumeBaseline)
          ? m_TidalVolumeBaseline->operator==(*rhs.m_TidalVolumeBaseline)
          : m_TidalVolumeBaseline == rhs.m_TidalVolumeBaseline)
    && ((m_HeartRateMaximum && rhs.m_HeartRateMaximum)
          ? m_HeartRateMaximum->operator==(*rhs.m_HeartRateMaximum)
          : m_HeartRateMaximum == rhs.m_HeartRateMaximum)
    && ((m_HeartRateMinimum && rhs.m_HeartRateMinimum)
          ? m_HeartRateMinimum->operator==(*rhs.m_HeartRateMinimum)
          : m_HeartRateMinimum == rhs.m_HeartRateMinimum)
    && ((m_ExpiratoryReserveVolume && rhs.m_ExpiratoryReserveVolume)
          ? m_ExpiratoryReserveVolume->operator==(*rhs.m_ExpiratoryReserveVolume)
          : m_ExpiratoryReserveVolume == rhs.m_ExpiratoryReserveVolume)
    && ((m_FunctionalResidualCapacity && rhs.m_FunctionalResidualCapacity)
          ? m_FunctionalResidualCapacity->operator==(*rhs.m_FunctionalResidualCapacity)
          : m_FunctionalResidualCapacity == rhs.m_FunctionalResidualCapacity)
    && ((m_InspiratoryCapacity && rhs.m_InspiratoryCapacity)
          ? m_InspiratoryCapacity->operator==(*rhs.m_InspiratoryCapacity)
          : m_InspiratoryCapacity == rhs.m_InspiratoryCapacity)
    && ((m_InspiratoryReserveVolume && rhs.m_InspiratoryReserveVolume)
          ? m_InspiratoryReserveVolume->operator==(*rhs.m_InspiratoryReserveVolume)
          : m_InspiratoryReserveVolume == rhs.m_InspiratoryReserveVolume)
    && ((m_ResidualVolume && rhs.m_ResidualVolume)
          ? m_ResidualVolume->operator==(*rhs.m_ResidualVolume)
          : m_ResidualVolume == rhs.m_ResidualVolume)
    && ((m_TotalLungCapacity && rhs.m_TotalLungCapacity)
          ? m_TotalLungCapacity->operator==(*rhs.m_TotalLungCapacity)
          : m_TotalLungCapacity == rhs.m_TotalLungCapacity)
    && ((m_VitalCapacity && rhs.m_VitalCapacity)
          ? m_VitalCapacity->operator==(*rhs.m_VitalCapacity)
          : m_VitalCapacity == rhs.m_VitalCapacity)
    && m_EventState == rhs.m_EventState;
}
//-------------------------------------------------------------------------------
bool SEPatient::operator!=(SEPatient const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}
