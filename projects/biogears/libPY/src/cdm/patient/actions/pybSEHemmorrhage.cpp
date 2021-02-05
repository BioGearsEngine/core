#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEHemorrhage, m)
{

  py::class_<biogears::SEHemorrhage>(m, "SEHemorrhage")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEHemorrhage::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEHemorrhage::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEHemorrhage::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEHemorrhage::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEHemorrhage::IsActive, py::const_))
    .def("Load", &biogears::SEHemorrhage::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEHemorrhage::Unload, py::const_))
    .def("GetCompartment_cStr", &biogears::SEHemorrhage::GetCompartment_cStr)
    .def("GetCompartment", &biogears::SEHemorrhage::GetCompartment)
    .def("HasCompartment", &biogears::SEHemorrhage::HasCompartment)
    .def("SetCompartment", py::overload_cast<const char*>(&biogears::SEHemorrhage::SetCompartment))
    .def("SetCompartment", py::overload_cast<const std::string&>(&biogears::SEHemorrhage::SetCompartment))
    .def("InvalidateCompartment", &biogears::SEHemorrhage::InvalidateCompartment)
    .def("HasMCIS", &biogears::SEHemorrhage::HasMCIS)
    .def("GetMCIS", &biogears::SEHemorrhage::GetMCIS)
    .def("SetMCIS", &biogears::SEHemorrhage::SetMCIS)
    .def("HasInitialRate", &biogears::SEHemorrhage::HasInitialRate)
    .def("GetInitialRate", &biogears::SEHemorrhage::GetInitialRate)
    .def("HasBleedResistance", &biogears::SEHemorrhage::HasBleedResistance)
    .def("GetBleedResistance", &biogears::SEHemorrhage::GetBleedResistance)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEHemorrhage::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}