#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>          
#include <biogears/container/Tree.tci.h>


namespace py = pybind11;

PYBIND11_MODULE(pybSEGastrointestinalSystem, m)
{
  py::module_::import("pybSESystem");

  py::class_<biogears::SEGastrointestinalSystem, biogears::SESystem>(m, "SEGastrointestinalSystem")
    .def(py::init<biogears::Logger*>())
    .def("HasChymeAbsorptionRate",&biogears::SEGastrointestinalSystem::HasChymeAbsorptionRate)
    .def("GetChymeAbsorptionRate", py::overload_cast<>(&biogears::SEGastrointestinalSystem::GetChymeAbsorptionRate))
    .def("GetChymeAbsorptionRate", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SEGastrointestinalSystem::GetChymeAbsorptionRate, py::const_))
    .def("HasStomachContents",&biogears::SEGastrointestinalSystem::HasStomachContents)
    .def("GetStomachContents", py::overload_cast<>(&biogears::SEGastrointestinalSystem::GetStomachContents))
    .def("GetStomachContents", py::overload_cast<>(&biogears::SEGastrointestinalSystem::GetStomachContents, py::const_))
    .def("RemoveStomachContents",&biogears::SEGastrointestinalSystem::RemoveStomachContents)
    .def("GetDrugTransitStates", py::overload_cast<>(&biogears::SEGastrointestinalSystem::GetDrugTransitStates))
    .def("GetDrugTransitState", &biogears::SEGastrointestinalSystem::GetDrugTransitState)
    .def("NewDrugTransitState",&biogears::SEGastrointestinalSystem::NewDrugTransitState)
    .def("GetPhysiologyRequestGraph", py::overload_cast<>(&biogears::SEGastrointestinalSystem::GetPhysiologyRequestGraph, py::const_));
  


  py::class_<biogears::SEDrugTransitState>(m, "SEDrugTransitState")
    .def(py::init<const biogears::SESubstance&>())
    .def("GetLumenSolidMasses",&biogears::SEDrugTransitState::GetLumenSolidMasses)
    .def("GetLumenDissolvedMasses",&biogears::SEDrugTransitState::GetLumenDissolvedMasses)
    .def("GetEnterocyteMasses",&biogears::SEDrugTransitState::GetEnterocyteMasses)
    .def("SetLumenSolidMasses",&biogears::SEDrugTransitState::SetLumenSolidMasses)
    .def("SetLumenDissolvedMasses",&biogears::SEDrugTransitState::SetLumenDissolvedMasses)
    .def("SetEnterocyteMasses",&biogears::SEDrugTransitState::SetEnterocyteMasses)
    .def("IncrementStomachDissolvedMass",&biogears::SEDrugTransitState::IncrementStomachDissolvedMass)
    .def("IncrementStomachSolidMass",&biogears::SEDrugTransitState::IncrementStomachSolidMass)
    .def("GetTotalSolidMassInLumen",&biogears::SEDrugTransitState::GetTotalSolidMassInLumen)
    .def("GetTotalDissolvedMassInLumen",&biogears::SEDrugTransitState::GetTotalDissolvedMassInLumen)
    .def("GetTotalMassInEnterocytes",&biogears::SEDrugTransitState::GetTotalMassInEnterocytes)
    .def("GetTotalMassMetabolized",&biogears::SEDrugTransitState::GetTotalMassMetabolized)
    .def("GetTotalMassExcreted",&biogears::SEDrugTransitState::GetTotalMassExcreted);


}

