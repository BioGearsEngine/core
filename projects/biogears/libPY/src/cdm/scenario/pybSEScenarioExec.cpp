#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEScenarioExec.h>

namespace py = pybind11;

void define_pybSEScenarioExec(py::module_ &m)
{
  py::class_<biogears::SEScenarioCustomExec>(m, "SEScenarioCustomExec")
    .def("CustomExec", &biogears::SEScenarioCustomExec::CustomExec);

  py::class_<biogears::SEScenarioExec, biogears::Loggable>(m, "SEScenarioExec")
    .def(py::init<biogears::PhysiologyEngine&>())
    .def("GetEngine", &biogears::SEScenarioExec::GetEngine)
    .def("Cancel", &biogears::SEScenarioExec::Cancel)
    .def("Execute", py::overload_cast<const char*, const char*, biogears::SEScenarioCustomExec*>(&biogears::SEScenarioExec::Execute))
    .def("Execute", py::overload_cast<biogears::SEScenario const&, const char*, biogears::SEScenarioCustomExec*>(&biogears::SEScenarioExec::Execute))
    .def("Execute", py::overload_cast<const std::string&, const std::string&, biogears::SEScenarioCustomExec*>(&biogears::SEScenarioExec::Execute))
    .def("Execute", py::overload_cast<biogears::SEScenario const&, const std::string&, biogears::SEScenarioCustomExec*>(&biogears::SEScenarioExec::Execute));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
