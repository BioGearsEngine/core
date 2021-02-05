#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEConditionManager.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEConditionManager, m)
{

  py::class_<biogears::SEConditionManager, biogears::Loggable>(m, "SEConditionManager")
    .def(py::init<biogears::SESubstanceManager&>())
    .def("Clear", &biogears::SEConditionManager::Clear)
    .def("Unload", &biogears::SEConditionManager::Unload)
    .def("ProcessCondition", py::overload_cast<const biogears::SECondition&>(&biogears::SEConditionManager::ProcessCondition))
    .def("ProcessCondition", py::overload_cast<const CDM::ConditionData&>(&biogears::SEConditionManager::ProcessCondition))

    .def("HasChronicAnemia", &biogears::SEConditionManager::HasChronicAnemia)
    .def("GetChronicAnemia", &biogears::SEConditionManager::GetChronicAnemia)

    .def("HasChronicObstructivePulmonaryDisease", &biogears::SEConditionManager::HasChronicObstructivePulmonaryDisease)
    .def("GetChronicObstructivePulmonaryDisease", &biogears::SEConditionManager::GetChronicObstructivePulmonaryDisease)

    .def("HasChronicHeartFailure", &biogears::SEConditionManager::HasChronicHeartFailure)
    .def("HasChronicVentricularSystolicDysfunction", &biogears::SEConditionManager::HasChronicVentricularSystolicDysfunction)
    .def("GetChronicVentricularSystolicDysfunction", &biogears::SEConditionManager::GetChronicVentricularSystolicDysfunction)

    .def("HasChronicPericardialEffusion", &biogears::SEConditionManager::HasChronicPericardialEffusion)
    .def("GetChronicPericardialEffusion", &biogears::SEConditionManager::GetChronicPericardialEffusion)

    .def("HasChronicRenalStenosis", &biogears::SEConditionManager::HasChronicRenalStenosis)
    .def("GetChronicRenalStenosis", &biogears::SEConditionManager::GetChronicRenalStenosis)

    .def("HasStarvation", &biogears::SEConditionManager::HasStarvation)
    .def("GetStarvation", &biogears::SEConditionManager::GetStarvation)

    .def("HasDehydration", &biogears::SEConditionManager::HasDehydration)
    .def("GetDehydration", &biogears::SEConditionManager::GetDehydration)

    .def("HasDiabetesType1", &biogears::SEConditionManager::HasDiabetesType1)
    .def("GetDiabetesType1", &biogears::SEConditionManager::GetDiabetesType1)

    .def("HasDiabetesType2", &biogears::SEConditionManager::HasDiabetesType2)
    .def("GetDiabetesType2", &biogears::SEConditionManager::GetDiabetesType2)

    .def("HasImpairedAlveolarExchange", &biogears::SEConditionManager::HasImpairedAlveolarExchange)
    .def("GetImpairedAlveolarExchange", &biogears::SEConditionManager::GetImpairedAlveolarExchange)

    .def("HasLobarPneumonia", &biogears::SEConditionManager::HasLobarPneumonia)
    .def("GetLobarPneumonia", &biogears::SEConditionManager::GetLobarPneumonia)

    .def("HasInitialEnvironment", &biogears::SEConditionManager::HasInitialEnvironment)
    .def("GetInitialEnvironment", &biogears::SEConditionManager::GetInitialEnvironment);

#ifdef VERSION_INFO
  m.attr("__version__")
    = VERSION_INFO;
#else
  m.attr("__version__")
    = "dev";
#endif
}