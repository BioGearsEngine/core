#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEInfection, m)
{

  py::class_<biogears::SEInfection>(m, "SEInfection")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEInfection::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEInfection::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEInfection::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEInfection::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEInfection::IsActive, py::const_))
    .def("Load", &biogears::SEInfection::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEInfection::Unload, py::const_))
    .def("GetLocation_cStr", &biogears::SEInfection::GetLocation_cStr)
    .def("GetLocation", &biogears::SEInfection::GetLocation)
    .def("HasLocation", &biogears::SEInfection::HasLocation)
    .def("SetLocation", py::overload_cast<const char*>(&biogears::SEInfection::SetLocation))
    .def("SetLocation", py::overload_cast<const std::string&>(&biogears::SEInfection::SetLocation))
    .def("InvalidateLocation", &biogears::SEInfection::InvalidateLocation)
    .def("HasSeverity", &biogears::SEInfection::HasSeverity)
    .def("GetSeverity", &biogears::SEInfection::GetSeverity)
    .def("SetSeverity", &biogears::SEInfection::SetSeverity)
    .def("InvalidateSeverity", &biogears::SEInfection::InvalidateSeverity)
    .def("HasMinimumInhibitoryConcentration", &biogears::SEInfection::HasMinimumInhibitoryConcentration)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEInfection::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}