// Hello World! program

using System;
using System.Text;

using biogears;
using CDM = mil.tatrc.physiology.datamodel;

namespace bio
{

  public class BioGearsEventHandler : biogears.SEEventHandler
  {
    static int g_event_count = 0;

    public static string ToString( CDM.enumPatientEvent.value ev ) {

      switch (ev) {
      case CDM.enumPatientEvent.value.AcuteLungInjury:
        return "AcuteLungInjury";

      case CDM.enumPatientEvent.value.AcuteRespiratoryDistress:
        return "AcuteRespiratoryDistress";

      case CDM.enumPatientEvent.value.Antidiuresis:
        return "Antidiuresis";

      case CDM.enumPatientEvent.value.Asystole:
        return "Asystoley";

      case CDM.enumPatientEvent.value.Bradycardia:
        return "Bradycardia";

      case CDM.enumPatientEvent.value.Bradypnea:
        return "Bradypnea";

      case CDM.enumPatientEvent.value.BrainOxygenDeficit:
        return "BrainOxygenDeficit";

      case CDM.enumPatientEvent.value.CardiacArrest:
        return "CardiacArrest";

      case CDM.enumPatientEvent.value.CardiogenicShock:
        return "CardiogenicShock";

      case CDM.enumPatientEvent.value.CriticalBrainOxygenDeficit:
        return "CriticalBrainOxygenDeficit";

      case CDM.enumPatientEvent.value.Dehydration:
        return "Dehydration";

      case CDM.enumPatientEvent.value.MildDiaphoresis:
        return "MildDiaphoresis";

      case CDM.enumPatientEvent.value.ModerateDiaphoresis:
        return "ModerateDiaphoresis";

      case CDM.enumPatientEvent.value.SevereDiaphoresis:
        return "SevereDiaphoresis";

        case CDM.enumPatientEvent.value.Diuresis:
        return "AcuteLungInjury";

      case CDM.enumPatientEvent.value.Fasciculation:
        return "Fasciculation";

      case CDM.enumPatientEvent.value.Fatigue:
        return "Fatigue";

      case CDM.enumPatientEvent.value.FlaccidParalysis:
        return "FlaccidParalysis";

        case CDM.enumPatientEvent.value.FunctionalIncontinence:
        return "FunctionalIncontinence";

      case CDM.enumPatientEvent.value.HemolyticTransfusionReaction:
        return "HemolyticTransfusionReaction";

      case CDM.enumPatientEvent.value.Hypercapnia:
        return "Hypercapnia";

      case CDM.enumPatientEvent.value.Hyperglycemia:
        return "Hyperglycemia";

      case CDM.enumPatientEvent.value.MildHyperkalemia:
        return "MildHyperkalemia";

      case CDM.enumPatientEvent.value.SevereHyperkalemia:
        return "SevereHyperkalemia";

      case CDM.enumPatientEvent.value.MildHypernatremia:
        return "MildHypernatremia";

      case CDM.enumPatientEvent.value.SevereHypernatremia:
        return "SevereHypernatremia";

      case CDM.enumPatientEvent.value.Hyperthermia:
        return "Hyperthermia";

      case CDM.enumPatientEvent.value.Hypoglycemia:
        return "Hypoglycemia";

      case CDM.enumPatientEvent.value.HypoglycemicShock:
        return "HypoglycemicShock";

      case CDM.enumPatientEvent.value.HypoglycemicComa:
        return "HypoglycemicComa";

      case CDM.enumPatientEvent.value.Hypothermia:
        return "Hypothermia";

      case CDM.enumPatientEvent.value.MildHypokalemia:
        return "MildHypokalemia";

      case CDM.enumPatientEvent.value.SevereHypokalemia:
        return "SevereHypokalemia";

      case CDM.enumPatientEvent.value.MildHyponatremia:
        return "MildHyponatremia";

      case CDM.enumPatientEvent.value.SevereHyponatremia:
        return "SevereHyponatremia";

      case CDM.enumPatientEvent.value.Hypoxia:
        return "Hypoxia";

      case CDM.enumPatientEvent.value.HypovolemicShock:
        return "HypovolemicShock";

      case CDM.enumPatientEvent.value.IntracranialHypertension:
        return "IntracranialHypertension";

      case CDM.enumPatientEvent.value.IntracranialHypotension:
        return "IntracranialHypotension";

      case CDM.enumPatientEvent.value.IrreversibleState:
        return "IrreversibleState";

      case CDM.enumPatientEvent.value.Ketoacidosis:
        return "Ketoacidosis";

      case CDM.enumPatientEvent.value.LacticAcidosis:
        return "LacticAcidosis";

      case CDM.enumPatientEvent.value.LiverGlycogenDepleted:
        return "LiverGlycogenDepleted";

      case CDM.enumPatientEvent.value.MaximumPulmonaryVentilationRate:
        return "MaximumPulmonaryVentilationRate";

      case CDM.enumPatientEvent.value.MetabolicAcidosis:
        return "MetabolicAcidosis";

      case CDM.enumPatientEvent.value.MetabolicAlkalosis:
        return "MetabolicAlkalosis";

      case CDM.enumPatientEvent.value.MuscleCatabolism:
        return "MuscleCatabolism";

      case CDM.enumPatientEvent.value.MuscleGlycogenDepleted:
        return "MuscleGlycogenDepleted";

      case CDM.enumPatientEvent.value.MyocardiumOxygenDeficit:
        return "MyocardiumOxygenDeficit";

      case CDM.enumPatientEvent.value.Natriuresis:
        return "Natriuresis";

      case CDM.enumPatientEvent.value.Nausea:
        return "Nausea";

        case CDM.enumPatientEvent.value.NutritionDepleted:
        return "NutritionDepleted";

      case CDM.enumPatientEvent.value.PulselessRhythm:
        return "PulselessRhythm";

      case CDM.enumPatientEvent.value.RenalHypoperfusion:
        return "RenalHypoperfusion";

      case CDM.enumPatientEvent.value.RespiratoryAcidosis:
        return "RespiratoryAcidosis";

      case CDM.enumPatientEvent.value.RespiratoryAlkalosis:
        return "RespiratoryAlkalosis";

      case CDM.enumPatientEvent.value.SevereAcuteRespiratoryDistress:
        return "SevereAcuteRespiratoryDistress";

      case CDM.enumPatientEvent.value.StartOfCardiacCycle:
        return "AcuteLungInjury";

      case CDM.enumPatientEvent.value.StartOfExhale:
        return "StartOfExhale";

      case CDM.enumPatientEvent.value.StartOfInhale:
        return "StartOfInhale";

      case CDM.enumPatientEvent.value.MildSecretions:
        return "MildSecretions";

      case CDM.enumPatientEvent.value.ModerateSecretions:
        return "ModerateSecretions";

      case CDM.enumPatientEvent.value.SevereSecretions:
        return "SevereSecretions";

        case CDM.enumPatientEvent.value.Seizures:
        return "Seizures";

      case CDM.enumPatientEvent.value.SevereSepsis:
        return "SevereSepsis";

      case CDM.enumPatientEvent.value.Tachycardia:
        return "Tachycardia";

      case CDM.enumPatientEvent.value.Tachypnea:
        return "Tachypnea";

      case CDM.enumPatientEvent.value.MildWeakness:
        return "MildWeakness";

      case CDM.enumPatientEvent.value.ModerateWeakness:
        return "ModerateWeakness";

      case CDM.enumPatientEvent.value.Vomiting:
        return "Vomiting";

        default:
        return "";

      }

    }

