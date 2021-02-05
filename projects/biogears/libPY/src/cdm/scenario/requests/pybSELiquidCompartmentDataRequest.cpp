#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/scenario/requests/SELiquidCompartmentDataRequest.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSELiquidCompartmentDataRequest, m)
{

  py::class_<biogears::SELiquidCompartmentDataRequest, biogears::SECompartmentSubstanceDataRequest>(m, "SELiquidCompartmentDataRequest")
    .def("Load", &biogears::SELiquidCompartmentDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SELiquidCompartmentDataRequest::Unload, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}