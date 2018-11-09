#include "argumentparser.hpp"

#include <biogears/exports.h>

#include <iostream>
#include <string>
#include <thread>

#include "StateGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/PatientGenerator.h"

#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "data/CompoundGenerator.h"
#include "data/NutritionGenerator.h"

//!
//! \brief Reads command line argument and executes corresponding operation
//! \param argc Number of command line arguments
//! \param argv Pointers to command line arguments
//! \return int 0 on success, other numbers on failure
//! 
int main(int argc, char** argv)
{
  namespace bg = biogears;
  ArgumentParser parser;

  parser.addArgument("DATA");
  parser.addArgument("STATES");
  
  parser.parse(argc,argv);
  if (argc > 1) {
    if (parser.exists("STATES")) {
      biogears::StateGenerator generator{ 5 };

      generator.GenStates();
      generator.run();
      
      while ( !generator.stop_if_empty() )
      {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      generator.join();

    } else if (parser.exists("DATA")) {
      std::vector<std::unique_ptr<bg::CSVToXMLConvertor>> generators;
      generators.push_back(std::make_unique<bg::SubstanceGenerator>());
      generators.push_back(std::make_unique<bg::EnvironmentGenerator>());
      generators.push_back(std::make_unique<bg::PatientGenerator>());
      generators.push_back(std::make_unique<bg::StabilizationGenerator>());
      generators.push_back(std::make_unique<bg::NutritionGenerator>());
      generators.push_back(std::make_unique<bg::CompoundGenerator>());
      for (auto& gen : generators) {
        gen->parse();
        gen->save();
      }
    } else {
      std::cout << "Input not recognized" << std::endl;
      std::cout << "Usage " + std::string(argv[0]) + ": [STATES|DATA]";
    }
  }
  return 0;
}
