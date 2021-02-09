#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/SEAction.h>

namespace py = pybind11;

void define_pybSEAction(py::module_ &m)
{



  //SEAction is an abstract class

  //py::class_<biogears::SEAction, biogears::Loggable>(m, "SEAction")
  //  .def(py::init<>())
  //  .def("Clear",&biogears::SEAction::Clear)
  //  .def("IsValid",&biogears::SEAction::IsValid)
  //  .def("Load",&biogears::SEAction::Load)
  //  .def("Unload",py::overload_cast<>(&biogears::SEAction::Unload,py::const_))
  //  .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEAction::ToString,py::const_));
  //;

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
