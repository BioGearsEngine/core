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

    public static string ToString(CDM.enumPatientEvent.value ev)
    {

      switch (ev)
      {
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

    public static string ToString(CDM.enumAnesthesiaMachineEvent.value ev)
    {
      switch (ev)
      {
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

    public BioGearsEventHandler() : base()
    {
    }

    public override void HandlePatientEvent(CDM.enumPatientEvent.value type, bool active, biogears.SEScalarTime time)
    {
      if (g_event_count++ % 31 != 0)
      {
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
      if (ev.Length < marquee_1.Length)
      {
        marquee_1.Remove((marquee_1.Length / 2) - (ev.Length / 2), ev.Length);
        marquee_1.Insert((marquee_1.Length / 2), ev);
      }
      else
      {
        marquee_1 = new StringBuilder(ev);
      }
      if (state.Length < marquee_2.Length)
      {
        marquee_2.Remove((marquee_2.Length / 2) - (state.Length / 2), state.Length);
        marquee_2.Insert((marquee_2.Length / 2), state);
      }
      else
      {
        marquee_1 = new StringBuilder(state);
      }
      Console.WriteLine(prefix + marquee_1 + end);
      Console.WriteLine(prefix + marquee_2 + end);
      Console.WriteLine("(__________|_[______]_|______________________________________|");
      Console.WriteLine("  0--0--0      0  0      0       0     0        0        0    ");
      Console.WriteLine("\n");
    }

    public override void HandlePatientEvent(CDM.enumPatientEvent.value type, bool active)
    {
      var eventTime = new SEScalarTime();
      eventTime.SetValue(0, TimeUnit.s);
      HandlePatientEvent(type, active, eventTime);
    }

    public override void HandleAnesthesiaMachineEvent(CDM.enumAnesthesiaMachineEvent.value type, bool active, biogears.SEScalarTime time)
    {
      if (g_event_count++ % 31 != 0)
      {
        return;
      }

      Console.WriteLine(@"");
      Console.WriteLine(@"                           (_)");
      Console.WriteLine(@"            --""-------   0/      ^^");
      Console.WriteLine(@"  .___...../ /__| |__\ \_/H__,      ^^");

      string ev = ToString(type);
      var marquee_1 = new StringBuilder(@"                        |");
      var marquee_2 = new StringBuilder(@"                        /");
      string prefix_1 = @"|";
      string prefix_2 = @"\";
      string end_1 = "|";
      string end_2 = "/";


      string state = (active) ? "(On)" : "(Off)";
      if (ev.Length < marquee_1.Length)
      {
        marquee_1.Remove((marquee_1.Length / 2) - (ev.Length / 2), ev.Length);
        marquee_1.Insert((marquee_1.Length / 2) - (ev.Length / 2), ev);
      }
      else
      {
        marquee_1 = new StringBuilder(ev);
      }
      if (state.Length < marquee_2.Length)
      {
        marquee_2.Remove((marquee_2.Length / 2) - (state.Length / 2), state.Length);
        marquee_2.Insert((marquee_2.Length / 2) - (state.Length / 2), state);
      }
      else
      {
        marquee_1 = new StringBuilder(state);
      }
      Console.WriteLine(prefix_1 + marquee_1 + end_1);
      Console.WriteLine(prefix_2 + marquee_2 + end_2);

      Console.WriteLine(@"#####^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~\O/~~\Q/~^~^~rr\n");
      Console.WriteLine("\n");
    }


    public override void HandleAnesthesiaMachineEvent(CDM.enumAnesthesiaMachineEvent.value type, bool active)
    {
      var eventTime = new SEScalarTime();
      eventTime.SetValue(0, TimeUnit.s);
      HandleAnesthesiaMachineEvent(type, active, eventTime);
    }
  };


  static class csharp_test
  {

    public static biogears.Logger logger;
    //!
    //! Very Terse testing of the SWIG_Bindings
    //!
    //! We will simply test the ability
    //!   1. Create a logger and log to a file
    //!   2. Initialize a BioGearsEngine
    //!   3. Advance Model Time 
    //!   4. Apply Actions to an exsiting BioGearsEngine
    //!   5. Create and Apply all BioGears Actions



    static void Main(string[] args)
    {
      //Create a Log File 
      logger = new Logger("chsarp_debugging.log");
      logger.Warning("Playground for running csharp binding debuggs");



      //Initialize an Engine by Patient Requiring Stabilization
      var engine = new BioGearsEngine(logger);
      engine.LoadState("States/DefaultMale@0s.xml");
      engine.GetEngineTrack().GetDataRequestManager().SetResultsFilename("csharp_debugging.csv");
      engine.GetEngineTrack().GetDataRequestManager().GetSamplesPerSecond();
      engine.GetEngineTrack().GetDataRequestManager().SetSamplesPerSecond(60);
      var eh = new BioGearsEventHandler();
      engine.SetEventHandler(eh);

      engine.AdvanceModelTimeBy(30, TimeUnit.s);

      double radiation = 70.0;
      SERadiationAbsorbedDose action = new SERadiationAbsorbedDose();
      action.GetDose().SetValue(radiation, biogears.EnergyPerMassUnit.J_Per_kg);
      engine.ProcessAction(action);

      engine.GetEngineTrack().GetDataRequestManager().HasResultsFilename();

      engine.AdvanceModelTimeBy(30, TimeUnit.s);

      logger.Warning("Done.");
      logger.Warning("Catch Me for Break Points.");
    }
  }
}
