#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/engine/Controller/BioGearsEngine.h>

namespace py = pybind11;

PYBIND11_MODULE(pybBioGearsEngineHeader, m)
{

  py::class_<biogears::SEScalarTime>(m, "SEScalarTime");
  py::class_<biogears::TimeUnit>(m, "TimeUnit", py::module_local());

  py::class_<biogears::BioGearsEngine>(m, "BioGearsEngine")
    .def(py::init<biogears::Logger*>())
    .def(py::init<const char*>())
    .def(py::init<std::string&>())
    .def(py::init<biogears::Logger*, std::string&>())
    .def(py::init<biogears::Logger*, const char*>())
    .def(py::init<std::string&, std::string&>())
    .def(py::init<char*, char*>())
    .def("LoadState", py::overload_cast<const char*, const biogears::SEScalarTime*>(&biogears::BioGearsEngine::LoadState), py::arg("file"), py::arg("simTime") = nullptr)
    .def("LoadState", py::overload_cast<const std::string&, const biogears::SEScalarTime*>(&biogears::BioGearsEngine::LoadState), py::arg("file"), py::arg("simTime") = nullptr)
    .def("LoadState", py::overload_cast<const CDM::PhysiologyEngineStateData&, const biogears::SEScalarTime*>(&biogears::BioGearsEngine::LoadState))
    .def("SaveState", py::overload_cast<const char*>(&biogears::BioGearsEngine::SaveState))
    .def("SaveState", py::overload_cast<const std::string&>(&biogears::BioGearsEngine::SaveState))
    .def("GetLogger", py::overload_cast<>(&biogears::BioGearsEngine::GetLogger), py::return_value_policy::reference)
    .def("GetEngineTrack", py::overload_cast<>(&biogears::BioGearsEngine::GetEngineTrack), py::return_value_policy::reference)
    .def("InitializeEngine", py::overload_cast<const char*, const std::vector<const biogears::SECondition*>*, const biogears::PhysiologyEngineConfiguration*>(&biogears::BioGearsEngine::InitializeEngine))
    .def("InitializeEngine", py::overload_cast<const std::string&, const std::vector<const biogears::SECondition*>*, const biogears::PhysiologyEngineConfiguration*>(&biogears::BioGearsEngine::InitializeEngine))
    .def("InitializeEngine", py::overload_cast<const biogears::SEPatient&, const std::vector<const biogears::SECondition*>*, const biogears::PhysiologyEngineConfiguration*>(&biogears::BioGearsEngine::InitializeEngine))
    .def("GetConfiguration", py::overload_cast<>(&biogears::BioGearsEngine::GetConfiguration))
    .def("GetTimeStep", py::overload_cast<const biogears::TimeUnit&>(&biogears::BioGearsEngine::GetTimeStep))
    .def("GetSimulationTime", py::overload_cast<const biogears::TimeUnit&>(&biogears::BioGearsEngine::GetSimulationTime))
    .def("ProcessAction", py::overload_cast<const biogears::SEAction&>(&biogears::BioGearsEngine::ProcessAction))
    .def("GetSubstanceManager", py::overload_cast<>(&biogears::BioGearsEngine::GetSubstanceManager))
    .def("SetEventHandler", py::overload_cast<biogears::SEEventHandler*>(&biogears::BioGearsEngine::SetEventHandler))
    .def("GetPatient", py::overload_cast<>(&biogears::BioGearsEngine::GetPatient))
    .def("GetPatientAssessment", py::overload_cast<biogears::SEPatientAssessment&>(&biogears::BioGearsEngine::GetPatientAssessment))
    .def("GetEnvironment", py::overload_cast<>(&biogears::BioGearsEngine::GetEnvironment))
    .def("GetBloodChemistrySystem", py::overload_cast<>(&biogears::BioGearsEngine::GetBloodChemistrySystem))
    .def("GetCardiovascularSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetCardiovascularSystem), pybind11::return_value_policy::reference)
    .def("GetDrugSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetDrugSystem))
    .def("GetEndocrineSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetEndocrineSystem))
    .def("GetEnergySystem", py::overload_cast<>(&biogears::BioGearsEngine::GetEnergySystem), pybind11::return_value_policy::reference)
    .def("GetGastrointestinalSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetGastrointestinalSystem))
    .def("GetHepaticSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetHepaticSystem))
    .def("GetNervousSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetNervousSystem))
    .def("GetRenalSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetRenalSystem))
    .def("GetRespiratorySystem", py::overload_cast<>(&biogears::BioGearsEngine::GetRespiratorySystem), pybind11::return_value_policy::reference)
    .def("GetTissueSystem", py::overload_cast<>(&biogears::BioGearsEngine::GetTissueSystem))
    .def("GetAnesthesiaMachine", py::overload_cast<>(&biogears::BioGearsEngine::GetAnesthesiaMachine))
    .def("GetElectroCardioGram", py::overload_cast<>(&biogears::BioGearsEngine::GetElectroCardioGram))
    .def("GetInhaler", py::overload_cast<>(&biogears::BioGearsEngine::GetInhaler))
    .def("GetCompartments", py::overload_cast<>(&biogears::BioGearsEngine::GetCompartments))
    .def("GetDataRequestGraph", &biogears::BioGearsEngine::GetDataRequestGraph)
    // .def("GetDataRequestGraph",static_cast<biogears::Tree<const char*>(biogears::BioGearsEngine::*)()>(&biogears::BioGearsEngine::GetDataRequestGraph,py::const_))
    // .def("GetDataRequestGraph",py::overload_cast<>(&biogears::BioGearsEngine::GetDataRequestGraph,py::const_))
    .def("IsAutoTracking", py::overload_cast<>(&biogears::BioGearsEngine::IsAutoTracking, py::const_))
    .def("SetAutoTrackFlag", py::overload_cast<bool>(&biogears::BioGearsEngine::SetAutoTrackFlag))
    .def("IsTrackingStabilization", py::overload_cast<>(&biogears::BioGearsEngine::IsTrackingStabilization, py::const_))
    .def("SetTrackStabilizationFlag", py::overload_cast<bool>(&biogears::BioGearsEngine::SetTrackStabilizationFlag))
    .def("AdvanceModelTime", py::overload_cast<bool>(&biogears::BioGearsEngine::AdvanceModelTime), py::arg("appendDataTrack") = false)
    .def("AdvanceModelTime", py::overload_cast<double, const biogears::TimeUnit&, bool>(&biogears::BioGearsEngine::AdvanceModelTime), py::arg("time"), py::arg("unit") = biogears::TimeUnit::s, py::arg("appendDataTrack") = false);
}