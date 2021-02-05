#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEDataRequestManager, m)
{

  py::class_<biogears::SEDecimalFormat>(m, "SEDecimalFormat");
  // py::class_<biogears::SEPhysiologyDataRequest>(m,"SEPhysiologyDataRequest");

  py::class_<biogears::SEDataRequestManager>(m, "SEDataRequestManager")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SEDataRequestManager::Clear)
    .def("Load", &biogears::SEDataRequestManager::Load)
    // .def("Unload",&biogears::SEDataRequestManager::Unload)
    .def("HasResultsFilename", &biogears::SEDataRequestManager::HasResultsFilename)
    .def("GetResultsFilename", &biogears::SEDataRequestManager::GetResultsFilename)
    .def("GetResultsFilename_cStr", &biogears::SEDataRequestManager::GetResultsFilename_cStr)
    .def("SetResultsFilename", py::overload_cast<const char*>(&biogears::SEDataRequestManager::SetResultsFilename))
    .def("SetResultsFilename", py::overload_cast<const std::string&>(&biogears::SEDataRequestManager::SetResultsFilename))
    .def("HasWorkingDir", &biogears::SEDataRequestManager::HasWorkingDir)
    .def("GetWorkingDir", &biogears::SEDataRequestManager::GetWorkingDir)
    .def("GetWorkingDir_cStr", &biogears::SEDataRequestManager::GetWorkingDir_cStr)
    .def("SetWorkingDir", py::overload_cast<const char*>(&biogears::SEDataRequestManager::SetWorkingDir))
    .def("SetWorkingDir", py::overload_cast<const std::string&>(&biogears::SEDataRequestManager::SetWorkingDir))
    .def("GetResovedFilePath", &biogears::SEDataRequestManager::GetResovedFilePath)
    .def("GetSamplesPerSecond", &biogears::SEDataRequestManager::GetSamplesPerSecond)
    .def("SetSamplesPerSecond", &biogears::SEDataRequestManager::SetSamplesPerSecond)
    .def("HasDataRequests", &biogears::SEDataRequestManager::HasDataRequests)
    .def("GetDataRequests", &biogears::SEDataRequestManager::GetDataRequests)
    .def("HasDefaultDecimalFormatting", &biogears::SEDataRequestManager::HasDefaultDecimalFormatting)
    .def("GetDefaultDecimalFormatting", &biogears::SEDataRequestManager::GetDefaultDecimalFormatting)
    .def("RemoveDefaultDecimalFormatting", &biogears::SEDataRequestManager::RemoveDefaultDecimalFormatting)
    .def("HasOverrideDecimalFormatting", &biogears::SEDataRequestManager::HasOverrideDecimalFormatting)
    .def("GetOverrideDecimalFormatting", &biogears::SEDataRequestManager::GetOverrideDecimalFormatting)
    .def("RemoveOverrideDecimalFormatting", &biogears::SEDataRequestManager::RemoveOverrideDecimalFormatting)
    .def("CreateEnvironmentDataRequest", &biogears::SEDataRequestManager::CreateEnvironmentDataRequest)
    .def("CreateEquipmentDataRequest", &biogears::SEDataRequestManager::CreateEquipmentDataRequest)
    .def("CreatePatientDataRequest", &biogears::SEDataRequestManager::CreatePatientDataRequest)
    // .def("CreatePhysiologyDataRequest",&biogears::SEDataRequestManager::CreatePhysiologyDataRequest, py::arg("dfault").none(true))
    .def("CreatePhysiologyDataRequest", &biogears::SEDataRequestManager::CreatePhysiologyDataRequest, py::arg("dfault") = nullptr, py::return_value_policy::reference)
    .def("CreateSubstanceDataRequest", &biogears::SEDataRequestManager::CreateSubstanceDataRequest)
    .def("CreateGasCompartmentDataRequest", &biogears::SEDataRequestManager::CreateGasCompartmentDataRequest)
    .def("CreateLiquidCompartmentDataRequest", &biogears::SEDataRequestManager::CreateLiquidCompartmentDataRequest)
    .def("CreateThermalCompartmentDataRequest", &biogears::SEDataRequestManager::CreateThermalCompartmentDataRequest)
    .def("CreateTissueCompartmentDataRequest", &biogears::SEDataRequestManager::CreateTissueCompartmentDataRequest)
    .def("CreateFromBind", &biogears::SEDataRequestManager::CreateFromBind);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}