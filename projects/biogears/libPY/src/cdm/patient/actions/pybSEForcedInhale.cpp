#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEForcedInhale.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEForcedInhale(py::module_ &m)
{

  py::class_<biogears::SEForcedInhale>(m, "SEForcedInhale")
    // .def(py::init<>())
    // .def("TypeTag",&biogears::SEForcedExhale::TypeTag)
    // .def("classname",py::overload_cast<>(&biogears::SEForcedExhale::classname,py::const_))
    .def("Clear", &biogears::SEForcedInhale::Clear)
    .def("IsValid", &biogears::SEForcedInhale::IsValid)
    .def("IsActive", &biogears::SEForcedInhale::IsActive)
    .def("Load", &biogears::SEForcedInhale::Load)
    // .def("Unload",&biogears::SEForcedExhale::Unload)
    .def("HasInspiratoryCapacityFraction", &biogears::SEForcedInhale::HasInspiratoryCapacityFraction)
    .def("GetInspiratoryCapacityFraction", &biogears::SEForcedInhale::GetInspiratoryCapacityFraction)
    .def("HasPeriod", &biogears::SEForcedInhale::HasPeriod)
    .def("GetPeriod", &biogears::SEForcedInhale::GetPeriod)

    .def("ToString", &biogears::SEForcedInhale::ToString);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
