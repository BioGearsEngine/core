#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEExercise(py::module_ &m)
{

  py::class_<biogears::SEExercise::SEGeneric>(m, "SEExerciseSEGeneric")
    .def(py::init<>())
    .def_readwrite("DesiredWorkRate", &biogears::SEExercise::SEGeneric::DesiredWorkRate)
    .def_readwrite("Intensity", &biogears::SEExercise::SEGeneric::Intensity);

  py::class_<biogears::SEExercise::SECycling>(m, "SEExerciseSECycling")
    .def(py::init<>())
    .def_readwrite("CadenceCycle", &biogears::SEExercise::SECycling::CadenceCycle)
    .def_readwrite("PowerCycle", &biogears::SEExercise::SECycling::PowerCycle)
    .def_readwrite("AddedWeight", &biogears::SEExercise::SECycling::AddedWeight);

  py::class_<biogears::SEExercise::SERunning>(m, "SEExerciseSERunning")
    .def(py::init<>())
    .def_readwrite("SpeedRun", &biogears::SEExercise::SERunning::SpeedRun)
    .def_readwrite("InclineRun", &biogears::SEExercise::SERunning::InclineRun)
    .def_readwrite("AddedWeight", &biogears::SEExercise::SERunning::AddedWeight);

  py::class_<biogears::SEExercise::SEStrengthTraining>(m, "SEExerciseSEStrengthTraining")
    .def(py::init<>())
    .def_readwrite("WeightStrength", &biogears::SEExercise::SEStrengthTraining::WeightStrength)
    .def_readwrite("RepsStrength", &biogears::SEExercise::SEStrengthTraining::RepsStrength);

  // py::enum_<biogears::SEExercise::ExerciseType>(ExerciseType, "ExerciseType")
  // .value("GENERIC", biogears::SEExercise::ExerciseType::GENERIC)
  // .value("CYCLING", biogears::SEExercise::ExerciseType::CYCLING)
  // .value("RUNNING", biogears::SEExercise::ExerciseType::RUNNING)
  // .value("STRENGTH_TRAINING", biogears::SEExercise::ExerciseType::STRENGTH_TRAINING)
  // .value("NONE", biogears::SEExercise::ExerciseType::NONE)

  // .export_values();



  py::class_<biogears::SEExercise>(m, "SEExercise")
    .def(py::init<>())
    .def(py::init<biogears::SEExercise::SEGeneric>())
    .def(py::init<biogears::SEExercise::SECycling>())
    .def(py::init<biogears::SEExercise::SERunning>())
    .def(py::init<biogears::SEExercise::SEStrengthTraining>())

    .def("TypeTag", &biogears::SEExercise::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEExercise::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEExercise::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEExercise::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEExercise::IsActive, py::const_))
    .def("Load", &biogears::SEExercise::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEExercise::Unload, py::const_))
    .def("HasGenericExercise", &biogears::SEExercise::HasGenericExercise)
    .def("HasCyclingExercise", &biogears::SEExercise::HasCyclingExercise)
    .def("HasRunningExercise", &biogears::SEExercise::HasRunningExercise)
    .def("HasStrengthExercise", &biogears::SEExercise::HasStrengthExercise)
    .def("GetExerciseType", py::overload_cast<>(&biogears::SEExercise::GetExerciseType, py::const_))
    .def("GetCyclingExercise", py::overload_cast<>(&biogears::SEExercise::GetCyclingExercise, py::const_))
    .def("GetRunningExercise", py::overload_cast<>(&biogears::SEExercise::GetRunningExercise, py::const_))
    .def("GetStrengthExercise", py::overload_cast<>(&biogears::SEExercise::GetStrengthExercise, py::const_))
    .def("GetExerciseType", &biogears::SEExercise::GetExerciseType)
    .def("GetCyclingExercise", py::overload_cast<>(&biogears::SEExercise::GetCyclingExercise))
    .def("GetRunningExercise", py::overload_cast<>(&biogears::SEExercise::GetRunningExercise))
    .def("GetStrengthExercise", py::overload_cast<>(&biogears::SEExercise::GetStrengthExercise))
    .def("SetGenericExercise", &biogears::SEExercise::SetGenericExercise)
    .def("SetRunningExercise", &biogears::SEExercise::SetRunningExercise)
    .def("SetCyclingExercise", &biogears::SEExercise::SetCyclingExercise)
    .def("SetStrengthExercise", &biogears::SEExercise::SetStrengthExercise)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEExercise::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
