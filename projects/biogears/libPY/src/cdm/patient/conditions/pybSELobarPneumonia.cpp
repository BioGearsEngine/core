#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSELobarPneumonia(py::module_ &m)
{

  py::class_<biogears::SELobarPneumonia, biogears::SEPatientCondition>(m, "SELobarPneumonia")
    .def(py::init<>())
    .def("Clear", &biogears::SELobarPneumonia::Clear)
    .def("IsValid", &biogears::SELobarPneumonia::IsValid)
    .def("Load", &biogears::SELobarPneumonia::Load)
    .def("Unload", py::overload_cast<>(&biogears::SELobarPneumonia::Unload, py::const_))
    .def("GetName", &biogears::SELobarPneumonia::GetName)
    .def("GetName_cStr", &biogears::SELobarPneumonia::GetName_cStr)
    .def("HasSeverity", &biogears::SELobarPneumonia::HasSeverity)
    .def("GetSeverity", &biogears::SELobarPneumonia::GetSeverity)
    .def("HasLeftLungAffected", &biogears::SELobarPneumonia::HasLeftLungAffected)
    .def("GetLeftLungAffected", &biogears::SELobarPneumonia::GetLeftLungAffected)
    .def("HasRightLungAffected", &biogears::SELobarPneumonia::HasRightLungAffected)
    .def("GetRightLungAffected", &biogears::SELobarPneumonia::GetRightLungAffected)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SELobarPneumonia::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
