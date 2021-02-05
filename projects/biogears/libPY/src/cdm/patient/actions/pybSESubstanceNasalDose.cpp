#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESubstanceNasalDose, m)
{

  py::class_<biogears::SESubstanceNasalDose, biogears::SESubstanceAdministration>(m, "SESubstanceNasalDose")
    .def(py::init<const biogears::SESubstance&>())
    .def("Clear", &biogears::SESubstanceNasalDose::Clear)
    .def("IsValid", &biogears::SESubstanceNasalDose::IsValid)
    .def("IsActive", &biogears::SESubstanceNasalDose::IsActive)
    .def("Load", &biogears::SESubstanceNasalDose::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceNasalDose::Unload, py::const_))
    .def("HasDose", &biogears::SESubstanceNasalDose::HasDose)
    .def("GetDose", &biogears::SESubstanceNasalDose::GetDose)
    .def("GetSubstance", &biogears::SESubstanceNasalDose::GetSubstance)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SESubstanceNasalDose::ToString, py::const_));

  py::class_<biogears::SENasalState>(m, "SENasalState")
    .def(py::init<const biogears::SESubstance&>())
    .def("Clear", &biogears::SENasalState::Clear)
    .def("Load", &biogears::SENasalState::Load)
    .def("Unload", py::overload_cast<>(&biogears::SENasalState::Unload, py::const_))
    .def("Initialize", &biogears::SENasalState::Initialize)
    .def("GetTotalNasalDose", &biogears::SENasalState::GetTotalNasalDose)
    .def("GetVenaCavaConcentration", &biogears::SENasalState::GetVenaCavaConcentration)
    .def("GetUnreleasedNasalMasses", py::overload_cast<>(&biogears::SENasalState::GetUnreleasedNasalMasses))
    .def("GetReleasedNasalMasses", py::overload_cast<>(&biogears::SENasalState::GetReleasedNasalMasses))
    .def("GetUnreleasedNasalMasses", py::overload_cast<const biogears::MassUnit&>(&biogears::SENasalState::GetUnreleasedNasalMasses))
    .def("GetReleasedNasalMasses", py::overload_cast<const biogears::MassUnit&>(&biogears::SENasalState::GetReleasedNasalMasses))
    .def("SetUnreleasedNasalMasses", &biogears::SENasalState::SetUnreleasedNasalMasses)
    .def("SetReleasedNasalMasses", &biogears::SENasalState::SetReleasedNasalMasses);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}