#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEForcedExhale.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEForcedExhale, m)
{

  py::class_<biogears::SEForcedExhale>(m, "SEForcedExhale")
    // .def(py::init<>())
    // .def("TypeTag",&biogears::SEForcedExhale::TypeTag)
    // .def("classname",py::overload_cast<>(&biogears::SEForcedExhale::classname,py::const_))
    .def("Clear", &biogears::SEForcedExhale::Clear)
    .def("IsValid", &biogears::SEForcedExhale::IsValid)
    .def("IsActive", &biogears::SEForcedExhale::IsActive)
    .def("Load", &biogears::SEForcedExhale::Load)
    // .def("Unload",&biogears::SEForcedExhale::Unload)
    .def("HasExpiratoryReserveVolumeFraction", &biogears::SEForcedExhale::HasExpiratoryReserveVolumeFraction)
    .def("GetExpiratoryReserveVolumeFraction", &biogears::SEForcedExhale::GetExpiratoryReserveVolumeFraction)
    .def("HasPeriod", &biogears::SEForcedExhale::HasPeriod)
    .def("GetPeriod", &biogears::SEForcedExhale::GetPeriod)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEForcedExhale::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}