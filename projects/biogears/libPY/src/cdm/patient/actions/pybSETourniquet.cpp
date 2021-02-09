#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SETourniquet.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSETourniquet(py::module_ &m)
{

  py::class_<biogears::SETourniquet, biogears::SEPatientAction>(m, "SETourniquet")
    .def(py::init<>())
    .def("TypeTag", &biogears::SETourniquet::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SETourniquet::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SETourniquet::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SETourniquet::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SETourniquet::IsActive, py::const_))
    .def("Load", &biogears::SETourniquet::Load)
    .def("Unload", py::overload_cast<>(&biogears::SETourniquet::Unload, py::const_))
    .def("GetCompartment_cStr", &biogears::SETourniquet::GetCompartment_cStr)
    .def("GetCompartment", &biogears::SETourniquet::GetCompartment)
    .def("HasCompartment", &biogears::SETourniquet::HasCompartment)
    .def("SetCompartment", py::overload_cast<const char*>(&biogears::SETourniquet::SetCompartment))
    .def("SetCompartment", py::overload_cast<const std::string&>(&biogears::SETourniquet::SetCompartment))
    .def("InvalidateCompartment", &biogears::SETourniquet::InvalidateCompartment)
    .def("HasTourniquetLevel", &biogears::SETourniquet::HasTourniquetLevel)
    .def("HasTourniquetLevel", &biogears::SETourniquet::HasTourniquetLevel)
    .def("SetTourniquetLevel", &biogears::SETourniquet::SetTourniquetLevel)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SETourniquet::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
