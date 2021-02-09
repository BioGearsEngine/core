#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
#include <biogears/container/Tree.tci.h>

namespace py = pybind11;

void define_pybSEGasCompartmentDataRequest(py::module_ &m)
{

  py::class_<biogears::SEGasCompartmentDataRequest, biogears::SECompartmentSubstanceDataRequest>(m, "SEGasCompartmentDataRequest")
    .def("Load", &biogears::SEGasCompartmentDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEGasCompartmentDataRequest::Unload, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
