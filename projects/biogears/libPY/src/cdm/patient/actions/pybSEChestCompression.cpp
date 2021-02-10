#include <pybind11/pybind11.h>

#include <string>


#include <biogears/cdm/patient/actions/SEChestCompression.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEChestCompression(py::module_ &m)
{
  py::class_<biogears::SEChestCompression>(m, "SEChestCompression")
    .def(py::init<>())
.def("TypeTag", &biogears::SEChestCompression::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEChestCompression::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEChestCompression::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEChestCompression::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEChestCompression::IsActive, py::const_))
    .def("Load", &biogears::SEChestCompression::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChestCompression::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEChestCompression::ToString, py::const_));
    ;

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
