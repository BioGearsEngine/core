#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEUrinalysis, m)
{

  py::class_<biogears::SEUrinalysis, biogears::SEPatientAssessment>(m, "SEUrinalysis")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEUrinalysis::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEUrinalysis::classname, py::const_))
    .def("Reset", &biogears::SEUrinalysis::Reset)
    .def("Clear", &biogears::SEUrinalysis::Clear)
    .def("Load", &biogears::SEUrinalysis::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEUrinalysis::Unload))
    .def("HasColorResult", &biogears::SEUrinalysis::HasColorResult)
    .def("GetColorResult", &biogears::SEUrinalysis::GetColorResult)
    .def("SetColorResult", &biogears::SEUrinalysis::SetColorResult)
    .def("InvalidateColorResult", &biogears::SEUrinalysis::InvalidateColorResult)
    .def("HasAppearanceResult", &biogears::SEUrinalysis::HasAppearanceResult)
    .def("GetAppearanceResult", &biogears::SEUrinalysis::GetAppearanceResult)
    .def("SetAppearanceResult", &biogears::SEUrinalysis::SetAppearanceResult)
    .def("InvalidateAppearanceResult", &biogears::SEUrinalysis::InvalidateAppearanceResult)
    .def("HasGlucoseResult", &biogears::SEUrinalysis::HasGlucoseResult)
    .def("GetGlucoseResult", &biogears::SEUrinalysis::GetGlucoseResult)
    .def("SetGlucoseResult", &biogears::SEUrinalysis::SetGlucoseResult)
    .def("InvalidateGlucoseResult", &biogears::SEUrinalysis::InvalidateGlucoseResult)
    .def("HasKetoneResult", &biogears::SEUrinalysis::HasKetoneResult)
    .def("GetKetoneResult", &biogears::SEUrinalysis::GetKetoneResult)
    .def("SetKetoneResult", &biogears::SEUrinalysis::SetKetoneResult)
    .def("InvalidateKetoneResult", &biogears::SEUrinalysis::InvalidateKetoneResult)
    .def("HasBilirubinResult", &biogears::SEUrinalysis::HasBilirubinResult)
    .def("GetBilirubinResult", &biogears::SEUrinalysis::GetBilirubinResult)
    .def("HasSpecificGravityResult", &biogears::SEUrinalysis::HasSpecificGravityResult)
    .def("GetSpecificGravityResult", &biogears::SEUrinalysis::GetSpecificGravityResult)
    .def("HasBloodResult", &biogears::SEUrinalysis::HasBloodResult)
    .def("GetBloodResult", &biogears::SEUrinalysis::GetBloodResult)
    .def("SetBloodResult", &biogears::SEUrinalysis::SetBloodResult)
    .def("InvalidateBloodResult", &biogears::SEUrinalysis::InvalidateBloodResult)
    .def("HasPHResult", &biogears::SEUrinalysis::HasPHResult)
    .def("GetPHResult", &biogears::SEUrinalysis::GetPHResult)
    .def("HasProteinResult", &biogears::SEUrinalysis::HasProteinResult)
    .def("GetProteinResult", &biogears::SEUrinalysis::GetProteinResult)

    .def("SetProteinResult", &biogears::SEUrinalysis::SetProteinResult)
    .def("InvalidateProteinResult", &biogears::SEUrinalysis::InvalidateProteinResult)
    .def("HasUrobilinogenResult", &biogears::SEUrinalysis::HasUrobilinogenResult)
    .def("GetUrobilinogenResult", &biogears::SEUrinalysis::GetUrobilinogenResult)
    .def("HasNitriteResult", &biogears::SEUrinalysis::HasNitriteResult)
    .def("GetNitriteResult", &biogears::SEUrinalysis::GetNitriteResult)
    .def("SetNitriteResult", &biogears::SEUrinalysis::SetNitriteResult)
    .def("InvalidateNitriteResult", &biogears::SEUrinalysis::InvalidateNitriteResult)
    .def("HasLeukocyteEsteraseResult", &biogears::SEUrinalysis::HasLeukocyteEsteraseResult)
    .def("GetLeukocyteEsteraseResult", &biogears::SEUrinalysis::GetLeukocyteEsteraseResult)
    .def("SetLeukocyteEsteraseResult", &biogears::SEUrinalysis::SetLeukocyteEsteraseResult)
    .def("InvalidateLeukocyteEsteraseResult", &biogears::SEUrinalysis::InvalidateLeukocyteEsteraseResult)
    .def("HasMicroscopicResult", &biogears::SEUrinalysis::HasMicroscopicResult)
    .def("GetMicroscopicResult", &biogears::SEUrinalysis::GetMicroscopicResult)
    .def("RemoveMicroscopicResult", &biogears::SEUrinalysis::RemoveMicroscopicResult);
  // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SECompleteBloodCount::ToString,py::const_))

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}