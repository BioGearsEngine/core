// Hello World! program

using System;
using biogears;

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
            var logger = new Logger("biogears_csharp.log");
            logger.Warning("Perfect Now lets Try to compliate things with two modules!");

            var percent = new SEScalar0To1();
            percent.SetValue(0.1);

            logger.Info(String.Format("{0}",percent.GetValue()));

            var time = new SEScalarTime();
            time.SetValue(4, UnitTime::s);
            logger.Info(String.Format("{0}",time.GetValue()));


            var engine = new BioGearsEngine(logger);
            engine.InitializeEngine("StandardMale.xml");
            engine.AdvanceModelTime();

        }
    }
}
