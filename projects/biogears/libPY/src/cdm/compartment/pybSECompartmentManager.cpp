#include <pybind11/pybind11.h>

#include <string>

#include <biogears/schema/cdm/Compartment.hxx>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/SECompartmentNodes.inl>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.inl>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.inl>

namespace py = pybind11;

void define_pybSECompartmentManager(py::module_& m)
{



  py::class_<biogears::SECompartmentManager, biogears::Loggable>(m, "SECompartmentManager")
    .def(py::init<biogears::SESubstanceManager&>())
    .def("Clear", &biogears::SECompartmentManager::Clear)
    .def("Load", &biogears::SECompartmentManager::Load)
    .def("Unload", py::overload_cast<>(&biogears::SECompartmentManager::Unload, py::const_))
    .def("StateChange", &biogears::SECompartmentManager::StateChange)
    // .def("UpdateLinks", py::overload_cast<>(&biogears::SECompartmentManager::UpdateLinks))
    // .def("UpdateLinks", py::overload_cast<biogears::SEGasCompartmentGraph&>(&biogears::SECompartmentManager::UpdateLinks))
    // .def("UpdateLinks", py::overload_cast<biogears::SELiquidCompartmentGraph&>(&biogears::SECompartmentManager::UpdateLinks))
    // .def("HasCompartment", py::overload_cast<CDM::enumCompartmentType::value, const char*>(&biogears::SECompartmentManager::HasCompartment, py::const_))
    // .def("HasCompartment", py::overload_cast<CDM::enumCompartmentType::value, const std::string&>(&biogears::SECompartmentManager::HasCompartment, py::const_))
    // .def("GetCompartment", py::overload_cast<CDM::enumCompartmentType::value, const char*>(&biogears::SECompartmentManager::GetCompartment))
    // .def("GetCompartment", py::overload_cast<CDM::enumCompartmentType::value, const std::string&>(&biogears::SECompartmentManager::GetCompartment))
    // .def("GetCompartment", py::overload_cast<CDM::enumCompartmentType::value, const char*>(&biogears::SECompartmentManager::GetCompartment, py::const_))
    // .def("GetCompartment", py::overload_cast<CDM::enumCompartmentType::value, const std::string&>(&biogears::SECompartmentManager::GetCompartment, py::const_));
    .def("CreateGasCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::CreateGasCompartment))
    .def("CreateGasCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::CreateGasCompartment))
    .def("HasGasCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasGasCompartment, py::const_))
    .def("HasGasCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasGasCompartment, py::const_))
    .def("GetGasCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetGasCompartment))
    .def("GetGasCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetGasCompartment))
    .def("GetGasCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetGasCompartment, py::const_))
    .def("GetGasCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetGasCompartment, py::const_))
    .def("GetGasCompartments", &biogears::SECompartmentManager::GetGasCompartments)
    .def("GetGasLeafCompartments", &biogears::SECompartmentManager::GetGasLeafCompartments)
    .def("CreateGasLink", py::overload_cast<biogears::SEGasCompartment&, biogears::SEGasCompartment&, const char*>(&biogears::SECompartmentManager::CreateGasLink))
    .def("CreateGasLink", py::overload_cast<biogears::SEGasCompartment&, biogears::SEGasCompartment&, const std::string&>(&biogears::SECompartmentManager::CreateGasLink))
    .def("DeleteGasLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteGasLink))
    .def("DeleteGasLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteGasLink))
    .def("HasGasLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasGasLink, py::const_))
    .def("HasGasLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasGasLink, py::const_))
    .def("GetGasGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetGasGraph))
    .def("GetGasGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetGasGraph))
    .def("GetGasGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetGasGraph, py::const_))
    .def("GetGasGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetGasGraph, py::const_))
    .def("GetGasGraphs", &biogears::SECompartmentManager::GetGasGraphs)
    .def("AddGasCompartmentSubstance", &biogears::SECompartmentManager::AddGasCompartmentSubstance)
    .def("GetGasCompartmentSubstances", &biogears::SECompartmentManager::GetGasCompartmentSubstances)
    .def("CreateLiquidCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::CreateLiquidCompartment))
    .def("CreateLiquidCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::CreateLiquidCompartment))
    .def("DeleteLiquidCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteLiquidCompartment))
    .def("DeleteLiquidCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteLiquidCompartment))
    .def("HasLiquidCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasLiquidCompartment, py::const_))
    .def("HasLiquidCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasLiquidCompartment, py::const_))
    .def("GetLiquidCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetLiquidCompartment))
    .def("GetLiquidCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetLiquidCompartment))
    .def("GetLiquidCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetLiquidCompartment, py::const_))
    .def("GetLiquidCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetLiquidCompartment, py::const_))
    .def("GetLiquidCompartments", &biogears::SECompartmentManager::GetLiquidCompartments)
    .def("GetLiquidLeafCompartments", &biogears::SECompartmentManager::GetLiquidLeafCompartments)
    .def("CreateLiquidLink", py::overload_cast<biogears::SELiquidCompartment&, biogears::SELiquidCompartment&, const char*>(&biogears::SECompartmentManager::CreateLiquidLink))
    .def("CreateLiquidLink", py::overload_cast<biogears::SELiquidCompartment&, biogears::SELiquidCompartment&, const std::string&>(&biogears::SECompartmentManager::CreateLiquidLink))
    .def("DeleteLiquidLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteLiquidLink))
    .def("DeleteLiquidLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteLiquidLink))
    .def("HasLiquidLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasLiquidLink, py::const_))
    .def("HasLiquidLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasLiquidLink, py::const_))
    .def("GetLiquidLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetLiquidLink))
    .def("GetLiquidLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetLiquidLink))
    .def("GetLiquidLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetLiquidLink, py::const_))
    .def("GetLiquidLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetLiquidLink, py::const_))
    .def("GetLiquidLinks", &biogears::SECompartmentManager::GetLiquidLinks)
    .def("CreateLiquidGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::CreateLiquidGraph))
    .def("CreateLiquidGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::CreateLiquidGraph))
    .def("DeleteLiquidGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteLiquidGraph))
    .def("DeleteLiquidGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteLiquidGraph))
    .def("HasLiquidGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasLiquidGraph, py::const_))
    .def("HasLiquidGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasLiquidGraph, py::const_))
    .def("GetLiquidGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetLiquidGraph))
    .def("GetLiquidGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetLiquidGraph))
    .def("GetLiquidGraph", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetLiquidGraph, py::const_))
    .def("GetLiquidGraph", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetLiquidGraph, py::const_))
    .def("GetLiquidGraphs", &biogears::SECompartmentManager::GetLiquidGraphs)
    .def("AddLiquidCompartmentSubstance", &biogears::SECompartmentManager::AddLiquidCompartmentSubstance)
    .def("GetLiquidCompartmentSubstances", &biogears::SECompartmentManager::GetLiquidCompartmentSubstances)

    .def("CreateThermalCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::CreateThermalCompartment))
    .def("CreateThermalCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::CreateThermalCompartment))
    .def("DeleteThermalCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteThermalCompartment))
    .def("DeleteThermalCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteThermalCompartment))
    .def("HasThermalCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasThermalCompartment, py::const_))
    .def("HasThermalCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasThermalCompartment, py::const_))
    .def("GetThermalCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetThermalCompartment))
    .def("GetThermalCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetThermalCompartment))
    .def("GetThermalCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetThermalCompartment, py::const_))
    .def("GetThermalCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetThermalCompartment, py::const_))

    .def("GetThermalCompartments", &biogears::SECompartmentManager::GetThermalCompartments)
    .def("GetThermalLeafCompartments", &biogears::SECompartmentManager::GetThermalLeafCompartments)
    .def("CreateThermalLink", py::overload_cast<biogears::SEThermalCompartment&, biogears::SEThermalCompartment&, const char*>(&biogears::SECompartmentManager::CreateThermalLink))
    .def("CreateThermalLink", py::overload_cast<biogears::SEThermalCompartment&, biogears::SEThermalCompartment&, const std::string&>(&biogears::SECompartmentManager::CreateThermalLink))

    .def("DeleteThermalLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteThermalLink))
    .def("DeleteThermalLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteThermalLink))
    .def("HasThermalLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasThermalLink, py::const_))
    .def("HasThermalLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasThermalLink, py::const_))
    .def("GetThermalLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetThermalLink))
    .def("GetThermalLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetThermalLink))
    .def("GetThermalLink", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetThermalLink, py::const_))
    .def("GetThermalLink", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetThermalLink, py::const_))
    .def("GetThermalLinks", &biogears::SECompartmentManager::GetThermalLinks)

    .def("HasTissueCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::HasTissueCompartment, py::const_))
    .def("HasTissueCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::HasTissueCompartment, py::const_))
    .def("DeleteTissueCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::DeleteTissueCompartment))
    .def("DeleteTissueCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::DeleteTissueCompartment))
    .def("CreateTissueCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::CreateTissueCompartment))
    .def("CreateTissueCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::CreateTissueCompartment))
    .def("GetTissueCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetTissueCompartment))
    .def("GetTissueCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetTissueCompartment))
    .def("GetTissueCompartment", py::overload_cast<const char*>(&biogears::SECompartmentManager::GetTissueCompartment, py::const_))
    .def("GetTissueCompartment", py::overload_cast<const std::string&>(&biogears::SECompartmentManager::GetTissueCompartment, py::const_))

    .def("GetTissueCompartments", &biogears::SECompartmentManager::GetTissueCompartments)
    .def("GetTissueLeafCompartments", &biogears::SECompartmentManager::GetTissueLeafCompartments);

#ifdef VERSION_INFO
  m.attr("__version__")
    = VERSION_INFO;
#else
  m.attr("__version__")
    = "dev";
#endif
}
