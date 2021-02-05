#include <pybind11/pybind11.h>

#include <string>

#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSECardiovascularSystem, m)
{
  py::module_::import("pybSESystem");

  py::class_<biogears::SECardiovascularSystem, biogears::SESystem>(m, "SECardiovascularSystem")
    .def(py::init<biogears::Logger*>())
    .def("TypeHash", &biogears::SECardiovascularSystem::TypeHash)
    .def("TypeTag", &biogears::SECardiovascularSystem::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SECardiovascularSystem::classname, py::const_))
    .def("hash_code", py::overload_cast<>(&biogears::SECardiovascularSystem::hash_code, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SECardiovascularSystem::Clear))
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SECardiovascularSystem::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SECardiovascularSystem::GetScalar))
    .def("Load", &biogears::SECardiovascularSystem::Load)
    .def("Unload", py::overload_cast<>(&biogears::SECardiovascularSystem::Unload, py::const_))
    .def("GetPhysiologyRequestGraph", &biogears::SECardiovascularSystem::GetPhysiologyRequestGraph)

    .def("HasArterialPressure", &biogears::SECardiovascularSystem::HasArterialPressure)
    .def("GetArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetArterialPressure))
    .def("GetArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetArterialPressure, py::const_))

    .def("HasBloodVolume", &biogears::SECardiovascularSystem::HasBloodVolume)
    .def("GetBloodVolume", py::overload_cast<>(&biogears::SECardiovascularSystem::GetBloodVolume))
    .def("GetBloodVolume", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SECardiovascularSystem::GetBloodVolume, py::const_))

    .def("HasCardiacIndex", &biogears::SECardiovascularSystem::HasCardiacIndex)
    .def("GetCardiacIndex", py::overload_cast<>(&biogears::SECardiovascularSystem::GetCardiacIndex))
    .def("GetCardiacIndex", py::overload_cast<const biogears::VolumePerTimeAreaUnit&>(&biogears::SECardiovascularSystem::GetCardiacIndex, py::const_))

    .def("HasCardiacOutput", &biogears::SECardiovascularSystem::HasCardiacOutput)
    .def("GetCardiacOutput", py::overload_cast<>(&biogears::SECardiovascularSystem::GetCardiacOutput))
    .def("GetCardiacOutput", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SECardiovascularSystem::GetCardiacOutput, py::const_))

    .def("HasCentralVenousPressure", &biogears::SECardiovascularSystem::HasCentralVenousPressure)
    .def("GetCentralVenousPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetCentralVenousPressure))
    .def("GetCentralVenousPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetCentralVenousPressure, py::const_))

    .def("HasCerebralBloodFlow", &biogears::SECardiovascularSystem::HasCerebralBloodFlow)
    .def("GetCerebralBloodFlow", py::overload_cast<>(&biogears::SECardiovascularSystem::GetCerebralBloodFlow))
    .def("GetCerebralBloodFlow", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SECardiovascularSystem::GetCerebralBloodFlow, py::const_))

    .def("HasCerebralPerfusionPressure", &biogears::SECardiovascularSystem::HasCerebralPerfusionPressure)
    .def("GetCerebralPerfusionPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetCerebralPerfusionPressure))
    .def("GetCerebralPerfusionPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetCerebralPerfusionPressure, py::const_))

    .def("HasDiastolicArterialPressure", &biogears::SECardiovascularSystem::HasDiastolicArterialPressure)
    .def("GetDiastolicArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetDiastolicArterialPressure))
    .def("GetDiastolicArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetDiastolicArterialPressure, py::const_))

    .def("HasHeartEjectionFraction", &biogears::SECardiovascularSystem::HasHeartEjectionFraction)
    .def("GetHeartEjectionFraction", py::overload_cast<>(&biogears::SECardiovascularSystem::GetHeartEjectionFraction))
    .def("GetHeartEjectionFraction", py::overload_cast<>(&biogears::SECardiovascularSystem::GetHeartEjectionFraction, py::const_))

    .def("HasHeartRate", &biogears::SECardiovascularSystem::HasHeartRate)
    .def("GetHeartRate", py::overload_cast<>(&biogears::SECardiovascularSystem::GetHeartRate))
    .def("GetHeartRate", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SECardiovascularSystem::GetHeartRate, py::const_))

    .def("GetHeartRhythm", &biogears::SECardiovascularSystem::GetHeartRhythm)
    .def("SetHeartRhythm", &biogears::SECardiovascularSystem::SetHeartRhythm)
    .def("HasHeartRhythm", &biogears::SECardiovascularSystem::HasHeartRhythm)
    .def("InvalidateHeartRhythm", &biogears::SECardiovascularSystem::InvalidateHeartRhythm)

    .def("HasHeartStrokeVolume", &biogears::SECardiovascularSystem::HasHeartStrokeVolume)
    .def("GetHeartStrokeVolume", py::overload_cast<>(&biogears::SECardiovascularSystem::GetHeartStrokeVolume))
    .def("GetHeartStrokeVolume", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SECardiovascularSystem::GetHeartStrokeVolume, py::const_))

    .def("HasIntracranialPressure", &biogears::SECardiovascularSystem::HasIntracranialPressure)
    .def("GetIntracranialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetIntracranialPressure))
    .def("GetIntracranialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetIntracranialPressure, py::const_))

    .def("HasMeanArterialPressure", &biogears::SECardiovascularSystem::HasMeanArterialPressure)
    .def("GetMeanArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetMeanArterialPressure))
    .def("GetMeanArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetMeanArterialPressure, py::const_))

    .def("HasMeanArterialCarbonDioxidePartialPressure", &biogears::SECardiovascularSystem::HasMeanArterialCarbonDioxidePartialPressure)
    .def("GetMeanArterialCarbonDioxidePartialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetMeanArterialCarbonDioxidePartialPressure))
    .def("GetMeanArterialCarbonDioxidePartialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetMeanArterialCarbonDioxidePartialPressure, py::const_))

    .def("HasMeanArterialCarbonDioxidePartialPressureDelta", &biogears::SECardiovascularSystem::HasMeanArterialCarbonDioxidePartialPressureDelta)
    .def("GetMeanArterialCarbonDioxidePartialPressureDelta", py::overload_cast<>(&biogears::SECardiovascularSystem::GetMeanArterialCarbonDioxidePartialPressureDelta))
    .def("GetMeanArterialCarbonDioxidePartialPressureDelta", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetMeanArterialCarbonDioxidePartialPressureDelta, py::const_))

    .def("HasMeanCentralVenousPressure", &biogears::SECardiovascularSystem::HasMeanCentralVenousPressure)
    .def("GetMeanCentralVenousPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetMeanCentralVenousPressure))
    .def("GetMeanCentralVenousPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetMeanCentralVenousPressure, py::const_))

    .def("HasMeanSkinFlow", &biogears::SECardiovascularSystem::HasMeanSkinFlow)
    .def("GetMeanSkinFlow", py::overload_cast<>(&biogears::SECardiovascularSystem::GetMeanSkinFlow))
    .def("GetMeanSkinFlow", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SECardiovascularSystem::GetMeanSkinFlow, py::const_))

    .def("HasPulmonaryArterialPressure", &biogears::SECardiovascularSystem::HasPulmonaryArterialPressure)
    .def("GetPulmonaryArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryArterialPressure))
    .def("GetPulmonaryArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryArterialPressure, py::const_))

    .def("HasPulmonaryCapillariesWedgePressure", &biogears::SECardiovascularSystem::HasPulmonaryCapillariesWedgePressure)
    .def("GetPulmonaryCapillariesWedgePressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryCapillariesWedgePressure))
    .def("GetPulmonaryCapillariesWedgePressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryCapillariesWedgePressure, py::const_))

    .def("HasPulmonaryDiastolicArterialPressure", &biogears::SECardiovascularSystem::HasPulmonaryDiastolicArterialPressure)
    .def("GetPulmonaryDiastolicArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryDiastolicArterialPressure))
    .def("GetPulmonaryDiastolicArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryDiastolicArterialPressure, py::const_))

    .def("HasPulmonaryMeanArterialPressure", &biogears::SECardiovascularSystem::HasPulmonaryMeanArterialPressure)
    .def("GetPulmonaryMeanArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryMeanArterialPressure))
    .def("GetPulmonaryMeanArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryMeanArterialPressure, py::const_))

    .def("HasPulmonaryMeanCapillaryFlow", &biogears::SECardiovascularSystem::HasPulmonaryMeanCapillaryFlow)
    .def("GetPulmonaryMeanCapillaryFlow", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryMeanCapillaryFlow))
    .def("GetPulmonaryMeanCapillaryFlow", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryMeanCapillaryFlow, py::const_))

    .def("HasPulmonaryMeanShuntFlow", &biogears::SECardiovascularSystem::HasPulmonaryMeanShuntFlow)
    .def("GetPulmonaryMeanShuntFlow", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryMeanShuntFlow))
    .def("GetPulmonaryMeanShuntFlow", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryMeanShuntFlow, py::const_))

    .def("HasPulmonarySystolicArterialPressure", &biogears::SECardiovascularSystem::HasPulmonarySystolicArterialPressure)
    .def("GetPulmonarySystolicArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonarySystolicArterialPressure))
    .def("GetPulmonarySystolicArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetPulmonarySystolicArterialPressure, py::const_))

    .def("HasPulmonaryVascularResistance", &biogears::SECardiovascularSystem::HasPulmonaryVascularResistance)
    .def("GetPulmonaryVascularResistance", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryVascularResistance))
    .def("GetPulmonaryVascularResistance", py::overload_cast<const biogears::FlowResistanceUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryVascularResistance, py::const_))

    .def("HasPulmonaryVascularResistanceIndex", &biogears::SECardiovascularSystem::HasPulmonaryVascularResistanceIndex)
    .def("GetPulmonaryVascularResistanceIndex", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulmonaryVascularResistanceIndex))
    .def("GetPulmonaryVascularResistanceIndex", py::overload_cast<const biogears::PressureTimePerVolumeAreaUnit&>(&biogears::SECardiovascularSystem::GetPulmonaryVascularResistanceIndex, py::const_))

    .def("HasPulsePressure", &biogears::SECardiovascularSystem::HasPulsePressure)
    .def("GetPulsePressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetPulsePressure))
    .def("GetPulsePressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetPulsePressure, py::const_))

    .def("HasSystolicArterialPressure", &biogears::SECardiovascularSystem::HasSystolicArterialPressure)
    .def("GetSystolicArterialPressure", py::overload_cast<>(&biogears::SECardiovascularSystem::GetSystolicArterialPressure))
    .def("GetSystolicArterialPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetSystolicArterialPressure, py::const_))

    .def("HasSystemicVascularResistance", &biogears::SECardiovascularSystem::HasSystemicVascularResistance)
    .def("GetSystemicVascularResistance", py::overload_cast<>(&biogears::SECardiovascularSystem::GetSystemicVascularResistance))
    .def("GetSystemicVascularResistance", py::overload_cast<const biogears::FlowResistanceUnit&>(&biogears::SECardiovascularSystem::GetSystemicVascularResistance, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__")
    = VERSION_INFO;
#else
  m.attr("__version__")
    = "dev";
#endif
}