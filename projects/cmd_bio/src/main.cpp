#include "argumentparser.hpp"

#include <biogears/exports.h>

#include <iostream>
#include <string>

#include "StateGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/PatientGenerator.h"

#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "data/CompoundGenerator.h"
#include "data/NutritionGenerator.h"

int main(int argc, char** argv)
{
  namespace bg = biogears;
  ArgumentParser parser;
  parser.parse(argc, argv);

  if (argc > 1) {
    if (parser.exists("STATES")) {
      auto generator = bg::StateGenerator();

      std::string var1{ argv[2] };
      std::string var2{ " Foo" };
      generator.runScenario(0, var1, var2);
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
      std::cout << "Usage " + std::string(argv[0]) + ": [SUBS|ENVS|PATIENTS|CONF|NEUTRITION]";
    }
  }
  return 0;
}