    public static string ToString( CDM.enumAnesthesiaMachineEvent.value ev ) {
      switch (ev) {
      case CDM.enumAnesthesiaMachineEvent.value.OxygenBottle1Exhausted:
        return "OxygenBottle1Exhausted";
      case CDM.enumAnesthesiaMachineEvent.value.OxygenBottle2Exhausted:
        return "OxygenBottle2Exhausted";
      case CDM.enumAnesthesiaMachineEvent.value.ReliefValveActive:
        return "ReliefValveActive";
      default:
        return "";
      }
    }

    public BioGearsEventHandler() : base() {
    }

    public override void HandlePatientEvent( CDM.enumPatientEvent.value type, bool active, biogears.SEScalarTime time ) {
      if (g_event_count++ % 31 != 0) {
        return;
      }

      Console.WriteLine("\n");
      Console.WriteLine("  OO O o o o...      _______________________________________ ");
      Console.WriteLine("  O     ____          |                                      |");

      string ev = ToString(type);
      var marquee_1 = new StringBuilder("                                      ");
      var marquee_2 = new StringBuilder("                                      ");
      string prefix = " ][_n_i_| (   ooo___  |";
      string end = "|";


      string state = (active) ? "(On)" : "(Off)";
      if (ev.Length < marquee_1.Length) {
        marquee_1.Remove((marquee_1.Length / 2) - (ev.Length / 2), ev.Length);
        marquee_1.Insert((marquee_1.Length / 2), ev);
      } else {
        marquee_1 = new StringBuilder(ev);
      }
      if (state.Length < marquee_2.Length) {
        marquee_2.Remove((marquee_2.Length / 2) - (state.Length / 2), state.Length);
        marquee_2.Insert((marquee_2.Length / 2), state);
      } else {
        marquee_1 = new StringBuilder(state);
      }
      Console.WriteLine(prefix + marquee_1 + end);
      Console.WriteLine(prefix + marquee_2 + end);
      Console.WriteLine("(__________|_[______]_|______________________________________|");
      Console.WriteLine("  0--0--0      0  0      0       0     0        0        0    ");
      Console.WriteLine("\n");
    }

