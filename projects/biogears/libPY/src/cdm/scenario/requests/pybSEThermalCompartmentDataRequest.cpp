#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.h>

namespace py = pybind11;

void define_pybSEThermalCompartmentDataRequest(py::module_ &m)
{

  py::class_<biogears::SEThermalCompartmentDataRequest, biogears::SECompartmentDataRequest>(m, "SEThermalCompartmentDataRequest")
    .def("Load", &biogears::SEThermalCompartmentDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEThermalCompartmentDataRequest::Unload, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
