
#include "Patient.h"

#include "Property.h"

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/enums/SEPatientEnums.h>
#include <biogears/cdm/patient/SEPatient.h>

namespace biogears {
namespace io {
  //----------------------------------------------------------------------------------
  // class SEPatient
  void Patient::UnMarshall(const CDM::PatientData& in, SEPatient& out)
  {
    out.Clear();

    out.m_Name = in.Name();
    if (in.Annotation().present()) {
      out.SetAnnotation(in.Annotation().get());
    }

    Patient::UnMarshall(in.Sex(), out.m_Sex);

    io::Property::UnMarshall(in.Age(), out.GetAge());
    io::Property::UnMarshall(in.Weight(), out.GetWeight());
    io::Property::UnMarshall(in.Height(), out.GetHeight());

    if (in.BodyMassIndex().present()) {
    if (!in.Weight().present() && in.Height().present()) {
        out.CalculateWeightByBMI(in.BodyMassIndex().get().value());
    } else if (in.Weight().present() && !in.Height().present()) {
        out.CalculateHeightByBMI(in.BodyMassIndex().get().value());
    } else {
        std::stringstream ss;
        ss << "BodyMassIndex as an input must be provided with height OR weight (and not neither/both). BMI input value is not being used and default inputs will be used." << std::endl;
        out.Warning(ss);
    }
}


    io::Property::UnMarshall(in.AlveoliSurfaceArea(), out.GetAlveoliSurfaceArea());
    io::Property::UnMarshall(in.BasalMetabolicRate(), out.GetBasalMetabolicRate());

    if (in.BloodTypeRh().present()) {
      out.m_BloodRh = in.BloodTypeRh().get();
    }
    if (in.BloodTypeABO().present()) {
      io::Patient::UnMarshall(in.BloodTypeABO(), out.m_BloodType);
    }

    io::Property::UnMarshall(in.BloodVolumeBaseline(), out.GetBloodVolumeBaseline());
    io::Property::UnMarshall(in.BodyDensity(), out.GetBodyDensity());
    io::Property::UnMarshall(in.BodyFatFraction(), out.GetBodyFatFraction());
    io::Property::UnMarshall(in.DiastolicArterialPressureBaseline(), out.GetDiastolicArterialPressureBaseline());
    io::Property::UnMarshall(in.ExpiratoryReserveVolume(), out.GetExpiratoryReserveVolume());
    io::Property::UnMarshall(in.FunctionalResidualCapacity(), out.GetFunctionalResidualCapacity());
    io::Property::UnMarshall(in.HeartRateBaseline(), out.GetHeartRateBaseline());
    io::Property::UnMarshall(in.HeartRateMaximum(), out.GetHeartRateMaximum());
    io::Property::UnMarshall(in.HeartRateMinimum(), out.GetHeartRateMinimum());
    io::Property::UnMarshall(in.Hyperhidrosis(), out.GetHyperhidrosis());
    io::Property::UnMarshall(in.InspiratoryCapacity(), out.GetInspiratoryCapacity());
    io::Property::UnMarshall(in.InspiratoryReserveVolume(), out.GetInspiratoryReserveVolume());
    io::Property::UnMarshall(in.LeanBodyMass(), out.GetLeanBodyMass());
    io::Property::UnMarshall(in.MaxWorkRate(), out.GetMaxWorkRate());
    io::Property::UnMarshall(in.MuscleMass(), out.GetMuscleMass());
    io::Property::UnMarshall(in.MeanArterialPressureBaseline(), out.GetMeanArterialPressureBaseline());
    io::Property::UnMarshall(in.PainSusceptibility(), out.GetPainSusceptibility());
    io::Property::UnMarshall(in.ResidualVolume(), out.GetResidualVolume());
    io::Property::UnMarshall(in.RespirationRateBaseline(), out.GetRespirationRateBaseline());
    io::Property::UnMarshall(in.RespiratoryDriverAmplitudeBaseline(), out.GetRespiratoryDriverAmplitudeBaseline());
    io::Property::UnMarshall(in.RightLungRatio(), out.GetRightLungRatio());
    io::Property::UnMarshall(in.SkinSurfaceArea(), out.GetSkinSurfaceArea());
    io::Property::UnMarshall(in.SleepAmount(), out.GetSleepAmount());
    io::Property::UnMarshall(in.SystolicArterialPressureBaseline(), out.GetSystolicArterialPressureBaseline());
    io::Property::UnMarshall(in.TotalVentilationBaseline(), out.GetTotalVentilationBaseline());
    io::Property::UnMarshall(in.TidalVolumeBaseline(), out.GetTidalVolumeBaseline());
    io::Property::UnMarshall(in.TotalLungCapacity(), out.GetTotalLungCapacity());
    io::Property::UnMarshall(in.VitalCapacity(), out.GetVitalCapacity());

    SEScalarTime time;
    SEPatientEventType key = SEPatientEventType::Invalid;
    for (auto e : in.ActiveEvent()) {
      io::Property::UnMarshall(e.Duration(), time);
      Patient::UnMarshall(e.Event(), key);
      out.m_EventState[key] = true;
      out.m_EventDuration_s[key] = time.GetValue(TimeUnit::s);
    }
  }
  void Patient::Marshall(const SEPatient& in, CDM::PatientData& out)
  {
    if (in.HasName()) {
      out.Name(in.m_Name);
    }
    if (in.HasAnnotation()) {
      out.Annotation(in.m_Annotation);
    }
    SE_OPTIONAL_PATIENT_ENUM_MARSHALL_HELPER(in, out, Sex)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Age);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Weight)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Height)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AlveoliSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BasalMetabolicRate)
    if (in.HasBloodRh()) {
      out.BloodTypeRh(in.m_BloodRh);
    }

    Patient::Marshall(in.m_BloodType, out.BloodTypeABO());

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodVolumeBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BodyDensity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BodyFatFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DiastolicArterialPressureBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExpiratoryReserveVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FunctionalResidualCapacity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRateBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRateMaximum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRateMinimum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Hyperhidrosis)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryCapacity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryReserveVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeanBodyMass)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MaxWorkRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MuscleMass)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanArterialPressureBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PainSusceptibility)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ResidualVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationRateBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespiratoryDriverAmplitudeBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightLungRatio)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SleepAmount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SystolicArterialPressureBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalVentilationBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TidalVolumeBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalLungCapacity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VitalCapacity)

    SEScalarTime time;
    for (auto itr : in.m_EventState) {
      if (!itr.second) {
        continue;
      }

      auto it2 = in.m_EventDuration_s.find(itr.first);
      if (it2 == in.m_EventDuration_s.end()) // This should not happen...
      {
        time.SetValue(0, TimeUnit::s);
      } else {
        time.SetValue(it2->second, TimeUnit::s);
      }
      CDM::ActivePatientEventData* eData = new CDM::ActivePatientEventData();

      eData->Event(std::make_unique<std::remove_reference<decltype(eData->Event())>::type>());
      io::Patient::Marshall(itr.first, eData->Event());

      eData->Duration(std::make_unique<std::remove_reference<decltype(eData->Duration())>::type>());
      io::Property::Marshall(time, eData->Duration());
      ;
      out.ActiveEvent().push_back(std::unique_ptr<CDM::ActivePatientEventData>(eData));
    }
  }
  //----------------------------------------------------------------------------------
  // SEOpenClosed
  void Patient::UnMarshall(const CDM::enumSex& in, SESex& out)
  {
    try {
      switch (in) {
      case CDM::enumSex::Male:
        out = SESex::Male;
        break;
      case CDM::enumSex::Female:
        out = SESex::Female;
        break;
      default:
        out = SESex::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SESex::Invalid;
    }
  }
  void Patient::Marshall(const SESex& in, CDM::enumSex& out)
  {
    switch (in) {
    case SESex::Male:
      out = CDM::enumSex::Male;
      break;
    case SESex::Female:
      out = CDM::enumSex::Female;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
  // SEOnOff
  void Patient::UnMarshall(const CDM::enumBloodType& in, SEBloodType& out)
  {
    try {
      switch (in) {
      case CDM::enumBloodType::A:
        out = SEBloodType::A;
        break;
      case CDM::enumBloodType::B:
        out = SEBloodType::B;
        break;
      case CDM::enumBloodType::AB:
        out = SEBloodType::AB;
        break;
      case CDM::enumBloodType::O:
        out = SEBloodType::O;
        break;
      default:
        out = SEBloodType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEBloodType::Invalid;
    }
  }
  void Patient::Marshall(const SEBloodType& in, CDM::enumBloodType& out)
  {
    switch (in) {
    case SEBloodType::A:
      out = CDM::enumBloodType::A;
      break;
    case SEBloodType::B:
      out = CDM::enumBloodType::B;
      break;
    case SEBloodType::AB:
      out = CDM::enumBloodType::AB;
      break;
    case SEBloodType::O:
      out = CDM::enumBloodType::O;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
  //  SEErrorType
  void Patient::UnMarshall(const CDM::enumPatientEvent& in, SEPatientEventType& out)
  {
    try {
      switch (in) {
      case CDM::enumPatientEvent::AcuteLungInjury:
        out = SEPatientEventType::AcuteLungInjury;
        break;
      case CDM::enumPatientEvent::AcuteRespiratoryDistress:
        out = SEPatientEventType::AcuteRespiratoryDistress;
        break;
      case CDM::enumPatientEvent::Antidiuresis:
        out = SEPatientEventType::Antidiuresis;
        break;
      case CDM::enumPatientEvent::Asystole:
        out = SEPatientEventType::Asystole;
        break;
      case CDM::enumPatientEvent::Bradycardia:
        out = SEPatientEventType::Bradycardia;
        break;
      case CDM::enumPatientEvent::Bradypnea:
        out = SEPatientEventType::Bradypnea;
        break;
      case CDM::enumPatientEvent::BrainOxygenDeficit:
        out = SEPatientEventType::BrainOxygenDeficit;
        break;
      case CDM::enumPatientEvent::CardiacArrest:
        out = SEPatientEventType::CardiacArrest;
        break;
      case CDM::enumPatientEvent::CardiogenicShock:
        out = SEPatientEventType::CardiogenicShock;
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_LeftArm:
        out = SEPatientEventType::CompartmentSyndromeLeftArm;
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_RightArm:
        out = SEPatientEventType::CompartmentSyndromeRightArm;
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_LeftLeg:
        out = SEPatientEventType::CompartmentSyndromeLeftLeg;
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_RightLeg:
        out = SEPatientEventType::CompartmentSyndromeRightLeg;
        break;
      case CDM::enumPatientEvent::CompartmentSyndrome_Abdominal:
        out = SEPatientEventType::CompartmentSyndromeAbdominal;
        break;
      case CDM::enumPatientEvent::CriticalBrainOxygenDeficit:
        out = SEPatientEventType::CriticalBrainOxygenDeficit;
        break;
      case CDM::enumPatientEvent::Dehydration:
        out = SEPatientEventType::Dehydration;
        break;
      case CDM::enumPatientEvent::Diuresis:
        out = SEPatientEventType::Diuresis;
        break;
      case CDM::enumPatientEvent::MildDiarrhea:
        out = SEPatientEventType::MildDiarrhea;
        break;
      case CDM::enumPatientEvent::SevereDiarrhea:
        out = SEPatientEventType::SevereDiarrhea;
        break;
      case CDM::enumPatientEvent::Fasciculation:
        out = SEPatientEventType::Fasciculation;
        break;
      case CDM::enumPatientEvent::Fatigue:
        out = SEPatientEventType::Fatigue;
        break;
      case CDM::enumPatientEvent::FlaccidParalysis:
        out = SEPatientEventType::FlaccidParalysis;
        break;
      case CDM::enumPatientEvent::FunctionalIncontinence:
        out = SEPatientEventType::FunctionalIncontinence;
        break;
      case CDM::enumPatientEvent::MildHeadache:
        out = SEPatientEventType::MildHeadache;
        break;
      case CDM::enumPatientEvent::SevereHeadache:
        out = SEPatientEventType::SevereHeadache;
        break;
      case CDM::enumPatientEvent::HemolyticTransfusionReaction:
        out = SEPatientEventType::HemolyticTransfusionReaction;
        break;
      case CDM::enumPatientEvent::Hypercapnia:
        out = SEPatientEventType::Hypercapnia;
        break;
      case CDM::enumPatientEvent::Hyperglycemia:
        out = SEPatientEventType::Hyperglycemia;
        break;
      case CDM::enumPatientEvent::MildDiaphoresis:
        out = SEPatientEventType::MildDiaphoresis;
        break;
      case CDM::enumPatientEvent::ModerateDiaphoresis:
        out = SEPatientEventType::ModerateDiaphoresis;
        break;
      case CDM::enumPatientEvent::SevereDiaphoresis:
        out = SEPatientEventType::SevereDiaphoresis;
        break;
      case CDM::enumPatientEvent::MildHyperkalemia:
        out = SEPatientEventType::MildHyperkalemia;
        break;
      case CDM::enumPatientEvent::SevereHyperkalemia:
        out = SEPatientEventType::SevereHyperkalemia;
        break;
      case CDM::enumPatientEvent::MildHypernatremia:
        out = SEPatientEventType::MildHypernatremia;
        break;
      case CDM::enumPatientEvent::SevereHypernatremia:
        out = SEPatientEventType::SevereHypernatremia;
        break;
      case CDM::enumPatientEvent::Hyperthermia:
        out = SEPatientEventType::Hyperthermia;
        break;
      case CDM::enumPatientEvent::Hypoglycemia:
        out = SEPatientEventType::Hypoglycemia;
        break;
      case CDM::enumPatientEvent::HypoglycemicShock:
        out = SEPatientEventType::HypoglycemicShock;
        break;
      case CDM::enumPatientEvent::HypoglycemicComa:
        out = SEPatientEventType::HypoglycemicComa;
        break;
      case CDM::enumPatientEvent::MildHypothermia:
        out = SEPatientEventType::MildHypothermia;
        break;
      case CDM::enumPatientEvent::ModerateHypothermia:
        out = SEPatientEventType::ModerateHypothermia;
        break;
      case CDM::enumPatientEvent::SevereHypothermia:
        out = SEPatientEventType::SevereHypothermia;
        break;
      case CDM::enumPatientEvent::MildHypokalemia:
        out = SEPatientEventType::MildHypokalemia;
        break;
      case CDM::enumPatientEvent::SevereHypokalemia:
        out = SEPatientEventType::SevereHypokalemia;
        break;
      case CDM::enumPatientEvent::MildHyponatremia:
        out = SEPatientEventType::MildHyponatremia;
        break;
      case CDM::enumPatientEvent::SevereHyponatremia:
        out = SEPatientEventType::SevereHyponatremia;
        break;
      case CDM::enumPatientEvent::Hypoxia:
        out = SEPatientEventType::Hypoxia;
        break;
      case CDM::enumPatientEvent::HypovolemicShock:
        out = SEPatientEventType::HypovolemicShock;
        break;
      case CDM::enumPatientEvent::IntracranialHypertension:
        out = SEPatientEventType::IntracranialHypertension;
        break;
      case CDM::enumPatientEvent::IntracranialHypotension:
        out = SEPatientEventType::IntracranialHypotension;
        break;
      case CDM::enumPatientEvent::IrreversibleState:
        out = SEPatientEventType::IrreversibleState;
        break;
      case CDM::enumPatientEvent::Ketoacidosis:
        out = SEPatientEventType::Ketoacidosis;
        break;
      case CDM::enumPatientEvent::LacticAcidosis:
        out = SEPatientEventType::LacticAcidosis;
        break;
      case CDM::enumPatientEvent::LiverGlycogenDepleted:
        out = SEPatientEventType::LiverGlycogenDepleted;
        break;
      case CDM::enumPatientEvent::MaximumPulmonaryVentilationRate:
        out = SEPatientEventType::MaximumPulmonaryVentilationRate;
        break;
      case CDM::enumPatientEvent::MetabolicAcidosis:
        out = SEPatientEventType::MetabolicAcidosis;
        break;
      case CDM::enumPatientEvent::MetabolicAlkalosis:
        out = SEPatientEventType::MetabolicAlkalosis;
        break;
      case CDM::enumPatientEvent::MildWeakness:
        out = SEPatientEventType::MildWeakness;
        break;
      case CDM::enumPatientEvent::ModerateWeakness:
        out = SEPatientEventType::ModerateWeakness;
        break;
      case CDM::enumPatientEvent::MildSecretions:
        out = SEPatientEventType::MildSecretions;
        break;
      case CDM::enumPatientEvent::ModerateSecretions:
        out = SEPatientEventType::ModerateSecretions;
        break;
      case CDM::enumPatientEvent::MuscleCatabolism:
        out = SEPatientEventType::MuscleCatabolism;
        break;
      case CDM::enumPatientEvent::MuscleGlycogenDepleted:
        out = SEPatientEventType::MuscleGlycogenDepleted;
        break;
      case CDM::enumPatientEvent::MyocardiumOxygenDeficit:
        out = SEPatientEventType::MyocardiumOxygenDeficit;
        break;
      case CDM::enumPatientEvent::Natriuresis:
        out = SEPatientEventType::Natriuresis;
        break;
      case CDM::enumPatientEvent::Nausea:
        out = SEPatientEventType::Nausea;
        break;
      case CDM::enumPatientEvent::NutritionDepleted:
        out = SEPatientEventType::NutritionDepleted;
        break;
      case CDM::enumPatientEvent::PulselessRhythm:
        out = SEPatientEventType::PulselessRhythm;
        break;
      case CDM::enumPatientEvent::RenalHypoperfusion:
        out = SEPatientEventType::RenalHypoperfusion;
        break;
      case CDM::enumPatientEvent::RespiratoryAcidosis:
        out = SEPatientEventType::RespiratoryAcidosis;
        break;
      case CDM::enumPatientEvent::RespiratoryAlkalosis:
        out = SEPatientEventType::RespiratoryAlkalosis;
        break;
      case CDM::enumPatientEvent::SevereAcuteRespiratoryDistress:
        out = SEPatientEventType::SevereAcuteRespiratoryDistress;
        break;
      case CDM::enumPatientEvent::SevereSecretions:
        out = SEPatientEventType::SevereSecretions;
        break;
      case CDM::enumPatientEvent::Seizures:
        out = SEPatientEventType::Seizures;
        break;
      case CDM::enumPatientEvent::Shivering:
        out = SEPatientEventType::Shivering;
        break;
      case CDM::enumPatientEvent::StartOfCardiacCycle:
        out = SEPatientEventType::StartOfCardiacCycle;
        break;
      case CDM::enumPatientEvent::StartOfExhale:
        out = SEPatientEventType::StartOfExhale;
        break;
      case CDM::enumPatientEvent::StartOfInhale:
        out = SEPatientEventType::StartOfInhale;
        break;
      case CDM::enumPatientEvent::SevereSepsis:
        out = SEPatientEventType::SevereSepsis;
        break;
      case CDM::enumPatientEvent::Tachycardia:
        out = SEPatientEventType::Tachycardia;
        break;
      case CDM::enumPatientEvent::Tachypnea:
        out = SEPatientEventType::Tachypnea;
        break;
      case CDM::enumPatientEvent::Vomiting:
        out = SEPatientEventType::Vomiting;
        break;
      default:
        out = SEPatientEventType::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEPatientEventType::Invalid;
    }
  }
  void Patient::Marshall(const SEPatientEventType& in, CDM::enumPatientEvent& out)
  {
    switch (in) {
    case SEPatientEventType::AcuteLungInjury:
      out = CDM::enumPatientEvent::AcuteLungInjury;
      break;
    case SEPatientEventType::AcuteRespiratoryDistress:
      out = CDM::enumPatientEvent::AcuteRespiratoryDistress;
      break;
    case SEPatientEventType::Antidiuresis:
      out = CDM::enumPatientEvent::Antidiuresis;
      break;
    case SEPatientEventType::Asystole:
      out = CDM::enumPatientEvent::Asystole;
      break;
    case SEPatientEventType::Bradycardia:
      out = CDM::enumPatientEvent::Bradycardia;
      break;
    case SEPatientEventType::Bradypnea:
      out = CDM::enumPatientEvent::Bradypnea;
      break;
    case SEPatientEventType::BrainOxygenDeficit:
      out = CDM::enumPatientEvent::BrainOxygenDeficit;
      break;
    case SEPatientEventType::CardiacArrest:
      out = CDM::enumPatientEvent::CardiacArrest;
      break;
    case SEPatientEventType::CardiogenicShock:
      out = CDM::enumPatientEvent::CardiogenicShock;
      break;
    case SEPatientEventType::CompartmentSyndromeLeftArm:
      out = CDM::enumPatientEvent::CompartmentSyndrome_LeftArm;
      break;
    case SEPatientEventType::CompartmentSyndromeRightArm:
      out = CDM::enumPatientEvent::CompartmentSyndrome_RightArm;
      break;
    case SEPatientEventType::CompartmentSyndromeLeftLeg:
      out = CDM::enumPatientEvent::CompartmentSyndrome_LeftLeg;
      break;
    case SEPatientEventType::CompartmentSyndromeRightLeg:
      out = CDM::enumPatientEvent::CompartmentSyndrome_RightLeg;
      break;
    case SEPatientEventType::CompartmentSyndromeAbdominal:
      out = CDM::enumPatientEvent::CompartmentSyndrome_Abdominal;
      break;
    case SEPatientEventType::CriticalBrainOxygenDeficit:
      out = CDM::enumPatientEvent::CriticalBrainOxygenDeficit;
      break;
    case SEPatientEventType::Dehydration:
      out = CDM::enumPatientEvent::Dehydration;
      break;
    case SEPatientEventType::Diuresis:
      out = CDM::enumPatientEvent::Diuresis;
      break;
    case SEPatientEventType::MildDiarrhea:
      out = CDM::enumPatientEvent::MildDiarrhea;
      break;
    case SEPatientEventType::SevereDiarrhea:
      out = CDM::enumPatientEvent::SevereDiarrhea;
      break;
    case SEPatientEventType::Fasciculation:
      out = CDM::enumPatientEvent::Fasciculation;
      break;
    case SEPatientEventType::Fatigue:
      out = CDM::enumPatientEvent::Fatigue;
      break;
    case SEPatientEventType::FlaccidParalysis:
      out = CDM::enumPatientEvent::FlaccidParalysis;
      break;
    case SEPatientEventType::FunctionalIncontinence:
      out = CDM::enumPatientEvent::FunctionalIncontinence;
      break;
    case SEPatientEventType::MildHeadache:
      out = CDM::enumPatientEvent::MildHeadache;
      break;
    case SEPatientEventType::SevereHeadache:
      out = CDM::enumPatientEvent::SevereHeadache;
      break;
    case SEPatientEventType::HemolyticTransfusionReaction:
      out = CDM::enumPatientEvent::HemolyticTransfusionReaction;
      break;
    case SEPatientEventType::Hypercapnia:
      out = CDM::enumPatientEvent::Hypercapnia;
      break;
    case SEPatientEventType::Hyperglycemia:
      out = CDM::enumPatientEvent::Hyperglycemia;
      break;
    case SEPatientEventType::MildDiaphoresis:
      out = CDM::enumPatientEvent::MildDiaphoresis;
      break;
    case SEPatientEventType::ModerateDiaphoresis:
      out = CDM::enumPatientEvent::ModerateDiaphoresis;
      break;
    case SEPatientEventType::SevereDiaphoresis:
      out = CDM::enumPatientEvent::SevereDiaphoresis;
      break;
    case SEPatientEventType::MildHyperkalemia:
      out = CDM::enumPatientEvent::MildHyperkalemia;
      break;
    case SEPatientEventType::SevereHyperkalemia:
      out = CDM::enumPatientEvent::SevereHyperkalemia;
      break;
    case SEPatientEventType::MildHypernatremia:
      out = CDM::enumPatientEvent::MildHypernatremia;
      break;
    case SEPatientEventType::SevereHypernatremia:
      out = CDM::enumPatientEvent::SevereHypernatremia;
      break;
    case SEPatientEventType::Hyperthermia:
      out = CDM::enumPatientEvent::Hyperthermia;
      break;
    case SEPatientEventType::Hypoglycemia:
      out = CDM::enumPatientEvent::Hypoglycemia;
      break;
    case SEPatientEventType::HypoglycemicShock:
      out = CDM::enumPatientEvent::HypoglycemicShock;
      break;
    case SEPatientEventType::HypoglycemicComa:
      out = CDM::enumPatientEvent::HypoglycemicComa;
      break;
    case SEPatientEventType::MildHypothermia:
      out = CDM::enumPatientEvent::MildHypothermia;
      break;
    case SEPatientEventType::ModerateHypothermia:
      out = CDM::enumPatientEvent::ModerateHypothermia;
      break;
    case SEPatientEventType::SevereHypothermia:
      out = CDM::enumPatientEvent::SevereHypothermia;
      break;
    case SEPatientEventType::MildHypokalemia:
      out = CDM::enumPatientEvent::MildHypokalemia;
      break;
    case SEPatientEventType::SevereHypokalemia:
      out = CDM::enumPatientEvent::SevereHypokalemia;
      break;
    case SEPatientEventType::MildHyponatremia:
      out = CDM::enumPatientEvent::MildHyponatremia;
      break;
    case SEPatientEventType::SevereHyponatremia:
      out = CDM::enumPatientEvent::SevereHyponatremia;
      break;
    case SEPatientEventType::Hypoxia:
      out = CDM::enumPatientEvent::Hypoxia;
      break;
    case SEPatientEventType::HypovolemicShock:
      out = CDM::enumPatientEvent::HypovolemicShock;
      break;
    case SEPatientEventType::IntracranialHypertension:
      out = CDM::enumPatientEvent::IntracranialHypertension;
      break;
    case SEPatientEventType::IntracranialHypotension:
      out = CDM::enumPatientEvent::IntracranialHypotension;
      break;
    case SEPatientEventType::IrreversibleState:
      out = CDM::enumPatientEvent::IrreversibleState;
      break;
    case SEPatientEventType::Ketoacidosis:
      out = CDM::enumPatientEvent::Ketoacidosis;
      break;
    case SEPatientEventType::LacticAcidosis:
      out = CDM::enumPatientEvent::LacticAcidosis;
      break;
    case SEPatientEventType::LiverGlycogenDepleted:
      out = CDM::enumPatientEvent::LiverGlycogenDepleted;
      break;
    case SEPatientEventType::MaximumPulmonaryVentilationRate:
      out = CDM::enumPatientEvent::MaximumPulmonaryVentilationRate;
      break;
    case SEPatientEventType::MetabolicAcidosis:
      out = CDM::enumPatientEvent::MetabolicAcidosis;
      break;
    case SEPatientEventType::MetabolicAlkalosis:
      out = CDM::enumPatientEvent::MetabolicAlkalosis;
      break;
    case SEPatientEventType::MildWeakness:
      out = CDM::enumPatientEvent::MildWeakness;
      break;
    case SEPatientEventType::ModerateWeakness:
      out = CDM::enumPatientEvent::ModerateWeakness;
      break;
    case SEPatientEventType::MildSecretions:
      out = CDM::enumPatientEvent::MildSecretions;
      break;
    case SEPatientEventType::ModerateSecretions:
      out = CDM::enumPatientEvent::ModerateSecretions;
      break;
    case SEPatientEventType::MuscleCatabolism:
      out = CDM::enumPatientEvent::MuscleCatabolism;
      break;
    case SEPatientEventType::MuscleGlycogenDepleted:
      out = CDM::enumPatientEvent::MuscleGlycogenDepleted;
      break;
    case SEPatientEventType::MyocardiumOxygenDeficit:
      out = CDM::enumPatientEvent::MyocardiumOxygenDeficit;
      break;
    case SEPatientEventType::Natriuresis:
      out = CDM::enumPatientEvent::Natriuresis;
      break;
    case SEPatientEventType::Nausea:
      out = CDM::enumPatientEvent::Nausea;
      break;
    case SEPatientEventType::NutritionDepleted:
      out = CDM::enumPatientEvent::NutritionDepleted;
      break;
    case SEPatientEventType::PulselessRhythm:
      out = CDM::enumPatientEvent::PulselessRhythm;
      break;
    case SEPatientEventType::RenalHypoperfusion:
      out = CDM::enumPatientEvent::RenalHypoperfusion;
      break;
    case SEPatientEventType::RespiratoryAcidosis:
      out = CDM::enumPatientEvent::RespiratoryAcidosis;
      break;
    case SEPatientEventType::RespiratoryAlkalosis:
      out = CDM::enumPatientEvent::RespiratoryAlkalosis;
      break;
    case SEPatientEventType::SevereAcuteRespiratoryDistress:
      out = CDM::enumPatientEvent::SevereAcuteRespiratoryDistress;
      break;
    case SEPatientEventType::SevereSecretions:
      out = CDM::enumPatientEvent::SevereSecretions;
      break;
    case SEPatientEventType::Seizures:
      out = CDM::enumPatientEvent::Seizures;
      break;
    case SEPatientEventType::Shivering:
      out = CDM::enumPatientEvent::Shivering;
      break;
    case SEPatientEventType::StartOfCardiacCycle:
      out = CDM::enumPatientEvent::StartOfCardiacCycle;
      break;
    case SEPatientEventType::StartOfExhale:
      out = CDM::enumPatientEvent::StartOfExhale;
      break;
    case SEPatientEventType::StartOfInhale:
      out = CDM::enumPatientEvent::StartOfInhale;
      break;
    case SEPatientEventType::SevereSepsis:
      out = CDM::enumPatientEvent::SevereSepsis;
      break;
    case SEPatientEventType::Tachycardia:
      out = CDM::enumPatientEvent::Tachycardia;
      break;
    case SEPatientEventType::Tachypnea:
      out = CDM::enumPatientEvent::Tachypnea;
      break;
    case SEPatientEventType::Vomiting:
      out = CDM::enumPatientEvent::Vomiting;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
}

bool operator==(CDM::enumSex const& lhs, SESex const& rhs)
{
  switch (rhs) {
  case SESex::Male:
    return (CDM::enumSex::Male == lhs);
  case SESex::Female:
    return (CDM::enumSex::Female == lhs);
  case SESex::Invalid:
    return ((CDM::enumSex::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumBloodType const& lhs, SEBloodType const& rhs)
{
  switch (rhs) {
  case SEBloodType::A:
    return (CDM::enumBloodType::A == lhs);
  case SEBloodType::AB:
    return (CDM::enumBloodType::AB == lhs);
  case SEBloodType::B:
    return (CDM::enumBloodType::B == lhs);
  case SEBloodType::O:
    return (CDM::enumBloodType::O == lhs);
  case SEBloodType::Invalid:
    return ((CDM::enumBloodType::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumPatientEvent const& lhs, SEPatientEventType const& rhs)
{
  switch (rhs) {
  case SEPatientEventType::AcuteLungInjury:
    return (CDM::enumPatientEvent::AcuteLungInjury == lhs);
  case SEPatientEventType::AcuteRespiratoryDistress:
    return (CDM::enumPatientEvent::AcuteRespiratoryDistress == lhs);
  case SEPatientEventType::Antidiuresis:
    return (CDM::enumPatientEvent::Antidiuresis == lhs);
  case SEPatientEventType::Asystole:
    return (CDM::enumPatientEvent::Asystole == lhs);
  case SEPatientEventType::Bradycardia:
    return (CDM::enumPatientEvent::Bradycardia == lhs);
  case SEPatientEventType::Bradypnea:
    return (CDM::enumPatientEvent::Bradypnea == lhs);
  case SEPatientEventType::BrainOxygenDeficit:
    return (CDM::enumPatientEvent::BrainOxygenDeficit == lhs);
  case SEPatientEventType::CardiacArrest:
    return (CDM::enumPatientEvent::CardiacArrest == lhs);
  case SEPatientEventType::CardiogenicShock:
    return (CDM::enumPatientEvent::CardiogenicShock == lhs);
  case SEPatientEventType::CompartmentSyndromeLeftArm:
    return (CDM::enumPatientEvent::CompartmentSyndrome_LeftArm == lhs);
  case SEPatientEventType::CompartmentSyndromeRightArm:
    return (CDM::enumPatientEvent::CompartmentSyndrome_RightArm == lhs);
  case SEPatientEventType::CompartmentSyndromeLeftLeg:
    return (CDM::enumPatientEvent::CompartmentSyndrome_LeftLeg == lhs);
  case SEPatientEventType::CompartmentSyndromeRightLeg:
    return (CDM::enumPatientEvent::CompartmentSyndrome_RightLeg == lhs);
  case SEPatientEventType::CompartmentSyndromeAbdominal:
    return (CDM::enumPatientEvent::CompartmentSyndrome_Abdominal == lhs);
  case SEPatientEventType::CriticalBrainOxygenDeficit:
    return (CDM::enumPatientEvent::CriticalBrainOxygenDeficit == lhs);
  case SEPatientEventType::Dehydration:
    return (CDM::enumPatientEvent::Dehydration == lhs);
  case SEPatientEventType::Diuresis:
    return (CDM::enumPatientEvent::Diuresis == lhs);
  case SEPatientEventType::MildDiarrhea:
    return (CDM::enumPatientEvent::MildDiarrhea == lhs);
  case SEPatientEventType::SevereDiarrhea:
    return (CDM::enumPatientEvent::SevereDiarrhea == lhs);
  case SEPatientEventType::Fasciculation:
    return (CDM::enumPatientEvent::Fasciculation == lhs);
  case SEPatientEventType::Fatigue:
    return (CDM::enumPatientEvent::Fatigue == lhs);
  case SEPatientEventType::FlaccidParalysis:
    return (CDM::enumPatientEvent::FlaccidParalysis == lhs);
  case SEPatientEventType::FunctionalIncontinence:
    return (CDM::enumPatientEvent::FunctionalIncontinence == lhs);
  case SEPatientEventType::MildHeadache:
    return (CDM::enumPatientEvent::MildHeadache == lhs);
  case SEPatientEventType::SevereHeadache:
    return (CDM::enumPatientEvent::SevereHeadache == lhs);
  case SEPatientEventType::HemolyticTransfusionReaction:
    return (CDM::enumPatientEvent::HemolyticTransfusionReaction == lhs);
  case SEPatientEventType::Hypercapnia:
    return (CDM::enumPatientEvent::Hypercapnia == lhs);
  case SEPatientEventType::Hyperglycemia:
    return (CDM::enumPatientEvent::Hyperglycemia == lhs);
  case SEPatientEventType::MildDiaphoresis:
    return (CDM::enumPatientEvent::MildDiaphoresis == lhs);
  case SEPatientEventType::ModerateDiaphoresis:
    return (CDM::enumPatientEvent::ModerateDiaphoresis == lhs);
  case SEPatientEventType::SevereDiaphoresis:
    return (CDM::enumPatientEvent::SevereDiaphoresis == lhs);
  case SEPatientEventType::MildHyperkalemia:
    return (CDM::enumPatientEvent::MildHyperkalemia == lhs);
  case SEPatientEventType::SevereHyperkalemia:
    return (CDM::enumPatientEvent::SevereHyperkalemia == lhs);
  case SEPatientEventType::MildHypernatremia:
    return (CDM::enumPatientEvent::MildHypernatremia == lhs);
  case SEPatientEventType::SevereHypernatremia:
    return (CDM::enumPatientEvent::SevereHypernatremia == lhs);
  case SEPatientEventType::Hyperthermia:
    return (CDM::enumPatientEvent::Hyperthermia == lhs);
  case SEPatientEventType::Hypoglycemia:
    return (CDM::enumPatientEvent::Hypoglycemia == lhs);
  case SEPatientEventType::HypoglycemicShock:
    return (CDM::enumPatientEvent::HypoglycemicShock == lhs);
  case SEPatientEventType::HypoglycemicComa:
    return (CDM::enumPatientEvent::HypoglycemicComa == lhs);
  case SEPatientEventType::MildHypothermia:
    return (CDM::enumPatientEvent::MildHypothermia == lhs);
  case SEPatientEventType::ModerateHypothermia:
    return (CDM::enumPatientEvent::ModerateHypothermia == lhs);
  case SEPatientEventType::SevereHypothermia:
    return (CDM::enumPatientEvent::SevereHypothermia == lhs);
  case SEPatientEventType::MildHypokalemia:
    return (CDM::enumPatientEvent::MildHypokalemia == lhs);
  case SEPatientEventType::SevereHypokalemia:
    return (CDM::enumPatientEvent::SevereHypokalemia == lhs);
  case SEPatientEventType::MildHyponatremia:
    return (CDM::enumPatientEvent::MildHyponatremia == lhs);
  case SEPatientEventType::SevereHyponatremia:
    return (CDM::enumPatientEvent::SevereHyponatremia == lhs);
  case SEPatientEventType::Hypoxia:
    return (CDM::enumPatientEvent::Hypoxia == lhs);
  case SEPatientEventType::HypovolemicShock:
    return (CDM::enumPatientEvent::HypovolemicShock == lhs);
  case SEPatientEventType::IntracranialHypertension:
    return (CDM::enumPatientEvent::IntracranialHypertension == lhs);
  case SEPatientEventType::IntracranialHypotension:
    return (CDM::enumPatientEvent::IntracranialHypotension == lhs);
  case SEPatientEventType::IrreversibleState:
    return (CDM::enumPatientEvent::IrreversibleState == lhs);
  case SEPatientEventType::Ketoacidosis:
    return (CDM::enumPatientEvent::Ketoacidosis == lhs);
  case SEPatientEventType::LacticAcidosis:
    return (CDM::enumPatientEvent::LacticAcidosis == lhs);
  case SEPatientEventType::LiverGlycogenDepleted:
    return (CDM::enumPatientEvent::LiverGlycogenDepleted == lhs);
  case SEPatientEventType::MaximumPulmonaryVentilationRate:
    return (CDM::enumPatientEvent::MaximumPulmonaryVentilationRate == lhs);
  case SEPatientEventType::MetabolicAcidosis:
    return (CDM::enumPatientEvent::MetabolicAcidosis == lhs);
  case SEPatientEventType::MetabolicAlkalosis:
    return (CDM::enumPatientEvent::MetabolicAlkalosis == lhs);
  case SEPatientEventType::MildWeakness:
    return (CDM::enumPatientEvent::MildWeakness == lhs);
  case SEPatientEventType::ModerateWeakness:
    return (CDM::enumPatientEvent::ModerateWeakness == lhs);
  case SEPatientEventType::MildSecretions:
    return (CDM::enumPatientEvent::MildSecretions == lhs);
  case SEPatientEventType::ModerateSecretions:
    return (CDM::enumPatientEvent::ModerateSecretions == lhs);
  case SEPatientEventType::MuscleCatabolism:
    return (CDM::enumPatientEvent::MuscleCatabolism == lhs);
  case SEPatientEventType::MuscleGlycogenDepleted:
    return (CDM::enumPatientEvent::MuscleGlycogenDepleted == lhs);
  case SEPatientEventType::MyocardiumOxygenDeficit:
    return (CDM::enumPatientEvent::MyocardiumOxygenDeficit == lhs);
  case SEPatientEventType::Natriuresis:
    return (CDM::enumPatientEvent::Natriuresis == lhs);
  case SEPatientEventType::Nausea:
    return (CDM::enumPatientEvent::Nausea == lhs);
  case SEPatientEventType::NutritionDepleted:
    return (CDM::enumPatientEvent::NutritionDepleted == lhs);
  case SEPatientEventType::PulselessRhythm:
    return (CDM::enumPatientEvent::PulselessRhythm == lhs);
  case SEPatientEventType::RenalHypoperfusion:
    return (CDM::enumPatientEvent::RenalHypoperfusion == lhs);
  case SEPatientEventType::RespiratoryAcidosis:
    return (CDM::enumPatientEvent::RespiratoryAcidosis == lhs);
  case SEPatientEventType::RespiratoryAlkalosis:
    return (CDM::enumPatientEvent::RespiratoryAlkalosis == lhs);
  case SEPatientEventType::SevereAcuteRespiratoryDistress:
    return (CDM::enumPatientEvent::SevereAcuteRespiratoryDistress == lhs);
  case SEPatientEventType::SevereSecretions:
    return (CDM::enumPatientEvent::SevereSecretions == lhs);
  case SEPatientEventType::Seizures:
    return (CDM::enumPatientEvent::Seizures == lhs);
  case SEPatientEventType::Shivering:
    return (CDM::enumPatientEvent::Shivering == lhs);
  case SEPatientEventType::StartOfCardiacCycle:
    return (CDM::enumPatientEvent::StartOfCardiacCycle == lhs);
  case SEPatientEventType::StartOfExhale:
    return (CDM::enumPatientEvent::StartOfExhale == lhs);
  case SEPatientEventType::StartOfInhale:
    return (CDM::enumPatientEvent::StartOfInhale == lhs);
  case SEPatientEventType::SevereSepsis:
    return (CDM::enumPatientEvent::SevereSepsis == lhs);
  case SEPatientEventType::Tachycardia:
    return (CDM::enumPatientEvent::Tachycardia == lhs);
  case SEPatientEventType::Tachypnea:
    return (CDM::enumPatientEvent::Tachypnea == lhs);
  case SEPatientEventType::Vomiting:
    return (CDM::enumPatientEvent::Vomiting == lhs);
  case SEPatientEventType::Invalid:
    return ((CDM::enumPatientEvent::value)-1 == lhs);
  default:
    return false;
  }
}
}
