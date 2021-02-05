#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESubstanceDataRequest, m)
{

  py::class_<biogears::SESubstanceDataRequest, biogears::SEDataRequest>(m, "SESubstanceDataRequest")
    .def("Clear", &biogears::SESubstanceDataRequest::Clear)
    .def("Load", &biogears::SESubstanceDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceDataRequest::Unload, py::const_))
    .def("HashCode", py::overload_cast<>(&biogears::SESubstanceDataRequest::HashCode, py::const_))
    .def("GetCompartment", &biogears::SESubstanceDataRequest::GetCompartment)

    .def("SetCompartment", py::overload_cast<const char*>(&biogears::SESubstanceDataRequest::SetCompartment))
    .def("SetCompartment", py::overload_cast<const std::string&>(&biogears::SESubstanceDataRequest::SetCompartment))
    .def("HasCompartment", &biogears::SESubstanceDataRequest::HasCompartment)
    .def("InvalidateCompartment", &biogears::SESubstanceDataRequest::InvalidateCompartment)

    .def("GetSubstance", &biogears::SESubstanceDataRequest::GetSubstance)
    .def("SetSubstance", &biogears::SESubstanceDataRequest::SetSubstance)

    .def("HasSubstance", &biogears::SESubstanceDataRequest::HasSubstance)
    .def("InvalidateSubstance", &biogears::SESubstanceDataRequest::InvalidateSubstance)

    .def("Set", py::overload_cast<const biogears::SESubstance&, const char*, const biogears::CCompoundUnit&>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const std::string&, const biogears::CCompoundUnit&>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const char*, const char*>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const std::string&, const std::string&>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const char*, const char*, const biogears::CCompoundUnit&>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const std::string&, const std::string&, const biogears::CCompoundUnit&>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const char*, const char*, const char*>(&biogears::SESubstanceDataRequest::Set))
    .def("Set", py::overload_cast<const biogears::SESubstance&, const std::string&, const std::string&, const std::string&>(&biogears::SESubstanceDataRequest::Set));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}