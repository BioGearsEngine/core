#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSECompartmentSubstanceDataRequest, m)
{

  py::class_<biogears::SECompartmentSubstanceDataRequest, biogears::SECompartmentDataRequest>(m, "SECompartmentSubstanceDataRequest")
    .def("Clear", py::overload_cast<>(&biogears::SECompartmentSubstanceDataRequest::Clear))
    .def("Load", &biogears::SECompartmentSubstanceDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SECompartmentSubstanceDataRequest::Unload, py::const_))
    .def("HashCode", py::overload_cast<>(&biogears::SECompartmentSubstanceDataRequest::HashCode, py::const_))
    .def("GetSubstance", &biogears::SECompartmentSubstanceDataRequest::GetSubstance)

    .def("SetSubstance", &biogears::SECompartmentSubstanceDataRequest::SetSubstance)
    .def("HasSubstance", &biogears::SECompartmentSubstanceDataRequest::HasSubstance)
    .def("InvalidateSubstance", &biogears::SECompartmentSubstanceDataRequest::InvalidateSubstance)

    .def("Set", py::overload_cast<const char*, biogears::SESubstance&, const char*, const char*>(&biogears::SECompartmentSubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const std::string&, biogears::SESubstance&, const std::string&, const std::string&>(&biogears::SECompartmentSubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const char*, biogears::SESubstance&, const char*, const biogears::CCompoundUnit&>(&biogears::SECompartmentSubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const std::string&, biogears::SESubstance&, const std::string&, const biogears::CCompoundUnit&>(&biogears::SECompartmentSubstanceDataRequest::Set));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}