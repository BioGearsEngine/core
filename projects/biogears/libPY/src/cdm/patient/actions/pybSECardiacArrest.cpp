#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SECardiacArrest.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSECardiacArrest, m)
{

  py::class_<biogears::SECardiacArrest>(m, "SECardiacArrest")
    .def(py::init<>())
    .def("TypeTag", &biogears::SECardiacArrest::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SECardiacArrest::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SECardiacArrest::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SECardiacArrest::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SECardiacArrest::IsActive, py::const_))
    .def("Load", &biogears::SECardiacArrest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SECardiacArrest::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SECardiacArrest::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}