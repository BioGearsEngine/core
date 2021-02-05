#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEComprehensiveMetabolicPanel, m)
{

  py::class_<biogears::SEComprehensiveMetabolicPanel, biogears::SEPatientAssessment>(m, "SEComprehensiveMetabolicPanel")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEComprehensiveMetabolicPanel::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEComprehensiveMetabolicPanel::classname, py::const_))
    .def("Reset", &biogears::SEComprehensiveMetabolicPanel::Reset)
    .def("Clear", &biogears::SEComprehensiveMetabolicPanel::Clear)
    .def("Load", &biogears::SEComprehensiveMetabolicPanel::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEComprehensiveMetabolicPanel::Unload))
    .def("HasAlbumin", &biogears::SEComprehensiveMetabolicPanel::HasAlbumin)
    .def("GetAlbumin", &biogears::SEComprehensiveMetabolicPanel::GetAlbumin)
    .def("HasALP", &biogears::SEComprehensiveMetabolicPanel::HasALP)
    .def("GetALP", &biogears::SEComprehensiveMetabolicPanel::GetALP)
    .def("HasALT", &biogears::SEComprehensiveMetabolicPanel::HasALT)
    .def("GetALT", &biogears::SEComprehensiveMetabolicPanel::GetALT)
    .def("HasAST", &biogears::SEComprehensiveMetabolicPanel::HasAST)
    .def("GetAST", &biogears::SEComprehensiveMetabolicPanel::GetAST)
    .def("HasBUN", &biogears::SEComprehensiveMetabolicPanel::HasBUN)
    .def("GetBUN", &biogears::SEComprehensiveMetabolicPanel::GetBUN)
    .def("HasCalcium", &biogears::SEComprehensiveMetabolicPanel::HasCalcium)
    .def("GetCalcium", &biogears::SEComprehensiveMetabolicPanel::GetCalcium)
    .def("HasChloride", &biogears::SEComprehensiveMetabolicPanel::HasChloride)
    .def("GetChloride", &biogears::SEComprehensiveMetabolicPanel::GetChloride)
    .def("HasCO2", &biogears::SEComprehensiveMetabolicPanel::HasCO2)
    .def("GetCO2", &biogears::SEComprehensiveMetabolicPanel::GetCO2)
    .def("HasCreatinine", &biogears::SEComprehensiveMetabolicPanel::HasCreatinine)
    .def("GetCreatinine", &biogears::SEComprehensiveMetabolicPanel::GetCreatinine)
    .def("HasGlucose", &biogears::SEComprehensiveMetabolicPanel::HasGlucose)
    .def("GetGlucose", &biogears::SEComprehensiveMetabolicPanel::GetGlucose)

    .def("HasPotassium", &biogears::SEComprehensiveMetabolicPanel::HasPotassium)
    .def("GetPotassium", &biogears::SEComprehensiveMetabolicPanel::GetPotassium)
    .def("HasSodium", &biogears::SEComprehensiveMetabolicPanel::HasSodium)
    .def("GetSodium", &biogears::SEComprehensiveMetabolicPanel::GetSodium)

    .def("HasTotalBilirubin", &biogears::SEComprehensiveMetabolicPanel::HasTotalBilirubin)
    .def("GetTotalBilirubin", &biogears::SEComprehensiveMetabolicPanel::GetTotalBilirubin)
    .def("HasTotalProtein", &biogears::SEComprehensiveMetabolicPanel::HasTotalProtein)
    .def("GetTotalProtein", &biogears::SEComprehensiveMetabolicPanel::GetTotalProtein);

  // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SECompleteBloodCount::ToString,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}