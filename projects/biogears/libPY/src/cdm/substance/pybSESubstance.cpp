#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSESubstance(py::module_ &m)
{

  py::class_<biogears::SESubstance, biogears::Loggable>(m, "SESubstance")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SESubstance::Clear)
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstance::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstance::GetScalar))
    .def("Load", &biogears::SESubstance::Load)
    .def("GetScalar", py::overload_cast<>(&biogears::SESubstance::Unload, py::const_))
    .def("GetName", &biogears::SESubstance::GetName)
    .def("GetName_cStr", &biogears::SESubstance::GetName_cStr)
    .def("SetName", py::overload_cast<const char*>(&biogears::SESubstance::SetName))
    .def("SetName", py::overload_cast<const std::string&>(&biogears::SESubstance::SetName))
    .def("HasName", &biogears::SESubstance::HasName)
    .def("InvalidateName", &biogears::SESubstance::InvalidateName)

    .def("GetState", &biogears::SESubstance::GetState)
    .def("SetState", &biogears::SESubstance::SetState)
    .def("HasState", &biogears::SESubstance::HasState)
    .def("InvalidateState", &biogears::SESubstance::InvalidateState)

    .def("GetClassification", &biogears::SESubstance::GetClassification)
    .def("SetClassification", &biogears::SESubstance::SetClassification)
    .def("HasClassification", &biogears::SESubstance::HasClassification)
    .def("InvalidateClassification", &biogears::SESubstance::InvalidateClassification)

    .def("HasDensity", &biogears::SESubstance::HasDensity)
    .def("GetDensity", py::overload_cast<>(&biogears::SESubstance::GetDensity))
    .def("GetDensity", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SESubstance::GetDensity, py::const_))

    .def("HasMolarMass", &biogears::SESubstance::HasMolarMass)
    .def("GetMolarMass", py::overload_cast<>(&biogears::SESubstance::GetMolarMass))
    .def("GetMolarMass", py::overload_cast<const biogears::MassPerAmountUnit&>(&biogears::SESubstance::GetMolarMass, py::const_))

    .def("HasMaximumDiffusionFlux", &biogears::SESubstance::HasMaximumDiffusionFlux)
    .def("GetMaximumDiffusionFlux", py::overload_cast<>(&biogears::SESubstance::GetMaximumDiffusionFlux))
    .def("GetMaximumDiffusionFlux", py::overload_cast<const biogears::MassPerAreaTimeUnit&>(&biogears::SESubstance::GetMaximumDiffusionFlux, py::const_))

    .def("HasMichaelisCoefficient", &biogears::SESubstance::HasMichaelisCoefficient)
    .def("GetMichaelisCoefficient", py::overload_cast<>(&biogears::SESubstance::GetMichaelisCoefficient))
    .def("GetMichaelisCoefficient", py::overload_cast<>(&biogears::SESubstance::GetMichaelisCoefficient, py::const_))

    .def("HasMembraneResistance", &biogears::SESubstance::HasMembraneResistance)
    .def("GetMembraneResistance", py::overload_cast<>(&biogears::SESubstance::GetMembraneResistance))
    .def("GetMembraneResistance", py::overload_cast<const biogears::ElectricResistanceUnit&>(&biogears::SESubstance::GetMembraneResistance, py::const_))

    .def("HasAerosolization", &biogears::SESubstance::HasAerosolization)
    .def("GetAerosolization", py::overload_cast<>(&biogears::SESubstance::GetAerosolization))
    .def("GetAerosolization", py::overload_cast<>(&biogears::SESubstance::GetAerosolization, py::const_))
    .def("RemoveAerosolization", &biogears::SESubstance::RemoveAerosolization)

    .def("HasAreaUnderCurve", &biogears::SESubstance::HasAreaUnderCurve)
    .def("GetAreaUnderCurve", py::overload_cast<>(&biogears::SESubstance::GetAreaUnderCurve))
    .def("GetAreaUnderCurve", py::overload_cast<const biogears::TimeMassPerVolumeUnit&>(&biogears::SESubstance::GetAreaUnderCurve, py::const_))

    .def("HasBloodConcentration", &biogears::SESubstance::HasBloodConcentration)
    .def("GetBloodConcentration", py::overload_cast<>(&biogears::SESubstance::GetBloodConcentration))
    .def("GetBloodConcentration", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SESubstance::GetBloodConcentration, py::const_))

    .def("HasEffectSiteConcentration", &biogears::SESubstance::HasEffectSiteConcentration)
    .def("GetEffectSiteConcentration", py::overload_cast<>(&biogears::SESubstance::GetEffectSiteConcentration))
    .def("GetEffectSiteConcentration", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SESubstance::GetEffectSiteConcentration, py::const_))

    .def("HasMassInBody", &biogears::SESubstance::HasMassInBody)
    .def("GetMassInBody", py::overload_cast<>(&biogears::SESubstance::GetMassInBody))
    .def("GetMassInBody", py::overload_cast<const biogears::MassUnit&>(&biogears::SESubstance::GetMassInBody, py::const_))

    .def("HasMassInBlood", &biogears::SESubstance::HasMassInBlood)
    .def("GetMassInBlood", py::overload_cast<>(&biogears::SESubstance::GetMassInBlood))
    .def("GetMassInBlood", py::overload_cast<const biogears::MassUnit&>(&biogears::SESubstance::GetMassInBlood, py::const_))

    .def("HasMassInTissue", &biogears::SESubstance::HasMassInTissue)
    .def("GetMassInTissue", py::overload_cast<>(&biogears::SESubstance::GetMassInTissue))
    .def("GetMassInTissue", py::overload_cast<const biogears::MassUnit&>(&biogears::SESubstance::GetMassInTissue, py::const_))

    .def("HasPlasmaConcentration", &biogears::SESubstance::HasPlasmaConcentration)
    .def("GetPlasmaConcentration", py::overload_cast<>(&biogears::SESubstance::GetPlasmaConcentration))
    .def("GetPlasmaConcentration", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SESubstance::GetPlasmaConcentration, py::const_))

    .def("HasSystemicMassCleared", &biogears::SESubstance::HasSystemicMassCleared)
    .def("GetSystemicMassCleared", py::overload_cast<>(&biogears::SESubstance::GetSystemicMassCleared))
    .def("GetSystemicMassCleared", py::overload_cast<const biogears::MassUnit&>(&biogears::SESubstance::GetSystemicMassCleared, py::const_))

    .def("HasTissueConcentration", &biogears::SESubstance::HasTissueConcentration)
    .def("GetTissueConcentration", py::overload_cast<>(&biogears::SESubstance::GetTissueConcentration))
    .def("GetTissueConcentration", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SESubstance::GetTissueConcentration, py::const_))

    .def("HasAlveolarTransfer", &biogears::SESubstance::HasAlveolarTransfer)
    .def("GetAlveolarTransfer", py::overload_cast<>(&biogears::SESubstance::GetAlveolarTransfer))
    .def("GetAlveolarTransfer", py::overload_cast<const biogears::VolumePerTimeUnit&>(&biogears::SESubstance::GetAlveolarTransfer, py::const_))

    .def("HasDiffusingCapacity", &biogears::SESubstance::HasDiffusingCapacity)
    .def("GetDiffusingCapacity", py::overload_cast<>(&biogears::SESubstance::GetDiffusingCapacity))
    .def("GetDiffusingCapacity", py::overload_cast<const biogears::VolumePerTimePressureUnit&>(&biogears::SESubstance::GetDiffusingCapacity, py::const_))

    .def("HasEndTidalFraction", &biogears::SESubstance::HasEndTidalFraction)
    .def("GetEndTidalFraction", py::overload_cast<>(&biogears::SESubstance::GetEndTidalFraction))
    .def("GetEndTidalFraction", py::overload_cast<>(&biogears::SESubstance::GetEndTidalFraction, py::const_))

    .def("HasEndTidalPressure", &biogears::SESubstance::HasEndTidalPressure)
    .def("GetEndTidalPressure", py::overload_cast<>(&biogears::SESubstance::GetEndTidalPressure))
    .def("GetEndTidalPressure", py::overload_cast<const biogears::PressureUnit&>(&biogears::SESubstance::GetEndTidalPressure, py::const_))

    .def("HasRelativeDiffusionCoefficient", &biogears::SESubstance::HasRelativeDiffusionCoefficient)
    .def("GetRelativeDiffusionCoefficient", py::overload_cast<>(&biogears::SESubstance::GetRelativeDiffusionCoefficient))
    .def("GetRelativeDiffusionCoefficient", py::overload_cast<>(&biogears::SESubstance::GetRelativeDiffusionCoefficient, py::const_))

    .def("HasSolubilityCoefficient", &biogears::SESubstance::HasSolubilityCoefficient)
    .def("GetSolubilityCoefficient", py::overload_cast<>(&biogears::SESubstance::GetSolubilityCoefficient))
    .def("GetSolubilityCoefficient", py::overload_cast<const biogears::InversePressureUnit&>(&biogears::SESubstance::GetSolubilityCoefficient, py::const_))

    .def("HasClearance", &biogears::SESubstance::HasClearance)
    .def("GetClearance", py::overload_cast<>(&biogears::SESubstance::GetClearance))
    .def("GetClearance", py::overload_cast<>(&biogears::SESubstance::GetClearance, py::const_))
    .def("RemoveClearance", &biogears::SESubstance::RemoveClearance)

    .def("HasPK", &biogears::SESubstance::HasPK)
    .def("GetPK", py::overload_cast<>(&biogears::SESubstance::GetPK))
    .def("GetPK", py::overload_cast<>(&biogears::SESubstance::GetPK, py::const_))
    .def("RemovePK", &biogears::SESubstance::RemovePK)

    .def("HasPD", &biogears::SESubstance::HasPD)
    .def("GetPD", py::overload_cast<>(&biogears::SESubstance::GetPD))
    .def("GetPD", py::overload_cast<>(&biogears::SESubstance::GetPD, py::const_))
    .def("RemovePK", &biogears::SESubstance::RemovePD);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
