#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/schema/cdm/Substance.hxx>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESubstanceConcentration, m)
{

  py::class_<biogears::SESubstanceConcentration, biogears::Loggable>(m, "SESubstanceConcentration")
    .def(py::init<biogears::SESubstance&>())
    .def(py::init<biogears::SESubstance&, biogears::SEScalarMassPerVolume>())
    .def(py::init<biogears::SESubstance&, double, biogears::MassPerVolumeUnit&>())

    .def("Clear", &biogears::SESubstanceConcentration::Clear)
    .def("Load", &biogears::SESubstanceConcentration::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceConcentration::Unload, py::const_))

    .def("HasConcentration", &biogears::SESubstanceConcentration::HasConcentration)

    .def("GetConcentration", py::overload_cast<>(&biogears::SESubstanceConcentration::GetConcentration))
    .def("GetConcentration", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SESubstanceConcentration::GetConcentration,py::const_))
    .def("GetSubstance", &biogears::SESubstanceConcentration::GetSubstance);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}