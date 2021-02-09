#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSECompleteBloodCount(py::module_ &m)
{

  py::class_<biogears::SECompleteBloodCount, biogears::SEPatientAssessment>(m, "SECompleteBloodCount")
    .def(py::init<>())
    .def("TypeTag", &biogears::SECompleteBloodCount::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SECompleteBloodCount::classname, py::const_))
    .def("Reset", &biogears::SECompleteBloodCount::Reset)
    .def("Clear", &biogears::SECompleteBloodCount::Clear)
    .def("Load", &biogears::SECompleteBloodCount::Load)
    .def("Unload", py::overload_cast<>(&biogears::SECompleteBloodCount::Unload))
    .def("HasHematocrit", &biogears::SECompleteBloodCount::HasHematocrit)
    .def("GetHematocrit", &biogears::SECompleteBloodCount::GetHematocrit)
    .def("HasHemoglobin", &biogears::SECompleteBloodCount::HasHemoglobin)
    .def("GetHemoglobin", &biogears::SECompleteBloodCount::GetHemoglobin)
    .def("HasPlateletCount", &biogears::SECompleteBloodCount::HasPlateletCount)
    .def("GetPlateletCount", &biogears::SECompleteBloodCount::GetPlateletCount)
    .def("HasMeanCorpuscularHemoglobin", &biogears::SECompleteBloodCount::HasMeanCorpuscularHemoglobin)
    .def("GetMeanCorpuscularHemoglobin", &biogears::SECompleteBloodCount::GetMeanCorpuscularHemoglobin)
    .def("HasMeanCorpuscularHemoglobinConcentration", &biogears::SECompleteBloodCount::HasMeanCorpuscularHemoglobinConcentration)
    .def("GetMeanCorpuscularHemoglobinConcentration", &biogears::SECompleteBloodCount::GetMeanCorpuscularHemoglobinConcentration)
    .def("HasMeanCorpuscularVolume", &biogears::SECompleteBloodCount::HasMeanCorpuscularVolume)
    .def("GetMeanCorpuscularVolume", &biogears::SECompleteBloodCount::GetMeanCorpuscularVolume)
    .def("HasRedBloodCellCount", &biogears::SECompleteBloodCount::HasRedBloodCellCount)
    .def("GetRedBloodCellCount", &biogears::SECompleteBloodCount::GetRedBloodCellCount)
    .def("HasWhiteBloodCellCount", &biogears::SECompleteBloodCount::HasWhiteBloodCellCount)
    .def("GetWhiteBloodCellCount", &biogears::SECompleteBloodCount::GetWhiteBloodCellCount);

  // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SECompleteBloodCount::ToString,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