    public override void HandlePatientEvent( CDM.enumPatientEvent.value type, bool active ) {
      var eventTime = new SEScalarTime();
      eventTime.SetValue(0, TimeUnit.s);
      HandlePatientEvent(type, active, eventTime);
    }

    public override void HandleAnesthesiaMachineEvent( CDM.enumAnesthesiaMachineEvent.value type, bool active, biogears.SEScalarTime time ) {
      if (g_event_count++ % 31 != 0) {
        return;
      }

      Console.WriteLine(@"");
      Console.WriteLine(@"                           (_)");
      Console.WriteLine(@"            --""-------   0/      ^^");
      Console.WriteLine(@"  .___...../ /__| |__\ \_/H__,      ^^");

      string ev = ToString(type);
      var marquee_1 = new StringBuilder(@"                        |");
      var marquee_2 = new StringBuilder(@"                        /");
      string prefix_1 =  @"|";
      string prefix_2 =  @"\";
      string end_1 = "|";
      string end_2 = "/";


      string state = (active) ? "(On)" : "(Off)";
      if (ev.Length < marquee_1.Length) {
        marquee_1.Remove((marquee_1.Length / 2) - (ev.Length / 2), ev.Length);
        marquee_1.Insert((marquee_1.Length / 2) - (ev.Length / 2), ev);
      } else {
        marquee_1 = new StringBuilder(ev);
      }
      if (state.Length < marquee_2.Length) {
        marquee_2.Remove((marquee_2.Length / 2) - (state.Length / 2), state.Length);
        marquee_2.Insert((marquee_2.Length / 2) - (state.Length / 2), state);
      } else {
        marquee_1= new StringBuilder(state) ;
      }
      Console.WriteLine(prefix_1 + marquee_1 + end_1);
      Console.WriteLine(prefix_2 + marquee_2 + end_2);

      Console.WriteLine(@"#####^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~\O/~~\Q/~^~^~rr\n");
      Console.WriteLine("\n");
    }


