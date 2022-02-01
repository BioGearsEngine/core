%import "windows.i"

#pragma SWIG nowarn=320

#if defined(SWIGCSHARP)
  %import "csharp/std_string.i"
  %import "csharp/std_vector.i"
  %import "csharp/std_map.i"
  %import "csharp/std_except.i"

namespace std 
{
  %ignore runtime_error;
  struct runtime_error {};
}

#elif defind(SWIGPYTHON)
  %import "python/std_string.i"
  %import "python/std_vector.i"
  %import "python/std_map.i"
  %import "python/std_except.i"
#else
  %import "std_string.i"
  %import "std_vector.i"
  %import "std_map.i"
  %import "std_except.i"
#endif 
%import"exception.i"


%{
#include <exception>
#include<biogears/cdm/CommonDataModel.h>
#include <biogears/schema/cdm/Anesthesia.hxx>
#include <biogears/schema/cdm/Environment.hxx>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/schema/cdm/PatientActions.hxx>
#include <biogears/schema/cdm/PatientAssessments.hxx>
#include <biogears/schema/cdm/Physiology.hxx>
#include <biogears/schema/cdm/Substance.hxx>
%}

%include <biogears/exports.h>

%nspace "biogears::CommonDataModelException"

%ignore biogears::CommonDataModelException::CommonDataModelException(std::string const &);

%ignore Unload;
%ignore Load; 
%ignore ToString(std::ostream& ); 
%ignore ToString(std::ostream& str) const;

%ignore  operator+;
%ignore  operator+=;
%ignore  operator-;
%ignore  operator-=;
%ignore  operator/;
%ignore  operator/=;
%ignore  operator*;
%ignore  operator*=;

%ignore operator<;
%ignore operator<=;
%ignore operator>;
%ignore operator>=;
%ignore  operator==;
%ignore  operator!=;

%ignore  operator<<;
%ignore  operator>>;

#if defined(SWIGCSHARP)
%csmethodmodifiers ToString() "public override";
#endif 

%nspace std::vector<double>;
%nspace std::vector<unsigned int>;
%nspace std::map<std::string, std::vector<unsigned int> >;
%template(vector_d) std::vector<double>;
%template(vector_ui) std::vector<unsigned int>;
%template(map_string_vector_ui) std::map<std::string, std::vector<unsigned int> >; 
%include <biogears/cdm/CommonDataModel.h>

