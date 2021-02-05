#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>    
#include <biogears/container/Tree.tci.h>


namespace py = pybind11;

PYBIND11_MODULE(pybSENervousSystem, m)
{
  py::module_::import("pybSESystem");

  py::class_<biogears::SENervousSystem, biogears::SESystem>(m, "SENervousSystem")
    .def(py::init<biogears::Logger*>())
    .def("GetComplianceScale", py::overload_cast<>(&biogears::SENervousSystem::GetComplianceScale))
    .def("GetComplianceScale", py::overload_cast<>(&biogears::SENervousSystem::GetComplianceScale, py::const_))
    .def("GetHeartRateScale", py::overload_cast<>(&biogears::SENervousSystem::GetHeartRateScale))
    .def("GetHeartRateScale", py::overload_cast<>(&biogears::SENervousSystem::GetHeartRateScale, py::const_))
    .def("GetHeartElastanceScale", py::overload_cast<>(&biogears::SENervousSystem::GetHeartElastanceScale))
    .def("GetHeartElastanceScale", py::overload_cast<>(&biogears::SENervousSystem::GetHeartElastanceScale, py::const_))
    .def("GetResistanceScaleExtrasplanchnic", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleExtrasplanchnic))
    .def("GetResistanceScaleExtrasplanchnic", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleExtrasplanchnic, py::const_))
    .def("GetResistanceScaleMuscle", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleMuscle))
    .def("GetResistanceScaleMuscle", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleMuscle, py::const_))
    .def("GetResistanceScaleMyocardium", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleMyocardium))
    .def("GetResistanceScaleMyocardium", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleMyocardium, py::const_))
    .def("GetResistanceScaleSplanchnic", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleSplanchnic))
    .def("GetResistanceScaleSplanchnic", py::overload_cast<>(&biogears::SENervousSystem::GetResistanceScaleSplanchnic, py::const_))
    .def("HasLeftEyePupillaryResponse", &biogears::SENervousSystem::HasLeftEyePupillaryResponse)
    .def("GetLeftEyePupillaryResponse", py::overload_cast<>(&biogears::SENervousSystem::GetLeftEyePupillaryResponse))
    .def("GetLeftEyePupillaryResponse", py::overload_cast<>(&biogears::SENervousSystem::GetLeftEyePupillaryResponse, py::const_))
    .def("RemoveLeftEyePupillaryResponse", &biogears::SENervousSystem::RemoveLeftEyePupillaryResponse)
    .def("HasPainVisualAnalogueScale", &biogears::SENervousSystem::HasPainVisualAnalogueScale)
    .def("GetPainVisualAnalogueScale", py::overload_cast<>(&biogears::SENervousSystem::GetPainVisualAnalogueScale))
    .def("GetPainVisualAnalogueScale", py::overload_cast<>(&biogears::SENervousSystem::GetPainVisualAnalogueScale, py::const_))
    .def("HasRichmondAgitationSedationScale", &biogears::SENervousSystem::HasRichmondAgitationSedationScale)
    .def("GetRichmondAgitationSedationScale", py::overload_cast<>(&biogears::SENervousSystem::GetRichmondAgitationSedationScale))
    .def("GetRichmondAgitationSedationScale", py::overload_cast<>(&biogears::SENervousSystem::GetRichmondAgitationSedationScale, py::const_))
    .def("HasRightEyePupillaryResponse", &biogears::SENervousSystem::HasRightEyePupillaryResponse)
    .def("GetRightEyePupillaryResponse", py::overload_cast<>(&biogears::SENervousSystem::GetRightEyePupillaryResponse))
    .def("GetRightEyePupillaryResponse", py::overload_cast<>(&biogears::SENervousSystem::GetRightEyePupillaryResponse, py::const_))
    .def("RemoveRightEyePupillaryResponse", &biogears::SENervousSystem::RemoveRightEyePupillaryResponse)
    .def("IsAsleep", &biogears::SENervousSystem::IsAsleep)
    .def("GetSleepTime", py::overload_cast<>(&biogears::SENervousSystem::GetSleepTime))
    .def("GetSleepTime", py::overload_cast<const biogears::TimeUnit&>(&biogears::SENervousSystem::GetSleepTime, py::const_))
    .def("GetSleepState", &biogears::SENervousSystem::GetSleepState)
    .def("SetSleepState", &biogears::SENervousSystem::SetSleepState)
    .def("InvalidateSleepState", &biogears::SENervousSystem::InvalidateSleepState)
    .def("HasSleepState", &biogears::SENervousSystem::HasSleepState)
    .def("IsAwake", &biogears::SENervousSystem::IsAwake)
    .def("GetWakeTime", py::overload_cast<>(&biogears::SENervousSystem::GetWakeTime))
    .def("GetWakeTime", py::overload_cast<const biogears::TimeUnit&>(&biogears::SENervousSystem::GetWakeTime, py::const_))
    .def("HasBiologicalDebt", &biogears::SENervousSystem::HasBiologicalDebt)
    .def("GetBiologicalDebt", py::overload_cast<>(&biogears::SENervousSystem::GetBiologicalDebt))
    .def("GetBiologicalDebt", py::overload_cast<>(&biogears::SENervousSystem::GetBiologicalDebt, py::const_))
    .def("HasReactionTime", &biogears::SENervousSystem::HasReactionTime)
    .def("GetReactionTime", py::overload_cast<>(&biogears::SENervousSystem::GetReactionTime))
    .def("GetReactionTime", py::overload_cast<const biogears::TimeUnit&>(&biogears::SENervousSystem::GetReactionTime, py::const_))
    .def("HasAttentionLapses", &biogears::SENervousSystem::HasAttentionLapses)
    .def("GetAttentionLapses", py::overload_cast<>(&biogears::SENervousSystem::GetAttentionLapses))
    .def("GetAttentionLapses", py::overload_cast<>(&biogears::SENervousSystem::GetAttentionLapses, py::const_));



#ifdef VERSION_INFO
      m.attr("__version__")
    = VERSION_INFO;
#else
      m.attr("__version__")
    = "dev";
#endif
}