    public override void HandleAnesthesiaMachineEvent( CDM.enumAnesthesiaMachineEvent.value type, bool active ) {
      var eventTime = new SEScalarTime();
      eventTime.SetValue(0, TimeUnit.s);
      HandleAnesthesiaMachineEvent(type, active, eventTime);
    }
  };


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

      var eh = new BioGearsEventHandler();
      engine.SetEventHandler(eh);

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


      //TODO: Get Substance and GetSubstanceCompound need better exception handeling
      var oxygen = engine.GetSubstanceManager().GetSubstance("Oxygen");
      var carbonDioxide = engine.GetSubstanceManager().GetSubstance("CarbonDioxide");

      //Final exonicus data requests
      var ecg = engine.GetEngineTrack().GetDataRequestManager().CreateEquipmentDataRequest(); ecg.SetDataRequestType("ECG"); ecg.Set("Lead3ElectricPotential", ElectricPotentialUnit.mV);
      var MeanUrineRequest = engine.GetEngineTrack().GetDataRequestManager().CreatePhysiologyDataRequest(); MeanUrineRequest.Set("MeanUrineOutput", VolumePerTimeUnit.mL_Per_hr);
      var BrainVasculatureRequest = engine.GetEngineTrack().GetDataRequestManager().CreateLiquidCompartmentDataRequest(); BrainVasculatureRequest.Set("BrainVasculature", oxygen, "PartialPressure", PressureUnit.mmHg);
      var TracheaPartialPressure = engine.GetEngineTrack().GetDataRequestManager().CreateGasCompartmentDataRequest(); TracheaPartialPressure.Set("Trachea", carbonDioxide, "PartialPressure", PressureUnit.mmHg);
  

      engine.GetEngineTrack().SetupRequests();

      var HeartRate = engine.GetEngineTrack().GetScalar(HeartRateRequest);
      var SystolicArterialPressure = engine.GetEngineTrack().GetScalar(HeartRateRequest);
      var DiastolicArterialPressure = engine.GetEngineTrack().GetScalar(DiastolicArterialPressureRequest);
      var RespirationRate = engine.GetEngineTrack().GetScalar(RespirationRateRequest);
      var TidalVolume = engine.GetEngineTrack().GetScalar(TidalVolumeRequest);
      var TotalLungVolume = engine.GetEngineTrack().GetScalar(TotalLungVolumeRequest);
      var OxygenSaturation = engine.GetEngineTrack().GetScalar(OxygenSaturationRequest);
      var PartialPressure = engine.GetEngineTrack().GetScalar(BrainVasculatureRequest);
      var CO2PartialPressure = engine.GetEngineTrack().GetScalar(TracheaPartialPressure);
      var Urine = engine.GetEngineTrack().GetScalar(MeanUrineRequest);
      var ecgData = engine.GetEngineTrack().GetScalar(ecg);

      
      //!
      //!  Testing format outputs of using DataRequest
      //!  UrineOutput is stored in ml/Day in biogears but we want it in ml/hr
      logger.Info(Urine.ToString());
      logger.Info(String.Format("Urine Value {0} {1}", Urine.GetValue(MeanUrineRequest.GetUnit().ToString()), MeanUrineRequest.GetUnit().ToString()));
      logger.Info(String.Format("Urine Value {0} {1}", Urine.GetValue(), Urine.GetUnit().ToString()));
      logger.Info(String.Format("BrainVasculature oxygen partial pressure Value {0} {1}", PartialPressure.GetValue(), PartialPressure.GetUnit().ToString()));
      logger.Info(String.Format("BrainVasculature oxygen partial pressure Value {0} {1}", PartialPressure.GetValue("mmHg"), PartialPressure.GetUnit().ToString()));


      apply_acute_respiratory_distress(engine);

