#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEMechanicalVentilation, m)
{

  py::class_<biogears::SEMechanicalVentilation, biogears::SEPatientAction>(m, "SEMechanicalVentilation")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEMechanicalVentilation::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEMechanicalVentilation::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEMechanicalVentilation::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEMechanicalVentilation::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEMechanicalVentilation::IsActive, py::const_))
    .def("Load", &biogears::SEMechanicalVentilation::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEMechanicalVentilation::Unload, py::const_))
    .def("GetState", &biogears::SEMechanicalVentilation::Load)
    .def("SetState", &biogears::SEMechanicalVentilation::SetState)
    .def("HasState", &biogears::SEMechanicalVentilation::HasState)
    .def("InvalidateState", &biogears::SEMechanicalVentilation::InvalidateState)
    .def("HasFlow", &biogears::SEMechanicalVentilation::HasFlow)
    .def("HasFlow", &biogears::SEMechanicalVentilation::HasFlow)
    .def("GetFlow", py::overload_cast<>(&biogears::SEMechanicalVentilation::GetFlow))
    .def("GetFlow", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SEMechanicalVentilation::GetFlow, py::const_))
    .def("HasPressure", &biogears::SEMechanicalVentilation::HasPressure)
    .def("GetPressure", py::overload_cast<>(&biogears::SEMechanicalVentilation::GetPressure))
    .def("GetPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SEMechanicalVentilation::GetPressure, py::const_))
    .def("HasGasFraction", py::overload_cast<>(&biogears::SEMechanicalVentilation::HasGasFraction, py::const_))
    .def("GetGasFractions", py::overload_cast<>(&biogears::SEMechanicalVentilation::GetGasFractions, py::const_))
    .def("GetGasFractions", py::overload_cast<>(&biogears::SEMechanicalVentilation::GetGasFractions))
    .def("GetGasFraction", py::overload_cast<biogears::SESubstance&>(&biogears::SEMechanicalVentilation::GetGasFraction))
    .def("GetGasFraction", py::overload_cast<const biogears::SESubstance&>(&biogears::SEMechanicalVentilation::GetGasFraction, py::const_))
    .def("RemoveGasFraction", &biogears::SEMechanicalVentilation::RemoveGasFraction)
    .def("RemoveGasFractions", &biogears::SEMechanicalVentilation::RemoveGasFractions)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEMechanicalVentilation::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}