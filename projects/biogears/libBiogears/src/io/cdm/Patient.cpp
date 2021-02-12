#include "Patient.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

#include <biogears/cdm/patient/SEPatient.h>

namespace biogears {
namespace io {
  //----------------------------------------------------------------------------------
  //class SEPatient
  void Patient::Marshall(const CDM::PatientData& in, SEPatient& out)
  {
    out.Clear();

    out.m_Name = in.Name();
    out.m_Gender = in.Sex().get();

    io::Property::Marshall(in.Age(), out.GetAge());
    io::Property::Marshall(in.Weight(), out.GetWeight());
    io::Property::Marshall(in.Height(), out.GetHeight());
    io::Property::Marshall(in.AlveoliSurfaceArea(), out.GetAlveoliSurfaceArea());
    io::Property::Marshall(in.BasalMetabolicRate(), out.GetBasalMetabolicRate());
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
    io::Property::Marshall(in.RightLungRatio(), out.GetRightLungRatio());
    io::Property::Marshall(in.SkinSurfaceArea(), out.GetSkinSurfaceArea());
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
    if (in.HasGender()) {
      out.Sex(in.m_Gender);
    }
    if (in.m_Age != nullptr) {
      io::Property::UnMarshall(*in.m_Age, out.Age());
    }
    if (in.m_Weight != nullptr) {
      io::Property::UnMarshall(*in.m_Weight, out.Weight());
    }
    if (in.m_Height != nullptr) {
      io::Property::UnMarshall(*in.m_Height, out.Height());
    }
    if (in.m_AlveoliSurfaceArea != nullptr) {
      io::Property::UnMarshall(*in.m_AlveoliSurfaceArea, out.AlveoliSurfaceArea());
    }
    if (in.m_BasalMetabolicRate != nullptr) {
      io::Property::UnMarshall(*in.m_BasalMetabolicRate, out.BasalMetabolicRate());
    }
    if (in.m_BloodVolumeBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_BloodVolumeBaseline, out.BloodVolumeBaseline());
    }
    if (in.m_BodyDensity != nullptr) {
      io::Property::UnMarshall(*in.m_BodyDensity, out.BodyDensity());
    }
    if (in.m_BodyFatFraction != nullptr) {
      io::Property::UnMarshall(*in.m_BodyFatFraction, out.BodyFatFraction());
    }
    if (in.m_DiastolicArterialPressureBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_DiastolicArterialPressureBaseline, out.DiastolicArterialPressureBaseline());
    }
    if (in.m_ExpiratoryReserveVolume != nullptr) {
      io::Property::UnMarshall(*in.m_ExpiratoryReserveVolume, out.ExpiratoryReserveVolume());
    }
    if (in.m_FunctionalResidualCapacity != nullptr) {
      io::Property::UnMarshall(*in.m_FunctionalResidualCapacity, out.FunctionalResidualCapacity());
    }
    if (in.m_HeartRateBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_HeartRateBaseline, out.HeartRateBaseline());
    }
    if (in.m_HeartRateMaximum != nullptr) {
      io::Property::UnMarshall(*in.m_HeartRateMaximum, out.HeartRateMaximum());
    }
    if (in.m_HeartRateMinimum != nullptr) {
      io::Property::UnMarshall(*in.m_HeartRateMinimum, out.HeartRateMinimum());
    }
    if (in.m_Hyperhidrosis != nullptr) {
      io::Property::UnMarshall(*in.m_Hyperhidrosis, out.Hyperhidrosis());
    }
    if (in.m_InspiratoryCapacity != nullptr) {
      io::Property::UnMarshall(*in.m_InspiratoryCapacity, out.InspiratoryCapacity());
    }
    if (in.m_InspiratoryReserveVolume != nullptr) {
      io::Property::UnMarshall(*in.m_InspiratoryReserveVolume, out.InspiratoryReserveVolume());
    }
    if (in.m_LeanBodyMass != nullptr) {
      io::Property::UnMarshall(*in.m_LeanBodyMass, out.LeanBodyMass());
    }
    if (in.m_MaxWorkRate != nullptr) {
      io::Property::UnMarshall(*in.m_MaxWorkRate, out.MaxWorkRate());
    }
    if (in.m_MuscleMass != nullptr) {
      io::Property::UnMarshall(*in.m_MuscleMass, out.MuscleMass());
    }
    if (in.m_MeanArterialPressureBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_MeanArterialPressureBaseline, out.MeanArterialPressureBaseline());
    }
    if (in.m_PainSusceptibility != nullptr) {
      io::Property::UnMarshall(*in.m_PainSusceptibility, out.PainSusceptibility());
    }
    if (in.m_ResidualVolume != nullptr) {
      io::Property::UnMarshall(*in.m_ResidualVolume, out.ResidualVolume());
    }
    if (in.m_RespirationRateBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_RespirationRateBaseline, out.RespirationRateBaseline());
    }
    if (in.m_RightLungRatio != nullptr) {
      io::Property::UnMarshall(*in.m_RightLungRatio, out.RightLungRatio());
    }
    if (in.m_SkinSurfaceArea != nullptr) {
      io::Property::UnMarshall(*in.m_SkinSurfaceArea, out.SkinSurfaceArea());
    }
    if (in.m_SystolicArterialPressureBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_SystolicArterialPressureBaseline, out.SystolicArterialPressureBaseline());
    }
    if (in.m_TotalVentilationBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_TotalVentilationBaseline, out.TotalVentilationBaseline());
    }
    if (in.m_TidalVolumeBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_TidalVolumeBaseline, out.TidalVolumeBaseline());
    }
    if (in.m_TotalLungCapacity != nullptr) {
      io::Property::UnMarshall(*in.m_TotalLungCapacity, out.TotalLungCapacity());
    }
    if (in.m_VitalCapacity != nullptr) {
      io::Property::UnMarshall(*in.m_VitalCapacity, out.VitalCapacity());
    }

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