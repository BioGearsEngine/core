#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/requests/SEDataRequest.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEDataRequest, m)
{
  // py::arg("dfault") = nullptr

  py::class_<biogears::SEDataRequest>(m, "SEDataRequest")
    .def("Clear", &biogears::SEDataRequest::Clear)
    .def("Load", &biogears::SEDataRequest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEDataRequest::Unload, py::const_))
    .def("HashCode", &biogears::SEDataRequest::HashCode)
    .def("GetName", &biogears::SEDataRequest::GetName)
    .def("GetName_cStr", &biogears::SEDataRequest::GetName_cStr)
    .def("SetName", py::overload_cast<const char*>(&biogears::SEDataRequest::SetName))
    .def("SetName", py::overload_cast<const std::string&>(&biogears::SEDataRequest::SetName))
    .def("HasName", &biogears::SEDataRequest::HasName)
    .def("InvalidateName", &biogears::SEDataRequest::InvalidateName)
    .def("GetRequestedUnit", &biogears::SEDataRequest::GetRequestedUnit)
    .def("SetRequestedUnit", py::overload_cast<const char*>(&biogears::SEDataRequest::SetRequestedUnit))
    .def("SetRequestedUnit", py::overload_cast<const std::string&>(&biogears::SEDataRequest::SetRequestedUnit))
    .def("HasRequestedUnit", &biogears::SEDataRequest::HasRequestedUnit)
    .def("InvalidateRequestedUnit", &biogears::SEDataRequest::InvalidateRequestedUnit)
    .def("GetUnit", &biogears::SEDataRequest::GetUnit)
    .def("SetUnit", &biogears::SEDataRequest::SetUnit)
    .def("HasUnit", &biogears::SEDataRequest::HasUnit)
    .def("InvalidateUnit", &biogears::SEDataRequest::InvalidateUnit)
    .def("Set", py::overload_cast<const char*, const char*>(&biogears::SEDataRequest::Set), py::arg("name"), py::arg("unit") = "")
    .def("Set", py::overload_cast<const std::string&, const std::string&>(&biogears::SEDataRequest::Set), py::arg("name"), py::arg("unit") = "")
    .def("Set", py::overload_cast<const char*, const biogears::CCompoundUnit&>(&biogears::SEDataRequest::Set))
    .def("Set", py::overload_cast<const std::string&, const biogears::CCompoundUnit&>(&biogears::SEDataRequest::Set));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}