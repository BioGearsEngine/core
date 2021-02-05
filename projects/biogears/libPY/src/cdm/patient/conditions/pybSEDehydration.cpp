#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEDehydration.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEDehydration, m)
{

  py::class_<biogears::SEDehydration, biogears::SEPatientCondition>(m, "SEDehydration")
    .def(py::init<>())
    .def("Clear", &biogears::SEDehydration::Clear)
    .def("IsValid", &biogears::SEDehydration::IsValid)
    .def("Load", &biogears::SEDehydration::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEDehydration::Unload, py::const_))
    .def("GetName", &biogears::SEDehydration::GetName)
    .def("GetName_cStr", &biogears::SEDehydration::GetName_cStr)
    .def("HasDehydrationFraction", &biogears::SEDehydration::HasDehydrationFraction)
    .def("GetDehydrationFraction", &biogears::SEDehydration::GetDehydrationFraction)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEDehydration::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}