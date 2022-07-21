%{
#include <biogears/cdm/scenario/SEPatientActionCollection.h>
%}

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

%nspace biogears::SEPatientActionCollection;

%ignore biogears::SEPatientActionCollection::Unload;
%ignore biogears::SEPatientActionCollection::Load;

%ignore biogears::SEPatientActionCollection::RemoveHemorrhage(std::string const &);
%ignore biogears::SEPatientActionCollection::RemovePainStimulus(std::string const &);
%ignore biogears::SEPatientActionCollection::RemoveTourniquet(std::string const &);
%ignore biogears::SEPatientActionCollection::RemoveEscharotomy(std::string const&);

%ignore biogears::SEPatientActionCollection::GetAnesthesiaMachineActions();
%ignore biogears::SEPatientActionCollection::GetInhalerActions();
%ignore biogears::SEPatientActionCollection::ProcessAction(const CDM::PatientActionData& in);

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

%nspace std::map<std::string, biogears::SEHemorrhage*>;
%nspace std::map<std::string, biogears::SETourniquet*>;
%nspace std::map<std::string, biogears::SEPainStimulus*>;
%nspace std::map<std::string, biogears::SEEscharotomy*>;

%nspace std::map<const biogears::SESubstance*, biogears::SESubstanceBolus*> ;
%nspace std::map<const biogears::SESubstance*, biogears::SESubstanceInfusion*> ;
%nspace std::map<const biogears::SESubstance*, biogears::SESubstanceOralDose*> ;
%nspace std::map<const biogears::SESubstance*, biogears::SESubstanceNasalDose*> ;
%nspace std::map<const biogears::SESubstanceCompound*, biogears::SESubstanceCompoundInfusion*> ;

%template (map_string__SEHemorrhage_ptr) std::map<std::string, biogears::SEHemorrhage*> ;
%template (map_string__SEEscharotomy_ptr) std::map<std::string, biogears::SEEscharotomy*> ;
%template (map_string__SETourniquet_ptr) std::map<std::string, biogears::SETourniquet*> ;
%template (map_string__SEPainStimulus_ptr) std::map<std::string, biogears::SEPainStimulus*> ;

%template (map_SESubstance_ptr__SESubstanceBolus_ptr) std::map<const biogears::SESubstance*, biogears::SESubstanceBolus*> ;
%template (map_SESubstance_ptr__SESubstanceInfusion_ptr) std::map<const biogears::SESubstance*, biogears::SESubstanceInfusion*> ;
%template (map_SESubstance_ptr__SESubstanceOralDose_ptr) std::map<const biogears::SESubstance*, biogears::SESubstanceOralDose*> ;
%template (map_SESubstance_ptr__SESubstanceNasalDose_ptr) std::map<const biogears::SESubstance*, biogears::SESubstanceNasalDose*> ;
%template (map_SESubstanceCompound_ptr__SESubstanceCompoundInfusion_ptr) std::map<const biogears::SESubstanceCompound*, biogears::SESubstanceCompoundInfusion*> ;
