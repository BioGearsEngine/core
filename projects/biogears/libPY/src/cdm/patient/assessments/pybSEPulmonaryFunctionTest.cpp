#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEPulmonaryFunctionTest, m)
{

  py::class_<biogears::SEPulmonaryFunctionTest, biogears::SEPatientAssessment>(m, "SEPulmonaryFunctionTest")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEPulmonaryFunctionTest::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEPulmonaryFunctionTest::classname, py::const_))
    .def("Reset", &biogears::SEPulmonaryFunctionTest::Reset)
    .def("Clear", &biogears::SEPulmonaryFunctionTest::Clear)
    .def("Load", &biogears::SEPulmonaryFunctionTest::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPulmonaryFunctionTest::Unload))
    .def("GetNumberOfPlotPoints", &biogears::SEPulmonaryFunctionTest::GetNumberOfPlotPoints)
    .def("SetNumberOfPlotPoints", &biogears::SEPulmonaryFunctionTest::SetNumberOfPlotPoints)
    .def("HasExpiratoryReserveVolume", &biogears::SEPulmonaryFunctionTest::HasExpiratoryReserveVolume)
    .def("GetExpiratoryReserveVolume", &biogears::SEPulmonaryFunctionTest::GetExpiratoryReserveVolume)
    .def("HasForcedVitalCapacity", &biogears::SEPulmonaryFunctionTest::HasForcedVitalCapacity)
    .def("GetForcedVitalCapacity", &biogears::SEPulmonaryFunctionTest::GetForcedVitalCapacity)
    .def("HasForcedExpiratoryVolume", &biogears::SEPulmonaryFunctionTest::HasForcedExpiratoryVolume)
    .def("GetForcedExpiratoryVolume", &biogears::SEPulmonaryFunctionTest::GetForcedExpiratoryVolume)
    .def("HasForcedExpiratoryFlow", &biogears::SEPulmonaryFunctionTest::HasForcedExpiratoryFlow)
    .def("GetForcedExpiratoryFlow", &biogears::SEPulmonaryFunctionTest::GetForcedExpiratoryFlow)
    .def("HasFunctionalResidualCapacity", &biogears::SEPulmonaryFunctionTest::HasFunctionalResidualCapacity)
    .def("GetFunctionalResidualCapacity", &biogears::SEPulmonaryFunctionTest::GetFunctionalResidualCapacity)
    .def("HasInspiratoryCapacity", &biogears::SEPulmonaryFunctionTest::HasInspiratoryCapacity)
    .def("GetInspiratoryCapacity", &biogears::SEPulmonaryFunctionTest::GetInspiratoryCapacity)
    .def("HasInspiratoryReserveVolume", &biogears::SEPulmonaryFunctionTest::HasInspiratoryReserveVolume)
    .def("GetInspiratoryReserveVolume", &biogears::SEPulmonaryFunctionTest::GetInspiratoryReserveVolume)
    .def("HasMaximumVoluntaryVentilation", &biogears::SEPulmonaryFunctionTest::HasMaximumVoluntaryVentilation)
    .def("GetMaximumVoluntaryVentilation", &biogears::SEPulmonaryFunctionTest::GetMaximumVoluntaryVentilation)
    .def("HasPeakExpiratoryFlow", &biogears::SEPulmonaryFunctionTest::HasPeakExpiratoryFlow)
    .def("GetPeakExpiratoryFlow", &biogears::SEPulmonaryFunctionTest::GetPeakExpiratoryFlow)

    .def("HasResidualVolume", &biogears::SEPulmonaryFunctionTest::HasResidualVolume)
    .def("GetResidualVolume", &biogears::SEPulmonaryFunctionTest::GetResidualVolume)
    .def("HasSlowVitalCapacity", &biogears::SEPulmonaryFunctionTest::HasSlowVitalCapacity)
    .def("GetSlowVitalCapacity", &biogears::SEPulmonaryFunctionTest::GetSlowVitalCapacity)

    .def("HasTotalLungCapacity", &biogears::SEPulmonaryFunctionTest::HasTotalLungCapacity)
    .def("GetTotalLungCapacity", &biogears::SEPulmonaryFunctionTest::GetTotalLungCapacity)
    .def("HasVitalCapacity", &biogears::SEPulmonaryFunctionTest::HasVitalCapacity)
    .def("GetVitalCapacity", &biogears::SEPulmonaryFunctionTest::GetVitalCapacity)
    .def("HasLungVolumePlot", &biogears::SEPulmonaryFunctionTest::HasLungVolumePlot)
    .def("GetLungVolumePlot", &biogears::SEPulmonaryFunctionTest::GetLungVolumePlot);

  // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SECompleteBloodCount::ToString,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}