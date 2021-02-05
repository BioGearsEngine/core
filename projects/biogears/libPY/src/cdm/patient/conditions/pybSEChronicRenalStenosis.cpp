#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEChronicRenalStenosis, m)
{

  py::class_<biogears::SEChronicRenalStenosis, biogears::SEPatientCondition>(m, "SEChronicRenalStenosis")
    .def(py::init<>())
    // .def("TypeTag",&biogears::SEChronicAnemia::TypeTag)
    // .def("classname",py::overload_cast<>(&biogears::SEChronicAnemia::classname,py::const_))
    // .def("Reset",&biogears::SEChronicAnemia::Reset)
    .def("Clear", &biogears::SEChronicRenalStenosis::Clear)
    .def("IsValid", &biogears::SEChronicRenalStenosis::IsValid)
    .def("Load", &biogears::SEChronicRenalStenosis::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChronicRenalStenosis::Unload, py::const_))
    .def("GetName", &biogears::SEChronicRenalStenosis::GetName)
    .def("GetName_cStr", &biogears::SEChronicRenalStenosis::GetName_cStr)
    .def("HasLeftKidneySeverity", &biogears::SEChronicRenalStenosis::HasLeftKidneySeverity)
    .def("GetLeftKidneySeverity", &biogears::SEChronicRenalStenosis::GetLeftKidneySeverity)

    .def("HasRightKidneySeverity", &biogears::SEChronicRenalStenosis::HasRightKidneySeverity)
    .def("GetRightKidneySeverity", &biogears::SEChronicRenalStenosis::GetRightKidneySeverity)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEChronicRenalStenosis::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}