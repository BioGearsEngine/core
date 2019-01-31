#include <limits>
#include <thread>

#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>

#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Systems/Gastrointestinal.h>

#include <iostream>
#include <string>
#include <fstream>

#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/schema/cdm/EnvironmentConditions.hxx>
#include <biogears/engine/test/BioGearsEngineTest.h>


#include <biogears/chrono/stop_watch.tci.h>
//
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>

//
#include "circuit_tester.h"
#include <biogears/cdm/test/CommonDataModelTest.h>


#include "biogears_circuit_tester.h"





int main(int argc,char** argv)
{
  biogears::BioGearsCircuitTester* anesthesia_bct = new biogears::BioGearsCircuitTester("Anesthesia.log");
  biogears::BioGearsCircuitTester* cardiovascular_bct = new biogears::BioGearsCircuitTester("Cardiovascular.log");
  biogears::BioGearsCircuitTester* environment_bct = new biogears::BioGearsCircuitTester("Environment.log");
  biogears::BioGearsCircuitTester* renal_bct = new biogears::BioGearsCircuitTester("Renal.log");
  biogears::BioGearsCircuitTester* respiratory_bct = new biogears::BioGearsCircuitTester("Respiratory.log");
  biogears::BioGearsCircuitTester* temperature_bct = new biogears::BioGearsCircuitTester("Temperature.log");
  std::ofstream t_file("TestResults/CircuitTestTime.csv");
  t_file << "Time(milliseconds),10000 Iterations," << std::endl;

  biogears::StopWatch<std::chrono::milliseconds> nano_watch;
  anesthesia_bct->AnesthesiaCircuitTest();
  t_file << "AnesthesiaCircuitTest," << nano_watch.lap() << "," << std::endl;

  nano_watch.reset();
  cardiovascular_bct->CardiovascularCircuitTest();
  t_file << "CardiovascularCircuitTest," << nano_watch.lap() << "," << std::endl;

  nano_watch.reset();
  renal_bct->RenalCircuitTest();
  t_file << "RenalCircuitTest," << nano_watch.lap() << "," << std::endl;

  nano_watch.reset();
  respiratory_bct->RespiratoryCircuitTest();
  t_file << "RespiratoryCircuitTest," << nano_watch.lap() << "," << std::endl;

  nano_watch.reset();
  temperature_bct->TemperatureCircuitTest();
  t_file << "TemperatureCircuitTest," << nano_watch.lap() << "," << std::endl;

  //bct->EnvironmentCircuitTest();
}




