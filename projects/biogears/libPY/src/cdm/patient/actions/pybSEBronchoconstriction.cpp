#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEBronchoconstriction, m)
{

  py::class_<biogears::SEBronchoconstriction>(m, "SEBronchoconstriction")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEBronchoconstriction::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEBronchoconstriction::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEBronchoconstriction::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEBronchoconstriction::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEBronchoconstriction::IsActive, py::const_))
    .def("Load", &biogears::SEBronchoconstriction::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEBronchoconstriction::Unload, py::const_))
    .def("HasSeverity", &biogears::SEBronchoconstriction::HasSeverity)
    .def("GetSeverity", &biogears::SEBronchoconstriction::GetSeverity)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEBronchoconstriction::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}