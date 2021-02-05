#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSECompartmentDataRequest, m)
{

  py::class_<biogears::SECompartmentDataRequest, biogears::SEDataRequest>(m, "SECompartmentDataRequest")
    .def("Clear", py::overload_cast<>(&biogears::SECompartmentDataRequest::Clear))
    .def("Load", &biogears::SECompartmentDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SECompartmentDataRequest::Unload, py::const_))
    .def("HashCode", &biogears::SECompartmentDataRequest::HashCode)
    .def("GetCompartment", &biogears::SECompartmentDataRequest::GetCompartment)

    .def("SetCompartment", py::overload_cast<const char*>(&biogears::SECompartmentDataRequest::SetCompartment))
    .def("SetCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentDataRequest::SetCompartment))
    .def("HasCompartment", &biogears::SECompartmentDataRequest::HasCompartment)
    .def("InvalidateCompartment", &biogears::SECompartmentDataRequest::InvalidateCompartment)

    .def("Set", py::overload_cast<const char*, const char*, const char*>(&biogears::SECompartmentDataRequest::Set))
    .def("Set", py::overload_cast<const std::string&, const std::string&, const std::string&>(&biogears::SECompartmentDataRequest::Set))
    .def("Set", py::overload_cast<const char*, const char*, const biogears::CCompoundUnit&>(&biogears::SECompartmentDataRequest::Set))
    .def("Set", py::overload_cast<const std::string&, const std::string&, const biogears::CCompoundUnit&>(&biogears::SECompartmentDataRequest::Set));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}