#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSESequentialOrganFailureAssessment(py::module_ &m)
{

  py::class_<biogears::SESequentialOrganFailureAssessment, biogears::SEPatientAssessment>(m, "SESequentialOrganFailureAssessment")
    .def(py::init<>())
    .def("TypeTag", &biogears::SESequentialOrganFailureAssessment::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SESequentialOrganFailureAssessment::classname, py::const_))
    .def("Reset", &biogears::SESequentialOrganFailureAssessment::Reset)
    .def("Clear", &biogears::SESequentialOrganFailureAssessment::Clear)
    .def("Load", &biogears::SESequentialOrganFailureAssessment::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESequentialOrganFailureAssessment::Unload))
    .def("HasRespirationSOFA", &biogears::SESequentialOrganFailureAssessment::HasRespirationSOFA)
    .def("GetRespirationSOFA", &biogears::SESequentialOrganFailureAssessment::GetRespirationSOFA)
    .def("HasCoagulationSOFA", &biogears::SESequentialOrganFailureAssessment::HasCoagulationSOFA)
    .def("GetCoagulationSOFA", &biogears::SESequentialOrganFailureAssessment::GetCoagulationSOFA)
    .def("HasLiverSOFA", &biogears::SESequentialOrganFailureAssessment::HasLiverSOFA)
    .def("GetLiverSOFA", &biogears::SESequentialOrganFailureAssessment::GetLiverSOFA)
    .def("HasCardiovascularSOFA", &biogears::SESequentialOrganFailureAssessment::HasCardiovascularSOFA)
    .def("GetCardiovascularSOFA", &biogears::SESequentialOrganFailureAssessment::GetCardiovascularSOFA)
    .def("HasCentralNervousSOFA", &biogears::SESequentialOrganFailureAssessment::HasCentralNervousSOFA)
    .def("GetCentralNervousSOFA", &biogears::SESequentialOrganFailureAssessment::GetCentralNervousSOFA)
    .def("HasRenalSOFA", &biogears::SESequentialOrganFailureAssessment::HasRenalSOFA)
    .def("GetRenalSOFA", &biogears::SESequentialOrganFailureAssessment::GetRenalSOFA);

  // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SECompleteBloodCount::ToString,py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
