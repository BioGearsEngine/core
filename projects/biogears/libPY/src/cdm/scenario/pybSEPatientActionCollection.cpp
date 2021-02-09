#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/scenario/SEPatientActionCollection.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEPatientActionCollection(py::module_ &m)
{

  py::class_<biogears::SEPatientActionCollection, biogears::Loggable>(m, "SEPatientActionCollection")
    .def(py::init<biogears::SESubstanceManager&>())
    .def("Clear", &biogears::SEPatientActionCollection::Clear)
    .def("Unload", &biogears::SEPatientActionCollection::Unload)
    .def("ProcessAction", py::overload_cast<const biogears::SEPatientAction&>(&biogears::SEPatientActionCollection::ProcessAction))
    .def("ProcessAction", py::overload_cast<const CDM::PatientActionData&>(&biogears::SEPatientActionCollection::ProcessAction))

    .def("HasAcuteRespiratoryDistress", &biogears::SEPatientActionCollection::HasAcuteRespiratoryDistress)
    .def("GetAcuteRespiratoryDistress", &biogears::SEPatientActionCollection::GetAcuteRespiratoryDistress)
    .def("RemoveAcuteRespiratoryDistress", &biogears::SEPatientActionCollection::RemoveAcuteRespiratoryDistress)

    .def("HasAcuteStress", &biogears::SEPatientActionCollection::HasAcuteStress)
    .def("GetAcuteStress", &biogears::SEPatientActionCollection::GetAcuteStress)
    .def("RemoveAcuteStress", &biogears::SEPatientActionCollection::RemoveAcuteStress)

    .def("HasAirwayObstruction", &biogears::SEPatientActionCollection::HasAirwayObstruction)
    .def("GetAirwayObstruction", &biogears::SEPatientActionCollection::GetAirwayObstruction)
    .def("RemoveAirwayObstruction", &biogears::SEPatientActionCollection::RemoveAirwayObstruction)

    .def("HasApnea", &biogears::SEPatientActionCollection::HasApnea)
    .def("GetApnea", &biogears::SEPatientActionCollection::GetApnea)
    .def("RemoveApnea", &biogears::SEPatientActionCollection::RemoveApnea)

    .def("HasAsthmaAttack", &biogears::SEPatientActionCollection::HasAsthmaAttack)
    .def("GetAsthmaAttack", &biogears::SEPatientActionCollection::GetAsthmaAttack)
    .def("RemoveAsthmaAttack", &biogears::SEPatientActionCollection::RemoveAsthmaAttack)

    .def("HasBrainInjury", &biogears::SEPatientActionCollection::HasBrainInjury)
    .def("GetBrainInjury", &biogears::SEPatientActionCollection::GetBrainInjury)
    .def("RemoveBrainInjury", &biogears::SEPatientActionCollection::RemoveBrainInjury)

    .def("HasBronchoconstriction", &biogears::SEPatientActionCollection::HasBronchoconstriction)
    .def("GetBronchoconstriction", &biogears::SEPatientActionCollection::GetBronchoconstriction)
    .def("RemoveBronchoconstriction", &biogears::SEPatientActionCollection::RemoveBronchoconstriction)

    .def("HasBurnWound", &biogears::SEPatientActionCollection::HasBurnWound)
    .def("GetBurnWound", &biogears::SEPatientActionCollection::GetBurnWound)
    .def("RemoveBurnWound", &biogears::SEPatientActionCollection::RemoveBurnWound)

    .def("HasCardiacArrest", &biogears::SEPatientActionCollection::HasCardiacArrest)
    .def("GetCardiacArrest", &biogears::SEPatientActionCollection::GetCardiacArrest)
    .def("RemoveCardiacArrest", &biogears::SEPatientActionCollection::RemoveCardiacArrest)

    .def("HasChestCompression", &biogears::SEPatientActionCollection::HasChestCompression)
    .def("RemoveChestCompression", &biogears::SEPatientActionCollection::RemoveChestCompression)
    .def("HasChestCompressionForce", &biogears::SEPatientActionCollection::HasChestCompressionForce)
    .def("GetChestCompressionForce", &biogears::SEPatientActionCollection::GetChestCompressionForce)
    .def("HasChestCompressionForceScale", &biogears::SEPatientActionCollection::HasChestCompressionForceScale)
    .def("GetChestCompressionForceScale", &biogears::SEPatientActionCollection::GetChestCompressionForceScale)

    .def("HasChestOcclusiveDressing", &biogears::SEPatientActionCollection::HasChestOcclusiveDressing)
    .def("HasLeftChestOcclusiveDressing", &biogears::SEPatientActionCollection::HasLeftChestOcclusiveDressing)
    .def("GetLeftChestOcclusiveDressing", &biogears::SEPatientActionCollection::GetLeftChestOcclusiveDressing)
    .def("RemoveLeftChestOcclusiveDressing", &biogears::SEPatientActionCollection::RemoveLeftChestOcclusiveDressing)
    .def("HasRightChestOcclusiveDressing", &biogears::SEPatientActionCollection::HasRightChestOcclusiveDressing)
    .def("GetRightChestOcclusiveDressing", &biogears::SEPatientActionCollection::GetRightChestOcclusiveDressing)
    .def("RemoveRightChestOcclusiveDressing", &biogears::SEPatientActionCollection::RemoveRightChestOcclusiveDressing)

    .def("HasConsciousRespiration", &biogears::SEPatientActionCollection::HasConsciousRespiration)
    .def("GetConsciousRespiration", &biogears::SEPatientActionCollection::GetConsciousRespiration)
    .def("RemoveConsciousRespiration", &biogears::SEPatientActionCollection::RemoveConsciousRespiration)

    .def("HasConsumeNutrients", &biogears::SEPatientActionCollection::HasConsumeNutrients)
    .def("GetConsumeNutrients", &biogears::SEPatientActionCollection::GetConsumeNutrients)
    .def("RemoveConsumeNutrients", &biogears::SEPatientActionCollection::RemoveConsumeNutrients)

    .def("HasExercise", &biogears::SEPatientActionCollection::HasExercise)
    .def("GetExercise", &biogears::SEPatientActionCollection::GetExercise)
    .def("RemoveExercise", &biogears::SEPatientActionCollection::RemoveExercise)

    .def("HasHemorrhage", &biogears::SEPatientActionCollection::HasHemorrhage)
    .def("GetHemorrhages", &biogears::SEPatientActionCollection::GetHemorrhages)
    .def("RemoveHemorrhage", py::overload_cast<const char*>(&biogears::SEPatientActionCollection::RemoveHemorrhage))
    .def("RemoveHemorrhage", py::overload_cast<const std::string&>(&biogears::SEPatientActionCollection::RemoveHemorrhage))

    .def("HasInfection", &biogears::SEPatientActionCollection::HasInfection)
    .def("GetInfection", &biogears::SEPatientActionCollection::GetInfection)
    .def("RemoveInfection", &biogears::SEPatientActionCollection::RemoveInfection)

    .def("HasIntubation", &biogears::SEPatientActionCollection::HasIntubation)
    .def("GetIntubation", &biogears::SEPatientActionCollection::GetIntubation)
    .def("RemoveIntubation", &biogears::SEPatientActionCollection::RemoveIntubation)

    .def("HasMechanicalVentilation", &biogears::SEPatientActionCollection::HasMechanicalVentilation)
    .def("GetMechanicalVentilation", &biogears::SEPatientActionCollection::GetMechanicalVentilation)
    .def("RemoveMechanicalVentilation", &biogears::SEPatientActionCollection::RemoveMechanicalVentilation)

    .def("HasNeedleDecompression", &biogears::SEPatientActionCollection::HasNeedleDecompression)

    .def("HasLeftNeedleDecompression", &biogears::SEPatientActionCollection::HasLeftNeedleDecompression)
    .def("GetLeftNeedleDecompression", &biogears::SEPatientActionCollection::GetLeftNeedleDecompression)
    .def("RemoveLeftNeedleDecompression", &biogears::SEPatientActionCollection::RemoveLeftNeedleDecompression)


    .def("HasRightNeedleDecompression", &biogears::SEPatientActionCollection::HasRightNeedleDecompression)
    .def("GetRightNeedleDecompression", &biogears::SEPatientActionCollection::GetRightNeedleDecompression)
    .def("RemoveRightNeedleDecompression", &biogears::SEPatientActionCollection::RemoveRightNeedleDecompression)


    .def("HasPainStimulus", &biogears::SEPatientActionCollection::HasPainStimulus)
    .def("GetPainStimuli", &biogears::SEPatientActionCollection::GetPainStimuli)
    .def("RemovePainStimulus", py::overload_cast<const char*>(&biogears::SEPatientActionCollection::RemovePainStimulus))
    .def("RemovePainStimulus", py::overload_cast<const std::string&>(&biogears::SEPatientActionCollection::RemovePainStimulus))

    .def("HasPericardialEffusion", &biogears::SEPatientActionCollection::HasPericardialEffusion)
    .def("GetPericardialEffusion", &biogears::SEPatientActionCollection::GetPericardialEffusion)
    .def("RemovePericardialEffusion", &biogears::SEPatientActionCollection::RemovePericardialEffusion)

    .def("HasSleepState", &biogears::SEPatientActionCollection::HasSleepState)
    .def("GetSleepState", &biogears::SEPatientActionCollection::GetSleepState)
    .def("RemoveSleepState", &biogears::SEPatientActionCollection::RemoveSleepState)

    .def("HasTensionPneumothorax", &biogears::SEPatientActionCollection::HasTensionPneumothorax)

    .def("HasLeftClosedTensionPneumothorax", &biogears::SEPatientActionCollection::HasLeftClosedTensionPneumothorax)
    .def("GetLeftClosedTensionPneumothorax", &biogears::SEPatientActionCollection::GetLeftClosedTensionPneumothorax)
    .def("RemoveLeftClosedTensionPneumothorax", &biogears::SEPatientActionCollection::RemoveLeftClosedTensionPneumothorax)

    .def("HasLeftOpenTensionPneumothorax", &biogears::SEPatientActionCollection::HasLeftOpenTensionPneumothorax)
    .def("GetLeftOpenTensionPneumothorax", &biogears::SEPatientActionCollection::GetLeftOpenTensionPneumothorax)
    .def("RemoveLeftOpenTensionPneumothorax", &biogears::SEPatientActionCollection::RemoveLeftOpenTensionPneumothorax)

    .def("HasRightClosedTensionPneumothorax", &biogears::SEPatientActionCollection::HasRightClosedTensionPneumothorax)
    .def("GetRightClosedTensionPneumothorax", &biogears::SEPatientActionCollection::GetRightClosedTensionPneumothorax)
    .def("RemoveRightClosedTensionPneumothorax", &biogears::SEPatientActionCollection::RemoveRightClosedTensionPneumothorax)

    .def("HasRightOpenTensionPneumothorax", &biogears::SEPatientActionCollection::HasRightOpenTensionPneumothorax)
    .def("GetRightOpenTensionPneumothorax", &biogears::SEPatientActionCollection::GetRightOpenTensionPneumothorax)
    .def("RemoveRightOpenTensionPneumothorax", &biogears::SEPatientActionCollection::RemoveRightOpenTensionPneumothorax)

    .def("GetSubstanceBoluses", &biogears::SEPatientActionCollection::GetSubstanceBoluses)
    .def("RemoveSubstanceBolus", &biogears::SEPatientActionCollection::RemoveSubstanceBolus)

    .def("GetSubstanceInfusions", &biogears::SEPatientActionCollection::GetSubstanceInfusions)
    .def("RemoveSubstanceInfusion", &biogears::SEPatientActionCollection::RemoveSubstanceInfusion)

    .def("GetSubstanceNasalDoses", &biogears::SEPatientActionCollection::GetSubstanceNasalDoses)
    .def("RemoveSubstanceNasalDose", &biogears::SEPatientActionCollection::RemoveSubstanceNasalDose)

    .def("GetSubstanceOralDoses", &biogears::SEPatientActionCollection::GetSubstanceOralDoses)
    .def("RemoveSubstanceOralDose", &biogears::SEPatientActionCollection::RemoveSubstanceOralDose)

    .def("GetSubstanceCompoundInfusions", &biogears::SEPatientActionCollection::GetSubstanceCompoundInfusions)
    .def("RemoveSubstanceCompoundInfusion", &biogears::SEPatientActionCollection::RemoveSubstanceCompoundInfusion)


    .def("HasTourniquet", &biogears::SEPatientActionCollection::HasTourniquet)
    .def("GetTourniquets", &biogears::SEPatientActionCollection::GetTourniquets)
    .def("RemoveTourniquet", py::overload_cast<const char*>(&biogears::SEPatientActionCollection::RemoveTourniquet))
    .def("RemoveTourniquet", py::overload_cast<const std::string&>(&biogears::SEPatientActionCollection::RemoveTourniquet))

    .def("HasUrinate", &biogears::SEPatientActionCollection::HasUrinate)
    .def("GetUrinate", &biogears::SEPatientActionCollection::GetUrinate)
    .def("RemoveUrinate", &biogears::SEPatientActionCollection::RemoveUrinate)


    .def("HasOverride", &biogears::SEPatientActionCollection::HasOverride)
    .def("GetOverride", &biogears::SEPatientActionCollection::GetOverride)
    .def("RemoveOverride", &biogears::SEPatientActionCollection::RemoveOverride);




#ifdef VERSION_INFO
      m.attr("__version__")
    = VERSION_INFO;
#else
      m.attr("__version__")
    = "dev";
#endif
}
