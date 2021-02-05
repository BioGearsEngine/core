#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/SEActionManager.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEActionManager, m)
{

  py::class_<biogears::SEActionManager, biogears::Loggable>(m, "SEActionManager")
    .def(py::init<biogears::SESubstanceManager&>())
    .def("Clear", py::overload_cast<>(&biogears::SEActionManager::Clear))
    .def("Unload", &biogears::SEActionManager::Unload)
    .def("ProcessAction", py::overload_cast<const biogears::SEAction&>(&biogears::SEActionManager::ProcessAction))
    .def("ProcessAction", py::overload_cast<const CDM::ActionData&>(&biogears::SEActionManager::ProcessAction))
    .def("GetEnvironmentActions", &biogears::SEActionManager::GetEnvironmentActions)
    .def("GetPatientActions", &biogears::SEActionManager::GetPatientActions)
    .def("GetAnesthesiaMachineActions", &biogears::SEActionManager::GetAnesthesiaMachineActions)
    .def("GetInhalerActions", &biogears::SEActionManager::GetInhalerActions);
#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}