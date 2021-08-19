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

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/io/io-manager.h>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/patients.h>
#endif

#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {

template class std::map<CDM::enumPatientEvent::value, bool>;
template class std::map<CDM::enumPatientEvent::value, void (*)(bool)>;
template class std::map<CDM::enumPatientEvent::value, double>;

SEPatient::SEPatient(Logger* logger)
  : Loggable(logger)
{
  m_EventHandler = nullptr;

  m_Name = "";
  m_Gender = (CDM::enumSex::value)-1;
  m_Age = nullptr;
  m_Weight = nullptr;
  m_Height = nullptr;

  m_AlveoliSurfaceArea = nullptr;
  m_BasalMetabolicRate = nullptr;
  m_BloodRh = true;
  m_BloodType = (CDM::enumBloodType::value)-1;
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

  for (CDM::enumPatientEvent::value key = static_cast<CDM::enumPatientEvent::value>(0); key < CDM::enumPatientEvent::TotalPatientEvents; key = static_cast<CDM::enumPatientEvent::value>(key + 1)) {
    //CDM::enumPatientEvent::value is a code generated C style enum.
    //To my knowladge it must be conitnguous as XSD does not support assinigng manual values to XML Enums that are use dto generated C++ counterpart
    //Short story is the goal is to set every possible enum to a value of false and accomidate future extensions ot the enum.
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

  for (CDM::enumPatientEvent::value key = static_cast<CDM::enumPatientEvent::value>(0); key < CDM::enumPatientEvent::TotalPatientEvents; key = static_cast<CDM::enumPatientEvent::value>(key + 1)) {
    //CDM::enumPatientEvent::value is a code generated C style enum.
    //To my knowladge it must be conitnguous as XSD does not support assinigng manual values to XML Enums that are use dto generated C++ counterpart
    //Short story is the goal is to set every possible enum to a value of false and accomidate future extensions ot the enum.
    m_EventState[key] = false;
    m_EventDuration_s[key] = 0;
  }

  m_Name = "";
  m_Gender = (CDM::enumSex::value)-1;
  SAFE_DELETE(m_Age);
  SAFE_DELETE(m_Weight);
  SAFE_DELETE(m_Height);

  SAFE_DELETE(m_AlveoliSurfaceArea);
  SAFE_DELETE(m_BasalMetabolicRate);
  m_BloodRh = true;
  m_BloodType = (CDM::enumBloodType::value)-1;
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
  Clear();

  m_Name = in.Name();
  if (in.Annotation().present()) {
    m_Annotation = in.Annotation().get();
  }
  if (in.Sex().present()) {
    m_Gender = in.Sex().get();
  }
  if (in.Age().present()) {
    GetAge().Load(in.Age().get());
  }
  if (in.Weight().present()) {
    GetWeight().Load(in.Weight().get());
  }
  if (in.Height().present()) {
    GetHeight().Load(in.Height().get());
  }
  if (in.AlveoliSurfaceArea().present()) {
    GetAlveoliSurfaceArea().Load(in.AlveoliSurfaceArea().get());
  }
  if (in.BasalMetabolicRate().present()) {
    GetBasalMetabolicRate().Load(in.BasalMetabolicRate().get());
  }
  if (in.BloodTypeRh().present()) {
    m_BloodRh = in.BloodTypeRh().get();
  }
  if (in.BloodTypeABO().present()) {
    m_BloodType = in.BloodTypeABO().get();
  }
  if (in.BloodVolumeBaseline().present()) {
    GetBloodVolumeBaseline().Load(in.BloodVolumeBaseline().get());
  }
  if (in.BodyDensity().present()) {
    GetBodyDensity().Load(in.BodyDensity().get());
  }
  if (in.BodyFatFraction().present()) {
    GetBodyFatFraction().Load(in.BodyFatFraction().get());
  }
  if (in.DiastolicArterialPressureBaseline().present()) {
    GetDiastolicArterialPressureBaseline().Load(in.DiastolicArterialPressureBaseline().get());
  }
  if (in.ExpiratoryReserveVolume().present()) {
    GetExpiratoryReserveVolume().Load(in.ExpiratoryReserveVolume().get());
  }
  if (in.FunctionalResidualCapacity().present()) {
    GetFunctionalResidualCapacity().Load(in.FunctionalResidualCapacity().get());
  }
  if (in.HeartRateBaseline().present()) {
    GetHeartRateBaseline().Load(in.HeartRateBaseline().get());
  }
  if (in.HeartRateMaximum().present()) {
    GetHeartRateMaximum().Load(in.HeartRateMaximum().get());
  }
  if (in.HeartRateMinimum().present()) {
    GetHeartRateMinimum().Load(in.HeartRateMinimum().get());
  }
  if (in.Hyperhidrosis().present()) {
    GetHyperhidrosis().Load(in.Hyperhidrosis().get());
  }
  if (in.InspiratoryCapacity().present()) {
    GetInspiratoryCapacity().Load(in.InspiratoryCapacity().get());
  }
  if (in.InspiratoryReserveVolume().present()) {
    GetInspiratoryReserveVolume().Load(in.InspiratoryReserveVolume().get());
  }
  if (in.LeanBodyMass().present()) {
    GetLeanBodyMass().Load(in.LeanBodyMass().get());
  }
  if (in.MaxWorkRate().present()) {
    GetMaxWorkRate().Load(in.MaxWorkRate().get());
  }
  if (in.MuscleMass().present()) {
    GetMuscleMass().Load(in.MuscleMass().get());
  }
  if (in.MeanArterialPressureBaseline().present()) {
    GetMeanArterialPressureBaseline().Load(in.MeanArterialPressureBaseline().get());
  }
  if (in.PainSusceptibility().present()) {
    GetPainSusceptibility().Load(in.PainSusceptibility().get());
  }
  if (in.ResidualVolume().present()) {
    GetResidualVolume().Load(in.ResidualVolume().get());
  }
  if (in.RespirationRateBaseline().present()) {
    GetRespirationRateBaseline().Load(in.RespirationRateBaseline().get());
  }
  if (in.RespiratoryDriverAmplitudeBaseline().present()) {
    GetRespiratoryDriverAmplitudeBaseline().Load(in.RespiratoryDriverAmplitudeBaseline().get());
  }
  if (in.RightLungRatio().present()) {
    GetRightLungRatio().Load(in.RightLungRatio().get());
  }
  if (in.SkinSurfaceArea().present()) {
    GetSkinSurfaceArea().Load(in.SkinSurfaceArea().get());
  }
  if (in.SleepAmount().present()) {
    GetSleepAmount().Load(in.SleepAmount().get());
  }
  if (in.SystolicArterialPressureBaseline().present()) {
    GetSystolicArterialPressureBaseline().Load(in.SystolicArterialPressureBaseline().get());
  }
  if (in.TotalVentilationBaseline().present()) {
    GetTotalVentilationBaseline().Load(in.TotalVentilationBaseline().get());
  }
  if (in.TidalVolumeBaseline().present()) {
    GetTidalVolumeBaseline().Load(in.TidalVolumeBaseline().get());
  }
  if (in.TotalLungCapacity().present()) {
    GetTotalLungCapacity().Load(in.TotalLungCapacity().get());
  }
  if (in.VitalCapacity().present()) {
    GetVitalCapacity().Load(in.VitalCapacity().get());
  }

  SEScalarTime time;
  for (auto e : in.ActiveEvent()) {
    time.Load(e.Duration());
    m_EventState[e.Event()] = true;
    m_EventDuration_s[e.Event()] = time.GetValue(TimeUnit::s);
  }

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
  if (HasName()) {
    data.Name(m_Name);
  }
  if (HasAnnotation()) {
    data.Annotation(m_Annotation);
  }
  if (HasGender()) {
    data.Sex(m_Gender);
  }
  if (m_Age != nullptr) {
    data.Age(std::unique_ptr<CDM::ScalarTimeData>(m_Age->Unload()));
  }
  if (m_Weight != nullptr) {
    data.Weight(std::unique_ptr<CDM::ScalarMassData>(m_Weight->Unload()));
  }
  if (m_Height != nullptr) {
    data.Height(std::unique_ptr<CDM::ScalarLengthData>(m_Height->Unload()));
  }
  if (m_AlveoliSurfaceArea != nullptr) {
    data.AlveoliSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_AlveoliSurfaceArea->Unload()));
  }
  if (m_BasalMetabolicRate != nullptr) {
    data.BasalMetabolicRate(std::unique_ptr<CDM::ScalarPowerData>(m_BasalMetabolicRate->Unload()));
  }
  if (HasBloodRh()) {
    data.BloodTypeRh(m_BloodRh);
  }
  if (HasBloodType()) {
    data.BloodTypeABO(m_BloodType);
  }
  if (m_BloodVolumeBaseline != nullptr) {
    data.BloodVolumeBaseline(std::unique_ptr<CDM::ScalarVolumeData>(m_BloodVolumeBaseline->Unload()));
  }
  if (m_BodyDensity != nullptr) {
    data.BodyDensity(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_BodyDensity->Unload()));
  }
  if (m_BodyFatFraction != nullptr) {
    data.BodyFatFraction(std::unique_ptr<CDM::ScalarFractionData>(m_BodyFatFraction->Unload()));
  }
  if (m_DiastolicArterialPressureBaseline != nullptr) {
    data.DiastolicArterialPressureBaseline(std::unique_ptr<CDM::ScalarPressureData>(m_DiastolicArterialPressureBaseline->Unload()));
  }
  if (m_ExpiratoryReserveVolume != nullptr) {
    data.ExpiratoryReserveVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_ExpiratoryReserveVolume->Unload()));
  }
  if (m_FunctionalResidualCapacity != nullptr) {
    data.FunctionalResidualCapacity(std::unique_ptr<CDM::ScalarVolumeData>(m_FunctionalResidualCapacity->Unload()));
  }
  if (m_HeartRateBaseline != nullptr) {
    data.HeartRateBaseline(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateBaseline->Unload()));
  }
  if (m_HeartRateMaximum != nullptr) {
    data.HeartRateMaximum(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateMaximum->Unload()));
  }
  if (m_HeartRateMinimum != nullptr) {
    data.HeartRateMinimum(std::unique_ptr<CDM::ScalarFrequencyData>(m_HeartRateMinimum->Unload()));
  }
  if (m_Hyperhidrosis != nullptr) {
    data.Hyperhidrosis(std::unique_ptr<CDM::ScalarNeg1To1Data>(m_Hyperhidrosis->Unload()));
  }
  if (m_InspiratoryCapacity != nullptr) {
    data.InspiratoryCapacity(std::unique_ptr<CDM::ScalarVolumeData>(m_InspiratoryCapacity->Unload()));
  }
  if (m_InspiratoryReserveVolume != nullptr) {
    data.InspiratoryReserveVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_InspiratoryReserveVolume->Unload()));
  }
  if (m_LeanBodyMass != nullptr) {
    data.LeanBodyMass(std::unique_ptr<CDM::ScalarMassData>(m_LeanBodyMass->Unload()));
  }
  if (m_MaxWorkRate != nullptr) {
    data.MaxWorkRate(std::unique_ptr<CDM::ScalarPowerData>(m_MaxWorkRate->Unload()));
  }
  if (m_MuscleMass != nullptr) {
    data.MuscleMass(std::unique_ptr<CDM::ScalarMassData>(m_MuscleMass->Unload()));
  }
  if (m_MeanArterialPressureBaseline != nullptr) {
    data.MeanArterialPressureBaseline(std::unique_ptr<CDM::ScalarPressureData>(m_MeanArterialPressureBaseline->Unload()));
  }
  if (m_PainSusceptibility != nullptr) {
    data.PainSusceptibility(std::unique_ptr<CDM::ScalarNeg1To1Data>(m_PainSusceptibility->Unload()));
  }
  if (m_ResidualVolume != nullptr) {
    data.ResidualVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_ResidualVolume->Unload()));
  }
  if (m_RespirationRateBaseline != nullptr) {
    data.RespirationRateBaseline(std::unique_ptr<CDM::ScalarFrequencyData>(m_RespirationRateBaseline->Unload()));
  }
  if (m_RespiratoryDriverAmplitudeBaseline != nullptr) {
    data.RespiratoryDriverAmplitudeBaseline(std::unique_ptr<CDM::ScalarPressureData>(m_RespiratoryDriverAmplitudeBaseline->Unload()));
  }
  if (m_RightLungRatio != nullptr) {
    data.RightLungRatio(std::unique_ptr<CDM::ScalarFractionData>(m_RightLungRatio->Unload()));
  }
  if (m_SkinSurfaceArea != nullptr) {
    data.SkinSurfaceArea(std::unique_ptr<CDM::ScalarAreaData>(m_SkinSurfaceArea->Unload()));
  }
  if (m_SleepAmount != nullptr) {
    data.SleepAmount(std::unique_ptr<CDM::ScalarTimeData>(m_SleepAmount->Unload()));
  }
  if (m_SystolicArterialPressureBaseline != nullptr) {
    data.SystolicArterialPressureBaseline(std::unique_ptr<CDM::ScalarPressureData>(m_SystolicArterialPressureBaseline->Unload()));
  }
  if (m_TotalVentilationBaseline != nullptr) {
    data.TotalVentilationBaseline(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_TotalVentilationBaseline->Unload()));
  }
  if (m_TidalVolumeBaseline != nullptr) {
    data.TidalVolumeBaseline(std::unique_ptr<CDM::ScalarVolumeData>(m_TidalVolumeBaseline->Unload()));
  }
  if (m_TotalLungCapacity != nullptr) {
    data.TotalLungCapacity(std::unique_ptr<CDM::ScalarVolumeData>(m_TotalLungCapacity->Unload()));
  }
  if (m_VitalCapacity != nullptr) {
    data.VitalCapacity(std::unique_ptr<CDM::ScalarVolumeData>(m_VitalCapacity->Unload()));
  }

  SEScalarTime time;
  for (auto itr : m_EventState) {
    if (!itr.second) {
      continue;
    }

    auto it2 = m_EventDuration_s.find(itr.first);
    if (it2 == m_EventDuration_s.end()) // This should not happen...
    {
      time.SetValue(0, TimeUnit::s);
    } else {
      time.SetValue(it2->second, TimeUnit::s);
    }
    CDM::ActivePatientEventData* eData = new CDM::ActivePatientEventData();
    eData->Event(itr.first);
    eData->Duration(std::unique_ptr<CDM::ScalarTimeData>(time.Unload()));
    data.ActiveEvent().push_back(std::unique_ptr<CDM::ActivePatientEventData>(eData));
  }
};
void SEPatient::SetEventCallback(CDM::enumPatientEvent::value type, void (*callback)(bool))
{
  m_EventCallbacks[type] = callback;
}
//-----------------------------------------------------------------------------
void SEPatient::SetEvent(CDM::enumPatientEvent::value type, bool active, const SEScalarTime& time)
{
  if (m_EventState[type] == active) {
    return; //No Change
  } else {
    m_ss.str("");
    m_ss << "[Event] " << time << ", ";
    if (active) {
      switch (type) {
      case CDM::enumPatientEvent::AcuteLungInjury:
        m_ss << " Carrico Index < 300 mmHg:  Patient has Acute Lung Injury";
        break;
      case CDM::enumPatientEvent::AcuteRespiratoryDistress:
        m_ss << " Carrico Index < 200 mmHg: Patient has Acute Respiratory Distress";
        break;
      case CDM::enumPatientEvent::Antidiuresis:
        m_ss << " Patient has Antidiuresis";
        break;
      case CDM::enumPatientEvent::Asystole:
        m_ss << " Patient has Asystole";
        break;
      case CDM::enumPatientEvent::Bradycardia:
        m_ss << " Patient has Bradycardia";
        break;
      case CDM::enumPatientEvent::Bradypnea:
        m_ss << " Patient has Bradypnea";
        break;
      case CDM::enumPatientEvent::BrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain is dangerously low";
        break;
      case CDM::enumPatientEvent::CardiacArrest:
        m_ss << " Patient has Cardiac Arrest";
        break;
      case CDM::enumPatientEvent::CardiogenicShock:
        m_ss << " Patient has Cardiogenic Shock";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_Abdominal:
        m_ss << " Patient has Abdominal Compartment Syndrome";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_LeftArm:
        m_ss << " Patient has Compartment Syndrome in the Left Arm";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_LeftLeg:
        m_ss << " Patient has Compartment Syndrome in the Left Leg";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_RightArm:
        m_ss << " Patient has Compartment Syndrome in the Right Arm";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_RightLeg:
        m_ss << " Patient has Compartment Syndrome in the Right Leg";
        break;
      case CDM::enumPatientEvent::CriticalBrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain is critically low";
        break;
      case CDM::enumPatientEvent::Dehydration:
        m_ss << " Patient has entered state of Dehydration";
        break;
      case CDM::enumPatientEvent::Diuresis:
        m_ss << " Patient has entered Diuresis";
        break;
      case CDM::enumPatientEvent::Fasciculation:
        m_ss << "Patient has Fasciculation";
        break;
      case CDM::enumPatientEvent::FlaccidParalysis:
        m_ss << "Patient has Flaccid Paralysis";
        break;
      case CDM::enumPatientEvent::FunctionalIncontinence:
        m_ss << " Patient has involuntarily emptied their bladder";
        break;
      case CDM::enumPatientEvent::HemolyticTransfusionReaction:
        m_ss << " Patient is undergoing an incompatible blood transfusion";
        break;
      case CDM::enumPatientEvent::Hypercapnia:
        m_ss << " Patient has Hypercapnia";
        break;
      case CDM::enumPatientEvent::Hyperglycemia:
        m_ss << " Patient has Hyperglycemia";
        break;
      case CDM::enumPatientEvent::Hyperthermia:
        m_ss << " Patient is Hyperthermic";
        break;
      case CDM::enumPatientEvent::Hypoglycemia:
        m_ss << " Patient has Hypoglycemia";
        break;
      case CDM::enumPatientEvent::HypoglycemicShock:
        m_ss << " Patient is in Hypoglycemic Shock";
        break;
      case CDM::enumPatientEvent::HypoglycemicComa:
        m_ss << " Patient has fallen into Hypoglycemic Coma";
        break;
      case CDM::enumPatientEvent::Hypothermia:
        m_ss << " Patient is Hypothermic";
        break;
      case CDM::enumPatientEvent::Hypoxia:
        m_ss << " Patient has Hypoxia";
        break;
      case CDM::enumPatientEvent::IntracranialHypertension:
        m_ss << " Patient has Intracranial Hypertension";
        break;
      case CDM::enumPatientEvent::IntracranialHypotension:
        m_ss << " Patient has Intracranial Hypotension";
        break;
      case CDM::enumPatientEvent::HypovolemicShock:
        m_ss << " Patient is in Hypovolemic Shock";
        break;
      case CDM::enumPatientEvent::IrreversibleState:
        m_ss << " Patient has entered irreversible state";
        break;
      case CDM::enumPatientEvent::Ketoacidosis:
        m_ss << " Patient has Ketoacidosis";
        break;
      case CDM::enumPatientEvent::LacticAcidosis:
        m_ss << " Patient has Lactic Acidosis";
        break;
      case CDM::enumPatientEvent::LiverGlycogenDepleted:
        m_ss << " Patient's liver glycogen is depleted";
        break;
      case CDM::enumPatientEvent::MaximumPulmonaryVentilationRate:
        m_ss << " Patient's Respiratory Driver has exceeded the maximum target pulmonary ventilation rate, setting value to the maximum allowable rate";
        break;
      case CDM::enumPatientEvent::MetabolicAcidosis:
        m_ss << " The patient is in a state of metabolic acidosis";
        break;
      case CDM::enumPatientEvent::MetabolicAlkalosis:
        m_ss << " The patient is in a state of metabolic alkalosis";
        break;
      case CDM::enumPatientEvent::MildDiaphoresis:
        m_ss << " Patient has Mild Diaphoresis";
        break;
      case CDM::enumPatientEvent::ModerateDiaphoresis:
        m_ss << " Patient has Moderate Diaphoresis";
        break;
      case CDM::enumPatientEvent::SevereDiaphoresis:
        m_ss << " Patient has Severe Diaphoresis";
        break;
      case CDM::enumPatientEvent::MildSecretions:
        m_ss << " Patient has Mild Secretions";
        break;
      case CDM::enumPatientEvent::ModerateSecretions:
        m_ss << " Patient has Moderate Secretions";
        break;
      case CDM::enumPatientEvent::MildWeakness:
        m_ss << " Patient has Mild Weakness";
        break;
      case CDM::enumPatientEvent::ModerateWeakness:
        m_ss << " Patient has Moderate Weakness";
        break;
      case CDM::enumPatientEvent::MuscleCatabolism:
        m_ss << " Patient has begun muscle catabolism";
        break;
      case CDM::enumPatientEvent::MuscleGlycogenDepleted:
        m_ss << " Patient's muscle glycogen is depleted";
        break;
      case CDM::enumPatientEvent::MyocardiumOxygenDeficit:
        m_ss << " The patient's heart is not receiving enough oxygen";
        break;
      case CDM::enumPatientEvent::Natriuresis:
        m_ss << " Patient has Natriuresis";
        break;
      case CDM::enumPatientEvent::Nausea:
        m_ss << " Patient is Nauseous";
        break;
      case CDM::enumPatientEvent::NutritionDepleted:
        m_ss << " Patient has depleted all nutrition in body";
        break;
      case CDM::enumPatientEvent::PulselessRhythm:
        m_ss << " Patient has a Pulseless Rhythm";
        break;
      case CDM::enumPatientEvent::RenalHypoperfusion:
        m_ss << " Patient has Renal Hypoperfusion";
        break;
      case CDM::enumPatientEvent::Seizures:
        m_ss << " The patient is having a seizure";
        break;
      case CDM::enumPatientEvent::SevereAcuteRespiratoryDistress:
        m_ss << " Carrico Index < 100 : Patient has Severe Acute Respiratory Distress";
        break;
      case CDM::enumPatientEvent::SevereSecretions:
        m_ss << " The patient has severe Secretions";
        break;
      case CDM::enumPatientEvent::SevereSepsis:
        m_ss << " The patient has severe sepsis";
        break;
      case CDM::enumPatientEvent::Tachycardia:
        m_ss << " Patient has Tachycardia";
        break;
      case CDM::enumPatientEvent::Tachypnea:
        m_ss << " Patient has Tachypnea";
        break;
      case CDM::enumPatientEvent::Fatigue:
        m_ss << " Patient has fatigue";
        break;
      case CDM::enumPatientEvent::Vomiting:
        m_ss << " Patient is Vomiting";
        break;
      case CDM::enumPatientEvent::StartOfCardiacCycle:
      case CDM::enumPatientEvent::StartOfExhale:
      case CDM::enumPatientEvent::StartOfInhale:
        m_ss.str(""); // make m_ss empty and nothing will be logged, this event does not need to get logged each activation
        break;
      default:
        m_ss << " Patient has entered state : " << type; // TODO CDM::enumPatientEvent::_xsd_enumPatientEvent_literals_[type];
      }
    } else {
      switch (type) {
      case CDM::enumPatientEvent::AcuteLungInjury:
        m_ss << " Patient no longer has Acute Lung Injury";
        break;
      case CDM::enumPatientEvent::AcuteRespiratoryDistress:
        m_ss << " Patient no longer has Acute Respiratory Distress";
        break;
      case CDM::enumPatientEvent::Antidiuresis:
        m_ss << " Patient no longer is in Antidiuresis";
        break;
      case CDM::enumPatientEvent::Asystole:
        m_ss << " Patient no longer is in Asystole";
        break;
      case CDM::enumPatientEvent::Bradycardia:
        m_ss << " Patient no longer has Bradycardia";
        break;
      case CDM::enumPatientEvent::Bradypnea:
        m_ss << " Patient no longer has Bradypnea";
        break;
      case CDM::enumPatientEvent::BrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain has increased above the danger threshold";
        break;
      case CDM::enumPatientEvent::CardiacArrest:
        m_ss << " Patient no longer has Cardiac Arrest";
        break;
      case CDM::enumPatientEvent::CardiogenicShock:
        m_ss << " Patient no longer has Cardiogenic Shock";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_Abdominal:
        m_ss << " Patient no longer has Abdominal Compartment Syndrome";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_LeftArm:
        m_ss << " Patient no longer has Compartment Syndrome in the Left Arm";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_LeftLeg:
        m_ss << " Patient no longer has Compartment Syndrome in the Left Leg";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_RightArm:
        m_ss << " Patient no longer has Compartment Syndrome in the Right Arm";
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_RightLeg:
        m_ss << " Patient no longer has Compartment Syndrome in the Right Leg";
        break;
      case CDM::enumPatientEvent::CriticalBrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain has increased above the critical threshold";
        break;
      case CDM::enumPatientEvent::Dehydration:
        m_ss << " Patient no longer is in Dehydration state";
        break;
      case CDM::enumPatientEvent::Diuresis:
        m_ss << " Patient no longer has Diuresis";
        break;
      case CDM::enumPatientEvent::Fasciculation:
        m_ss << "Patient no longer has Fasciculation";
        break;
      case CDM::enumPatientEvent::FunctionalIncontinence:
        m_ss << " Patient has an empty bladder";
        break;
      case CDM::enumPatientEvent::HemolyticTransfusionReaction:
        m_ss << " Patient is no longer undergoing a hemolytic transfusion reaction";
        break;
      case CDM::enumPatientEvent::Hypercapnia:
        m_ss << " Patient no longer has Hypercapnia";
        break;
      case CDM::enumPatientEvent::Hyperglycemia:
        m_ss << " Patient no longer has Hyperglycemia";
        break;
      case CDM::enumPatientEvent::Hyperthermia:
        m_ss << " Patient is no longer has Hyperthermic";
        break;
      case CDM::enumPatientEvent::Hypoglycemia:
        m_ss << " Patient no longer has Hypoglycemia";
        break;
      case CDM::enumPatientEvent::HypoglycemicShock:
        m_ss << " Patient is no longer in Hypoglycemic Shock";
        break;
      case CDM::enumPatientEvent::HypoglycemicComa:
        m_ss << " Patient is no longer in a Hypoglycemic Coma";
        break;
      case CDM::enumPatientEvent::Hypothermia:
        m_ss << " Patient is no longer has Hypothermic";
        break;
      case CDM::enumPatientEvent::Hypoxia:
        m_ss << " Patient no longer has Hypoxia";
        break;
      case CDM::enumPatientEvent::HypovolemicShock:
        m_ss << " Patient is no longer in Hypovolemic Shock";
        break;
      case CDM::enumPatientEvent::IntracranialHypertension:
        m_ss << " Patient no longer has Intracranial Hypertension";
        break;
      case CDM::enumPatientEvent::IntracranialHypotension:
        m_ss << " Patient no longer has Intracranial Hypotension";
        break;
      case CDM::enumPatientEvent::IrreversibleState:
        m_ss << " Patient no longer is in irreversible state?!";
        break;
      case CDM::enumPatientEvent::Ketoacidosis:
        m_ss << " Patient no longer has Ketoacidosis";
        break;
      case CDM::enumPatientEvent::LacticAcidosis:
        m_ss << " Patient no longer has Lactic Acidosis";
        break;
      case CDM::enumPatientEvent::LiverGlycogenDepleted:
        m_ss << " Patient's liver glycogen is no longer depleted";
        break;
      case CDM::enumPatientEvent::MaximumPulmonaryVentilationRate:
        m_ss << " Patient's Respiratory Driver is no longer exceeding the maximum target pulmonary ventilation rate";
        break;
      case CDM::enumPatientEvent::MetabolicAcidosis:
        m_ss << " The patient is no longer in a state of metabolic acidosis";
        break;
      case CDM::enumPatientEvent::MetabolicAlkalosis:
        m_ss << " The patient is no longer in a state of metabolic alkalosis";
        break;
      case CDM::enumPatientEvent::MuscleGlycogenDepleted:
        m_ss << " Patient's muscle glycogen is no longer depleted";
        break;
      case CDM::enumPatientEvent::MyocardiumOxygenDeficit:
        m_ss << " Patient no longer has a Myocardium Oxygen Deficit";
        break;
      case CDM::enumPatientEvent::Natriuresis:
        m_ss << " Patient no longer has Natriuresis";
        break;
      case CDM::enumPatientEvent::NutritionDepleted:
        m_ss << " Patient has nutrition in body";
        break;
      case CDM::enumPatientEvent::PulselessRhythm:
        m_ss << " Patient no longer has a Pulseless Rhythm";
        break;
      case CDM::enumPatientEvent::RenalHypoperfusion:
        m_ss << " Patient no longer has Renal Hypoperfusion";
        break;
      case CDM::enumPatientEvent::SevereAcuteRespiratoryDistress:
        m_ss << " Patient no longer has a Severe Acute Respiratory Distress";
        break;
      case CDM::enumPatientEvent::SevereSepsis:
        m_ss << " Patient no longer has severe sepsis";
        break;
      case CDM::enumPatientEvent::Tachycardia:
        m_ss << " Patient no longer has Tachycardia";
        break;
      case CDM::enumPatientEvent::Tachypnea:
        m_ss << " Patient no longer has Tachypnea";
        break;
      case CDM::enumPatientEvent::Fatigue:
        m_ss << " Patient is no longer fatigued";
        break;
      case CDM::enumPatientEvent::StartOfCardiacCycle:
      case CDM::enumPatientEvent::StartOfExhale:
      case CDM::enumPatientEvent::StartOfInhale:
        m_ss.str(""); // make m_ss empty and nothing will be logged, this event does not need to get logged each activation
        break;
      default:
        m_ss << " Patient has exited state : " << type; //TODO CDM::enumPatientEvent::_xsd_enumPatientEvent_literals_[type];
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
bool SEPatient::IsEventActive(CDM::enumPatientEvent::value type) const
{
  return m_EventState.at(type);
}
//-----------------------------------------------------------------------------
double SEPatient::GetEventDuration(CDM::enumPatientEvent::value type, const TimeUnit& unit) const
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
CDM::enumSex::value SEPatient::GetGender() const
{
  return m_Gender;
}
//-----------------------------------------------------------------------------
void SEPatient::SetGender(CDM::enumSex::value sex)
{
  m_Gender = sex;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasGender() const
{
  return m_Gender == ((CDM::enumSex::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateGender()
{
  m_Gender = (CDM::enumSex::value)-1;
}
//-----------------------------------------------------------------------------
[[deprecated("Use GetGender instead")]] CDM::enumSex::value SEPatient::GetSex() const
{
  return m_Gender;
}
//-----------------------------------------------------------------------------
[[deprecated("Use SetGender instead")]] void SEPatient::SetSex(CDM::enumSex::value gender)
{
  m_Gender = gender;
}
//-----------------------------------------------------------------------------
[[deprecated("Use HasGender instead")]] bool SEPatient::HasSex() const
{
  return m_Gender == ((CDM::enumSex::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
[[deprecated("Use InvalidateGender instead")]] void SEPatient::InvalidateSex()
{
  m_Gender = (CDM::enumSex::value)-1;
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
CDM::enumBloodType::value SEPatient::GetBloodType() const
{
  return m_BloodType;
}
//-----------------------------------------------------------------------------
void SEPatient::SetBloodType(CDM::enumBloodType::value bloodAntigen)
{
  m_BloodType = bloodAntigen;
}
//-----------------------------------------------------------------------------
bool SEPatient::HasBloodType() const
{
  return m_BloodType == ((CDM::enumBloodType::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SEPatient::InvalidateBloodType()
{
  m_BloodType = (CDM::enumBloodType::value)-1;
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
}