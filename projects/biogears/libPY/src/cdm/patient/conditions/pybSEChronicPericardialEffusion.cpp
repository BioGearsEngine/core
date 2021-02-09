#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEChronicPericardialEffusion(py::module_ &m)
{

  py::class_<biogears::SEChronicPericardialEffusion, biogears::SEPatientCondition>(m, "SEChronicPericardialEffusion")
    .def(py::init<>())
    // .def("TypeTag",&biogears::SEChronicAnemia::TypeTag)
    // .def("classname",py::overload_cast<>(&biogears::SEChronicAnemia::classname,py::const_))
    // .def("Reset",&biogears::SEChronicAnemia::Reset)
    .def("Clear", &biogears::SEChronicPericardialEffusion::Clear)
    .def("IsValid", &biogears::SEChronicPericardialEffusion::IsValid)
    .def("Load", &biogears::SEChronicPericardialEffusion::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChronicPericardialEffusion::Unload, py::const_))
    .def("GetName", &biogears::SEChronicPericardialEffusion::GetName)
    .def("GetName_cStr", &biogears::SEChronicPericardialEffusion::GetName_cStr)
    .def("HasAccumulatedVolume", &biogears::SEChronicPericardialEffusion::HasAccumulatedVolume)
    .def("GetAccumulatedVolume", &biogears::SEChronicPericardialEffusion::GetAccumulatedVolume)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEChronicPericardialEffusion::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
