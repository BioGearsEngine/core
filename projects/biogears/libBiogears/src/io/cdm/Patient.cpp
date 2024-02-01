#include "Patient.h"

#include "Property.h"

#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/patient/SEPatient.h>

namespace biogears {
namespace io {
  //----------------------------------------------------------------------------------
  //class SEPatient
  void Patient::UnMarshall(const CDM::PatientData& in, SEPatient& out)
  {
    out.Clear();

    out.m_Name = in.Name();
    if (in.Annotation().present()) {
      out.SetAnnotation(in.Annotation().get());
    }

    if (in.Sex().present()) {
      out.m_Gender = in.Sex().get();
    }
    io::Property::UnMarshall(in.Age(), out.GetAge());
    io::Property::UnMarshall(in.Weight(), out.GetWeight());
    io::Property::UnMarshall(in.Height(), out.GetHeight());
    io::Property::UnMarshall(in.AlveoliSurfaceArea(), out.GetAlveoliSurfaceArea());
    io::Property::UnMarshall(in.BasalMetabolicRate(), out.GetBasalMetabolicRate());
    if (in.BloodTypeRh().present()) {
      out.m_BloodRh = in.BloodTypeRh().get();
    }
    if (in.BloodTypeABO().present()) {
      out.m_BloodType = in.BloodTypeABO().get();
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
    for (auto e : in.ActiveEvent()) {
      io::Property::UnMarshall(e.Duration(), time);
      out.m_EventState[e.Event()] = true;
      out.m_EventDuration_s[e.Event()] = time.GetValue(TimeUnit::s);
    }
  }
  //----------------------------------------------------------------------------------
  void Patient::Marshall(const SEPatient& in, CDM::PatientData& out)
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

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Age);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Weight)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Height)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AlveoliSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BasalMetabolicRate)
    if (in.HasBloodRh()) {
      out.BloodTypeRh(in.m_BloodRh);
    }
    if (in.HasBloodType()) {
      out.BloodTypeABO(in.m_BloodType);
    }
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
      eData->Event(itr.first);

      io::Property::Marshall(time, eData->Duration());
      ;
      out.ActiveEvent().push_back(std::unique_ptr<CDM::ActivePatientEventData>(eData));
    }
  }
  //----------------------------------------------------------------------------------
}
}