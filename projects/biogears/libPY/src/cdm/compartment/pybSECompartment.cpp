#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/compartment/SECompartment.h>

namespace py = pybind11;

void define_pybSECompartment(py::module_ &m)
{



  py::class_<biogears::SECompartment, biogears::Loggable>(m, "SECompartment")
    .def("Clear", &biogears::SECompartment::Clear)
    .def("Load", &biogears::SECompartment::Load)
    .def("GetName", &biogears::SECompartment::GetName)
    .def("GetName_cStr", &biogears::SECompartment::GetName_cStr);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
