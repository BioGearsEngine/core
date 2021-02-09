#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/requests/SEEnvironmentDataRequest.h>

namespace py = pybind11;

void define_pybSEEnvironmentDataRequest(py::module_ &m)
{

  py::class_<biogears::SEEnvironmentDataRequest, biogears::SEDataRequest>(m, "SEEnvironmentDataRequest")
    .def("Clear", py::overload_cast<>(&biogears::SEEnvironmentDataRequest::Clear))
    .def("Load", &biogears::SEEnvironmentDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEEnvironmentDataRequest::Unload, py::const_))
    .def("HashCode", py::overload_cast<>(&biogears::SEEnvironmentDataRequest::HashCode, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
