#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEChronicAnemia(py::module_ &m)
{

  py::class_<biogears::SEChronicAnemia, biogears::SEPatientCondition>(m, "SEChronicAnemia")
    .def(py::init<>())
    // .def("TypeTag",&biogears::SEChronicAnemia::TypeTag)
    // .def("classname",py::overload_cast<>(&biogears::SEChronicAnemia::classname,py::const_))
    // .def("Reset",&biogears::SEChronicAnemia::Reset)
    .def("Clear", py::overload_cast<>(&biogears::SEChronicAnemia::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEChronicAnemia::IsValid, py::const_))
    .def("Load", &biogears::SEChronicAnemia::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChronicAnemia::Unload, py::const_))
    .def("GetName", py::overload_cast<>(&biogears::SEChronicAnemia::GetName, py::const_))
    .def("GetName_cStr", py::overload_cast<>(&biogears::SEChronicAnemia::GetName_cStr, py::const_))
    .def("HasReductionFactor", &biogears::SEChronicAnemia::HasReductionFactor)
    .def("GetReductionFactor", &biogears::SEChronicAnemia::GetReductionFactor)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEChronicAnemia::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
