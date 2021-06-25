// Hello World! program

using System;
using biogears;
using CDM = mil.tatrc.physiology.datamodel;

namespace bio
{
  static class csharp_test
  {
    //!
    //! Very Terse testing of the SWIG_Bindings
    //!
    //! We will simply test the ability
    //!   1. Create a logger and log to a file
    //!   2. Initialize a BioGearsEngine
    //!   3. Advance Model Time 
    //!   4. Apply Actions to an exsiting BioGearsEngine
    //!   5. Create and Apply all BioGears Actions


    static void Main( string[] args ) {
      var logger = new Logger("");
      logger.Warning("Perfect Now lets Try to compliate things with two modules!");

      var percent = new SEScalar0To1();
      percent.SetValue(0.1);

      logger.Info(String.Format("{0}", percent.GetValue()));

      var time = new SEScalarTime();
      time.SetValue(4, TimeUnit.s);
      logger.Info(String.Format("{0}", time.GetValue()));


      var engine = new BioGearsEngine(logger);
      engine.InitializeEngine("StandardMale.xml");
      engine.AdvanceModelTime();
      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);

      var acuteStress = new SEAcuteStress();
      acuteStress.GetSeverity().SetValue(.75);

      logger.Warning(String.Format("AcuteStress @ Severity {0}", acuteStress.GetSeverity().GetValue()));
      engine.ProcessAction(acuteStress);

      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);

