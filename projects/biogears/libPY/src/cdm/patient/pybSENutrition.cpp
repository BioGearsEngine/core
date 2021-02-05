#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

CDM_BIND_DECL(NutritionData)

PYBIND11_MODULE(pybSENutrition, m)
{

  py::class_<biogears::SENutrition, biogears::Loggable>(m, "SENutrition")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SENutrition::Clear)
    .def("Load", py::overload_cast<const CDM::NutritionData&>(&biogears::SENutrition::Load))
    // .def("Load", py::overload_cast<biogears::NutritionData&>(&biogears::SENutrition::Load))

    .def("Unload", py::overload_cast<>(&biogears::SENutrition::Unload, py::const_))
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SENutrition::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SENutrition::GetScalar))
    .def("Load", py::overload_cast<const char*>(&biogears::SENutrition::Load))
    .def("Load", py::overload_cast<const std::string&>(&biogears::SENutrition::Load))
    .def("Increment", &biogears::SENutrition::Increment)
    .def("GetName", &biogears::SENutrition::GetName)
    .def("GetName_cStr", &biogears::SENutrition::GetName_cStr)
    .def("SetName", py::overload_cast<const char*>(&biogears::SENutrition::SetName))
    .def("SetName", py::overload_cast<const std::string&>(&biogears::SENutrition::SetName))
    .def("HasName", &biogears::SENutrition::HasName)
    .def("InvalidateName", &biogears::SENutrition::InvalidateName)
    .def("HasCarbohydrate", &biogears::SENutrition::HasCarbohydrate)
    .def("GetCarbohydrate", py::overload_cast<>(&biogears::SENutrition::GetCarbohydrate))
    .def("GetCarbohydrate", py::overload_cast<const biogears::MassUnit&>(&biogears::SENutrition::GetCarbohydrate, py::const_))
    .def("HasFat", &biogears::SENutrition::HasFat)
    .def("GetFat", py::overload_cast<>(&biogears::SENutrition::GetFat))
    .def("GetFat", py::overload_cast<const biogears::MassUnit&>(&biogears::SENutrition::GetFat, py::const_))
    .def("HasProtein", &biogears::SENutrition::HasProtein)
    .def("GetProtein", py::overload_cast<>(&biogears::SENutrition::GetProtein))
    .def("GetProtein", py::overload_cast<const biogears::MassUnit&>(&biogears::SENutrition::GetProtein, py::const_))
    .def("HasSodium", &biogears::SENutrition::HasSodium)
    .def("GetSodium", py::overload_cast<>(&biogears::SENutrition::GetSodium))
    .def("GetSodium", py::overload_cast<const biogears::MassUnit&>(&biogears::SENutrition::GetSodium, py::const_))
    .def("HasCalcium", &biogears::SENutrition::HasCalcium)
    .def("GetCalcium", py::overload_cast<>(&biogears::SENutrition::GetCalcium))
    .def("GetCalcium", py::overload_cast<const biogears::MassUnit&>(&biogears::SENutrition::GetCalcium, py::const_))
    .def("HasWater", &biogears::SENutrition::HasWater)
    .def("GetWater", py::overload_cast<>(&biogears::SENutrition::GetWater))
    .def("GetWater", py::overload_cast<const biogears::VolumeUnit&>(&biogears::SENutrition::GetWater, py::const_))
    .def("GetWeight", &biogears::SENutrition::GetWeight)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SENutrition::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}