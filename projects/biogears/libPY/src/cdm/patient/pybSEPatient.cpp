#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEPatient(py::module_ &m)
{

  py::class_<biogears::SEPatient, biogears::Loggable>(m, "SEPatient")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SEPatient::Clear)
    .def("Load", py::overload_cast<const CDM::PatientData&>(&biogears::SEPatient::Load))
    .def("Load", py::overload_cast<const char*>(&biogears::SEPatient::Load))
    .def("Load", py::overload_cast<const std::string&>(&biogears::SEPatient::Load))
    // .def("Load", py::overload_cast<biogears::NutritionData&>(&biogears::SEPatient::Load))

    .def("Unload", py::overload_cast<>(&biogears::SEPatient::Unload, py::const_))
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SEPatient::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SEPatient::GetScalar))

    .def("GetEventStates", &biogears::SEPatient::GetEventStates)
    .def("SetEvent", &biogears::SEPatient::SetEvent)
    .def("IsEventActive", &biogears::SEPatient::IsEventActive)
    .def("GetEventDuration", &biogears::SEPatient::GetEventDuration)
    .def("UpdateEvents", &biogears::SEPatient::UpdateEvents)
    .def("ForwardEvents", &biogears::SEPatient::ForwardEvents)
    .def("GetName", &biogears::SEPatient::GetName)
    .def("GetName_cStr", &biogears::SEPatient::GetName_cStr)
    .def("SetName", py::overload_cast<const char*>(&biogears::SEPatient::SetName))
    .def("SetName", py::overload_cast<const std::string&>(&biogears::SEPatient::SetName))
    .def("HasName", &biogears::SEPatient::HasName)
    .def("InvalidateName", &biogears::SEPatient::InvalidateName)
    .def("GetAnnotation", &biogears::SEPatient::GetAnnotation)
    .def("GetAnnotation_cStr", &biogears::SEPatient::GetAnnotation_cStr)
    .def("SetAnnotation", py::overload_cast<const char*>(&biogears::SEPatient::SetAnnotation))
    .def("SetAnnotation", py::overload_cast<const std::string&>(&biogears::SEPatient::SetAnnotation))
    .def("HasAnnotation", &biogears::SEPatient::HasName)
    .def("InvalidateAnnotation", &biogears::SEPatient::InvalidateAnnotation)

    .def("GetGender", &biogears::SEPatient::GetGender)
    .def("SetGender", &biogears::SEPatient::SetGender)
    .def("HasGender", &biogears::SEPatient::HasGender)
    .def("InvalidateGender", &biogears::SEPatient::InvalidateGender)

    .def("GetSex", &biogears::SEPatient::GetSex)
    .def("SetSex", &biogears::SEPatient::SetSex)
    .def("HasSex", &biogears::SEPatient::HasSex)
    .def("InvalidateSex", &biogears::SEPatient::InvalidateSex)

    .def("HasAge", &biogears::SEPatient::HasAge)
    .def("GetAge", py::overload_cast<>(&biogears::SEPatient::GetAge))
    .def("GetAge", py::overload_cast<const biogears::TimeUnit&>(&biogears::SEPatient::GetAge, py::const_))

    .def("HasWeight", &biogears::SEPatient::HasWeight)
    .def("GetWeight", py::overload_cast<>(&biogears::SEPatient::GetWeight))
    .def("GetWeight", py::overload_cast<const biogears::MassUnit&>(&biogears::SEPatient::GetWeight, py::const_))

    .def("HasHeight", &biogears::SEPatient::HasHeight)
    .def("GetHeight", py::overload_cast<>(&biogears::SEPatient::GetHeight))
    .def("GetHeight", py::overload_cast<const biogears::LengthUnit&>(&biogears::SEPatient::GetHeight, py::const_))

    .def("HasAlveoliSurfaceArea", &biogears::SEPatient::HasAlveoliSurfaceArea)
    .def("GetAlveoliSurfaceArea", py::overload_cast<>(&biogears::SEPatient::GetAlveoliSurfaceArea))
    .def("GetAlveoliSurfaceArea", py::overload_cast<const biogears::AreaUnit&>(&biogears::SEPatient::GetAlveoliSurfaceArea, py::const_))

    .def("HasBasalMetabolicRate", &biogears::SEPatient::HasBasalMetabolicRate)
    .def("GetBasalMetabolicRate", py::overload_cast<>(&biogears::SEPatient::GetBasalMetabolicRate))
    .def("GetBasalMetabolicRate", py::overload_cast<const biogears::PowerUnit&>(&biogears::SEPatient::GetBasalMetabolicRate, py::const_))

    .def("GetBloodType", &biogears::SEPatient::GetBloodType)
    .def("SetBloodType", &biogears::SEPatient::SetBloodType)
    .def("HasBloodType", &biogears::SEPatient::HasBloodType)
    .def("InvalidateBloodType", &biogears::SEPatient::InvalidateBloodType)

    .def("GetBloodRh", &biogears::SEPatient::GetBloodRh)
    .def("SetBloodRh", &biogears::SEPatient::SetBloodRh)
    .def("HasBloodRh", &biogears::SEPatient::HasBloodRh)
    .def("InvalidateBloodRh", &biogears::SEPatient::InvalidateBloodRh)

    .def("HasBloodVolumeBaseline", &biogears::SEPatient::HasBloodVolumeBaseline)
    .def("GetBloodVolumeBaseline", py::overload_cast<>(&biogears::SEPatient::GetBloodVolumeBaseline))
    .def("GetBloodVolumeBaseline", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetBloodVolumeBaseline, py::const_))

    .def("HasBodyDensity", &biogears::SEPatient::HasBodyDensity)
    .def("GetBodyDensity", py::overload_cast<>(&biogears::SEPatient::GetBodyDensity))
    .def("GetBodyDensity", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SEPatient::GetBodyDensity, py::const_))

    .def("HasBodyFatFraction", &biogears::SEPatient::HasBodyFatFraction)
    .def("GetBodyFatFraction", py::overload_cast<>(&biogears::SEPatient::GetBodyFatFraction))
    .def("GetBodyFatFraction", py::overload_cast<>(&biogears::SEPatient::GetBodyFatFraction, py::const_))

    .def("HasDiastolicArterialPressureBaseline", &biogears::SEPatient::HasDiastolicArterialPressureBaseline)
    .def("GetDiastolicArterialPressureBaseline", py::overload_cast<>(&biogears::SEPatient::GetDiastolicArterialPressureBaseline))
    .def("GetDiastolicArterialPressureBaseline", py::overload_cast<const biogears::PressureUnit&>(&biogears::SEPatient::GetDiastolicArterialPressureBaseline, py::const_))

    .def("HasExpiratoryReserveVolume", &biogears::SEPatient::HasExpiratoryReserveVolume)
    .def("GetExpiratoryReserveVolume", py::overload_cast<>(&biogears::SEPatient::GetExpiratoryReserveVolume))
    .def("GetExpiratoryReserveVolume", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetExpiratoryReserveVolume, py::const_))

    .def("HasFunctionalResidualCapacity", &biogears::SEPatient::HasFunctionalResidualCapacity)
    .def("GetFunctionalResidualCapacity", py::overload_cast<>(&biogears::SEPatient::GetFunctionalResidualCapacity))
    .def("GetFunctionalResidualCapacity", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetFunctionalResidualCapacity, py::const_))

    .def("HasHeartRateBaseline", &biogears::SEPatient::HasHeartRateBaseline)
    .def("GetHeartRateBaseline", py::overload_cast<>(&biogears::SEPatient::GetHeartRateBaseline))
    .def("GetHeartRateBaseline", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SEPatient::GetHeartRateBaseline, py::const_))

    .def("HasHeartRateMaximum", &biogears::SEPatient::HasHeartRateMaximum)
    .def("GetHeartRateMaximum", py::overload_cast<>(&biogears::SEPatient::GetHeartRateMaximum))
    .def("GetHeartRateMaximum", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SEPatient::GetHeartRateMaximum, py::const_))

    .def("HasHeartRateMinimum", &biogears::SEPatient::HasHeartRateMinimum)
    .def("GetHeartRateMinimum", py::overload_cast<>(&biogears::SEPatient::GetHeartRateMinimum))
    .def("GetHeartRateMinimum", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SEPatient::GetHeartRateMinimum, py::const_))

    .def("HasHyperhidrosis", &biogears::SEPatient::HasHyperhidrosis)
    .def("GetHyperhidrosis", py::overload_cast<>(&biogears::SEPatient::GetHyperhidrosis))
    .def("GetHyperhidrosis", py::overload_cast<>(&biogears::SEPatient::GetHyperhidrosis, py::const_))

    .def("HasInspiratoryCapacity", &biogears::SEPatient::HasInspiratoryCapacity)
    .def("GetInspiratoryCapacity", py::overload_cast<>(&biogears::SEPatient::GetInspiratoryCapacity))
    .def("GetInspiratoryCapacity", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetInspiratoryCapacity, py::const_))

    .def("HasInspiratoryReserveVolume", &biogears::SEPatient::HasInspiratoryReserveVolume)
    .def("GetInspiratoryReserveVolume", py::overload_cast<>(&biogears::SEPatient::GetInspiratoryReserveVolume))
    .def("GetInspiratoryReserveVolume", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetInspiratoryReserveVolume, py::const_))

    .def("HasLeanBodyMass", &biogears::SEPatient::HasLeanBodyMass)
    .def("GetLeanBodyMass", py::overload_cast<>(&biogears::SEPatient::GetLeanBodyMass))
    .def("GetLeanBodyMass", py::overload_cast<const biogears::MassUnit&>(&biogears::SEPatient::GetLeanBodyMass, py::const_))

    .def("HasMaxWorkRate", &biogears::SEPatient::HasMaxWorkRate)
    .def("GetMaxWorkRate", py::overload_cast<>(&biogears::SEPatient::GetMaxWorkRate))
    .def("GetMaxWorkRate", py::overload_cast<const biogears::PowerUnit&>(&biogears::SEPatient::GetMaxWorkRate, py::const_))

    .def("HasMuscleMass", &biogears::SEPatient::HasMuscleMass)
    .def("GetMuscleMass", py::overload_cast<>(&biogears::SEPatient::GetMuscleMass))
    .def("GetMuscleMass", py::overload_cast<const biogears::MassUnit&>(&biogears::SEPatient::GetMuscleMass, py::const_))

    .def("HasMeanArterialPressureBaseline", &biogears::SEPatient::HasMeanArterialPressureBaseline)
    .def("GetMeanArterialPressureBaseline", py::overload_cast<>(&biogears::SEPatient::GetMeanArterialPressureBaseline))
    .def("GetMeanArterialPressureBaseline", py::overload_cast<const biogears::PressureUnit&>(&biogears::SEPatient::GetMeanArterialPressureBaseline, py::const_))

    .def("HasPainSusceptibility", &biogears::SEPatient::HasPainSusceptibility)
    .def("GetPainSusceptibility", py::overload_cast<>(&biogears::SEPatient::GetPainSusceptibility))
    .def("GetPainSusceptibility", py::overload_cast<>(&biogears::SEPatient::GetPainSusceptibility, py::const_))

    .def("HasResidualVolume", &biogears::SEPatient::HasResidualVolume)
    .def("GetResidualVolume", py::overload_cast<>(&biogears::SEPatient::GetResidualVolume))
    .def("GetResidualVolume", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetResidualVolume, py::const_))

    .def("HasRespirationRateBaseline", &biogears::SEPatient::HasRespirationRateBaseline)
    .def("GetRespirationRateBaseline", py::overload_cast<>(&biogears::SEPatient::GetRespirationRateBaseline))
    .def("GetRespirationRateBaseline", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SEPatient::GetRespirationRateBaseline, py::const_))

    .def("HasRespiratoryDriverAmplitudeBaseline", &biogears::SEPatient::HasRespiratoryDriverAmplitudeBaseline)
    .def("GetRespiratoryDriverAmplitudeBaseline", py::overload_cast<>(&biogears::SEPatient::GetRespiratoryDriverAmplitudeBaseline))
    .def("GetRespiratoryDriverAmplitudeBaseline", py::overload_cast<const biogears::PressureUnit&>(&biogears::SEPatient::GetRespiratoryDriverAmplitudeBaseline, py::const_))

    .def("HasRightLungRatio", &biogears::SEPatient::HasRightLungRatio)
    .def("GetRightLungRatio", py::overload_cast<>(&biogears::SEPatient::GetRightLungRatio))
    .def("GetRightLungRatio", py::overload_cast<>(&biogears::SEPatient::GetRightLungRatio, py::const_))

    .def("HasSkinSurfaceArea", &biogears::SEPatient::HasSkinSurfaceArea)
    .def("GetSkinSurfaceArea", py::overload_cast<>(&biogears::SEPatient::GetSkinSurfaceArea))
    .def("GetSkinSurfaceArea", py::overload_cast<const biogears::AreaUnit&>(&biogears::SEPatient::GetSkinSurfaceArea, py::const_))

    .def("HasSleepAmount", &biogears::SEPatient::HasSleepAmount)
    .def("GetSleepAmount", py::overload_cast<>(&biogears::SEPatient::GetSleepAmount))
    .def("GetSleepAmount", py::overload_cast<const biogears::TimeUnit&>(&biogears::SEPatient::GetSleepAmount, py::const_))

    .def("HasSystolicArterialPressureBaseline", &biogears::SEPatient::HasSystolicArterialPressureBaseline)
    .def("GetSystolicArterialPressureBaseline", py::overload_cast<>(&biogears::SEPatient::GetSystolicArterialPressureBaseline))
    .def("GetSystolicArterialPressureBaseline", py::overload_cast<const biogears::PressureUnit&>(&biogears::SEPatient::GetSystolicArterialPressureBaseline, py::const_))

    .def("HasTotalVentilationBaseline", &biogears::SEPatient::HasTotalVentilationBaseline)
    .def("GetTotalVentilationBaseline", py::overload_cast<>(&biogears::SEPatient::GetTotalVentilationBaseline))
    .def("GetTotalVentilationBaseline", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SEPatient::GetTotalVentilationBaseline, py::const_))

    .def("HasTidalVolumeBaseline", &biogears::SEPatient::HasTidalVolumeBaseline)
    .def("GetTidalVolumeBaseline", py::overload_cast<>(&biogears::SEPatient::GetTidalVolumeBaseline))
    .def("GetTidalVolumeBaseline", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetTidalVolumeBaseline, py::const_))

    .def("HasTotalLungCapacity", &biogears::SEPatient::HasTotalLungCapacity)
    .def("GetTotalLungCapacity", py::overload_cast<>(&biogears::SEPatient::GetTotalLungCapacity))
    .def("GetTotalLungCapacity", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetTotalLungCapacity, py::const_))

    .def("HasVitalCapacity", &biogears::SEPatient::HasVitalCapacity)
    .def("GetVitalCapacity", py::overload_cast<>(&biogears::SEPatient::GetVitalCapacity))
    .def("GetVitalCapacity", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SEPatient::GetVitalCapacity, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
