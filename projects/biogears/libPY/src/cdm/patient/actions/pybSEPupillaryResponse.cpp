#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEPupillaryResponse(py::module_ &m)
{

  py::class_<biogears::SEPupillaryResponse>(m, "SEPupillaryResponse")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SEPupillaryResponse::Clear)
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SEPupillaryResponse::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SEPupillaryResponse::GetScalar))
    .def("Load", &biogears::SEPupillaryResponse::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPupillaryResponse::Unload, py::const_))
    .def("HasReactivityModifier", &biogears::SEPupillaryResponse::HasReactivityModifier)
    .def("GetReactivityModifier", py::overload_cast<>(&biogears::SEPupillaryResponse::GetReactivityModifier))
    .def("GetReactivityModifier", py::overload_cast<>(&biogears::SEPupillaryResponse::GetReactivityModifier, py::const_))
    .def("HasShapeModifier", &biogears::SEPupillaryResponse::HasShapeModifier)
    .def("GetShapeModifier", py::overload_cast<>(&biogears::SEPupillaryResponse::GetShapeModifier))
    .def("GetSizeModifier", py::overload_cast<>(&biogears::SEPupillaryResponse::GetSizeModifier, py::const_));

  // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEPupillaryResponse::ToString,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
