#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEScenarioAutoSerialization(py::module_ &m)
{

  py::class_<biogears::SEScenarioAutoSerialization, biogears::Loggable>(m, "SEScenarioAutoSerialization")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SEScenarioAutoSerialization::Clear)
    .def("IsValid", &biogears::SEScenarioAutoSerialization::IsValid)

    .def("Load", &biogears::SEScenarioAutoSerialization::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEScenarioAutoSerialization::Unload,py::const_))
    .def("HasPeriod", &biogears::SEScenarioAutoSerialization::HasPeriod)
    .def("GetPeriod", py::overload_cast<>(&biogears::SEScenarioAutoSerialization::GetPeriod))
    .def("GetPeriod", py::overload_cast<const biogears::TimeUnit&>(&biogears::SEScenarioAutoSerialization::GetPeriod,py::const_))
    
    .def("HasPeriodTimeStamps", &biogears::SEScenarioAutoSerialization::HasPeriodTimeStamps)
    .def("GetPeriodTimeStamps", &biogears::SEScenarioAutoSerialization::GetPeriodTimeStamps)
    .def("SetPeriodTimeStamps", &biogears::SEScenarioAutoSerialization::SetPeriodTimeStamps)
    .def("InvalidatePeriodTimeStamps", &biogears::SEScenarioAutoSerialization::InvalidatePeriodTimeStamps)

    .def("HasAfterActions", &biogears::SEScenarioAutoSerialization::HasAfterActions)
    .def("GetAfterActions", &biogears::SEScenarioAutoSerialization::GetAfterActions)
    .def("SetAfterActions", &biogears::SEScenarioAutoSerialization::SetAfterActions)
    .def("InvalidateAfterActions", &biogears::SEScenarioAutoSerialization::InvalidateAfterActions)
    
    .def("HasReloadState", &biogears::SEScenarioAutoSerialization::HasReloadState)
    .def("GetReloadState", &biogears::SEScenarioAutoSerialization::GetReloadState)
    .def("SetReloadState", &biogears::SEScenarioAutoSerialization::SetReloadState)
    .def("InvalidateReloadState", &biogears::SEScenarioAutoSerialization::InvalidateReloadState)

    .def("HasDirectory", &biogears::SEScenarioAutoSerialization::HasDirectory)
    .def("GetDirectory", &biogears::SEScenarioAutoSerialization::GetDirectory)
    .def("SetDirectory", py::overload_cast<const char*>(&biogears::SEScenarioAutoSerialization::SetDirectory))
    .def("SetDirectory", py::overload_cast<const std::string&>(&biogears::SEScenarioAutoSerialization::SetDirectory))
    .def("InvalidateDirectory", &biogears::SEScenarioAutoSerialization::InvalidateDirectory)

    .def("HasFileName", &biogears::SEScenarioAutoSerialization::HasFileName)
    .def("GetFileName", &biogears::SEScenarioAutoSerialization::GetFileName)
    .def("SetFileName", py::overload_cast<const char*>(&biogears::SEScenarioAutoSerialization::SetFileName))
    .def("SetFileName", py::overload_cast<const std::string&>(&biogears::SEScenarioAutoSerialization::SetFileName))
    .def("InvalidateFileName", &biogears::SEScenarioAutoSerialization::InvalidateFileName);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
