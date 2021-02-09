#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>

namespace py = pybind11;

void define_pybSESubstanceAerosolization(py::module_ &m)
{

  py::class_<biogears::SESubstanceAerosolization, biogears::Loggable>(m, "SESubstanceAerosolization")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SESubstanceAerosolization::Clear)
    .def("IsValid", &biogears::SESubstanceAerosolization::IsValid)
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstanceAerosolization::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstanceAerosolization::GetScalar))
    .def("Load", &biogears::SESubstanceAerosolization::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceAerosolization::Unload, py::const_))

    .def("HasBronchioleModifier", &biogears::SESubstanceAerosolization::HasBronchioleModifier)
    .def("GetBronchioleModifier", py::overload_cast<>(&biogears::SESubstanceAerosolization::GetBronchioleModifier))
    .def("GetBronchioleModifier", py::overload_cast<>(&biogears::SESubstanceAerosolization::GetBronchioleModifier, py::const_))

    .def("HasInflammationCoefficient", &biogears::SESubstanceAerosolization::HasInflammationCoefficient)
    .def("GetInflammationCoefficient", py::overload_cast<>(&biogears::SESubstanceAerosolization::GetInflammationCoefficient))
    .def("GetInflammationCoefficient", py::overload_cast<>(&biogears::SESubstanceAerosolization::GetInflammationCoefficient, py::const_))

    .def("HasParticulateSizeDistribution", &biogears::SESubstanceAerosolization::HasParticulateSizeDistribution)
    .def("GetParticulateSizeDistribution", py::overload_cast<>(&biogears::SESubstanceAerosolization::GetParticulateSizeDistribution))
    .def("GetParticulateSizeDistribution", py::overload_cast<>(&biogears::SESubstanceAerosolization::GetParticulateSizeDistribution, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
