#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEDiabetesType2, m)
{

  py::class_<biogears::SEDiabetesType2, biogears::SEPatientCondition>(m, "SEDiabetesType2")
    .def(py::init<>())
    .def("Clear", &biogears::SEDiabetesType2::Clear)
    .def("IsValid", &biogears::SEDiabetesType2::IsValid)
    .def("Load", &biogears::SEDiabetesType2::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEDiabetesType2::Unload, py::const_))
    .def("GetName", &biogears::SEDiabetesType2::GetName)
    .def("GetName_cStr", &biogears::SEDiabetesType2::GetName_cStr)
    .def("HasInsulinProductionSeverity", &biogears::SEDiabetesType2::HasInsulinProductionSeverity)
    .def("GetInsulinProductionSeverity", &biogears::SEDiabetesType2::GetInsulinProductionSeverity)
    .def("HasInsulinResistanceSeverity", &biogears::SEDiabetesType2::HasInsulinProductionSeverity)
    .def("GetInsulinResistanceSeverity", &biogears::SEDiabetesType2::GetInsulinProductionSeverity)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEDiabetesType2::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}