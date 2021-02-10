#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.h>

namespace py = pybind11;

void define_pybSEPhysiologyDataRequest(py::module_ &m)
{

  py::class_<biogears::SEPhysiologyDataRequest, biogears::SEDataRequest>(m, "SEPhysiologyDataRequest")
    .def("Clear", &biogears::SEPhysiologyDataRequest::Clear)
    .def("Load", &biogears::SEPhysiologyDataRequest::Load)
    .def("HashCode", py::overload_cast<>(&biogears::SEPhysiologyDataRequest::HashCode, py::const_))
    .def("Unload", py::overload_cast<>(&biogears::SEPhysiologyDataRequest::Unload, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
