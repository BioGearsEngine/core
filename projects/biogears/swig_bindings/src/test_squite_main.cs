// Hello World! program

using System;
using biogears;

namespace bio
{ 
    class csharp_test {         
        static void Main(string[] args)
        {
            var logger = new biogears.Logger("unit_test_cs.log");
            logger.Warning("Perfect Now lets Try to compliate things with two modules!");
        }
    }
}
 