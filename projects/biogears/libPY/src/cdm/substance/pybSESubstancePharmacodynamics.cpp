#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>

namespace py = pybind11;

void define_pybSESubstancePharmacodynamics(py::module_ &m)
{

  py::class_<biogears::SEPharmacodynamicModifier>(m, "SEPharmacodynamicModifier")
    .def(py::init<>())
    .def("Clear", &biogears::SEPharmacodynamicModifier::Clear)
    .def("IsValid", &biogears::SEPharmacodynamicModifier::IsValid)
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SEPharmacodynamicModifier::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SEPharmacodynamicModifier::GetScalar))
    .def("Load", &biogears::SEPharmacodynamicModifier::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPharmacodynamicModifier::Unload,py::const_))
    .def("HasEMax", &biogears::SEPharmacodynamicModifier::HasEMax)
    .def("GetEMax", py::overload_cast<>(&biogears::SEPharmacodynamicModifier::GetEMax))
    .def("GetEMax", py::overload_cast<>(&biogears::SEPharmacodynamicModifier::GetEMax, py::const_))
    .def("HasEC50", &biogears::SEPharmacodynamicModifier::HasEC50)
    .def("GetEC50", py::overload_cast<>(&biogears::SEPharmacodynamicModifier::GetEC50))
    .def("GetEC50", py::overload_cast<const biogears::MassPerVolumeUnit&>(&biogears::SEPharmacodynamicModifier::GetEC50, py::const_));

  py::class_<biogears::SESubstancePharmacodynamics>(m, "SESubstancePharmacodynamics")
    .def(py::init<biogears::Logger*>())
    .def("Clear", &biogears::SESubstancePharmacodynamics::Clear)
    .def("IsValid", &biogears::SESubstancePharmacodynamics::IsValid)
    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstancePharmacodynamics::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstancePharmacodynamics::GetScalar))
    .def("Load", &biogears::SESubstancePharmacodynamics::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::Unload, py::const_))

    .def("HasAntibacterialEffect", &biogears::SESubstancePharmacodynamics::HasAntibacterialEffect)
    .def("GetAntibacterialEffect", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetAntibacterialEffect))
    .def("GetAntibacterialEffect", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SESubstancePharmacodynamics::GetAntibacterialEffect, py::const_))

    .def("HasBronchodilation", &biogears::SESubstancePharmacodynamics::HasBronchodilation)
    .def("GetBronchodilation", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetBronchodilation))
    .def("GetBronchodilation", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetBronchodilation, py::const_))

    .def("HasDiastolicPressureModifier", &biogears::SESubstancePharmacodynamics::HasDiastolicPressureModifier)
    .def("GetDiastolicPressureModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetDiastolicPressureModifier))
    .def("GetDiastolicPressureModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetDiastolicPressureModifier, py::const_))

    .def("HasEMaxShapeParameter", &biogears::SESubstancePharmacodynamics::HasEMaxShapeParameter)
    .def("GetEMaxShapeParameter", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetEMaxShapeParameter))
    .def("GetEMaxShapeParameter", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetEMaxShapeParameter, py::const_))

    .def("HasFeverModifier", &biogears::SESubstancePharmacodynamics::HasFeverModifier)
    .def("GetFeverModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetFeverModifier))
    .def("GetFeverModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetFeverModifier, py::const_))

    .def("HasHeartRateModifier", &biogears::SESubstancePharmacodynamics::HasHeartRateModifier)
    .def("GetHeartRateModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetHeartRateModifier))
    .def("GetHeartRateModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetHeartRateModifier, py::const_))

    .def("HasHemorrhageModifier", &biogears::SESubstancePharmacodynamics::HasHemorrhageModifier)
    .def("GetHemorrhageModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetHemorrhageModifier))
    .def("GetHemorrhageModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetHemorrhageModifier, py::const_))

    .def("HasNeuromuscularBlock", &biogears::SESubstancePharmacodynamics::HasNeuromuscularBlock)
    .def("GetNeuromuscularBlock", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetNeuromuscularBlock))
    .def("GetNeuromuscularBlock", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetNeuromuscularBlock, py::const_))

    .def("HasPainModifier", &biogears::SESubstancePharmacodynamics::HasPainModifier)
    .def("GetPainModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetPainModifier))
    .def("GetPainModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetPainModifier, py::const_))

    .def("HasPupilReactivityModifier", &biogears::SESubstancePharmacodynamics::HasPupilReactivityModifier)
    .def("GetPupilReactivityModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetPupilReactivityModifier))
    .def("GetPupilReactivityModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetPupilReactivityModifier, py::const_))

    .def("HasPupilSizeModifier", &biogears::SESubstancePharmacodynamics::HasPupilSizeModifier)
    .def("GetPupilSizeModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetPupilSizeModifier))
    .def("GetPupilSizeModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetPupilSizeModifier, py::const_))

    .def("HasRespirationRateModifier", &biogears::SESubstancePharmacodynamics::HasRespirationRateModifier)
    .def("GetRespirationRateModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetRespirationRateModifier))
    .def("GetRespirationRateModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetRespirationRateModifier, py::const_))

    .def("HasSedation", &biogears::SESubstancePharmacodynamics::HasSedation)
    .def("GetSedation", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetSedation))
    .def("GetSedation", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetSedation, py::const_))

    .def("HasSystolicPressureModifier", &biogears::SESubstancePharmacodynamics::HasSystolicPressureModifier)
    .def("GetSystolicPressureModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetSystolicPressureModifier))
    .def("GetSystolicPressureModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetSystolicPressureModifier, py::const_))

    .def("HasTidalVolumeModifier", &biogears::SESubstancePharmacodynamics::HasTidalVolumeModifier)
    .def("GetTidalVolumeModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetTidalVolumeModifier))
    .def("GetTidalVolumeModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetTidalVolumeModifier, py::const_))

    .def("HasTubularPermeabilityModifier", &biogears::SESubstancePharmacodynamics::HasTubularPermeabilityModifier)
    .def("GetTubularPermeabilityModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetTubularPermeabilityModifier))
    .def("GetTubularPermeabilityModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetTubularPermeabilityModifier, py::const_))

    .def("HasCentralNervousModifier", &biogears::SESubstancePharmacodynamics::HasCentralNervousModifier)
    .def("GetCentralNervousModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetCentralNervousModifier))
    .def("GetCentralNervousModifier", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetCentralNervousModifier, py::const_))

    .def("HasEffectSiteRateConstant", &biogears::SESubstancePharmacodynamics::HasEffectSiteRateConstant)
    .def("GetEffectSiteRateConstant", py::overload_cast<>(&biogears::SESubstancePharmacodynamics::GetEffectSiteRateConstant))
    .def("GetEffectSiteRateConstant", py::overload_cast<const biogears::FrequencyUnit&>(&biogears::SESubstancePharmacodynamics::GetEffectSiteRateConstant, py::const_))

    .def("GetPharmacodynamicModifiers", &biogears::SESubstancePharmacodynamics::GetPharmacodynamicModifiers);

#ifdef VERSION_INFO
      m.attr("__version__")
    = VERSION_INFO;
#else
      m.attr("__version__")
    = "dev";
#endif
}
