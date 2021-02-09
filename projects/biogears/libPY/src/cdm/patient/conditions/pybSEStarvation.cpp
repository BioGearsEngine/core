#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEStarvation.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEStarvation(py::module_ &m)
{

  py::class_<biogears::SEStarvation, biogears::SEPatientCondition>(m, "SEStarvation")
    .def(py::init<>())
    .def("Clear", &biogears::SEStarvation::Clear)
    .def("IsValid", &biogears::SEStarvation::IsValid)
    .def("Load", &biogears::SEStarvation::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEStarvation::Unload, py::const_))
    .def("GetName", &biogears::SEStarvation::GetName)
    .def("GetName_cStr", &biogears::SEStarvation::GetName_cStr)
    .def("HasTimeSinceMeal", &biogears::SEStarvation::HasTimeSinceMeal)
    .def("GetTimeSinceMeal", &biogears::SEStarvation::GetTimeSinceMeal)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEStarvation::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
