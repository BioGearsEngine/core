#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>

namespace py = pybind11;

void define_pybSESubstanceClearance(py::module_ &m)
{

  py::class_<biogears::SESubstanceClearance, biogears::Loggable>(m, "SESubstanceClearance")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SESubstanceClearance::Clear)
    .def("IsValid", &biogears::SESubstanceClearance::IsValid)
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstanceClearance::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstanceClearance::GetScalar))
    .def("Load", &biogears::SESubstanceClearance::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceClearance::Unload, py::const_))

    .def("HasSystemic", &biogears::SESubstanceClearance::HasSystemic)
    .def("SetSystemic", &biogears::SESubstanceClearance::SetSystemic)
    .def("HasCellular", &biogears::SESubstanceClearance::HasCellular)
    .def("SetCellular", &biogears::SESubstanceClearance::SetCellular)

    .def("HasCellBirthRate", &biogears::SESubstanceClearance::HasCellBirthRate)
    .def("GetCellBirthRate", py::overload_cast<>(&biogears::SESubstanceClearance::GetCellBirthRate))
    .def("GetCellBirthRate", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SESubstanceClearance::GetCellBirthRate, py::const_))

    .def("HasCellDeathRate", &biogears::SESubstanceClearance::HasCellDeathRate)
    .def("GetCellDeathRate", py::overload_cast<>(&biogears::SESubstanceClearance::GetCellDeathRate))
    .def("GetCellDeathRate", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SESubstanceClearance::GetCellDeathRate, py::const_))

    .def("GetChargeInBlood", &biogears::SESubstanceClearance::GetChargeInBlood)
    .def("SetChargeInBlood", &biogears::SESubstanceClearance::SetChargeInBlood)
    .def("HasChargeInBlood", &biogears::SESubstanceClearance::HasChargeInBlood)
    .def("InvalidateChargeInBlood", &biogears::SESubstanceClearance::InvalidateChargeInBlood)

    .def("HasFractionExcretedInFeces", &biogears::SESubstanceClearance::HasFractionExcretedInFeces)
    .def("GetFractionExcretedInFeces", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionExcretedInFeces))
    .def("GetFractionExcretedInFeces", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionExcretedInFeces, py::const_))

    .def("HasFractionExcretedInUrine", &biogears::SESubstanceClearance::HasFractionExcretedInUrine)
    .def("GetFractionExcretedInUrine", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionExcretedInUrine))
    .def("GetFractionExcretedInUrine", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionExcretedInUrine, py::const_))

    .def("HasFractionMetabolizedInGut", &biogears::SESubstanceClearance::HasFractionMetabolizedInGut)
    .def("GetFractionMetabolizedInGut", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionMetabolizedInGut))
    .def("GetFractionMetabolizedInGut", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionMetabolizedInGut, py::const_))

    .def("HasFractionUnboundInPlasma", &biogears::SESubstanceClearance::HasFractionUnboundInPlasma)
    .def("GetFractionUnboundInPlasma", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionUnboundInPlasma))
    .def("GetFractionUnboundInPlasma", py::overload_cast<>(&biogears::SESubstanceClearance::GetFractionUnboundInPlasma, py::const_))

    .def("HasGlomerularFilterability", &biogears::SESubstanceClearance::HasGlomerularFilterability)
    .def("GetGlomerularFilterability", py::overload_cast<>(&biogears::SESubstanceClearance::GetGlomerularFilterability))
    .def("GetGlomerularFilterability", py::overload_cast<>(&biogears::SESubstanceClearance::GetGlomerularFilterability, py::const_))

    .def("HasIntrinsicClearance", &biogears::SESubstanceClearance::HasIntrinsicClearance)
    .def("GetIntrinsicClearance", py::overload_cast<>(&biogears::SESubstanceClearance::GetIntrinsicClearance))
    .def("GetIntrinsicClearance", py::overload_cast<const biogears::VolumePerTimeMassUnit&>(&biogears::SESubstanceClearance::GetIntrinsicClearance, py::const_))

    .def("GetRenalDynamic", &biogears::SESubstanceClearance::GetRenalDynamic)
    .def("SetRenalDynamic", &biogears::SESubstanceClearance::SetRenalDynamic)
    .def("HasRenalDynamic", &biogears::SESubstanceClearance::HasRenalDynamic)
    .def("InvalidateRenalDynamic", &biogears::SESubstanceClearance::InvalidateRenalDynamic)

    .def("HasRenalClearance", &biogears::SESubstanceClearance::HasRenalClearance)
    .def("GetRenalClearance", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalClearance))
    .def("GetRenalClearance", py::overload_cast<const biogears::VolumePerTimeMassUnit&>(&biogears::SESubstanceClearance::GetRenalClearance, py::const_))

    .def("HasRenalReabsorptionRatio", &biogears::SESubstanceClearance::HasRenalReabsorptionRatio)
    .def("GetRenalReabsorptionRatio", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalReabsorptionRatio))
    .def("GetRenalReabsorptionRatio", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalReabsorptionRatio, py::const_))

    .def("HasRenalTransportMaximum", &biogears::SESubstanceClearance::HasRenalTransportMaximum)
    .def("GetRenalTransportMaximum", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalTransportMaximum))
    .def("GetRenalTransportMaximum", py::overload_cast<const biogears::MassPerTimeUnit&>(&biogears::SESubstanceClearance::GetRenalTransportMaximum, py::const_))

    .def("HasRenalFiltrationRate", &biogears::SESubstanceClearance::HasRenalFiltrationRate)
    .def("GetRenalFiltrationRate", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalFiltrationRate))
    .def("GetRenalFiltrationRate", py::overload_cast<const biogears::MassPerTimeUnit&>(&biogears::SESubstanceClearance::GetRenalFiltrationRate, py::const_))

    .def("HasRenalReabsorptionRate", &biogears::SESubstanceClearance::HasRenalReabsorptionRate)
    .def("GetRenalReabsorptionRate", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalReabsorptionRate))
    .def("GetRenalReabsorptionRate", py::overload_cast<const biogears::MassPerTimeUnit&>(&biogears::SESubstanceClearance::GetRenalReabsorptionRate, py::const_))

    .def("HasRenalExcretionRate", &biogears::SESubstanceClearance::HasRenalExcretionRate)
    .def("GetRenalExcretionRate", py::overload_cast<>(&biogears::SESubstanceClearance::GetRenalExcretionRate))
    .def("GetRenalExcretionRate", py::overload_cast<const biogears::MassPerTimeUnit&>(&biogears::SESubstanceClearance::GetRenalExcretionRate, py::const_))

    .def("HasSystemicClearance", &biogears::SESubstanceClearance::HasSystemicClearance)
    .def("GetSystemicClearance", py::overload_cast<>(&biogears::SESubstanceClearance::GetSystemicClearance))
    .def("GetSystemicClearance", py::overload_cast<const biogears::VolumePerTimeMassUnit&>(&biogears::SESubstanceClearance::GetSystemicClearance, py::const_));
#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
