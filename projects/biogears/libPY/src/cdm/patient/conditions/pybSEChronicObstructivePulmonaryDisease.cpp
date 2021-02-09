#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEChronicObstructivePulmonaryDisease(py::module_ &m)
{

  py::class_<biogears::SEChronicObstructivePulmonaryDisease, biogears::SEPatientCondition>(m, "SEChronicObstructivePulmonaryDisease")
    .def(py::init<>())
    // .def("TypeTag",&biogears::SEChronicAnemia::TypeTag)
    // .def("classname",py::overload_cast<>(&biogears::SEChronicAnemia::classname,py::const_))
    // .def("Reset",&biogears::SEChronicAnemia::Reset)
    .def("Clear", &biogears::SEChronicObstructivePulmonaryDisease::Clear)
    .def("IsValid", &biogears::SEChronicObstructivePulmonaryDisease::IsValid)
    .def("Load", &biogears::SEChronicObstructivePulmonaryDisease::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChronicObstructivePulmonaryDisease::Unload, py::const_))
    .def("GetName", &biogears::SEChronicObstructivePulmonaryDisease::GetName)
    .def("GetName_cStr", &biogears::SEChronicObstructivePulmonaryDisease::GetName_cStr)
    .def("HasBronchitisSeverity", &biogears::SEChronicObstructivePulmonaryDisease::HasBronchitisSeverity)
    .def("GetBronchitisSeverity", &biogears::SEChronicObstructivePulmonaryDisease::GetBronchitisSeverity)
    .def("HasEmphysemaSeverity", &biogears::SEChronicObstructivePulmonaryDisease::HasEmphysemaSeverity)
    .def("GetEmphysemaSeverity", &biogears::SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverity)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEChronicObstructivePulmonaryDisease::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
