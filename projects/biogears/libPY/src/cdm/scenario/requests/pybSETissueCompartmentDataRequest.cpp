#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSETissueCompartmentDataRequest, m)
{

  py::class_<biogears::SETissueCompartmentDataRequest, biogears::SECompartmentDataRequest>(m, "SEThermalCompartmentDataRequest")
    .def("Clear", &biogears::SETissueCompartmentDataRequest::Clear)
    .def("Load", &biogears::SETissueCompartmentDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SETissueCompartmentDataRequest::Unload, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}