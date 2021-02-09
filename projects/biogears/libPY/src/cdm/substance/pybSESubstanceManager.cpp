#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace py = pybind11;

void define_pybSESubstanceManager(py::module_ &m)
{

  py::class_<biogears::SESubstanceManager, biogears::Loggable>(m, "SESubstanceManager")
    .def(py::init<biogears::Logger*>())
    
    .def("Clear", &biogears::SESubstanceManager::Clear)
    .def("Reset", &biogears::SESubstanceManager::Reset)
    .def("LoadSubstanceDirectory", &biogears::SESubstanceManager::LoadSubstanceDirectory)

    .def("GetSubstances", &biogears::SESubstanceManager::GetSubstances)

    .def("GetSubstance", py::overload_cast<const char*>(&biogears::SESubstanceManager::GetSubstance,py::const_))
    .def("GetSubstance", py::overload_cast<const std::string&>(&biogears::SESubstanceManager::GetSubstance,py::const_))

    .def("AddSubstance", &biogears::SESubstanceManager::AddSubstance)
    .def("IsActive", py::overload_cast<const biogears::SESubstance&>(&biogears::SESubstanceManager::IsActive,py::const_))
    .def("GetActiveSubstances", &biogears::SESubstanceManager::GetActiveSubstances)
    .def("AddActiveSubstance", &biogears::SESubstanceManager::AddActiveSubstance)
    .def("RemoveActiveSubstance", &biogears::SESubstanceManager::RemoveActiveSubstance)

    .def("RemoveActiveSubstances", py::overload_cast<const std::vector<biogears::SESubstance*>&>(&biogears::SESubstanceManager::RemoveActiveSubstances))
    .def("RemoveActiveSubstances", py::overload_cast<>(&biogears::SESubstanceManager::RemoveActiveSubstances))

    .def("GetActiveGases", &biogears::SESubstanceManager::GetActiveGases)
    .def("GetActiveLiquids", &biogears::SESubstanceManager::GetActiveLiquids)
    .def("GetActiveDrugs", &biogears::SESubstanceManager::GetActiveDrugs)

    .def("GetCompounds", &biogears::SESubstanceManager::GetCompounds)
    .def("GetCompound", py::overload_cast<const char*>(&biogears::SESubstanceManager::GetCompound,py::const_))
    .def("GetCompound", py::overload_cast<const std::string&>(&biogears::SESubstanceManager::GetCompound,py::const_))
    .def("AddCompound", &biogears::SESubstanceManager::AddCompound)

    .def("IsActive", py::overload_cast<const biogears::SESubstanceCompound&>(&biogears::SESubstanceManager::IsActive,py::const_))
    .def("GetActiveCompounds", &biogears::SESubstanceManager::GetActiveCompounds)
    .def("AddActiveCompound", &biogears::SESubstanceManager::AddActiveCompound)
    .def("RemoveActiveCompound", &biogears::SESubstanceManager::RemoveActiveCompound)
    .def("RemoveActiveCompounds", &biogears::SESubstanceManager::RemoveActiveCompounds)

    .def("ReadSubstanceFile", py::overload_cast<const char*>(&biogears::SESubstanceManager::ReadSubstanceFile))
    .def("ReadSubstanceFile", py::overload_cast<const std::string&>(&biogears::SESubstanceManager::ReadSubstanceFile));


#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
