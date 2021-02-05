#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESubstancePharmacokinetics, m)
{

  py::class_<biogears::SESubstancePharmacokinetics, biogears::Loggable>(m, "SESubstancePharmacokinetics")
    .def(py::init<biogears::Logger*>())
    
    .def("Clear", &biogears::SESubstancePharmacokinetics::Clear)
    .def("IsValid", &biogears::SESubstancePharmacokinetics::IsValid)

    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstancePharmacokinetics::GetScalar))
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstancePharmacokinetics::GetScalar))

    .def("Load", &biogears::SESubstancePharmacokinetics::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstancePharmacokinetics::Unload,py::const_))

    .def("HasPhysicochemicals", &biogears::SESubstancePharmacokinetics::HasPhysicochemicals)
    .def("GetPhysicochemicals", py::overload_cast<>(&biogears::SESubstancePharmacokinetics::GetPhysicochemicals))
    .def("GetPhysicochemicals", py::overload_cast<>(&biogears::SESubstancePharmacokinetics::GetPhysicochemicals,py::const_))
    
    .def("HasTissueKinetics", py::overload_cast<>(&biogears::SESubstancePharmacokinetics::HasTissueKinetics,py::const_))
    .def("HasTissueKinetics", py::overload_cast<const char*>(&biogears::SESubstancePharmacokinetics::HasTissueKinetics,py::const_))
    .def("HasTissueKinetics", py::overload_cast<const std::string&>(&biogears::SESubstancePharmacokinetics::HasTissueKinetics,py::const_))
    .def("GetTissueKinetics", py::overload_cast<const char*>(&biogears::SESubstancePharmacokinetics::GetTissueKinetics))
    .def("GetTissueKinetics", py::overload_cast<const std::string&>(&biogears::SESubstancePharmacokinetics::GetTissueKinetics))
    .def("GetTissueKinetics", py::overload_cast<const char*>(&biogears::SESubstancePharmacokinetics::GetTissueKinetics,py::const_))
    .def("GetTissueKinetics", py::overload_cast<const std::string&>(&biogears::SESubstancePharmacokinetics::GetTissueKinetics,py::const_))
    
    .def("RemoveTissueKinetics", py::overload_cast<const char*>(&biogears::SESubstancePharmacokinetics::RemoveTissueKinetics))
    .def("RemoveTissueKinetics", py::overload_cast<const std::string&>(&biogears::SESubstancePharmacokinetics::RemoveTissueKinetics));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}