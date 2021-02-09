#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEDiabetesType1(py::module_ &m)
{

  py::class_<biogears::SEDiabetesType1, biogears::SEPatientCondition>(m, "SEDiabetesType1")
    .def(py::init<>())
    .def("Clear", &biogears::SEDiabetesType1::Clear)
    .def("IsValid", &biogears::SEDiabetesType1::IsValid)
    .def("Load", &biogears::SEDiabetesType1::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEDiabetesType1::Unload, py::const_))
    .def("GetName", &biogears::SEDiabetesType1::GetName)
    .def("GetName_cStr", &biogears::SEDiabetesType1::GetName_cStr)
    .def("HasInsulinProductionSeverity", &biogears::SEDiabetesType1::HasInsulinProductionSeverity)
    .def("GetInsulinProductionSeverity", &biogears::SEDiabetesType1::GetInsulinProductionSeverity)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEDiabetesType1::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
