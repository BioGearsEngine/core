#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESubstanceCompoundInfusion, m)
{

  py::class_<biogears::SESubstanceCompoundInfusion, biogears::SESubstanceAdministration>(m, "SESubstanceCompoundInfusion")
    .def(py::init<const biogears::SESubstanceCompoundInfusion&>())
    .def("Clear", &biogears::SESubstanceCompoundInfusion::Clear)
    .def("IsValid", &biogears::SESubstanceCompoundInfusion::IsValid)
    .def("IsActive", &biogears::SESubstanceCompoundInfusion::IsActive)
    .def("Load", &biogears::SESubstanceCompoundInfusion::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceCompoundInfusion::Unload, py::const_))
    .def("HasBagVolume", &biogears::SESubstanceCompoundInfusion::HasBagVolume)
    .def("GetBagVolume", &biogears::SESubstanceCompoundInfusion::GetBagVolume)
    .def("HasRate", &biogears::SESubstanceCompoundInfusion::HasRate)
    .def("GetRate", &biogears::SESubstanceCompoundInfusion::GetRate)
    .def("GetSubstanceCompound", &biogears::SESubstanceCompoundInfusion::GetSubstanceCompound)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SESubstanceCompoundInfusion::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}