namespace mil {
namespace tatrc {
  namespace physiology {
   namespace datamodel {
    //Anesthesia.hxx
    class enumAnesthesiaMachineEvent {
    public:
      enum value {
        OxygenBottle1Exhausted,
        OxygenBottle2Exhausted,
        ReliefValveActive
      };
    };
    class enumAnesthesiaMachineOxygenSource {
    public:
      enum value {
        Wall,
        BottleOne,
        BottleTwo
      };
    };
    class enumAnesthesiaMachinePrimaryGas {
    public:
      enum value {
        Air,
        Nitrogen
      };
    };
    class enumAnesthesiaMachineConnection {
    public:
      enum value {
        Off,
        Mask,
        Tube
      };
    };
    //ElectroCardioGram.hxx
    class ElectroCardioGramWaveformLeadNumber {
    
    };
    //Environment.hxx
    class enumSurroundingType {
    public:
      enum value
      {
        Air,
        Water
      };
    };
    //Properties.hxx
    class  enumDecimalFormat {
    public:
      enum value
      {
        FixedMantissa,
        SignificantDigits
      };
    };
    class enumSide {
    public:
      enum value {
        Left,
        Right
      };
    };
    class enumOpenClosed {
    public:
      enum value {
        Closed,
        Open
      };
    };
    class  enumOnOff {
    public:
      enum value {
        Off,
        On
      };
    };
    class enumErrorType {
    public:
      enum value {
        Standard
      };  
    };    
    //Patient.hxx
    class  enumSex {
    public:
      enum value {
        Male,
        Female
      };
    };
    class enumBloodType {
    public:
      enum value {
        A,
        B,
        AB,
        O
      };
    };
    class enumPatientEvent {
    public:
      enum value {
        AcuteLungInjury,
        AcuteRespiratoryDistress,
        Antidiuresis,
        Asystole,
        Bradycardia,
        Bradypnea,
        BrainOxygenDeficit,
        CardiacArrest,
        CardiogenicShock,
        CompartmentSyndrome_LeftArm,
        CompartmentSyndrome_RightArm,
        CompartmentSyndrome_LeftLeg,
        CompartmentSyndrome_RightLeg,
        CompartmentSyndrome_Abdominal,
        CriticalBrainOxygenDeficit,
        Dehydration,
        Diuresis,
        MildDiarrhea,
        SevereDiarrhea,
        Fasciculation,
        Fatigue,
        FlaccidParalysis,
        FunctionalIncontinence,
        HemolyticTransfusionReaction,
        Hypercapnia,
        Hyperglycemia,
        MildDiaphoresis,
        ModerateDiaphoresis,
        SevereDiaphoresis,
        MildHyperkalemia,
        SevereHyperkalemia,
        MildHypernatremia,
        SevereHypernatremia,
        Hyperthermia,
        Hypoglycemia,
        HypoglycemicShock,
        HypoglycemicComa,
        Hypothermia,
        MildHypokalemia,
        SevereHypokalemia,
        MildHyponatremia,
        SevereHyponatremia,
        Hypoxia,
        HypovolemicShock,
        IntracranialHypertension,
        IntracranialHypotension,
        IrreversibleState,
        Ketoacidosis,
        LacticAcidosis,
        LiverGlycogenDepleted,
        MaximumPulmonaryVentilationRate,
        MetabolicAcidosis,
        MetabolicAlkalosis,
        MildWeakness,
        ModerateWeakness,
        MildSecretions,
        ModerateSecretions,
        MuscleCatabolism,
        MuscleGlycogenDepleted,
        MyocardiumOxygenDeficit,
        Natriuresis,
        Nausea,
        NutritionDepleted,
        PulselessRhythm,
        RenalHypoperfusion,
        RespiratoryAcidosis,
        RespiratoryAlkalosis,
        SevereAcuteRespiratoryDistress,
        SevereSecretions,
        Seizures,
        StartOfCardiacCycle,
        StartOfExhale,
        StartOfInhale,
        SevereSepsis,
        Tachycardia,
        Tachypnea,
        Vomiting
      };
    };
    //PatientActions.hxx
    class enumPatientAssessment {
    public:
      enum value{
        PulmonaryFunctionTest,
        CompleteBloodCount,
        ComprehensiveMetabolicPanel,
        SequentialOrganFailureAssessment,
        Urinalysis
      };
    };
    class  enumBrainInjuryType {
    public:
      enum value {
        Diffuse,
        LeftFocal,
        RightFocal
      };
    };
    class  enumInfectionSeverity {
    public:
      enum value {
        Eliminated,
        Mild,
        Moderate,
        Severe
      };
    };
    class enumIntubationType {
    public:
      enum value {
        Off,
        Esophageal,
        LeftMainstem,
        RightMainstem,
        Tracheal
      };
    };
    class  enumPneumothoraxType {
    public:
      enum value {
        Open,
        Closed
      };
    };
    class  enumBolusAdministration {
    public:  
      enum value {
        Intraarterial,
        Intramuscular,
        Intravenous
      };
    };
    class enumOralAdministration  {
    public:
      enum value {
        Transmucosal,
        Gastrointestinal
      };
    };
    class  enumTourniquetApplicationLevel  {
    public:
      enum value {
        Applied,
        Misapplied,
        None
      };
    };
    //Patient Assessment
    class  enumPresenceIndicator {
    public:
      enum value {
        Positive,
        Negative
      };
    };
    class enumClarityIndicator {
    public:
      enum value {
        Clear,
        SlightlyCloudy,
        Cloudy,
        Turbid
      };
    };
    class enumUrineColor {
    public:
      enum value {
        PaleYellow,
        Yellow,
        DarkYellow,
        Pink
      };
    };
    class enumMicroscopicObservationType {
    public:
      enum value {
          LowPowerField,
          HighPowerField
        };
    };
    class enumMicroscopicObservationAmount {
    public:
      enum value {
          Few,
          Moderate,
          Many
      };
    };
    //Physiology
    class  enumInflammationSource {
    public:
      enum value {
        Burn,
        Hemorrhage,
        Infection
      };
    };
    class enumHeartRhythm {
    public:
      enum value {
        Asystole,
        NormalSinus
      };
    };
    class enumSleepState {
    public:
      enum value
      {
        Awake,
        Asleep
      };
    };
    //Substance
    class  enumSubstanceState{
    public:
      enum value {
          Solid,
          Liquid,
          Gas,
          Molecular
        };
    };
    class enumSubstanceIonicState {
    public:
      enum value
        {
          WeakBase,
          Base,
          Acid,
          Neutral,
          Zwitterion
        };
    };
    class enumSubstanceBindingProtein {
    public:
      enum value {
        Albumin,
        Lipoprotein,
        AAG
      };
    };
    class enumCharge {
    public:
      enum value
      {
        Positive,
        Negative,
        Neutral
      };
    };
    class enumSubstanceClass {
    public:
      enum value {
        Anesthetic,
        Antibiotic,
        WholeBlood,
        Ion,
        Opioid,
        ReversalAgent,
        Sedative
      };
    };
  }
  }
}
}

%nspace std::vector<CDM::enumInflammationSource>;
%template (vector_enumInflammationSource) std::vector<CDM::enumInflammationSource>;
%nspace std::map<CDM::enumPatientEvent::value, bool>;
%template (map_enumPatientEvent__bool) std::map<CDM::enumPatientEvent::value, bool>;
%nspace std::map<CDM::enumPatientEvent::value, double>;
%template (map_enumPatientEvent__double) std::map<CDM::enumPatientEvent::value, double>;
%nspace std::map<CDM::enumAnesthesiaMachineEvent::value, bool>;
%template (map_enumAnesthesiaMachineEvent__bool) std::map<CDM::enumAnesthesiaMachineEvent::value, bool>;
%nspace std::map<CDM::enumAnesthesiaMachineEvent::value, double>;
%template (map_enumAnesthesiaMachineEvent__double) std::map<CDM::enumAnesthesiaMachineEvent::value, double>;
