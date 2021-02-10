#include <pybind11/pybind11.h>

#include <iostream>
#include <string>

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#include "PySEAction.h"
#include "PySEPatientAction.h"

namespace py = pybind11;

void define_pybSEPatientAction(py::module_& m)
{

  py::class_<biogears::SEAction, PySEAction>(m, "SEAction")
    .def(py::init<>())
    .def("Clear", &biogears::SEAction::Clear)
    .def("IsValid", &biogears::SEAction::IsValid)
    .def("Load", &biogears::SEAction::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEAction::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEAction::ToString, py::const_));

  py::class_<biogears::SEPatientAction, PySEPatientAction>(m, "SEPatientAction")
    .def(py::init<>())
    .def("Clear", &biogears::SEPatientAction::Clear)
    .def("IsValid", &biogears::SEPatientAction::IsValid)
    .def("Load", &biogears::SEPatientAction::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPatientAction::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEPatientAction::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
