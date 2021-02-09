#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/container/Tree.tci.h>

namespace py = pybind11;

void define_pybSESystem(py::module_ &m)
{
  py::class_<biogears::SESystem, biogears::Loggable>(m, "SESystem");
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
