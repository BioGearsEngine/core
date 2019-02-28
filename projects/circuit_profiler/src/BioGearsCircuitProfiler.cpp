#include <thread>

#include "BioGearsCircuitProfiler.h"
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/chrono/stop_watch.tci.h>

namespace biogears {
void BioGearsCircuitProfiler::AnesthesiaCircuitTest()
{
  biogears::Logger logger("AnesthesiaCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("AnesthesiaCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& anesthesia = circuits.GetAnesthesiaMachineCircuit();
  SEFluidCircuitCalculator fcc{ &logger };
  int64_t current_time = 0;
  std::ofstream timer_output("AnesthesiaCircuitProfile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    fcc.Process(anesthesia, 1.0);
    fcc.PostProcess(anesthesia);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
  anesthesia.GetPolarizedElementPaths();
  anesthesia.GetReferenceNodes();
}

void BioGearsCircuitProfiler::CardiovascularCircuitTest()
{
  biogears::Logger logger("CardiovascularCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("CardiovascularCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& Cardiovascular = circuits.GetCardiovascularCircuit();
  SEFluidCircuitCalculator fcc{ &logger };
  int64_t current_time = 0;
  std::ofstream timer_output("CardiovascularCircuitProfile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    fcc.Process(Cardiovascular, 1.0);
    fcc.PostProcess(Cardiovascular);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
}

void BioGearsCircuitProfiler::RenalCircuitTest()
{
  biogears::Logger logger("RenalCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("RenalCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& Renal = circuits.GetRenalCircuit();
  SEFluidCircuitCalculator fcc{ &logger };
  int64_t current_time = 0;
  std::ofstream timer_output("RenalCircuitProfile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    fcc.Process(Renal, 1.0);
    fcc.PostProcess(Renal);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
}

//nohidea. - happy
//Lost Son - oh look, it's raining again
//luv.ly - in luv.
//lovelystuff - a new romance
//HM Surf - Scopophobia
//Engelwood - La Fonda
//bil - figure (a)
//jeiku - we spent the day at the beach
void BioGearsCircuitProfiler::RespiratoryCircuitTest()
{
  biogears::Logger logger("RespiratoryCircuitTest.log");
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&logger);
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());
  bge->LoadState("./states/StandardMale@0s.xml");
  bge->AdvanceModelTime();

  bg->GetLogger()->Info("RespiratoryCircuitTest");
  BioGearsCircuits& circuits = bg->GetCircuits();
  SEFluidCircuit& Respiratory = circuits.GetRespiratoryCircuit();
  SEFluidCircuitCalculator fcc{ &logger };
  int64_t current_time = 0;
  std::ofstream timer_output("RespiratoryCircuitProfile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    fcc.Process(Respiratory, 1.0);
    fcc.PostProcess(Respiratory);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
}


void BioGearsCircuitProfiler::TemperatureCircuitTest()
{
  biogears::Logger logger("TemperatureCircuitTest.log");                 // Create logger
  std::unique_ptr<PhysiologyEngine> bge = CreateBioGearsEngine(&logger); // Create PhysiologyEngine*
  BioGears* bg = dynamic_cast<BioGears*>(bge.get());                     // Cast it to be a BioGears*
  bge->LoadState("./states/StandardMale@0s.xml");                        // Load the state
  bge->AdvanceModelTime();                                               // Advance model time
  int64_t current_time = 0;                                              // Not important
  bg->GetLogger()->Info("TemperatureCircuitTest");                       // Set logger info
  BioGearsCircuits& circuits = bg->GetCircuits();                        // Get full biogears circuit system
  SEThermalCircuit& thermal = circuits.GetTemperatureCircuit();          // Pull out temperature circuit
  SEThermalCircuitCalculator tcc{ &logger };                             // Stand up a circuit calculator

  std::ofstream timer_output("TemperatureCircuitProfile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    tcc.Process(thermal, 1.0);
    tcc.PostProcess(thermal);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
}

void BioGearsCircuitProfiler::ProfileThermalCircuit(SEThermalCircuit& thermal, std::string circuit_name)
{
  std::ofstream timer_output(circuit_name + "Profile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  int64_t current_time = 0;

  biogears::Logger logger(circuit_name+".log");
  SEThermalCircuitCalculator tcc{ &logger };
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    tcc.Process(thermal, 1.0);
    tcc.PostProcess(thermal);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
}

void BioGearsCircuitProfiler::ProfileFluidCircuit(SEFluidCircuit fluid, std::string circuit_name)
{
  std::ofstream timer_output(circuit_name + "Profile.csv");
  timer_output << "Iterations,Time(nanoseconds)" << '\n' << "0,0" << '\n';
  int64_t current_time = 0;

  biogears::Logger logger(circuit_name + ".log");
  SEFluidCircuitCalculator tcc{ &logger };
  biogears::StopWatch<std::chrono::nanoseconds> nano_watch;
  for (int i = 1; i <= 10000; ++i) {
    tcc.Process(fluid, 1.0);
    tcc.PostProcess(fluid);
    current_time += nano_watch.lap();
    timer_output << i << "," << current_time << '\n';
  }
  timer_output.close();
}

BioGearsCircuitProfiler::BioGearsCircuitProfiler()
{
}

BioGearsCircuitProfiler::~BioGearsCircuitProfiler()
{
};

} //end namespace biogears
