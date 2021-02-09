#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEConsumeNutrients(py::module_ &m)
{

  py::class_<biogears::SEConsumeNutrients>(m, "SEConsumeNutrients")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEConsumeNutrients::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEConsumeNutrients::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEConsumeNutrients::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEConsumeNutrients::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEConsumeNutrients::IsActive, py::const_))
    .def("Load", &biogears::SEConsumeNutrients::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEConsumeNutrients::Unload, py::const_))
    .def("HasNutrition", &biogears::SEConsumeNutrients::HasNutrition)
    .def("GetNutrition", py::overload_cast<>(&biogears::SEConsumeNutrients::GetNutrition))
    .def("GetNutrition", py::overload_cast<>(&biogears::SEConsumeNutrients::GetNutrition, py::const_))
    .def("GetNutritionFile", &biogears::SEConsumeNutrients::GetNutritionFile)
    .def("GetNutritionFile_cStr", &biogears::SEConsumeNutrients::GetNutritionFile_cStr)
    .def("SetNutritionFile", py::overload_cast<const char*>(&biogears::SEConsumeNutrients::SetNutritionFile))
    .def("SetNutritionFile", py::overload_cast<const std::string&>(&biogears::SEConsumeNutrients::SetNutritionFile))
    .def("HasNutritionFile", &biogears::SEConsumeNutrients::HasNutritionFile)
    .def("InvalidateNutritionFile", &biogears::SEConsumeNutrients::InvalidateNutritionFile)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEConsumeNutrients::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
