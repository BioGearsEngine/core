#include "HowTo-API_Integration.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/BioGearsSubstances.h>

#pragma optimize("", off)
int main(int argc, char* argv[])
{
  // BioGearsPlugin plugin("HowTo-API_Integration");
  //  plugin.run();
  //  std::this_thread::sleep_for(std::chrono::seconds(15));
  //  plugin.stop();
  //  auto start_time = std::chrono::steady_clock::now();
  //  std::stringstream ss;
  //  while (plugin.isRunning()) {
  //    using namespace std::chrono_literals;
  //    auto duration = std::chrono::steady_clock::now() - start_time;

  //  ss.str("");
  //  ss << "Biogears has been running for " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << "\n";
  //  std::cout << ss.str();

  //  std::this_thread::sleep_for(15s);
  //}
  // plugin.join();

  auto logger = std::make_unique<biogears::Logger>();
  auto substances = std::make_unique<biogears::SESubstanceManager>(logger.get());
  substances->LoadSubstanceDirectory();

      auto substance1 = substances->GetSubstance("Oxygen")->GetDefinition();
  auto substance2 = substances->GetSubstance("Nitrogen")->GetDefinition();
  auto substance3 = substances->GetSubstance("Sarin")->GetDefinition();

  auto biogears = std::make_unique<biogears::BioGearsEngine>(logger.get());
  auto patient = std::make_unique<biogears::SEPatient>(logger.get());
  patient->Load("DefaultTemplateMale.xml");

  biogears->LoadState("DefaultTemplateMale@0s.xml");

  for (int i = 0; i < 1000; ++i) {
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();

    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();

    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();

    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();

    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
    biogears->AdvanceModelTime();
  }
}