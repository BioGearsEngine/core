#include <iostream>
#include <string>
#include <fstream>

#include <biogears/chrono/stop_watch.tci.h>
#include "BioGearsCircuitProfiler.h"

int main()
{
  biogears::BioGearsCircuitProfiler profiler;
  profiler.AnesthesiaCircuitTest();
  profiler.CardiovascularCircuitTest();
  profiler.RenalCircuitTest();
  profiler.RespiratoryCircuitTest();
  profiler.TemperatureCircuitTest();
  return 0;
}




