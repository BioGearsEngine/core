#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEChronicVentricularSystolicDysfunction(py::module_ &m)
{

  py::class_<biogears::SEChronicVentricularSystolicDysfunction, biogears::SEChronicHeartFailure>(m, "SEChronicVentricularSystolicDysfunction")
    .def(py::init<>())
    .def("Clear", &biogears::SEChronicVentricularSystolicDysfunction::Clear)
    .def("IsValid", &biogears::SEChronicVentricularSystolicDysfunction::IsValid)
    .def("Load", &biogears::SEChronicVentricularSystolicDysfunction::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChronicVentricularSystolicDysfunction::Unload, py::const_))
    .def("GetName", &biogears::SEChronicVentricularSystolicDysfunction::GetName)
    .def("GetName_cStr", &biogears::SEChronicVentricularSystolicDysfunction::GetName_cStr)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEChronicVentricularSystolicDysfunction::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
