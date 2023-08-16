#include "Patient.h"

#include "Property.h"

#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/patient/SEPatient.h>

namespace biogears {
namespace io {
  //----------------------------------------------------------------------------------
  // class SEPatient
  void Patient::Marshall(const CDM::PatientData& in, SEPatient& out)
  {
    out.Clear();

    out.m_Name = in.Name();
    if (in.Annotation().present()) {
      out.SetAnnotation(in.Annotation().get());
    }

    if (in.Sex().present()) {
      out.m_Gender = in.Sex().get();
    }
    io::Property::Marshall(in.Age(), out.GetAge());
    io::Property::Marshall(in.Weight(), out.GetWeight());
    io::Property::Marshall(in.Height(), out.GetHeight());
    io::Property::Marshall(in.AlveoliSurfaceArea(), out.GetAlveoliSurfaceArea());
    io::Property::Marshall(in.BasalMetabolicRate(), out.GetBasalMetabolicRate());
    if (in.BloodTypeRh().present()) {
      out.m_BloodRh = in.BloodTypeRh().get();
    }
    if (in.BloodTypeABO().present()) {
      out.m_BloodType = in.BloodTypeABO().get();
    }
    io::Property::Marshall(in.BloodVolumeBaseline(), out.GetBloodVolumeBaseline());
    io::Property::Marshall(in.BodyDensity(), out.GetBodyDensity());
    io::Property::Marshall(in.BodyFatFraction(), out.GetBodyFatFraction());
    io::Property::Marshall(in.DiastolicArterialPressureBaseline(), out.GetDiastolicArterialPressureBaseline());
    io::Property::Marshall(in.ExpiratoryReserveVolume(), out.GetExpiratoryReserveVolume());
    io::Property::Marshall(in.FunctionalResidualCapacity(), out.GetFunctionalResidualCapacity());
    io::Property::Marshall(in.HeartRateBaseline(), out.GetHeartRateBaseline());
    io::Property::Marshall(in.HeartRateMaximum(), out.GetHeartRateMaximum());
    io::Property::Marshall(in.HeartRateMinimum(), out.GetHeartRateMinimum());
    io::Property::Marshall(in.Hyperhidrosis(), out.GetHyperhidrosis());
    io::Property::Marshall(in.InspiratoryCapacity(), out.GetInspiratoryCapacity());
    io::Property::Marshall(in.InspiratoryReserveVolume(), out.GetInspiratoryReserveVolume());
    io::Property::Marshall(in.LeanBodyMass(), out.GetLeanBodyMass());
    io::Property::Marshall(in.MaxWorkRate(), out.GetMaxWorkRate());
    io::Property::Marshall(in.MuscleMass(), out.GetMuscleMass());
    io::Property::Marshall(in.MeanArterialPressureBaseline(), out.GetMeanArterialPressureBaseline());
    io::Property::Marshall(in.PainSusceptibility(), out.GetPainSusceptibility());
    io::Property::Marshall(in.ResidualVolume(), out.GetResidualVolume());
    io::Property::Marshall(in.RespirationRateBaseline(), out.GetRespirationRateBaseline());
    io::Property::Marshall(in.RespiratoryDriverAmplitudeBaseline(), out.GetRespiratoryDriverAmplitudeBaseline());
    io::Property::Marshall(in.RightLungRatio(), out.GetRightLungRatio());
    io::Property::Marshall(in.SkinSurfaceArea(), out.GetSkinSurfaceArea());
    io::Property::Marshall(in.SleepAmount(), out.GetSleepAmount());
    io::Property::Marshall(in.SystolicArterialPressureBaseline(), out.GetSystolicArterialPressureBaseline());
    io::Property::Marshall(in.TotalVentilationBaseline(), out.GetTotalVentilationBaseline());
    io::Property::Marshall(in.TidalVolumeBaseline(), out.GetTidalVolumeBaseline());
    io::Property::Marshall(in.TotalLungCapacity(), out.GetTotalLungCapacity());
    io::Property::Marshall(in.VitalCapacity(), out.GetVitalCapacity());

    SEScalarTime time;
    for (auto e : in.ActiveEvent()) {
      io::Property::Marshall(e.Duration(), time);
      out.m_EventState[e.Event()] = true;
      out.m_EventDuration_s[e.Event()] = time.GetValue(TimeUnit::s);
    }
  }
  //----------------------------------------------------------------------------------
  void Patient::UnMarshall(const SEPatient& in, CDM::PatientData& out)
  {
    if (in.HasName()) {
      out.Name(in.m_Name);
    }
    if (in.HasAnnotation()) {
      out.Annotation(in.m_Annotation);
    }
    if (in.HasGender()) {
      out.Sex(in.m_Gender);
    }

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Age);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Weight)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Height)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AlveoliSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BasalMetabolicRate)
    if (in.HasBloodRh()) {
      out.BloodTypeRh(in.m_BloodRh);
    }
    if (in.HasBloodType()) {
      out.BloodTypeABO(in.m_BloodType);
    }
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BloodVolumeBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BodyDensity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BodyFatFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DiastolicArterialPressureBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExpiratoryReserveVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FunctionalResidualCapacity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartRateBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartRateMaximum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartRateMinimum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Hyperhidrosis)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, InspiratoryCapacity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, InspiratoryReserveVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeanBodyMass)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MaxWorkRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MuscleMass)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanArterialPressureBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PainSusceptibility)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ResidualVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationRateBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespiratoryDriverAmplitudeBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightLungRatio)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SleepAmount)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SystolicArterialPressureBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalVentilationBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TidalVolumeBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalLungCapacity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VitalCapacity)

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
      eData->Event(itr.first);

      io::Property::UnMarshall(time, eData->Duration());
      ;
      out.ActiveEvent().push_back(std::unique_ptr<CDM::ActivePatientEventData>(eData));
    }
  }
  //----------------------------------------------------------------------------------
}
}