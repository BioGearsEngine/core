#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>    
#include <biogears/container/Tree.tci.h>


namespace py = pybind11;

void define_pybSEEnergySystem(py::module_ &m)
{
  

  py::class_<biogears::SEEnergySystem, biogears::SESystem>(m, "SEEnergySystem")
    .def(py::init<biogears::Logger*>())
    .def("GetTotalMetabolicRate", py::overload_cast<>(&biogears::SEEnergySystem::GetTotalMetabolicRate))
    .def("GetTotalMetabolicRate", py::overload_cast<const biogears::PowerUnit&>(&biogears::SEEnergySystem::GetTotalMetabolicRate, py::const_))
    .def("GetCoreTemperature", py::overload_cast<>(&biogears::SEEnergySystem::GetCoreTemperature))
    .def("GetCoreTemperature", py::overload_cast<const biogears::TemperatureUnit&>(&biogears::SEEnergySystem::GetCoreTemperature, py::const_))
    .def("GetAchievedExerciseLevel", py::overload_cast<>(&biogears::SEEnergySystem::GetAchievedExerciseLevel))
    .def("GetAchievedExerciseLevel", py::overload_cast<>(&biogears::SEEnergySystem::GetAchievedExerciseLevel, py::const_))
    .def("GetFatigueLevel", py::overload_cast<>(&biogears::SEEnergySystem::GetFatigueLevel))
    .def("GetFatigueLevel", py::overload_cast<>(&biogears::SEEnergySystem::GetFatigueLevel, py::const_))
    .def("GetTotalWorkRateLevel", py::overload_cast<>(&biogears::SEEnergySystem::GetTotalWorkRateLevel))
    .def("GetTotalWorkRateLevel", py::overload_cast<>(&biogears::SEEnergySystem::GetTotalWorkRateLevel, py::const_))
    .def("GetChlorideLostToSweat", py::overload_cast<>(&biogears::SEEnergySystem::GetChlorideLostToSweat))
    .def("GetChlorideLostToSweat", py::overload_cast<const biogears::MassUnit&>(&biogears::SEEnergySystem::GetChlorideLostToSweat, py::const_))
    .def("GetCreatinineProductionRate", py::overload_cast<>(&biogears::SEEnergySystem::GetCreatinineProductionRate))
    .def("GetCreatinineProductionRate", py::overload_cast<const biogears::AmountPerTimeUnit&>(&biogears::SEEnergySystem::GetCreatinineProductionRate, py::const_))
    .def("GetEnergyDeficit", py::overload_cast<>(&biogears::SEEnergySystem::GetEnergyDeficit))
    .def("GetEnergyDeficit", py::overload_cast<const biogears::PowerUnit&>(&biogears::SEEnergySystem::GetEnergyDeficit, py::const_))
    .def("GetExerciseEnergyDemand", py::overload_cast<>(&biogears::SEEnergySystem::GetExerciseEnergyDemand))
    .def("GetExerciseEnergyDemand", py::overload_cast<const biogears::PowerUnit&>(&biogears::SEEnergySystem::GetExerciseEnergyDemand, py::const_))
    .def("GetExerciseMeanArterialPressureDelta", py::overload_cast<>(&biogears::SEEnergySystem::GetExerciseMeanArterialPressureDelta))
    .def("GetExerciseMeanArterialPressureDelta", py::overload_cast<const biogears::PressureUnit&>(&biogears::SEEnergySystem::GetExerciseMeanArterialPressureDelta, py::const_))
    .def("GetLactateProductionRate", py::overload_cast<>(&biogears::SEEnergySystem::GetLactateProductionRate))
    .def("GetLactateProductionRate", py::overload_cast<const biogears::AmountPerTimeUnit&>(&biogears::SEEnergySystem::GetLactateProductionRate, py::const_))
    .def("GetPotassiumLostToSweat", py::overload_cast<>(&biogears::SEEnergySystem::GetPotassiumLostToSweat))
    .def("GetPotassiumLostToSweat", py::overload_cast<const biogears::MassUnit&>(&biogears::SEEnergySystem::GetPotassiumLostToSweat, py::const_))
    .def("GetSkinTemperature", py::overload_cast<>(&biogears::SEEnergySystem::GetSkinTemperature))
    .def("GetSkinTemperature", py::overload_cast<const biogears::TemperatureUnit&>(&biogears::SEEnergySystem::GetSkinTemperature, py::const_))
    .def("GetSodiumLostToSweat", py::overload_cast<>(&biogears::SEEnergySystem::GetSodiumLostToSweat))
    .def("GetSodiumLostToSweat", py::overload_cast<const biogears::MassUnit&>(&biogears::SEEnergySystem::GetSodiumLostToSweat, py::const_))
   .def("GetSweatRate", py::overload_cast<>(&biogears::SEEnergySystem::GetSweatRate))
    .def("GetSweatRate", py::overload_cast<const biogears::MassPerTimeUnit&>(&biogears::SEEnergySystem::GetSweatRate, py::const_));

#ifdef VERSION_INFO
      m.attr("__version__")
    = VERSION_INFO;
#else
      m.attr("__version__")
    = "dev";
#endif
}
