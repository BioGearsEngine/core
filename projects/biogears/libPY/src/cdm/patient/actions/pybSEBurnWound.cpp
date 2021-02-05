#include <biogears/cdm/patient/actions/SEBurnWound.h>
// #include <biogears/cdm/utils/Logger.h>
#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEBurnWound.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEBurnWound, m)
{

  py::class_<biogears::SEBurnWound>(m, "SEBurnWound")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEBurnWound::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEBurnWound::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEBurnWound::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEBurnWound::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEBurnWound::IsActive, py::const_))
    .def("Load", &biogears::SEBurnWound::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEBurnWound::Unload, py::const_))
    .def("HasTotalBodySurfaceArea", &biogears::SEBurnWound::HasTotalBodySurfaceArea)
    .def("GetTotalBodySurfaceArea", &biogears::SEBurnWound::GetTotalBodySurfaceArea)
    .def("HasInflammation", &biogears::SEBurnWound::HasInflammation)
    .def("SetInflammation", &biogears::SEBurnWound::SetInflammation)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEBurnWound::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}