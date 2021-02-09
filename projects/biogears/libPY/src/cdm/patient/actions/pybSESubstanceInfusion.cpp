#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSESubstanceInfusion(py::module_ &m)
{

  py::class_<biogears::SESubstanceInfusion, biogears::SESubstanceAdministration>(m, "SESubstanceInfusion")
    .def(py::init<const biogears::SESubstance&>())
    .def("Clear", &biogears::SESubstanceInfusion::Clear)
    .def("IsValid", &biogears::SESubstanceInfusion::IsValid)
    .def("IsActive", &biogears::SESubstanceInfusion::IsActive)
    .def("Load", &biogears::SESubstanceInfusion::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceInfusion::Unload, py::const_))
    .def("HasConcentration", &biogears::SESubstanceInfusion::HasConcentration)
    .def("GetConcentration", &biogears::SESubstanceInfusion::GetConcentration)
    .def("HasRate", &biogears::SESubstanceInfusion::HasRate)
    .def("GetRate", &biogears::SESubstanceInfusion::GetRate)
    .def("GetSubstance", &biogears::SESubstanceInfusion::GetSubstance)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SESubstanceInfusion::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
