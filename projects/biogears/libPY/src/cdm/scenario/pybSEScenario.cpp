#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEScenario(py::module_ &m)
{

  py::class_<biogears::SEScenario, biogears::Loggable>(m, "SEScenario")
    .def(py::init<biogears::SESubstanceManager&>())
    .def("Clear", &biogears::SEScenario::Clear)
    .def("Load", py::overload_cast<const CDM::ScenarioData&>(&biogears::SEScenario::Load))
    .def("Unload", py::overload_cast<>(&biogears::SEScenario::Unload,py::const_))
    .def("Load", py::overload_cast<const char*>(&biogears::SEScenario::Load))
    .def("Load", py::overload_cast<const std::string&>(&biogears::SEScenario::Load))
    .def("IsValid", &biogears::SEScenario::IsValid)
    .def("GetName", &biogears::SEScenario::GetName)
    .def("GetName_cStr", &biogears::SEScenario::GetName_cStr)
    .def("SetName", py::overload_cast<const char*>(&biogears::SEScenario::SetName))
    .def("SetName", py::overload_cast<const std::string&>(&biogears::SEScenario::SetName))
    .def("HasName", &biogears::SEScenario::HasName)
    .def("InvalidateName", &biogears::SEScenario::InvalidateName)
    
    .def("GetPatientFile", &biogears::SEScenario::GetPatientFile)
    .def("SetPatientFile", py::overload_cast<const char*>(&biogears::SEScenario::SetPatientFile))
    .def("SetPatientFile", py::overload_cast<const std::string&>(&biogears::SEScenario::SetPatientFile))   
    .def("HasPatientFile", &biogears::SEScenario::HasPatientFile)
    .def("InvalidatePatientFile", &biogears::SEScenario::InvalidatePatientFile)

    .def("GetDescription", &biogears::SEScenario::GetDescription)
    .def("SetDescription", py::overload_cast<const char*>(&biogears::SEScenario::SetDescription))
    .def("SetDescription", py::overload_cast<const std::string&>(&biogears::SEScenario::SetDescription))   
    .def("HasDescription", &biogears::SEScenario::HasDescription)
    .def("InvalidateDescription", &biogears::SEScenario::InvalidateDescription)

    .def("GetEngineStateFile", &biogears::SEScenario::GetEngineStateFile)
    .def("SetEngineStateFile", py::overload_cast<const char*>(&biogears::SEScenario::SetEngineStateFile))
    .def("SetEngineStateFile", py::overload_cast<const std::string&>(&biogears::SEScenario::SetEngineStateFile))   
    .def("HasEngineStateFile", &biogears::SEScenario::HasEngineStateFile)
    .def("InvalidateEngineStateFile", &biogears::SEScenario::InvalidateEngineStateFile)

    .def("GetInitialParameters", py::overload_cast<>(&biogears::SEScenario::GetInitialParameters))
    .def("GetInitialParameters", py::overload_cast<>(&biogears::SEScenario::GetInitialParameters,py::const_))
    .def("HasInitialParameters", &biogears::SEScenario::HasInitialParameters)
    .def("InvalidateInitialParameters", &biogears::SEScenario::InvalidateInitialParameters)

    .def("GetAutoSerialization", py::overload_cast<>(&biogears::SEScenario::GetAutoSerialization))
    .def("GetAutoSerialization", py::overload_cast<>(&biogears::SEScenario::GetAutoSerialization,py::const_))
    .def("HasAutoSerialization", &biogears::SEScenario::HasAutoSerialization)
    .def("RemoveAutoSerialization", &biogears::SEScenario::RemoveAutoSerialization)

    .def("AddAction", &biogears::SEScenario::AddAction)
    .def("ClearActions", &biogears::SEScenario::ClearActions)
    .def("AddActionAfter", &biogears::SEScenario::AddActionAfter)
    .def("GetActions", &biogears::SEScenario::GetActions)

    .def("GetDataRequestManager", py::overload_cast<>(&biogears::SEScenario::GetDataRequestManager))
    .def("GetDataRequestManager", py::overload_cast<>(&biogears::SEScenario::GetDataRequestManager,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
