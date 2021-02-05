#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/SEAction.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEAction, m)
{

  py::module_::import("pybLogger");

  py::class_<biogears::SEAction, biogears::Loggable>(m, "SEAction");
  // .def(py::init<>())
  // .def("Clear",&biogears::SEPatientAction::Clear)
  // .def("IsValid",&biogears::SEPatientAction::IsValid)
  // .def("Load",&biogears::SEPatientAction::Load);
  // // .def("Unload",py::overload_cast<>(&biogears::SEPatientAction::Unload,py::const_))
  // // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEPatientAction::ToString,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}