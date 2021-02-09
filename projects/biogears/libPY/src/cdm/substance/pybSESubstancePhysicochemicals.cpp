#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>

namespace py = pybind11;

void define_pybSESubstancePhysicochemicals(py::module_ &m)
{

  py::class_<biogears::SESubstancePhysicochemicals, biogears::Loggable>(m, "SESubstancePhysicochemicals")
    .def(py::init<biogears::Logger*>())

    .def("Clear", &biogears::SESubstancePhysicochemicals::Clear)
    .def("IsValid", &biogears::SESubstancePhysicochemicals::IsValid)

    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstancePhysicochemicals::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstancePhysicochemicals::GetScalar))

    .def("Load", &biogears::SESubstancePhysicochemicals::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::Unload, py::const_))

    .def("HasPrimaryPKA", &biogears::SESubstancePhysicochemicals::HasPrimaryPKA)
    .def("GetPrimaryPKA", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetPrimaryPKA))
    .def("GetPrimaryPKA", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetPrimaryPKA, py::const_))

    .def("HasSecondaryPKA", &biogears::SESubstancePhysicochemicals::HasSecondaryPKA)
    .def("GetSecondaryPKA", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetSecondaryPKA))
    .def("GetSecondaryPKA", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetSecondaryPKA, py::const_))

    .def("GetBindingProtein", &biogears::SESubstancePhysicochemicals::GetBindingProtein)
    .def("SetBindingProtein", &biogears::SESubstancePhysicochemicals::SetBindingProtein)
    .def("HasBindingProtein", &biogears::SESubstancePhysicochemicals::HasBindingProtein)
    .def("InvalidateBindingProtein", &biogears::SESubstancePhysicochemicals::InvalidateBindingProtein)

    .def("HasBloodPlasmaRatio", &biogears::SESubstancePhysicochemicals::HasBloodPlasmaRatio)
    .def("GetBloodPlasmaRatio", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetBloodPlasmaRatio))
    .def("GetBloodPlasmaRatio", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetBloodPlasmaRatio, py::const_))

    .def("HasFractionUnboundInPlasma", &biogears::SESubstancePhysicochemicals::HasFractionUnboundInPlasma)
    .def("GetFractionUnboundInPlasma", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetFractionUnboundInPlasma))
    .def("GetFractionUnboundInPlasma", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetFractionUnboundInPlasma, py::const_))

    .def("GetIonicState", &biogears::SESubstancePhysicochemicals::GetIonicState)
    .def("SetIonicState", &biogears::SESubstancePhysicochemicals::SetIonicState)
    .def("HasIonicState", &biogears::SESubstancePhysicochemicals::HasIonicState)
    .def("InvalidateIonicState", &biogears::SESubstancePhysicochemicals::InvalidateIonicState)

    .def("HasLogP", &biogears::SESubstancePhysicochemicals::HasLogP)
    .def("GetLogP", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetLogP))
    .def("GetLogP", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetLogP, py::const_))

    .def("HasHydrogenBondCount", &biogears::SESubstancePhysicochemicals::HasHydrogenBondCount)
    .def("GetHydrogenBondCount", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetHydrogenBondCount))
    .def("GetHydrogenBondCount", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetHydrogenBondCount, py::const_))

    .def("HasPolarSurfaceArea", &biogears::SESubstancePhysicochemicals::HasPolarSurfaceArea)
    .def("GetPolarSurfaceArea", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetPolarSurfaceArea))
    .def("GetPolarSurfaceArea", py::overload_cast<>(&biogears::SESubstancePhysicochemicals::GetPolarSurfaceArea, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
