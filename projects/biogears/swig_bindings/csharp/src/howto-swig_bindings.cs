// Hello World! program

using System;
using biogears;
using CDM = mil.tatrc.physiology.datamodel;

namespace bio
{
    class csharp_test
    {
        public static void stack_test()
        {
            int[] array = new int[4 * 1024];
            for (int i = 0; i < 1000; ++i)
            {
                array[i] = i;
            }
            Console.Write(array[50]);

        }

        static void Main(string[] args)
        {
            //stack_test();
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

            var patient = engine.GetPatient();
            bool isTacycardia = patient.IsEventActive(CDM.enumPatientEvent.value.Tachycardia);
            bool isAlive = patient.IsEventActive(CDM.enumPatientEvent.value.IrreversibleState);
            bool isKetoacidosis = patient.IsEventActive(CDM.enumPatientEvent.value.Ketoacidosis);

            logger.Warning(String.Format("AcuteStress @ Severity {0}",acuteStress.GetSeverity().GetValue()));

        }
    }
}