      engine.GetEngineTrack().PullData();
      logger.Info(HeartRate.ToString());
      logger.Info(SystolicArterialPressure.ToString());
      logger.Info(DiastolicArterialPressure.ToString());
      logger.Info(RespirationRate.ToString());
      logger.Info(TidalVolume.ToString());
      logger.Info(TotalLungVolume.ToString());
      logger.Info(OxygenSaturation.ToString());
      logger.Info(PartialPressure.ToString());
      logger.Info(CO2PartialPressure.ToString());
      logger.Info(Urine.ToString());
      logger.Info("ecg: \n");
      logger.Info(ecgData.ToString());



      //!
      //!  Just to test most actions we will apply and unapply every action in 2 second sperts
      //!
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

      var AMConfig = new SEAnesthesiaMachineConfiguration(engine.GetSubstanceManager());
      var config = AMConfig.GetConfiguration();

      config.SetConnection(CDM.enumAnesthesiaMachineConnection.value.Mask);
      config.GetInletFlow().SetValue(2.0, VolumePerTimeUnit.L_Per_min);
      config.GetInspiratoryExpiratoryRatio().SetValue(.5);
      config.GetOxygenFraction().SetValue(.5);
      config.SetOxygenSource(CDM.enumAnesthesiaMachineOxygenSource.value.Wall);
      config.GetPositiveEndExpiredPressure().SetValue(0.0, PressureUnit.cmH2O);
      config.SetPrimaryGas(CDM.enumAnesthesiaMachinePrimaryGas.value.Nitrogen);
      config.GetReliefValvePressure().SetValue(20.0, PressureUnit.cmH2O);
      config.GetRespiratoryRate().SetValue(12, FrequencyUnit.Per_min);
      config.GetVentilatorPressure().SetValue(0.0, PressureUnit.cmH2O);
      config.GetOxygenBottleOne().GetVolume().SetValue(660.0, VolumeUnit.L);
      config.GetOxygenBottleTwo().GetVolume().SetValue(660.0, VolumeUnit.L);
      engine.ProcessAction(AMConfig);

      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);
      config.GetInletFlow().SetValue(5.0, VolumePerTimeUnit.L_Per_min);
      config.GetPositiveEndExpiredPressure().SetValue(3.0, PressureUnit.cmH2O);
      config.GetVentilatorPressure().SetValue(22.0, PressureUnit.cmH2O);
      engine.ProcessAction(AMConfig);

      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);
      config.GetInspiratoryExpiratoryRatio().SetValue(1.0);
      config.GetPositiveEndExpiredPressure().SetValue(1.0, PressureUnit.cmH2O);
      config.GetRespiratoryRate().SetValue(18.0, FrequencyUnit.Per_min);
      config.GetVentilatorPressure().SetValue(10.0, PressureUnit.cmH2O);
      engine.ProcessAction(AMConfig);

      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);
      var AMleak = new SEMaskLeak();
      AMleak.GetSeverity().SetValue(0.5);
      engine.ProcessAction(AMleak);
      engine.GetLogger().Info("Removing the mask leak.");

      engine.AdvanceModelTimeBy(2.0, TimeUnit.s);
      var AMpressureloss = new SEOxygenWallPortPressureLoss();
      AMpressureloss.SetActive(true);
      engine.ProcessAction(AMpressureloss);
      engine.GetLogger().Info("Testing the oxygen pressure loss failure mode. The oxygen pressure from the wall source is dropping.");

      AMpressureloss.SetActive(false);
      engine.ProcessAction(AMpressureloss);
      engine.GetLogger().Info("Removing the wall oxygen pressure loss action.");
      engine.AdvanceModelTimeBy(2, TimeUnit.s);

      engine.SaveStateToFile("SwigStandardMale@2m.xml");

      Console.Write("Simulation Finished Press any key to continue");
      var testString = Console.ReadLine();

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