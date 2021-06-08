// Hello World! program

using System;
using biogears;
using CDM = mil.tatrc.physiology.datamodel;

namespace bio
{
    class csharp_test
    {
        static void Main(string[] args)
        {
            var logger = new Logger("");
            logger.Warning("Perfect Now lets Try to compliate things with two modules!");

            var percent = new SEScalar0To1();
            percent.SetValue(0.1);

            logger.Info(String.Format("{0}",percent.GetValue()));

            var time = new SEScalarTime();
            time.SetValue(4, TimeUnit.s);
            logger.Info(String.Format("{0}",time.GetValue()));


            var engine = new BioGearsEngine(logger);
            engine.InitializeEngine("StandardMale.xml");
            engine.AdvanceModelTime();
            engine.AdvanceModelTimeBy( 2.0, TimeUnit.s);

            var acuteStress = new SEAcuteStress();
            acuteStress.GetSeverity().SetValue(.75);

            logger.Warning(String.Format("AcuteStress @ Severity {0}",acuteStress.GetSeverity().GetValue()));
            engine.ProcessAction(acuteStress);

            engine.AdvanceModelTimeBy( 2.0, TimeUnit.s);

            var severity = acuteStress.GetSeverity();
            severity.SetValue(0.5);

            engine.AdvanceModelTimeBy( 2.0, TimeUnit.s);

            SEBurnWound m_burnWound = new SEBurnWound();
            m_burnWound.GetTotalBodySurfaceArea().SetValue(50.0 / 100.0);
            engine.ProcessAction(m_burnWound);
            engine.AdvanceModelTimeBy( 2.0, TimeUnit.s);


            var m_AcuteRespiratoryDistress = new SEAcuteRespiratoryDistress();
            var m_AcuteStress = new SEAcuteStress();
            var m_AirwayObstruction = new SEAirwayObstruction();
            var m_Apnea = new SEApnea();
            var m_AsthmaAttack = new SEAsthmaAttack();
            var m_BrainInjury = new SEBrainInjury();
            var m_BreathHold = new SEBreathHold();
            var m_Bronchoconstriction = new SEBronchoconstriction();
            var m_BurnWound = new SEBurnWound();
            var m_CardiacArrest = new SECardiacArrest();
            var m_ChestCompression = new SEChestCompression();
            var m_ChestCompressionForce = new SEChestCompressionForce();
            var m_ChestCompressionForceScale = new SEChestCompressionForceScale();
            var m_ChestOcclusiveDressing = new SEChestOcclusiveDressing();
            var m_ConsciousRespiration = new SEConsciousRespiration();

            var m_ConsumeNutrients = new SEConsumeNutrients();
            var m_Exercise = new SEExercise();
            var m_ForcedExhale = new SEForcedExhale();
            var m_ForcedInhale = new SEForcedInhale();
            var m_Hemorrhage = new SEHemorrhage();
            var m_Infection = new SEInfection();
            var m_Intubation = new SEIntubation();
            var m_MechanicalVentilation = new SEMechanicalVentilation();
            var m_NeedleDecompression = new SENeedleDecompression();
            var m_Override = new SEOverride();
            var m_PainStimulus = new SEPainStimulus();


            var m_PatientAssessmentRequest = new SEPatientAssessmentRequest();
            var m_PericardialEffusion = new SEPericardialEffusion();
            var m_PulmonaryShunt = new SEPulmonaryShunt();
            var m_PupillaryResponse = new SEPupillaryResponse();
            var m_Sleep = new SESleep();

            var m_substances = engine.GetSubstanceManager();
            var m_albumine = m_substances.GetSubstance("Albumine");
            var m_saline = m_substances.GetCompound("Saline");

            var m_SubstanceBolus = new SESubstanceBolus(m_albumine);
            var m_SubstanceCompoundInfusion = new SESubstanceCompoundInfusion(m_saline);
            var m_SubstanceInfusion = new SESubstanceInfusion(m_albumine);
            var m_SubstanceNasalDose = new SESubstanceNasalDose(m_albumine);
            var m_SubstanceOralDose = new SESubstanceOralDose(m_albumine);
            var m_TensionPneumothorax = new SETensionPneumothorax();
            var m_Tourniquet = new SETourniquet();
            var m_Urinate = new SEUrinate();
            var m_UseInhaler = new SEUseInhaler();

            var patient = engine.GetPatient();
            bool isTacycardia = patient.IsEventActive(CDM.enumPatientEvent.value.Tachycardia);
            bool isAlive = patient.IsEventActive(CDM.enumPatientEvent.value.IrreversibleState);
            bool isKetoacidosis = patient.IsEventActive(CDM.enumPatientEvent.value.Ketoacidosis);

            logger.Warning(String.Format("AcuteStress @ Severity {0}",acuteStress.GetSeverity().GetValue()));

        }
    }
}
