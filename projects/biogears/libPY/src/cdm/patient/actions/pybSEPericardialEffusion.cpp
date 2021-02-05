#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEPericardialEffusion, m)
{

  py::class_<biogears::SEPericardialEffusion, biogears::SEPatientAction>(m, "SEPericardialEffusion")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEPericardialEffusion::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEPericardialEffusion::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEPericardialEffusion::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEPericardialEffusion::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEPericardialEffusion::IsActive, py::const_))
    .def("Load", &biogears::SEPericardialEffusion::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPericardialEffusion::Unload, py::const_))
    .def("HasEffusionRate", &biogears::SEPericardialEffusion::HasEffusionRate)
    .def("GetEffusionRate", &biogears::SEPericardialEffusion::GetEffusionRate)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEPericardialEffusion::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}