      var severity = acuteStress.GetSeverity();
      severity.SetValue(0.5);

      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);

      SEBurnWound m_burnWound = new SEBurnWound();
      m_burnWound.GetTotalBodySurfaceArea().SetValue(50.0 / 100.0);
      engine.ProcessAction(m_burnWound);
      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);


      var m_substances = engine.GetSubstanceManager();
      var m_albumine = m_substances.GetSubstance("Albumin");
      var m_saline = m_substances.GetCompound("Saline");

      try {
        var m_SubstanceBolus = new SESubstanceBolus(m_albumine);
        var m_SubstanceInfusion = new SESubstanceInfusion(m_albumine);
        var m_SubstanceNasalDose = new SESubstanceNasalDose(m_albumine);
        var m_SubstanceOralDose = new SESubstanceOralDose(m_albumine);
      } catch (global::System.Exception /*ex*/) {
        logger.Error("Definition of Albumin is missing.  Unable to test Substance Actions");
      }

      try {
        var m_SubstanceCompoundInfusion = new SESubstanceCompoundInfusion(m_saline);
      } catch (global::System.Exception /*ex*/) {
        logger.Error("Definition of Saline is missing.  Unable to test SubstanceCompound  Actions");
      }

      var m_TensionPneumothorax = new SETensionPneumothorax();
      var m_Tourniquet = new SETourniquet();
      var m_Urinate = new SEUrinate();
      var m_UseInhaler = new SEUseInhaler();

      var patient = engine.GetPatient();
      bool isTacycardia = patient.IsEventActive(CDM.enumPatientEvent.value.Tachycardia);
      bool isAlive = patient.IsEventActive(CDM.enumPatientEvent.value.IrreversibleState);
      bool isKetoacidosis = patient.IsEventActive(CDM.enumPatientEvent.value.Ketoacidosis);

      logger.Warning(String.Format("AcuteStress @ Severity {0}", acuteStress.GetSeverity().GetValue()));

      //!
      //! Very Quick Example of how to use Data Tracks to Poll data
      //! Performance wise this should really only be used for non PhysiologyDataRequest
      //!
      var HeartRateRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); HeartRateRequest.Set("HeartRate", FrequencyUnit.Per_min);
      var SystolicArterialPressureRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); SystolicArterialPressureRequest.Set("SystolicArterialPressure", PressureUnit.mmHg);
      var DiastolicArterialPressureRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); DiastolicArterialPressureRequest.Set("DiastolicArterialPressure", PressureUnit.mmHg);
      var RespirationRateRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); RespirationRateRequest.Set("RespirationRate", FrequencyUnit.Per_min);
      var TidalVolumeRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); TidalVolumeRequest.Set("TidalVolume", VolumeUnit.mL);
      var TotalLungVolumeRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); TotalLungVolumeRequest.Set("TotalLungVolume", VolumeUnit.mL);
      var OxygenSaturationRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); OxygenSaturationRequest.Set("OxygenSaturation");

      engine.GetEngineTrack().SetupRequests();

      var HeartRate = engine.GetEngineTrack().GetScalar(HeartRateRequest);
      var SystolicArterialPressure = engine.GetEngineTrack().GetScalar(HeartRateRequest);
      var DiastolicArterialPressure = engine.GetEngineTrack().GetScalar(DiastolicArterialPressureRequest);
      var RespirationRate = engine.GetEngineTrack().GetScalar(RespirationRateRequest);
      var TidalVolume = engine.GetEngineTrack().GetScalar(TidalVolumeRequest);
      var TotalLungVolume = engine.GetEngineTrack().GetScalar(TotalLungVolumeRequest);
      var OxygenSaturation = engine.GetEngineTrack().GetScalar(OxygenSaturationRequest);

      apply_acute_respiratory_distress(engine);

      engine.GetEngineTrack().PullData();
      logger.Info(HeartRate.ToString());
      logger.Info(SystolicArterialPressure.ToString());
      logger.Info(DiastolicArterialPressure.ToString());
      logger.Info(RespirationRate.ToString());
      logger.Info(TidalVolume.ToString());
      logger.Info(TotalLungVolume.ToString());
      logger.Info(OxygenSaturation.ToString());
      apply_acute_stress(engine);

      engine.GetEngineTrack().PullData();
      apply_airway_obstruction(engine);
      engine.GetEngineTrack().PullData();
      apply_apnea(engine);
      engine.GetEngineTrack().PullData();
      apply_asthma_attack(engine);
      engine.GetEngineTrack().PullData();
      apply_brain_injury(engine);
      engine.GetEngineTrack().PullData();
      apply_breath_hold(engine);
      engine.GetEngineTrack().PullData();
      apply_bronchoconstriction(engine);
      engine.GetEngineTrack().PullData();
      apply_burn_wound(engine);
      engine.GetEngineTrack().PullData();
      apply_cardiac_arrest(engine);
      engine.GetEngineTrack().PullData();
      logger.Info(HeartRate.ToString());
      logger.Info(SystolicArterialPressure.ToString());
      logger.Info(DiastolicArterialPressure.ToString());
      logger.Info(RespirationRate.ToString());
      logger.Info(TidalVolume.ToString());
      logger.Info(TotalLungVolume.ToString());
      logger.Info(OxygenSaturation.ToString());

      apply_chest_compression(engine);
      engine.GetEngineTrack().PullData();
      apply_chest_compression_force(engine);
      engine.GetEngineTrack().PullData();
      apply_chest_compression_force_scale(engine);
      engine.GetEngineTrack().PullData();
      apply_chest_occlusive_dressing(engine);
      engine.GetEngineTrack().PullData();
      apply_conscious_respiration(engine);
      engine.GetEngineTrack().PullData();
      apply_consume_nutrients(engine);
      engine.GetEngineTrack().PullData();
      apply_exercise(engine);
      engine.GetEngineTrack().PullData();
      apply_cycling_exercise(engine);
      engine.GetEngineTrack().PullData();
      apply_generic_exercise(engine);
      engine.GetEngineTrack().PullData();
      apply_running_exercise(engine);
      engine.GetEngineTrack().PullData();
      apply_strength_exercise(engine);
      engine.GetEngineTrack().PullData();
      apply_hemorrhage(engine);
      engine.GetEngineTrack().PullData();
      apply_infection(engine);
      engine.GetEngineTrack().PullData();
      apply_intubation(engine);
      engine.GetEngineTrack().PullData();
      apply_mechanical_ventilation(engine);
      engine.GetEngineTrack().PullData();
      apply_needle_decompression(engine);
      engine.GetEngineTrack().PullData();
      apply_override(engine);
      engine.GetEngineTrack().PullData();
      apply_pain_stimulus(engine);
      engine.GetEngineTrack().PullData();
      apply_patient_assessment_request(engine);
      engine.GetEngineTrack().PullData();
      apply_pericadial_effusion(engine);
      engine.GetEngineTrack().PullData();
      logger.Info(HeartRate.ToString());
      logger.Info(SystolicArterialPressure.ToString());
      logger.Info(DiastolicArterialPressure.ToString());
      logger.Info(RespirationRate.ToString());
      logger.Info(TidalVolume.ToString());
      logger.Info(TotalLungVolume.ToString());
      logger.Info(OxygenSaturation.ToString());
      apply_pulmonary_shunt(engine);
      engine.GetEngineTrack().PullData();
      apply_sleep(engine);
      engine.GetEngineTrack().PullData();
      logger.Info(HeartRate.ToString());
      logger.Info(SystolicArterialPressure.ToString());
      logger.Info(DiastolicArterialPressure.ToString());
      logger.Info(RespirationRate.ToString());
      logger.Info(TidalVolume.ToString());
      logger.Info(TotalLungVolume.ToString());
      logger.Info(OxygenSaturation.ToString());
    }

    //!
    //! Testing BioGears Actions  Each Action will be applied then reversed when possible. 
    //! engine [IN] - Valid BioGearsEngine
    //! 
    //! all apply_* functiosn take in a EnginePtr then apply an action and advance time 2 seconds
    //! When possible they reverse the action and advance time by another 2 seconds.
    //! 2 seconds is not really enough for any action to do anything notible
    //! But I want this demo to end in under 3 minutes.


    //!
    //! The backend code generation for BioGears XSD CodeSynthesis is really bad about not initializing its members
    //! For some legacy reaspon BioGears does not initialize its SE Types members with default values, so 
    //! If you try to process some actions with out setting the values to zero BioGears will segfault
    //! Our 8.0 Roadmap as we refactor our SE layer to hide implementation and expose Clone and Assignment operators

    public static void apply_and_cancel_patient_action( PhysiologyEngine engine, SEPatientAction on, SEPatientAction off ) {
      engine.ProcessAction(on);
      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);
      if (off.IsValid()) {
        engine.ProcessAction(off);
        engine.AdvanceModelTimeBy(2.0, TimeUnit.s);
      }

    }
    public static void apply_acute_respiratory_distress( BioGearsEngine engine ) {
      var on = new SEAcuteRespiratoryDistress();
      var off = new SEAcuteRespiratoryDistress();
      on.GetSeverity().SetValue(0.5);
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_acute_stress( BioGearsEngine engine ) {
      var on = new SEAcuteStress();
      var off = new SEAcuteStress();
      on.GetSeverity().SetValue(0.5);
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_airway_obstruction( BioGearsEngine engine ) {
      var on = new SEAirwayObstruction();
      var off = new SEAirwayObstruction();
      on.GetSeverity().SetValue(0.5);
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_apnea( BioGearsEngine engine ) {
      var on = new SEApnea();
      var off = new SEApnea();
      on.GetSeverity().SetValue(0.5);
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_asthma_attack( BioGearsEngine engine ) {
      var on = new SEAsthmaAttack();
      var off = new SEAsthmaAttack();
      on.GetSeverity().SetValue(0.5);
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_brain_injury( BioGearsEngine engine ) {
      var on = new SEBrainInjury();
      var off = new SEBrainInjury();
      on.GetSeverity().SetValue(0.5);
      on.SetBrainInjuryType(CDM.enumBrainInjuryType.value.LeftFocal);
      off.GetSeverity().SetValue(0.0);
      off.SetBrainInjuryType(CDM.enumBrainInjuryType.value.LeftFocal);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_breath_hold( BioGearsEngine engine ) {
      var on = new SEConsciousRespiration();
      var off = new SEConsciousRespiration();
      on.AddBreathHold().GetPeriod().SetValue(2, TimeUnit.s);
      off.AddBreathHold().GetPeriod().SetValue(0.0, TimeUnit.s);

      apply_and_cancel_patient_action(engine, on, off);
    }

    public static void apply_bronchoconstriction( BioGearsEngine engine ) {
      var on = new SEBronchoconstriction();
      var off = new SEBronchoconstriction();
      on.GetSeverity().SetValue(0.5);
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_burn_wound( BioGearsEngine engine ) {
      var on = new SEBurnWound();
      var off = new SEBurnWound();
      on.SetCompartment("Chest");
      on.SetInflammation(false);
      on.GetTotalBodySurfaceArea().SetValue(0.5);

      //Removing a BurnWound is a really bad idea
      //It causes issues with the inflamation model
      off.SetCompartment("Chest");
      off.SetInflammation(false);
      off.GetTotalBodySurfaceArea().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_cardiac_arrest( BioGearsEngine engine ) {
      var on = new SECardiacArrest();
      var off = new SECardiacArrest();
      on.SetActive(true);
      off.SetActive(false);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_chest_compression( BioGearsEngine engine ) {
      var on = new SEChestCompression();
      var off = new SEChestCompression();
      //Nothing to do
      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_chest_compression_force( BioGearsEngine engine ) {
      var on = new SEChestCompressionForce();
      var off = new SEChestCompressionForce();
      on.GetForce().SetValue(0.5, ForceUnit.N);
      off.GetForce().SetValue(0.0, ForceUnit.N);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_chest_compression_force_scale( BioGearsEngine engine ) {
      var on = new SEChestCompressionForceScale();
      var off = new SEChestCompressionForceScale();
      on.GetForcePeriod().SetValue(0.5, TimeUnit.s);
      on.GetForceScale().SetValue(0.5);
      off.GetForcePeriod().SetValue(0.0, TimeUnit.s);
      on.GetForceScale().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_chest_occlusive_dressing( BioGearsEngine engine ) {
      var on = new SEChestOcclusiveDressing();
      var off = new SEChestOcclusiveDressing();
      on.SetSide(CDM.enumSide.value.Left);
      off.SetSide(CDM.enumSide.value.Left);

      on.SetActive(true);
      off.SetActive(false);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_conscious_respiration( BioGearsEngine engine ) {
      var on = new SEConsciousRespiration();
      var off = new SEConsciousRespiration();
      var inhale_1 = on.AddForcedInhale();
      inhale_1.GetPeriod().SetValue(.125, TimeUnit.s);
      inhale_1.GetInspiratoryCapacityFraction().SetValue(.5);
      var exhale_1 = on.AddForcedExhale();
      exhale_1.GetPeriod().SetValue(.125, TimeUnit.s);
      exhale_1.GetExpiratoryReserveVolumeFraction().SetValue(.5);
      var inhale_2 = on.AddForcedInhale();
      inhale_2.GetPeriod().SetValue(.125, TimeUnit.s);
      inhale_2.GetInspiratoryCapacityFraction().SetValue(.5);
      var exhale_2 = on.AddForcedExhale();
      exhale_2.GetPeriod().SetValue(.125, TimeUnit.s);
      exhale_2.GetExpiratoryReserveVolumeFraction().SetValue(.5);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_consume_nutrients( BioGearsEngine engine ) {
      var on = new SEConsumeNutrients();
      var off = new SEConsumeNutrients();
      on.GetNutrition().GetCalcium().SetValue(5, MassUnit.g);
      on.GetNutrition().GetCarbohydrate().SetValue(5, MassUnit.g);
      on.GetNutrition().GetFat().SetValue(5, MassUnit.g);
      on.GetNutrition().GetProtein().SetValue(5, MassUnit.g);
      on.GetNutrition().GetWater().SetValue(100, VolumeUnit.mL);

      apply_and_cancel_patient_action(engine, on, off);
    }

    public static void apply_exercise( BioGearsEngine engine ) {
      apply_cycling_exercise(engine);
      apply_generic_exercise(engine);
      apply_cycling_exercise(engine);
      apply_strength_exercise(engine);
    }

    public static void apply_cycling_exercise( BioGearsEngine engine ) {
      var on = new SEExercise();
      var off = new SEExercise();

      var cycling = new SEExercise.SECycling();
      cycling.AddedWeight.SetValue(5, MassUnit.lb);
      cycling.PowerCycle.SetValue(20, PowerUnit.W);
      cycling.CadenceCycle.SetValue(50, FrequencyUnit.Hz);
      on.SetCyclingExercise(cycling);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_generic_exercise( BioGearsEngine engine ) {
      var on = new SEExercise();
      var off = new SEExercise();

      var generic = new SEExercise.SEGeneric();
      generic.DesiredWorkRate.SetValue(5, PowerUnit.W);
      try {
        generic.Intensity.SetValue(0.8);
      } catch (global::System.Exception /*ex*/) {
        engine.GetLogger().Error("Intensity is an SEScalar0To1 which throws a CDMException if the set value is greater then 1.0");
      }

      on.SetGenericExercise(generic);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_running_exercise( BioGearsEngine engine ) {
      var on = new SEExercise();
      var off = new SEExercise();

      var running = new SEExercise.SERunning();
      running.AddedWeight.SetValue(5, MassUnit.kg);
      running.InclineRun.SetValue(0.2);
      running.SpeedRun.SetValue(5, LengthPerTimeUnit.ft_Per_min);


      on.SetRunningExercise(running);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_strength_exercise( BioGearsEngine engine ) {
      var on = new SEExercise();
      var off = new SEExercise();

      var strength = new SEExercise.SEStrengthTraining();
      strength.RepsStrength.SetValue(5);
      strength.WeightStrength.SetValue(0.2, MassUnit.kg);

      on.SetStrengthExercise(strength);

      apply_and_cancel_patient_action(engine, on, off);
    }

    public static void apply_hemorrhage( BioGearsEngine engine ) {
      var on = new SEHemorrhage();
      var off = new SEHemorrhage();
      on.GetInitialRate().SetValue(0.5);
      on.SetCompartment("RightLeg");
      off.GetInitialRate().SetValue(0.0);
      off.SetCompartment("RightLeg");

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_infection( BioGearsEngine engine ) {
      var on = new SEInfection();
      var off = new SEInfection();
      on.SetLocation("Liver");
      on.SetSeverity(CDM.enumInfectionSeverity.value.Moderate);
      on.GetMinimumInhibitoryConcentration().SetValue(.5, MassPerVolumeUnit.g_Per_cm3);
      off.SetLocation("Liver");
      off.SetSeverity(CDM.enumInfectionSeverity.value.Eliminated);
      off.GetMinimumInhibitoryConcentration().SetValue(.0, MassPerVolumeUnit.g_Per_cm3);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_intubation( BioGearsEngine engine ) {
      var on = new SEIntubation();
      var off = new SEIntubation();
      on.SetIntubationType(CDM.enumIntubationType.value.Tracheal);
      off.SetIntubationType(CDM.enumIntubationType.value.Off);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_mechanical_ventilation( BioGearsEngine engine ) {
      var oxygen = engine.GetSubstanceManager().GetSubstance("Oxygen");

      var on = new SEMechanicalVentilation();
      var off = new SEMechanicalVentilation();
      on.SetState(CDM.enumOnOff.value.On);
      on.GetFlow().SetValue(30, VolumePerTimeUnit.L_Per_day);

      //Only access gas fractiosn this way to avoid memory management issues
      var oxygenFraction = on.GetGasFraction(oxygen);
      oxygenFraction.GetFractionAmount().SetValue(.5);

      on.GetPressure().SetValue(5, PressureUnit.mmHg);

      off.SetState(CDM.enumOnOff.value.Off);


      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_needle_decompression( BioGearsEngine engine ) {
      var on = new SENeedleDecompression();
      var off = new SENeedleDecompression();
      on.SetSide(CDM.enumSide.value.Left);
      on.SetActive(true);
      off.SetSide(CDM.enumSide.value.Left);
      off.SetActive(false);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_override( BioGearsEngine engine ) {
      var on = new SEOverride();
      var off = new SEOverride();
      on.SetOverrideState(CDM.enumOnOff.value.On);
      on.GetHeartRateOverride().SetValue(72, FrequencyUnit.Per_min);
      off.SetOverrideState(CDM.enumOnOff.value.Off);


      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_pain_stimulus( BioGearsEngine engine ) {
      var on = new SEPainStimulus();
      var off = new SEPainStimulus();
      on.SetLocation("Neck");
      on.GetSeverity().SetValue(0.5);
      off.SetLocation("Neck");
      off.GetSeverity().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_patient_assessment_request( BioGearsEngine engine ) {
      var CompleteBloodCount = new SEPatientAssessmentRequest();
      var ComprehensiveMetabolicPanel = new SEPatientAssessmentRequest();
      var PulmonaryFunctionTest = new SEPatientAssessmentRequest();
      var SequentialOrganFailureAssessment = new SEPatientAssessmentRequest();
      var Urinalysis = new SEPatientAssessmentRequest();

      //!
      //! AssessmentRequest produces an XML format of CDM::PatientAssessment
      //!
      //! Use PhysiologyEngine::GetPatientAssessment to populate a SEPatientAssessment*
      //! For Programatic Use
      //!
      CompleteBloodCount.SetPatientAssessmentType(CDM.enumPatientAssessment.value.CompleteBloodCount);
      ComprehensiveMetabolicPanel.SetPatientAssessmentType(CDM.enumPatientAssessment.value.ComprehensiveMetabolicPanel);
      PulmonaryFunctionTest.SetPatientAssessmentType(CDM.enumPatientAssessment.value.PulmonaryFunctionTest);
      SequentialOrganFailureAssessment.SetPatientAssessmentType(CDM.enumPatientAssessment.value.SequentialOrganFailureAssessment);
      Urinalysis.SetPatientAssessmentType(CDM.enumPatientAssessment.value.Urinalysis);

      engine.ProcessAction(CompleteBloodCount);
      engine.AdvanceModelTimeBy(1, TimeUnit.s);

      engine.ProcessAction(ComprehensiveMetabolicPanel);
      engine.AdvanceModelTimeBy(1, TimeUnit.s);

      engine.ProcessAction(PulmonaryFunctionTest);
      engine.AdvanceModelTimeBy(1, TimeUnit.s);

      engine.ProcessAction(SequentialOrganFailureAssessment);
      engine.AdvanceModelTimeBy(1, TimeUnit.s);

      engine.ProcessAction(Urinalysis);
      engine.AdvanceModelTimeBy(1, TimeUnit.s);
    }
    public static void apply_pericadial_effusion( BioGearsEngine engine ) {
      var on = new SEPericardialEffusion();
      var off = new SEPericardialEffusion();
      on.GetEffusionRate().SetValue(0.5, VolumePerTimeUnit.mL_Per_s);
      off.GetEffusionRate().SetValue(0.0, VolumePerTimeUnit.mL_Per_s);
      //1.0 milliliters per second
      apply_and_cancel_patient_action(engine, on, off);
    }
    public static void apply_pulmonary_shunt( BioGearsEngine engine ) {
      var on = new SEPulmonaryShunt();
      var off = new SEPulmonaryShunt();
      on.GetFlowRateScale().SetValue(0.5);
      off.GetFlowRateScale().SetValue(0.0);

      apply_and_cancel_patient_action(engine, on, off);
    }

    public static void apply_sleep( BioGearsEngine engine ) {
      var on = new SESleep();
      var off = new SESleep();
      on.SetSleepState(CDM.enumOnOff.value.On);
      off.SetSleepState(CDM.enumOnOff.value.Off);

      apply_and_cancel_patient_action(engine, on, off);
    }
  }
}
