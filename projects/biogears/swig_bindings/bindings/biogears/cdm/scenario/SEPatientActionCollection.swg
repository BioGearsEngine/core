%{
#include <biogears/cdm/scenario/SEPatientActionCollection.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEPatientActionCollection
#endif

%import<biogears/exports.swg>


%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SEAction.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/patient/SEPatient.swg>
%import <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.swg>
%import <biogears/cdm/patient/actions/SEAcuteStress.swg>
%import <biogears/cdm/patient/actions/SEAirwayObstruction.swg>
%import <biogears/cdm/patient/actions/SEApnea.swg>
%import <biogears/cdm/patient/actions/SEAsthmaAttack.swg>
%import <biogears/cdm/patient/actions/SEBrainInjury.swg>
%import <biogears/cdm/patient/actions/SEBronchoconstriction.swg>
%import <biogears/cdm/patient/actions/SEBurnWound.swg>
%import <biogears/cdm/patient/actions/SECardiacArrest.swg>
%import <biogears/cdm/patient/actions/SEChestCompressionForce.swg>
%import <biogears/cdm/patient/actions/SEChestCompressionForceScale.swg>
%import <biogears/cdm/patient/actions/SEChestOcclusiveDressing.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespiration.swg>
%import <biogears/cdm/patient/actions/SEConsumeNutrients.swg>
%import <biogears/cdm/patient/actions/SEEbola.swg>
%import <biogears/cdm/patient/actions/SEEscharotomy.swg>
%import <biogears/cdm/patient/actions/SEExercise.swg>
%import <biogears/cdm/patient/actions/SEHemorrhage.swg>
%import <biogears/cdm/patient/actions/SEInfection.swg>
%import <biogears/cdm/patient/actions/SEIntubation.swg>
%import <biogears/cdm/patient/actions/SEMechanicalVentilation.swg>
%import <biogears/cdm/patient/actions/SENeedleDecompression.swg>
%import <biogears/cdm/patient/actions/SEOverride.swg>
%import <biogears/cdm/patient/actions/SEPainStimulus.swg>
%import <biogears/cdm/patient/actions/SEPatientAssessmentRequest.swg>
%import <biogears/cdm/patient/actions/SEPericardialEffusion.swg>
%import <biogears/cdm/patient/actions/SEPulmonaryShunt.swg>
%import <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
%import <biogears/cdm/patient/actions/SESleep.swg>
%import <biogears/cdm/patient/actions/SESubstanceBolus.swg>
%import <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.swg>
%import <biogears/cdm/patient/actions/SESubstanceInfusion.swg>
%import <biogears/cdm/patient/actions/SESubstanceNasalDose.swg>
%import <biogears/cdm/patient/actions/SESubstanceOralDose.swg>
%import <biogears/cdm/patient/actions/SETensionPneumothorax.swg>
%import <biogears/cdm/patient/actions/SETourniquet.swg>
%import <biogears/cdm/patient/actions/SEUrinate.swg>
%import <biogears/cdm/system/physiology/SEGastrointestinalSystem.swg>

%nspace BIOGEARS_NAMESPACE SEPatientActionCollection;

%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::Load;

%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::RemoveHemorrhage(std::string const &);
%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::RemovePainStimulus(std::string const &);
%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::RemoveTourniquet(std::string const &);
%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::RemoveEscharotomy(std::string const&);

%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::GetAnesthesiaMachineActions();
%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::GetInhalerActions();
%ignore BIOGEARS_NAMESPACE SEPatientActionCollection::ProcessAction(const CDM::PatientActionData& in);

%ignore GetEscharotomiesWrapper() const;
%ignore GetHemorrhageWrapper() const;
%ignore GetPainStimuliWrapper() const;
%ignore GetSubstanceBolusesWrapper() const;
%ignore GetSubstanceInfusionsWrapper() const;
%ignore GetSubstanceNasalDosesWrapper() const;
%ignore GetSubstanceOralDosesWrapper() const;
%ignore GetSubstanceCompoundInfusionsWrapper() const;
%ignore GetTourniquetsWrapper() const;

%include <biogears/cdm/scenario/SEPatientActionCollection.h>

%nspace std::map<std::string, BIOGEARS_NAMESPACE SEHemorrhage*>;
%nspace std::map<std::string, BIOGEARS_NAMESPACE SETourniquet*>;
%nspace std::map<std::string, BIOGEARS_NAMESPACE SEPainStimulus*>;
%nspace std::map<std::string, BIOGEARS_NAMESPACE SEEscharotomy*>;

%traits_swigtype(SEHemorrhage);
%traits_swigtype(SETourniquet);
%traits_swigtype(SEPainStimulus);
%traits_swigtype(SEEscharotomy);

%fragment(SWIG_Traits_frag(SEHemorrhage));
%fragment(SWIG_Traits_frag(SETourniquet));
%fragment(SWIG_Traits_frag(SEPainStimulus));
%fragment(SWIG_Traits_frag(SEEscharotomy));

%traits_swigtype(SESubstanceBolus);
%traits_swigtype(SESubstanceInfusion);
%traits_swigtype(SESubstanceOralDose);
%traits_swigtype(SESubstanceNasalDose);
%traits_swigtype(SESubstanceCompoundInfusion);

%fragment(SWIG_Traits_frag(SESubstanceBolus));
%fragment(SWIG_Traits_frag(SESubstanceInfusion));
%fragment(SWIG_Traits_frag(SESubstanceOralDose));
%fragment(SWIG_Traits_frag(SESubstanceNasalDose));
%fragment(SWIG_Traits_frag(SESubstanceCompoundInfusion));

%traits_swigtype(SESubstance);
%traits_swigtype(SESubstanceCompound);

%fragment(SWIG_Traits_frag(SESubstance));
%fragment(SWIG_Traits_frag(SESubstanceCompound));

%nspace std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceBolus*> ;
%nspace std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceInfusion*> ;
%nspace std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceOralDose*> ;
%nspace std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceNasalDose*> ;
%nspace std::map<const BIOGEARS_NAMESPACE SESubstanceCompound*, BIOGEARS_NAMESPACE SESubstanceCompoundInfusion*> ;


%template (map_string__SEHemorrhage_ptr) std::map<std::string, BIOGEARS_NAMESPACE SEHemorrhage*> ;
%template (map_string__SEEscharotomy_ptr) std::map<std::string, BIOGEARS_NAMESPACE SEEscharotomy*> ;
%template (map_string__SETourniquet_ptr) std::map<std::string, BIOGEARS_NAMESPACE SETourniquet*> ;
%template (map_string__SEPainStimulus_ptr) std::map<std::string, BIOGEARS_NAMESPACE SEPainStimulus*> ;

%template (map_SESubstance_ptr__SESubstanceBolus_ptr) std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceBolus*> ;
%template (map_SESubstance_ptr__SESubstanceInfusion_ptr) std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceInfusion*> ;
%template (map_SESubstance_ptr__SESubstanceOralDose_ptr) std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceOralDose*> ;
%template (map_SESubstance_ptr__SESubstanceNasalDose_ptr) std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SESubstanceNasalDose*> ;
%template (map_SESubstanceCompound_ptr__SESubstanceCompoundInfusion_ptr) std::map<const BIOGEARS_NAMESPACE SESubstanceCompound*, BIOGEARS_NAMESPACE SESubstanceCompoundInfusion*> ;
